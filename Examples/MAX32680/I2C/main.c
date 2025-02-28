/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

/**
 * @file        main.c
 * @brief       I2C Loopback Example
 * @details     This example uses the I2C Master to read/write from/to the I2C Slave. For
 *              this example you must connect P0.10 to P0.16 (SCL) and P0.11 to P0.17 (SCL). The Master
 *              will use P0.10 and P0.11. The Slave will use P0.16 and P0.17. You must also
 *              connect the pull-up jumpers (JP21 and JP22) to the proper I/O voltage.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2c.h"
#include "dma.h"

/***** Definitions *****/
// #define MASTERDMA

#define I2C_MASTER MXC_I2C1
#define I2C_SLAVE MXC_I2C0

#define I2C_FREQ 100000
// This example may become unreliable at I2C frequencies above 100kHz.
// This is only an issue in the loopback configuration, where the I2C block is
// connected to itself.
#define I2C_SLAVE_ADDR (0x51)
#define I2C_BYTES 100

/***** Globals *****/
static uint8_t Stxdata[I2C_BYTES];
static uint8_t Srxdata[I2C_BYTES];
static uint8_t txdata[I2C_BYTES];
static uint8_t rxdata[I2C_BYTES];
volatile uint8_t DMA_FLAG = 0;
volatile int I2C_FLAG;
volatile int txnum = 0;
volatile int txcnt = 0;
volatile int rxnum = 0;
volatile int num;
/***** Functions *****/

//Slave interrupt handler
void I2C0_IRQHandler(void)
{
    MXC_I2C_AsyncHandler(I2C_SLAVE);
    return;
}

//I2C callback function
void I2C_Callback(mxc_i2c_req_t *req, int error)
{
    I2C_FLAG = error;
    return;
}

int slaveHandler(mxc_i2c_regs_t *i2c, mxc_i2c_slave_event_t event, void *data)
{
    switch (event) {
    case MXC_I2C_EVT_MASTER_WR:
        // If we're being written to
        // Clear bytes written
        rxnum = 0;
        break;

    case MXC_I2C_EVT_MASTER_RD:

        // Serve as a 16 byte loopback, returning data*2
        for (int i = 0; i < I2C_BYTES; i++) {
            Stxdata[i] = Srxdata[i];
        }

        txnum = I2C_BYTES;
        txcnt = 0;
        i2c->intfl0 = MXC_F_I2C_INTFL0_TX_LOCKOUT | MXC_F_I2C_INTFL0_ADDR_MATCH;
        break;

    case MXC_I2C_EVT_RX_THRESH:
    case MXC_I2C_EVT_OVERFLOW:
        rxnum += MXC_I2C_ReadRXFIFO(i2c, &Srxdata[rxnum], MXC_I2C_GetRXFIFOAvailable(i2c));

        if (rxnum == I2C_BYTES) {
            i2c->inten0 |= MXC_F_I2C_INTEN0_ADDR_MATCH;
        }

        break;

    case MXC_I2C_EVT_TX_THRESH:
    case MXC_I2C_EVT_UNDERFLOW:

        // Write as much data as possible into TX FIFO
        // Unless we're at the end of the transaction (only write what's needed)
        if (txcnt >= txnum) {
            break;
        }

        int num = MXC_I2C_GetTXFIFOAvailable(i2c);
        num = (num > (txnum - txcnt)) ? (txnum - txcnt) : num;
        txcnt += MXC_I2C_WriteTXFIFO(i2c, &Stxdata[txcnt], num);
        break;

    default:
        if (*((int *)data) == E_COMM_ERR) {
            printf("I2C Slave Error!\n");
            printf("i2c->int_fl0 = 0x%08x\n", i2c->intfl0);
            printf("i2c->status  = 0x%08x\n", i2c->status);
            I2C_Callback(NULL, E_COMM_ERR);
            return 1;
        } else if (*((int *)data) == E_NO_ERROR) {
            rxnum += MXC_I2C_ReadRXFIFO(i2c, &Srxdata[rxnum], MXC_I2C_GetRXFIFOAvailable(i2c));
            I2C_Callback(NULL, E_NO_ERROR);
            return 1;
        }
    }

    return 0;
}

