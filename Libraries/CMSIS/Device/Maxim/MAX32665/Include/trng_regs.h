/**
 * @file    trng_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the TRNG Peripheral Module.
 * @note    This file is @generated.
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

#ifndef LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32665_INCLUDE_TRNG_REGS_H_
#define LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32665_INCLUDE_TRNG_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__ICCARM__)
  #pragma system_include
#endif

#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     trng
 * @defgroup    trng_registers TRNG_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the TRNG Peripheral Module.
 * @details     Random Number Generator.
 */

/**
 * @ingroup trng_registers
 * Structure type to access the TRNG Registers.
 */
typedef struct {
    __IO uint32_t cn;                   /**< <tt>\b 0x00:</tt> TRNG CN Register */
    __I  uint32_t st;                   /**< <tt>\b 0x04:</tt> TRNG ST Register */
    __I  uint32_t data;                 /**< <tt>\b 0x08:</tt> TRNG DATA Register */
} mxc_trng_regs_t;

/* Register offsets for module TRNG */
/**
 * @ingroup    trng_registers
 * @defgroup   TRNG_Register_Offsets Register Offsets
 * @brief      TRNG Peripheral Register Offsets from the TRNG Base Peripheral Address.
 * @{
 */
#define MXC_R_TRNG_CN                      ((uint32_t)0x00000000UL) /**< Offset from TRNG Base Address: <tt> 0x0000</tt> */
#define MXC_R_TRNG_ST                      ((uint32_t)0x00000004UL) /**< Offset from TRNG Base Address: <tt> 0x0004</tt> */
#define MXC_R_TRNG_DATA                    ((uint32_t)0x00000008UL) /**< Offset from TRNG Base Address: <tt> 0x0008</tt> */
/**@} end of group trng_registers */

/**
 * @ingroup  trng_registers
 * @defgroup TRNG_CN TRNG_CN
 * @brief    TRNG Control Register.
 * @{
 */
#define MXC_F_TRNG_CN_ODHT_POS                         0 /**< CN_ODHT Position */
#define MXC_F_TRNG_CN_ODHT                             ((uint32_t)(0x1UL << MXC_F_TRNG_CN_ODHT_POS)) /**< CN_ODHT Mask */

#define MXC_F_TRNG_CN_RND_IRQ_EN_POS                   1 /**< CN_RND_IRQ_EN Position */
#define MXC_F_TRNG_CN_RND_IRQ_EN                       ((uint32_t)(0x1UL << MXC_F_TRNG_CN_RND_IRQ_EN_POS)) /**< CN_RND_IRQ_EN Mask */

#define MXC_F_TRNG_CN_HEALTH_EN_POS                    2 /**< CN_HEALTH_EN Position */
#define MXC_F_TRNG_CN_HEALTH_EN                        ((uint32_t)(0x1UL << MXC_F_TRNG_CN_HEALTH_EN_POS)) /**< CN_HEALTH_EN Mask */

#define MXC_F_TRNG_CN_AESKG_MEU_POS                    3 /**< CN_AESKG_MEU Position */
#define MXC_F_TRNG_CN_AESKG_MEU                        ((uint32_t)(0x1UL << MXC_F_TRNG_CN_AESKG_MEU_POS)) /**< CN_AESKG_MEU Mask */

#define MXC_F_TRNG_CN_AESKG_MEMPROTE_POS               4 /**< CN_AESKG_MEMPROTE Position */
#define MXC_F_TRNG_CN_AESKG_MEMPROTE                   ((uint32_t)(0x1UL << MXC_F_TRNG_CN_AESKG_MEMPROTE_POS)) /**< CN_AESKG_MEMPROTE Mask */

#define MXC_F_TRNG_CN_AESKG_MEMPROTA_POS               5 /**< CN_AESKG_MEMPROTA Position */
#define MXC_F_TRNG_CN_AESKG_MEMPROTA                   ((uint32_t)(0x1UL << MXC_F_TRNG_CN_AESKG_MEMPROTA_POS)) /**< CN_AESKG_MEMPROTA Mask */

/**@} end of group TRNG_CN_Register */

/**
 * @ingroup  trng_registers
 * @defgroup TRNG_ST TRNG_ST
 * @brief    Data. The content of this register is valid only when RNG_IS = 1. When TRNG is
 *           disabled, read returns 0x0000 0000.
 * @{
 */
#define MXC_F_TRNG_ST_RND_RDY_POS                      0 /**< ST_RND_RDY Position */
#define MXC_F_TRNG_ST_RND_RDY                          ((uint32_t)(0x1UL << MXC_F_TRNG_ST_RND_RDY_POS)) /**< ST_RND_RDY Mask */

#define MXC_F_TRNG_ST_ODHTS_POS                        1 /**< ST_ODHTS Position */
#define MXC_F_TRNG_ST_ODHTS                            ((uint32_t)(0x1UL << MXC_F_TRNG_ST_ODHTS_POS)) /**< ST_ODHTS Mask */

#define MXC_F_TRNG_ST_HTS_POS                          2 /**< ST_HTS Position */
#define MXC_F_TRNG_ST_HTS                              ((uint32_t)(0x1UL << MXC_F_TRNG_ST_HTS_POS)) /**< ST_HTS Mask */

#define MXC_F_TRNG_ST_SRCFAIL_POS                      3 /**< ST_SRCFAIL Position */
#define MXC_F_TRNG_ST_SRCFAIL                          ((uint32_t)(0x1UL << MXC_F_TRNG_ST_SRCFAIL_POS)) /**< ST_SRCFAIL Mask */

#define MXC_F_TRNG_ST_AESKGD_MEU_S_POS                 4 /**< ST_AESKGD_MEU_S Position */
#define MXC_F_TRNG_ST_AESKGD_MEU_S                     ((uint32_t)(0x1UL << MXC_F_TRNG_ST_AESKGD_MEU_S_POS)) /**< ST_AESKGD_MEU_S Mask */

/**@} end of group TRNG_ST_Register */

/**
 * @ingroup  trng_registers
 * @defgroup TRNG_DATA TRNG_DATA
 * @brief    Data. The content of this register is valid only when RNG_IS = 1. When TRNG is
 *           disabled, read returns 0x0000 0000.
 * @{
 */
#define MXC_F_TRNG_DATA_DATA_POS                       0 /**< DATA_DATA Position */
#define MXC_F_TRNG_DATA_DATA                           ((uint32_t)(0xFFFFFFFFUL << MXC_F_TRNG_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group TRNG_DATA_Register */

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32665_INCLUDE_TRNG_REGS_H_
