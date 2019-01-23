/**
 * @file    IO_Config.h
 * @brief   
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// Override all defines if IO_CONFIG_OVERRIDE is defined
#ifdef IO_CONFIG_OVERRIDE
#include "IO_Config_Override.h"
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__
#endif
#endif


#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "LPC11Uxx.h"
#include "daplink.h"

// This GPIO configuration is only valid for the LPC11U35 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_LPC11U35);

// Peripheral register bit masks (used for pin inits)
#define FUNC_0                          0
#define FUNC_1                          1
#define PULL_DOWN_ENABLED               (1 << 3)
#define PULL_UP_ENABLED                 (2 << 3)
#define OPENDRAIN                       (1 << 10)
#define DIGITAL_FUN_MODE                (1 << 7)

// DAP LED                              PIO0_13
#define PIN_DAP_LED_PORT                0
#define PIN_DAP_LED_BIT                 13
#define PIN_DAP_LED                     (1 << PIN_DAP_LED_BIT)
#define PIN_DAP_LED_IOCON               LPC_IOCON->TDO_PIO0_13
#define PIN_DAP_LED_IOCON_INIT          (FUNC_1)

// CDC LED                              PIO0_12
#define PIN_CDC_LED_PORT                0
#define PIN_CDC_LED_BIT                 12
#define PIN_CDC_LED                     (1 << PIN_CDC_LED_BIT)
#define PIN_CDC_LED_IOCON               LPC_IOCON->TMS_PIO0_12
#define PIN_CDC_LED_IOCON_INIT          (FUNC_1)

// Non-Forwarded Reset in PIN           PIO0_1
//#define PIN_RESET_IN_PORT               0
//#define PIN_RESET_IN_BIT                1
//#define PIN_RESET_IN                    (1 << PIN_RESET_IN_BIT)
//#define PIN_RESET_IN_IOCON              LPC_IOCON->PIO0_1
//#define PIN_RESET_IN_IOCON_INIT         (FUNC_0 | OPENDRAIN | PULL_UP_ENABLED)

// Forwarded Reset in PIN               PIO0_2
#define PIN_RESET_IN_FWRD_PORT          0
#define PIN_RESET_IN_FWRD_BIT           2
#define PIN_RESET_IN_FWRD               (1 << PIN_RESET_IN_FWRD_BIT)
#define PIN_RESET_IN_FWRD_IOCON         LPC_IOCON->PIO0_2
#define PIN_RESET_IN_FWRD_IOCON_INIT    (FUNC_0)

// nRESET OUT Pin                       PIO0_23
#define PIN_nRESET_PORT                 0
#define PIN_nRESET_BIT                  23
#define PIN_nRESET                      (1 << PIN_nRESET_BIT)
#define PIN_nRESET_IOCON                LPC_IOCON->PIO0_23
#define PIN_nRESET_IOCON_INIT           (FUNC_0 | OPENDRAIN)

// SWCLK/TCK Pin                        PIO0_9
#define PIN_SWCLK_PORT                  0
#define PIN_SWCLK_BIT                   9
#define PIN_SWCLK                       (1 << PIN_SWCLK_BIT)
#define PIN_SWCLK_TCK_IOCON             LPC_IOCON->PIO0_9
#define PIN_SWCLK_TCK_IOCON_INIT        (FUNC_0)

// SWDIO/TMS In/Out Pin                 PIO0_8
#define PIN_SWDIO_PORT                  0
#define PIN_SWDIO_BIT                   8
#define PIN_SWDIO                       (1 << PIN_SWDIO_BIT)
#define PIN_SWDIO_TMS_IOCON             LPC_IOCON->PIO0_8
#define PIN_SWDIO_TMS_IOCON_INIT        (FUNC_0)

// TDI Pin                              PIO0_17
#define PIN_TDI_PORT                    0
#define PIN_TDI_BIT                     17
#define PIN_TDI                         (1 << PIN_TDI_BIT)
#define PIN_TDI_IOCON                   LPC_IOCON->PIO0_17
#define PIN_TDI_IOCON_INIT              (FUNC_0)

// SWO/TDO Pin                          PIO0_9
#define PIN_TDO_PORT                    0
#define PIN_TDO_BIT                     9
#define PIN_TDO                         (1 << PIN_TDO_BIT)
#define PIN_TDO_IOCON                   LPC_IOCON->PIO0_9
#define PIN_TDO_IOCON_INIT              (FUNC_0)

// A0 pin                             PIO0_20
#define PIN_A0_PORT                   0
#define PIN_A0_BIT                    20
#define PIN_A0                        (1 << PIN_A0_BIT)
#define PIN_A0_IOCON                  LPC_IOCON->PIO0_20
#define PIN_A0_IOCON_INIT             (FUNC_0)

// A1 pin                             PIO0_16
#define PIN_A1_PORT                   0
#define PIN_A1_BIT                    16
#define PIN_A1                        (1 << PIN_A1_BIT)
#define PIN_A1_IOCON                  LPC_IOCON->PIO0_16
#define PIN_A1_IOCON_INIT             (FUNC_0)

// A2 pin                             PIO0_11
#define PIN_A2_PORT                   0
#define PIN_A2_BIT                    11
#define PIN_A2                        (1 << PIN_A2_BIT)
#define PIN_A2_IOCON                  LPC_IOCON->TDI_PIO0_11
#define PIN_A2_IOCON_INIT             (FUNC_1)

// A3 pin                             PIO0_14
#define PIN_A3_PORT                   0
#define PIN_A3_BIT                    14
#define PIN_A3                        (1 << PIN_A3_BIT)
#define PIN_A3_IOCON                  LPC_IOCON->TRST_PIO0_14
#define PIN_A3_IOCON_INIT             (FUNC_1)

// A4 pin                             PIO0_21
#define PIN_A4_PORT                   0
#define PIN_A4_BIT                    21
#define PIN_A4                        (1 << PIN_A4_BIT)
#define PIN_A4_IOCON                  LPC_IOCON->PIO0_21
#define PIN_A4_IOCON_INIT             (FUNC_0)

// A5 pin                             PIO0_22
#define PIN_A5_PORT                   0
#define PIN_A5_BIT                    22
#define PIN_A5                        (1 << PIN_A5_BIT)
#define PIN_A5_IOCON                  LPC_IOCON->PIO0_22
#define PIN_A5_IOCON_INIT             (FUNC_0)

// A6 pin                             PIO1_15
#define PIN_A6_PORT                   1
#define PIN_A6_BIT                    15
#define PIN_A6                        (1 << PIN_A6_BIT)
#define PIN_A6_IOCON                  LPC_IOCON->PIO1_15
#define PIN_A6_IOCON_INIT             (FUNC_0)

// A7 pin                             PIO1_19
#define PIN_A7_PORT                   1
#define PIN_A7_BIT                    19
#define PIN_A7                        (1 << PIN_A7_BIT)
#define PIN_A7_IOCON                  LPC_IOCON->PIO1_19
#define PIN_A7_IOCON_INIT             (FUNC_0)

// A8 pin                             PIO0_4
#define PIN_A8_PORT                   0
#define PIN_A8_BIT                    4
#define PIN_A8                        (1 << PIN_A8_BIT)
#define PIN_A8_IOCON                  LPC_IOCON->PIO0_4
#define PIN_A8_IOCON_INIT             (FUNC_0)

// A9 pin                             PIO0_5
#define PIN_A9_PORT                   0
#define PIN_A9_BIT                    5
#define PIN_A9                        (1 << PIN_A9_BIT)
#define PIN_A9_IOCON                  LPC_IOCON->PIO0_5
#define PIN_A9_IOCON_INIT             (FUNC_0)

// A10 pin                             PIO0_10
#define PIN_A10_PORT                   0
#define PIN_A10_BIT                    10
#define PIN_A10                        (1 << PIN_A10_BIT)
#define PIN_A10_IOCON                  LPC_IOCON->SWCLK_PIO0_10
#define PIN_A10_IOCON_INIT             (FUNC_1)

// A11 pin                             PIO0_15
#define PIN_A11_PORT                   0
#define PIN_A11_BIT                    15
#define PIN_A11                        (1 << PIN_A11_BIT)
#define PIN_A11_IOCON                  LPC_IOCON->SWDIO_PIO0_15
#define PIN_A11_IOCON_INIT             (FUNC_1)

#endif