//Prints out human-friendly format to read txdata and rxdata
void printData(void)
{
    int i;
    printf("\n-->TxData: ");

    for (i = 0; i < sizeof(txdata); ++i) {
        printf("%02x ", txdata[i]);
    }

    printf("\n\n-->RxData: ");

    for (i = 0; i < sizeof(rxdata); ++i) {
        printf("%02x ", rxdata[i]);
    }

    printf("\n");

    return;
}

//Compare data to see if they are the same
int verifyData()
{
    int i, fails = 0;

    for (i = 0; i < I2C_BYTES; ++i) {
        if (txdata[i] != rxdata[i]) {
            ++fails;
        }
    }

    if (fails > 0) {
        return E_FAIL;
    }

    return E_NO_ERROR;
}

// *****************************************************************************
int main()
{
    printf("\n******** I2C SLAVE ASYNC TRANSACTION TEST *********\n");
    printf("\nThis example uses one I2C peripheral as a master to\n");
    printf("read and write to another I2C which acts as a slave.\n");

    printf("\nYou will need to connect P0.10 to P0.16 (SCL) and\n");
    printf("P0.11 to P0.17 (SDA).\n");

    int error, i = 0;

    //Setup the I2CM
    error = MXC_I2C_Init(I2C_MASTER, 1, 0);

    if (error != E_NO_ERROR) {
        printf("-->Failed master\n");
        return error;
    } else {
        printf("\n-->I2C Master Initialization Complete");
    }

    //Setup the I2CS
    error = MXC_I2C_Init(I2C_SLAVE, 0, I2C_SLAVE_ADDR);

    if (error != E_NO_ERROR) {
        printf("Failed slave\n");
        return error;
    } else {
        printf("\n-->I2C Slave Initialization Complete");
    }

    MXC_NVIC_SetVector(I2C0_IRQn, I2C0_IRQHandler);
    NVIC_EnableIRQ(I2C0_IRQn);

    MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ);

    MXC_I2C_SetFrequency(I2C_SLAVE, I2C_FREQ);

    // Initialize test data
    for (i = 0; i < I2C_BYTES; i++) {
        txdata[i] = i;
        rxdata[i] = 0;
        Stxdata[i] = i;
        Srxdata[i] = 0;
    }

    // This will write data to slave
    // Then read data back from slave
    mxc_i2c_req_t reqMaster;
    reqMaster.i2c = I2C_MASTER;
    reqMaster.addr = I2C_SLAVE_ADDR;
    reqMaster.tx_buf = txdata;
    reqMaster.tx_len = I2C_BYTES;
    reqMaster.rx_buf = rxdata;
    reqMaster.rx_len = I2C_BYTES;
    reqMaster.restart = 0;
    reqMaster.callback = I2C_Callback;
    I2C_FLAG = 1;

    printf("\n\n-->Writing data to slave, and reading the data back\n");

    if ((error = MXC_I2C_SlaveTransactionAsync(I2C_SLAVE, slaveHandler)) != 0) {
        printf("Error Starting Slave Transaction %d\n", error);
        return error;
    }

    if ((error = MXC_I2C_MasterTransaction(&reqMaster)) != 0) {
        printf("Error writing: %d\n", error);
        return error;
    }

    while (I2C_FLAG == 1) {}

    printf("\n-->Result: \n");

    printData();

    printf("\n");

    if (verifyData()) {
        printf("\n-->I2C Transaction Successful\n");
    } else {
        printf("\n-->I2C Transaction Failed\n");
        return E_FAIL;
    }

    return E_NO_ERROR;
}
