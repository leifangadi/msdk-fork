/**
 * @file    main.c
 * @brief   Demonstration of SRAM ECC
 * @details This program demonstrates single and double-bit error detection and
 *          single-bit correction for SRAM memories.
 */

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

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_pins.h"
#include "led.h"
#include "board.h"
#include "gcr_regs.h"
#include "mcr_regs.h"

/***** Definitions *****/
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/***** Globals *****/
volatile uint32_t badData;
volatile uint32_t eccFlag;
volatile uint32_t eccErr;
volatile uint32_t eccDErr;
volatile uint32_t eccAddr;

// Find the highest address in RAM, subtract the portion taken away by ECC
uint32_t ramTop = (MXC_SRAM_MEM_BASE + (MXC_SRAM_MEM_SIZE * 0.8));

/***** Functions *****/

void ECC_IRQHandler(void)
{
    eccErr = MXC_GCR->ecc_er;
    eccDErr = MXC_GCR->ecc_ced;
    eccAddr = MXC_GCR->ecc_errad;
    eccFlag = 1;

    MXC_GCR->ecc_er = eccErr;
    MXC_GCR->ecc_ced = eccDErr;
}

void clear_ram(void)
{
    extern uint32_t _ebss; // end of bss section, it is defined in linker file
    uint32_t *ptr;
    uint32_t sysram0_end = 0x20007fff; // end of SRAM0

    // clear SRAM0 (after bss section)
    for (ptr = (uint32_t *)&_ebss; (uint32_t)ptr < sysram0_end; ptr++) {
        *ptr = 0;
    }

    // clear SRAM1, SRAM2, ... SRAM6
    MXC_GCR->memzcn |= 0x3E;
    while (MXC_GCR->memzcn & 0x3E) {
        {
        }
        // wait until clear done
    }
}

// *****************************************************************************
int main(void)
{
    unsigned int i, test_fail, test_pass;
    volatile uint32_t *cursor;

    test_fail = test_pass = 0;

    printf("\n\n***** MAX" TOSTRING(TARGET) " SRAM ECC Example *****\n\n");
    printf("This example will corrupt a word of data\n");
    printf("and ensure that the ECC interrupts on an error\n");
    printf("when the corrupted address is read\n\n");

    /*
     * Any read from non-initialized RAM could trigger an ECC error
     * since the random check bits will most likely not match the random data bits.
     * Writing the memory to all zeroes at bootup can prevent this at the expense of the time required.
     * So that clear RAM before ECC check
     */
    clear_ram();

    // Clear all ECC Errors -- write-1-to-clear
    MXC_GCR->ecc_er = (volatile uint32_t)MXC_GCR->ecc_er;
    MXC_GCR->ecc_ced = (volatile uint32_t)MXC_GCR->ecc_ced;

    // Enable interrupts for ECC errors
    MXC_GCR->ecc_irqen |= MXC_F_GCR_ECC_IRQEN_SYSRAM0ECCEN | MXC_F_GCR_ECC_IRQEN_SYSRAM1ECCEN |
                          MXC_F_GCR_ECC_IRQEN_SYSRAM2ECCEN | MXC_F_GCR_ECC_IRQEN_SYSRAM3ECCEN |
                          MXC_F_GCR_ECC_IRQEN_SYSRAM4ECCEN | MXC_F_GCR_ECC_IRQEN_SYSRAM5ECCEN;
    NVIC_EnableIRQ(ECC_IRQn);

    // Scan all of memory, which should not cause any errors to be detected
    printf("Preliminary scan to ensure no pre-existing ECC errors\n");
    printf("RAM TOP: %X\n", ramTop);

    eccFlag = 0;

    for (i = MXC_SRAM_MEM_BASE; i < ramTop - sizeof(uint32_t); i += sizeof(uint32_t)) {
        cursor = (uint32_t *)i;
        // Force a read of the location.
        (volatile uint32_t) * cursor;

        if (eccFlag) {
            break;
        }
    }

    if (eccFlag) {
        printf("ECC Error:              0x%08x\n", eccErr);
        printf("ECC Not Double Error:   0x%08x\n", eccDErr);
        printf("ECC Error Address:      0x%08x\n", eccAddr);
        printf("FAIL: Error found in preliminary memory scan\n");
        eccFlag = 0;
        test_fail++;
    } else {
        printf("PASS: No errors\n");
        test_pass++;
    }

    // Initialize data
    badData = 0xDEADBEEF;

    printf("\nData before Corruption: 0x%08x\n", badData);
    printf("Address of data: 0x%08x\n", &badData);

    // Disable ECC so data can be corrupted
    MXC_MCR->eccen &= ~MXC_F_MCR_ECCEN_SYSRAM0ECCEN;
    badData = 0xDEADBEEE;
    MXC_MCR->eccen |= MXC_F_MCR_ECCEN_SYSRAM0ECCEN;

    printf("\nData after single-bit error: 0x%08x\n", badData);
    printf("ECC Error:              0x%08x\n", eccErr);
    printf("ECC Not Double Error:   0x%08x\n", eccDErr);
    printf("ECC Error Address:      0x%08x\n", eccAddr);

    if (eccFlag) {
        printf("PASS: An ECC Error was found\n");
        test_pass++;
    } else {
        printf("FAIL: Error not detected!\n");
        test_fail++;
    }
    eccFlag = 0;

    // Disable ECC so data can be corrupted
    MXC_MCR->eccen &= ~MXC_F_MCR_ECCEN_SYSRAM0ECCEN;
    badData = 0xDEADBEEC;
    MXC_MCR->eccen |= MXC_F_MCR_ECCEN_SYSRAM0ECCEN;

    printf("\nData after double-bit error: 0x%08x\n", badData);
    printf("ECC Error:              0x%08x\n", eccErr);
    printf("ECC Not Double Error:   0x%08x\n", eccDErr);
    printf("ECC Error Address:      0x%08x\n", eccAddr);

    if (eccFlag) {
        printf("PASS: An ECC Error was found\n");
        test_pass++;
    } else {
        printf("FAIL: Error not detected!\n");
        test_fail++;
    }
    eccFlag = 0;

    printf("\n# Passed: %u, # Failed: %u, Test %s\n", test_pass, test_fail,
           test_fail ? "FAIL!" : "Ok");

    if (test_fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
