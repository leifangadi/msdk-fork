/******************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All rights Reserved.
 * 
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
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

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "mxc_device.h"
#include "tft.h"
#include "spi.h"
#include "gpio.h"

/************************************ DEFINES ********************************/
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define TFT_SPI (MXC_SPI1)
#define TFT_RESET_GPIO_PIN (MXC_GPIO_PIN_17)
#define TFT_RESET_GPIO_PORT (MXC_GPIO1)
#define TFT_BLEN_GPIO_PIN (MXC_GPIO_PIN_16)
#define TFT_BLEN_GPIO_PORT (MXC_GPIO1)

//
#define PALETTE_OFFSET(x)                                                               \
    concat(images_start_addr + images_header.offset2info_palatte + 1 /* nb_palette */ + \
               (x) * sizeof(uint32_t),                                                  \
           4)
#define FONT_OFFSET(x)                                                               \
    concat(images_start_addr + images_header.offset2info_font + 1 /* nb_font    */ + \
               (x) * sizeof(uint32_t),                                               \
           4)
#define BITMAP_OFFSET(x)                                                               \
    concat(images_start_addr + images_header.offset2info_bitmap + 1 /* nb_bitmap  */ + \
               (x) * sizeof(uint32_t),                                                 \
           4)

/********************************* TYPE DEFINES ******************************/
#pragma pack(1)

typedef struct {
    uint32_t w;
    uint32_t h;
    uint8_t id_palette;
    uint8_t rle;
    uint32_t data_size;
} bitmap_info_t;

typedef struct {
    uint16_t x;
    uint8_t w;
    uint8_t hex_code;
} font_char_t;

typedef struct {
    //uint8_t size;
    uint8_t nb_char;
    uint8_t bitmap_id;
    //font_char_t    *char_info; // X nb_char
} font_info_t;

typedef struct {
    //uint16_t    type;                        /* Magic identifier               */
    //uint16_t    reserved1, reserved2;
    //uint16_t    bits;                        /* Bits per pixel                 */
    //
    uint32_t offset2info_palatte; // Palettes start address
    uint32_t offset2info_font; // Fonts start address
    uint32_t offset2info_bitmap; // Bitmap start address
    //
    uint32_t nb_palette; // number of palette
    uint32_t nb_font; // number of fonts
    uint32_t nb_bitmap; // number of bitmap
} Header_images_t;

#pragma pack()

extern uint32_t _bin_start_; // binary start address, defined in linker file
extern uint32_t _bin_end_; // binary end address, defined in linker file
static uint8_t *images_start_addr = NULL;
static Header_images_t images_header;

static uint32_t g_background_color;
static uint32_t g_palette_ram[256];
static uint32_t g_fifo[4];

static uint32_t cursor_x;
static uint32_t cursor_y;

static area_t pf_area;
static int g_font_id = 0;

/********************************* Static Functions **************************/
static void __attribute__((noinline)) halfClockDelay(void)
{
    __asm volatile("nop\n");
}

static int concat(uint8_t *var, int size)
{
    int result = 0;

    for (int i = 1; i <= size; i++) {
        result |= var[size - i] << (8 * (size - i));
    }

    return result;
}

static void get_font_info(int font_id, font_info_t *font_info, font_char_t **chr_pos)
{
    uint32_t offset;

    offset = FONT_OFFSET(font_id);
    memcpy(font_info, (images_start_addr + offset), sizeof(font_info_t));
    *chr_pos = (font_char_t *)(images_start_addr + offset + sizeof(font_info_t));
}

static void get_bitmap_info(int bitmap_id, bitmap_info_t *bitmap_info, uint8_t **pixel)
{
    uint32_t offset;

    offset = BITMAP_OFFSET(bitmap_id);
    memcpy(bitmap_info, (images_start_addr + offset), sizeof(bitmap_info_t));
    *pixel = (uint8_t *)((images_start_addr + offset + sizeof(bitmap_info_t)));
}

