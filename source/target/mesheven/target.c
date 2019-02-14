/**
 * @file    target.c
 * @brief   Target information for the multi-targets
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

#include "target_config.h"
#include "target_ids.h"

// The file flash_blob.c must only be included in target.c
//#include "flash_blob.c"
extern const program_target_t NRF51_flash;
extern const program_target_t stm32f051_flash;
extern const program_target_t stm32f071_flash;
extern const program_target_t stm32f103_flash;
extern const program_target_t stm32f405_flash;
extern const program_target_t stm32f031_flash;
extern const program_target_t stm32l486_flash;
extern const program_target_t NRF52_flash;
extern const program_target_t stm32f301k8_flash;
extern const sector_info_t stm32f405_flash_region[3];
    

// target information
const target_cfg_t target_device[] = 
{
    //nrf51822
    {
        .sector_size    = 1024,
        .sector_cnt     = 256,
        .flash_start    = 0x00000000,
        .flash_end      = 0x00040000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20008000,
        .flash_algo     = (program_target_t *) &NRF51_flash,

        .extra_flash[0].start = 0x10001000,
        .extra_flash[0].end   = 0x10001100,
    },
    //stm32f051kX
    {
        .sector_size    = 1024,
        .sector_cnt     = 64,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08010000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20002000,
        .flash_algo     = (program_target_t *) &stm32f051_flash,
    },
    //stm32f103rc
    {
        .sector_size    = 2048,
        .sector_cnt     = 128,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08040000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x2000C000,
        .flash_algo     = (program_target_t *) &stm32f103_flash,
    },
    //stm32f405
    {
        .sector_size    = 16384,
        .sector_cnt     = 64,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08100000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20020000,
        .flash_algo     = (program_target_t *) &stm32f405_flash,
        
        .sector_info_length = 3,
        .sectors_info       = stm32f405_flash_region,

        .extra_ram[0].start = 0x10000000,
        .extra_ram[0].end   = 0x10010000,        
    },
    //stm32f071
    {
        .sector_size    = 2048,
        .sector_cnt     = 64,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08020000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20004000,
        .flash_algo     = (program_target_t *) &stm32f071_flash,
    },
    //stm32f031
    {
        .sector_size    = 1024,
        .sector_cnt     = 32,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08008000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20001000,
        .flash_algo     = (program_target_t *) &stm32f031_flash,
    },    
    //stm32l486
    {
        .sector_size    = 2048,
        .sector_cnt     = 512,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08100000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20020000,
        .flash_algo     = (program_target_t *) &stm32l486_flash,
        
        .extra_ram[0].start = 0x10000000,
        .extra_ram[0].end   = 0x10008000,        
    },
    //nrf52832
    {
        .sector_size    = 4096,
        .sector_cnt     = 512,
        .flash_start    = 0x00000000,
        .flash_end      = 0x00080000,
        .ram_start      = 0x20000000,
        .ram_end        = 0x20010000,
        .flash_algo     = (program_target_t *) &NRF52_flash,

        .extra_flash[0].start = 0x10001000,
        .extra_flash[0].end   = 0x10001400,     
    },
    //stm32f301k8
    {
        .sector_size    = 2048,
        .sector_cnt     = 32,
        .flash_start    = 0x08000000,
        .flash_end      = 0x08010000,  // 64KB
        .ram_start      = 0x20000000,
        .ram_end        = 0x20004000,  // 16KB
        .flash_algo     = (program_target_t *) &stm32f301k8_flash,               
    },		
};