#if 0
static void spi_transmit(void* datain, uint32_t count)
{
    mxc_spi_req_t request = {
        TFT_SPI,    // spi
        0,      // ssIdx
        1,      // ssDeassert
        (uint8_t*) datain, // txData
        NULL,       // rxData
        count,      // txCnt
        0       // rxCnt
    };

    MXC_SPI_MasterTransaction(&request);
}
#else
static void spi_transmit(void *datain, uint32_t count)
{
    uint32_t offset;
    uint32_t fifo;
    volatile uint16_t *u16ptrin = (volatile uint16_t *)datain;
    uint32_t start = 0;

    // HW requires disabling/renabling SPI block at end of each transaction (when SS is inactive).
    TFT_SPI->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);

    // Setup the slave select
    MXC_SETFIELD(TFT_SPI->ctrl0, MXC_F_SPI_CTRL0_SS, ((1 << 0) << MXC_F_SPI_CTRL0_SS_POS));

    // number of RX Char is 0xffff
    TFT_SPI->ctrl1 &= ~(MXC_F_SPI_CTRL1_RX_NUM_CHAR);

    //DMA RX FIFO disabled
    TFT_SPI->dma &= ~(MXC_F_SPI_DMA_RX_FIFO_EN);

    // set number of char to be transmit
    MXC_SETFIELD(TFT_SPI->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR,
                 count << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS);
    // DMA TX fifo enable
    TFT_SPI->dma |= MXC_F_SPI_DMA_TX_FIFO_EN;

    /* Clear TX and RX FIFO in DMA
        TX: Set this bit to clear the TX FIFO and all TX FIFO flags in the QSPIn_INT_FL register.
            Note: The TX FIFO should be disabled (QSPIn_DMA.tx_fifo_en = 0) prior to setting this field.
            Note: Setting this field to 0 has no effect.
        RX: Clear the RX FIFO and any pending RX FIFO flags in QSPIn_INTFL.
            This should be done when the RX FIFO is inactive.
    */
    TFT_SPI->dma |= (MXC_F_SPI_DMA_TX_FIFO_CLEAR | MXC_F_SPI_DMA_RX_FIFO_CLEAR);
    // QSPIn port is enabled
    TFT_SPI->ctrl0 |= (MXC_F_SPI_CTRL0_EN);

    // Clear master done flag
    TFT_SPI->int_fl = MXC_F_SPI_INT_FL_M_DONE;

    /* Loop until all data is transmitted */
    offset = 0;

    do {
        fifo = (count > 8) ? 8 : count;
        count -= fifo;

        while (fifo > 0) {
            /* Send data */
            TFT_SPI->data16[0] = u16ptrin[offset];
            offset++;
            fifo--;
        }

        /*
            Master Start Data Transmission
                Set this field to 1 to start a SPI master mode transaction.
                0: No master mode transaction active.
                1: Master initiates a data transmission. Ensure that all pending transactions are
                complete before setting this field to 1.
                Note: This field is only used when the QSPIn is configured for Master Mode
                (QSPIn_CTRL0.master = 1).
        */
        if (start == 0) {
            TFT_SPI->ctrl0 |= MXC_F_SPI_CTRL0_START;
            start = 1;
        }

        /* Wait for data transmitting complete and then Deasserts nSS I/O */
        // Deassert slave select at the end of the transaction
        TFT_SPI->ctrl0 &= ~MXC_F_SPI_CTRL0_SS_CTRL;
    } while (count);

    while (!(TFT_SPI->int_fl & MXC_F_SPI_INT_FL_M_DONE)) {
        // wait until done
    }

    return;
}
#endif

static void write_command(uint16_t command)
{
    uint16_t val = command & 0xFF;
    spi_transmit(&val, 1);
}

static void write_data(uint16_t data)
{
    uint16_t val = ((data >> 8) & 0xFF) | 0x0100;

    spi_transmit(&val, 1);
    val = (data & 0xFF) | 0x0100;
    spi_transmit(&val, 1);
}

static void write_color(uint32_t color_palette)
{
    uint16_t val = color_palette & 0xFFFF;

    spi_transmit(&val, 1);
    val = (color_palette >> 16) & 0xFFFF;
    spi_transmit(&val, 1);
}

static void print_line(const uint8_t *line, int nb_of_pixel)
{
    int i;
    int x;
    int loop_counter = nb_of_pixel >> 2; // div 4

    for (x = 0; x < loop_counter; x++) {
        for (i = 0; i < 4; i++) {
            g_fifo[i] = *(g_palette_ram + line[(x << 2) + i]);
        }

        spi_transmit((uint16_t *)g_fifo, 8);
    }

    x <<= 2;

    for (; x < nb_of_pixel; x++) {
        write_color(*(g_palette_ram + line[x]));
    }
}

static void RLE_decode(uint8_t const *in, uint32_t length, int img_h, int img_w)
{
    uint8_t cmd, data;
    uint32_t i, inpos;
    uint8_t line[320] = {
        0,
    };
    uint32_t line_start_pos[320] = {
        0,
    };
    int index = 0;
    int is_ended = 0;
    uint32_t nb_of_pixel = 0;

    if (length < 1) {
        return;
    }

    /*
     *  Find start position for each line
     */
    inpos = 0;
    line_start_pos[index++] = inpos;

    do {
        cmd = in[inpos++];
        data = in[inpos++];

        if (cmd == 0x00) {
            switch (data) {
            case 0:
                line_start_pos[index++] = inpos;
                break;

            case 1: //end of image
                is_ended = 1;
                break;

            case 2:
                inpos += 2; // pass x and y
                break;

            default:
                inpos += data;

                if (data % 2) {
                    inpos++;
                }

                break;
            }
        }

        if (is_ended == 1) {
            break;
        } else if (index >= img_h) {
            break;
        }
    } while (inpos < length);

    for (index = index - 1; index >= 0; index--) {
        inpos = line_start_pos[index];
        is_ended = 0;
        nb_of_pixel = 0;

        /* Main decompression loop */
        do {
            cmd = in[inpos++];
            data = in[inpos++];

            if (cmd == 0x00) {
                switch (data) {
                case 0:
                case 1:
                    while (nb_of_pixel < (uint32_t)img_w) {
                        line[nb_of_pixel++] = 0;
                    }

                    print_line(line, img_w);
                    is_ended = 1;
                    break;

                case 2: {
                    uint32_t x, y;
                    x = in[inpos++];
                    y = in[inpos++];

                    for (i = 0; i < x; i++) {
                        line[nb_of_pixel++] = 0;
                    }

                    for (i = 0; i < y; i++) {
                        print_line(line, img_w);
                    }
                } break;

                default:
                    for (i = 0; i < data; i++) {
                        line[nb_of_pixel++] = in[inpos++];
                    }

                    if (data % 2) {
                        inpos++;
                    }

                    break;
                }
            } else {
                for (i = 0; i < cmd; i++) {
                    line[nb_of_pixel++] = data;
                }
            }

            if (is_ended == 1) {
                break;
            }
        } while (inpos < length);
    }
}

static void tft_spi_init(void)
{
    int master = 1;
    int quadMode = 0;
    int numSlaves = 1;
    int ssPol = 0;
    uint32_t tft_hz = 12 * 1000 * 1000;

    MXC_SPI_Init(TFT_SPI, master, quadMode, numSlaves, ssPol, tft_hz);

    // Enable SPI1_SS0 pin (on own port, so SPI_Init doesn't enable it)
    mxc_gpio_cfg_t SPI1_SS0 = { MXC_GPIO0, MXC_GPIO_PIN_31, MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE,
                                MXC_GPIO_VSSEL_VDDIO };

    MXC_GPIO_Config(&SPI1_SS0);

    // Set each spi pin to select VDDIOH (3.3V)
    SPI1_SS0.port->vssel |= SPI1_SS0.mask;
    gpio_cfg_spi1.port->vssel |= gpio_cfg_spi1.mask;

    MXC_SPI_SetDataSize(TFT_SPI, 9);
    MXC_SPI_SetWidth(TFT_SPI, SPI_WIDTH_STANDARD);
}

static void displayInit(void)
{
    int i;

    // CLR Reset pin;
    MXC_GPIO_OutClr(TFT_RESET_GPIO_PORT, TFT_RESET_GPIO_PIN);

    for (i = 0; i < 50000; i++) {
        halfClockDelay();
    }

    // SET Reset pin;
    MXC_GPIO_OutSet(TFT_RESET_GPIO_PORT, TFT_RESET_GPIO_PIN);

    for (i = 0; i < 150000; i++) {
        halfClockDelay();
    }

    write_command(0x0000);
    write_command(0x0028); // VCOM OTP
    write_data(0x0006); // Page 55-56 of SSD2119 datasheet

    write_command(0x0000); // start Oscillator
    write_data(0x0001); // Page 36 of SSD2119 datasheet

    write_command(0x0010); // Sleep mode
    write_data(0x0000); // Page 49 of SSD2119 datasheet

    write_command(0x0001); // Driver Output Control
    write_data(0x72EF); // Page 36-39 of SSD2119 datasheet

    write_command(0x0002); // LCD Driving Waveform Control
    write_data(0x0600); // Page 40-42 of SSD2119 datasheet

    write_command(0x0003); // Power Control 1
    write_data(0x6A38); // Page 43-44 of SSD2119 datasheet

    write_command(0x0011); // Entry Mode
    write_data(0x6870); // Page 50-52 of SSD2119 datasheet

    write_command(0X000F); // Gate Scan Position
    write_data(0x0000); // Page 49 of SSD2119 datasheet

    write_command(0X000B); // Frame Cycle Control
    write_data(0x5308); // Page 45 of SSD2119 datasheet

    write_command(0x000C); // Power Control 2
    write_data(0x0003); // Page 47 of SSD2119 datasheet

    write_command(0x000D); // Power Control 3
    write_data(0x000A); // Page 48 of SSD2119 datasheet

    write_command(0x000E); // Power Control 4
    write_data(0x2E00); // Page 48 of SSD2119 datasheet

    write_command(0x001E); // Power Control 5
    write_data(0x00B7); // Page 55 of SSD2119 datasheet

    write_command(0x0025); // Frame Frequency Control
    write_data(0x8000); // Page 53 of SSD2119 datasheet

    write_command(0x0026); // Analog setting
    write_data(0x3800); // Page 54 of SSD2119 datasheet

    write_command(0x0027); // Critical setting to avoid pixel defect
    write_data(0x0078); // per solomon systech, apparently undocumented.

    write_command(0x004E); // Ram Address Set
    write_data(0x0000); // Page 58 of SSD2119 datasheet

    write_command(0x004F); // Ram Address Set
    write_data(0x0000); // Page 58 of SSD2119 datasheet

    write_command(0x0012); // Sleep mode
    write_data(0x0D99); // Page 49 of SSD2119 datasheet

    // Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
    write_command(0x0030);
    write_data(0x0000);

    write_command(0x0031);
    write_data(0x0104);

    write_command(0x0032);
    write_data(0x0100);

    write_command(0x0033);
    write_data(0x0305);

    write_command(0x0034);
    write_data(0x0505);

    write_command(0x0035);
    write_data(0x0305);

    write_command(0x0036);
    write_data(0x0707);

    write_command(0x0037);
    write_data(0x0300);

    write_command(0x003A);
    write_data(0x1200);

    write_command(0x003B);
    write_data(0x0800);

    write_command(0x0007); // Display Control
    write_data(0x0033); // Page 45 of SSD2119 datasheet

    for (i = 0; i < 50000; i++) {
        halfClockDelay();
    }

    write_command(0x0022); // RAM data write/read
}

static void setPalette(uint8_t id)
{
    int i;
    uint8_t *palette;

    if (id >= images_header.nb_palette) {
        return;
    }

    palette = (uint8_t *)(images_start_addr + PALETTE_OFFSET(id));

    /* set palette only if it was changed */
    for (i = 0; i < 16; i++) { //only test the first 16
        if (g_palette_ram[i] !=
            (0x01000100 | ((palette[0] & 0xF8) << 13) | ((palette[1] & 0x1C) << 19) |
             ((palette[1] & 0xE0) >> 5) | (palette[2] & 0xF8))) {
            goto setup_palette;
        }

        palette += 4;
    }

    return;

setup_palette:

    /** Setup Palette */
    for (; i < 256; i++) {
        g_palette_ram[i] = (0x01000100 | ((palette[0] & 0xF8) << 13) | ((palette[1] & 0x1C) << 19) |
                            ((palette[1] & 0xE0) >> 5) | (palette[2] & 0xF8));
        palette += 4;
    }
}

static void displayAll(void)
{
    write_command(0x004E); // RAM address set
    write_data(0x0000); // Page 58 of SSD2119 datasheet
    write_command(0x004F); // RAM address set
    write_data(0x0000); // Page 58 of SSD2119 datasheet
    write_command(0x0044); // Vertical RAM address position
    write_data(0xEF00); // Page 57 of SSD2119 datasheet
    write_command(0x0045); // Horizontal RAM address position
    write_data(0x0000); // Page 57 of SSD2119 datasheet
    write_command(0x0046); // Horizontal RAM address position
    write_data(0x013F); // Page 57 of SSD2119 datasheet
    write_command(0x0022); // RAM data write/read
}

static void displaySub(int x0, int y0, int width, int height)
{
    write_command(0x004E); // RAM address set
    write_data(x0 & 0x1FF); // Page 58 of SSD2119 datasheet
    write_command(0x004F); // RAM address set
    write_data(y0 & 0xFF); // Page 58 of SSD2119 datasheet
    write_command(0x0044); // Vertical RAM address position
    write_data((((y0 + height - 1) & 0xFF) << 8) | (y0 & 0xFF)); // Page 57 of SSD2119 datasheet
    write_command(0x0045); // Horizontal RAM address position
    write_data(x0 & 0x1FF); // Page 57 of SSD2119 datasheet
    write_command(0x0046); // Horizontal RAM address position
    write_data((x0 + width - 1) & 0x1FF); // Page 57 of SSD2119 datasheet
    write_command(0x0022); // RAM data write/read
}

static void writeSubBitmap(int x0, int y0, int img_w, int img_h, const uint8_t *img_data, int sub_x,
                           int sub_w)
{
    __disable_irq();
    int y, x, i;
    int img_w_rounded = ((8 * img_w + 31) / 32) * 4;

    if ((x0 + sub_w) > DISPLAY_WIDTH) {
        sub_w = DISPLAY_WIDTH - x0;
    }

    if ((y0 + img_h) > DISPLAY_HEIGHT) {
        img_h = DISPLAY_HEIGHT - y0;
    }

    displaySub(x0, y0, sub_w, img_h);

    for (y = img_h - 1; y >= 0; y--) {
        for (x = 0; x < (sub_w >> 2); x++) {
            for (i = 0; i < 4; i++) {
                g_fifo[i] = *(g_palette_ram + img_data[y * img_w_rounded + sub_x + (x << 2) + i]);
            }

            spi_transmit((uint16_t *)g_fifo, 8);
        }

        x <<= 2;

        for (; x < sub_w; x++) {
            write_color(*(g_palette_ram + img_data[y * img_w_rounded + sub_x + x]));
        }
    }

    __enable_irq();
}

static void printfCheckBounds(int next_width, int line_height)
{
    area_t clear;

    if ((cursor_x + next_width) > (pf_area.w + pf_area.x)) {
        cursor_x = pf_area.x;
        cursor_y += line_height;

        if (cursor_y > (pf_area.h + pf_area.y)) {
            cursor_y = pf_area.y;
        }

        clear.x = cursor_x;
        clear.y = cursor_y;
        clear.w = pf_area.w;
        clear.h = line_height;

        MXC_TFT_FillRect(&clear, g_background_color);
    }
}

static void printCursor(char *str)
{
    int i;
    bitmap_info_t bitmap_info;
    font_info_t font_info;
    font_char_t *chr_pos;
    uint8_t chId;
    uint8_t *pixel;
    int len;

    if ((uint32_t)g_font_id >= images_header.nb_font) {
        return;
    }

    get_font_info(g_font_id, &font_info, &chr_pos);
    get_bitmap_info(font_info.bitmap_id, &bitmap_info, &pixel);
    setPalette(bitmap_info.id_palette);

    len = strlen(str);

    for (i = 0; i < len; i++) {
        if (str[i] == '\n') {
            printfCheckBounds(DISPLAY_WIDTH,
                              bitmap_info.h); // using display size will force cursor to next line
        } else if (str[i] == ' ') {
            printfCheckBounds(8, bitmap_info.h); // Check if space will need to wrap
            cursor_x += 8;
        } else {
            // find char
            for (chId = 0; chId < font_info.nb_char; chId++) {
                if (str[i] == chr_pos[chId].hex_code) {
                    printfCheckBounds(chr_pos[chId].w + 1, bitmap_info.h);
                    writeSubBitmap(cursor_x, cursor_y, bitmap_info.w, bitmap_info.h, pixel,
                                   chr_pos[chId].x, chr_pos[chId].w);
                    cursor_x += chr_pos[chId].w + 1; // font.intr_chr;
                    break;
                }
            }
        }
    }
}

/******************************** Public Functions ***************************/
int MXC_TFT_Init(void)
{
    int result = E_NO_ERROR;
    mxc_gpio_cfg_t config;

    // Initialize images_header to contain no image data.
    // This sets the number of palettes, fonts, and bitmaps to 0.
    memset(&images_header, 0, sizeof(Header_images_t));

    // Is there any image data to work with?
    if (_bin_start_ != _bin_end_) {
        images_start_addr = (uint8_t *)&_bin_start_;
        // set header
        memcpy(&images_header, images_start_addr, sizeof(Header_images_t));
    }

    /*
     *      Configure GPIO Pins
     */
    // Display backlight pin and reset pin
    config.pad = MXC_GPIO_PAD_NONE;
    config.port = TFT_BLEN_GPIO_PORT;
    config.mask = TFT_BLEN_GPIO_PIN | TFT_RESET_GPIO_PIN;
    config.vssel = MXC_GPIO_VSSEL_VDDIOH;
    config.func = MXC_GPIO_FUNC_OUT;

    MXC_GPIO_Config(&config);

    // Configure SPI Pins
    tft_spi_init();

    // Turn off backlight
    MXC_TFT_Backlight(0);

    // Send commands to configure display
    displayInit();

    // Set default palette
    setPalette(0);

    // Set background color
    MXC_TFT_SetBackGroundColor(0);

    // Turn on backlight
    MXC_TFT_Backlight(1);

    return result;
}

void MXC_TFT_SetBackGroundColor(uint32_t color)
{
    __disable_irq();
    uint32_t x, y, i;

    displayAll();

    for (y = 0; y < DISPLAY_HEIGHT; y++) {
        for (x = 0; x < (uint32_t)(DISPLAY_WIDTH >> 2); x++) {
            for (i = 0; i < 4; i++) {
                g_fifo[i] = g_palette_ram[color];
            }

            spi_transmit((uint16_t *)g_fifo, 8);
        }
    }

    __enable_irq();
    // keep color
    g_background_color = color;

    return;
}

int MXC_TFT_SetPalette(int img_id)
{
    bitmap_info_t bitmap_info;
    uint8_t *pixel;

    if ((uint32_t)img_id >= images_header.nb_bitmap) {
        return E_BAD_PARAM;
    }

    get_bitmap_info(img_id, &bitmap_info, &pixel);
    setPalette(bitmap_info.id_palette);

    return E_NO_ERROR;
}

void MXC_TFT_Backlight(int on)
{
    if (on) {
        MXC_GPIO_OutSet(TFT_BLEN_GPIO_PORT, TFT_BLEN_GPIO_PIN);
    } else {
        MXC_GPIO_OutClr(TFT_BLEN_GPIO_PORT, TFT_BLEN_GPIO_PIN);
    }
}

void MXC_TFT_ShowImage(int x0, int y0, int id)
{
    int y, width, height, img_w_rounded;
    bitmap_info_t bitmap_info;
    uint8_t *pixel;

    if ((uint32_t)id >= images_header.nb_bitmap) {
        return;
    }

    __disable_irq();

    get_bitmap_info(id, &bitmap_info, &pixel);

    width = bitmap_info.w;
    height = bitmap_info.h;

    if ((x0 + width) > DISPLAY_WIDTH) {
        width = DISPLAY_WIDTH - x0;
    }

    if ((y0 + height) > DISPLAY_HEIGHT) {
        height = DISPLAY_HEIGHT - y0;
    }

    //
    displaySub(x0, y0, width, height);
    setPalette(bitmap_info.id_palette);

    if (bitmap_info.rle == 1) {
        RLE_decode(pixel, bitmap_info.data_size, height, width);
    } else {
        img_w_rounded = ((8 * bitmap_info.w + 31) / 32) * 4;

        for (y = height - 1; y >= 0; y--) {
            print_line(&pixel[y * img_w_rounded], width);
        }
    }

    __enable_irq();
}

void MXC_TFT_ClearScreen(void)
{
    MXC_TFT_SetBackGroundColor(g_background_color);
}

void MXC_TFT_FillRect(area_t *area, int color)
{
    __disable_irq();
    int y, x, i, h, w;

    w = area->w;
    h = area->h;

    if ((area->x + w) > DISPLAY_WIDTH) {
        w = DISPLAY_WIDTH - area->x;
    }

    if ((area->y + h) > DISPLAY_HEIGHT) {
        h = DISPLAY_HEIGHT - area->y;
    }

    displaySub(area->x, area->y, w, h);

    for (y = 0; y < h; y++) {
        for (x = 0; x < (w >> 2); x++) {
            for (i = 0; i < 4; i++) {
                g_fifo[i] = g_palette_ram[color];
            }

            spi_transmit((uint16_t *)g_fifo, 8);
        }

        x <<= 2;

        for (; x < w; x++) {
            write_color(g_palette_ram[color]);
        }
    }

    __enable_irq();
}

void MXC_TFT_PrintPalette(void)
{
    int i;
    area_t area = { 10, 10, 2, 25 };

    for (i = 0; i < 256; i++) {
        area.x += 4;
        MXC_TFT_FillRect(&area, i);

        if ((i & 63) == 63) {
            area.y += 25;
            area.x = 10;
        }
    }
}

/***************************************************************
 *          Printf Functions
 ***************************************************************/
void MXC_TFT_ResetCursor(void)
{
    cursor_x = pf_area.x;
    cursor_y = pf_area.y;
}

void MXC_TFT_SetFont(int font_id)
{
    g_font_id = font_id;
}

void MXC_TFT_Printf(const char *format, ...)
{
    char str[100];

    snprintf(str, sizeof(str), format, *((&format) + 1), *((&format) + 2), *((&format) + 3));

    printCursor(str); //printf_message
}

void MXC_TFT_ConfigPrintf(area_t *area)
{
    pf_area.x = area->x;
    pf_area.y = area->y;
    pf_area.w = area->w;
    pf_area.h = area->h;

    MXC_TFT_ResetCursor();
}

void MXC_TFT_PrintFont(int x0, int y0, int id, text_t *str, area_t *area)
{
    int i;
    int x;
    bitmap_info_t bitmap_info;
    font_info_t font_info;
    font_char_t *chr_pos;
    uint8_t chId;
    uint8_t *pixel;

    if ((uint32_t)id >= images_header.nb_font) {
        return;
    }

    get_font_info(id, &font_info, &chr_pos);
    get_bitmap_info(font_info.bitmap_id, &bitmap_info, &pixel);
    setPalette(bitmap_info.id_palette);

    x = x0;

    for (i = 0; i < str->len; i++) {
        if (str->data[i] == ' ') {
            x += 8; //font.space; // TODO add space in font bitmap file
        } else {
            // find char
            for (chId = 0; chId < font_info.nb_char; chId++) {
                if (str->data[i] == chr_pos[chId].hex_code) {
                    writeSubBitmap(x, y0, bitmap_info.w, bitmap_info.h, pixel, chr_pos[chId].x,
                                   chr_pos[chId].w);
                    x += chr_pos[chId].w + 1; // font.intr_chr;
                    break;
                }
            }
        }
    }

    if (area) {
        area->x = x0;
        area->y = y0;
        area->h = bitmap_info.h;
        area->w = x - x0;
    }
}

void MXC_TFT_Print(int x0, int y0, text_t *str, area_t *area)
{
    MXC_TFT_PrintFont(x0, y0, g_font_id, str, area);
}

void MXC_TFT_ClearArea(area_t *area, int color)
{
    int y, x, i, h, w;

    __disable_irq();

    w = area->w;
    h = area->h;

    if ((area->x + w) > DISPLAY_WIDTH) {
        w = DISPLAY_WIDTH - area->x;
    }

    if ((area->y + h) > DISPLAY_HEIGHT) {
        h = DISPLAY_HEIGHT - area->y;
    }

    displaySub(area->x, area->y, w, h);

    for (y = 0; y < h; y++) {
        for (x = 0; x < (w >> 2); x++) {
            for (i = 0; i < 4; i++) {
                g_fifo[i] = *(g_palette_ram + color);
            }

            spi_transmit((uint16_t *)g_fifo, 8);
        }

        x <<= 2;

        for (; x < w; x++) {
            write_color(*(g_palette_ram + color));
        }
    }

    __enable_irq();
}
