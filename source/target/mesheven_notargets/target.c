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

#include <stdbool.h>
#include "target_config.h"
#include "error.h"
#include "flash_blob.h"

enum
{
    FLASH_SECTION_TYPE = 0x01,
    EXTRA_FLASH_TYPE   = 0x02,
    EXTRA_RAM_TYPE     = 0x03
};
// flash algorithm blob: now set max 4K, default is NRF52832
static uint32_t flash_prog_blob[1024] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    0x47702000, 0x47702000, 0x4c2bb570, 0x60202002, 0x20014929, 0x60083108, 0x68284d28, 0xd00207c0,
    0x60202000, 0xf000bd70, 0xe7f6f833, 0x4c22b570, 0x60212102, 0x2f10f1b0, 0x491fd303, 0x31102001,
    0x491de001, 0x60081d09, 0xf0004d1c, 0x6828f821, 0xd0fa07c0, 0x60202000, 0xe92dbd70, 0xf8df41f0,
    0x088e8058, 0x46142101, 0xf8c84605, 0x4f131000, 0xc501cc01, 0x07c06838, 0x1e76d007, 0x2100d1f8,
    0x1000f8c8, 0xe8bd4608, 0xf00081f0, 0xe7f1f801, 0x6800480b, 0x00fff010, 0x490ad00c, 0x29006809,
    0x4908d008, 0x31fc4a08, 0xd00007c3, 0x1d09600a, 0xd1f90840, 0x00004770, 0x4001e504, 0x4001e400,
    0x40010404, 0x40010504, 0x6e524635, 0x00000000,    
};
static uint32_t flash_write_index = 0;
// flash has  difference sectors size number: now set max 7
static sector_info_t sectors_info[8];

// flash algorithm config, default is NRF52832
static program_target_t flash = {
    .init                   = 0x20000021,
    .uninit                 = 0x20000025,
    .erase_chip             = 0x20000029,
    .erase_sector           = 0x2000004D,
    .program_page           = 0x2000007B,
    .verify                 = 0x00000000,
    .sys_call_s             = { 0x20000001, 0x20000110, 0x20001000},
    .program_buffer         = 0x20000200,
    .algo_start             = 0x20000000,
    .algo_size              = 240,
    .algo_blob              = flash_prog_blob,
    .program_buffer_size    = 512,
    .hardware_reset_support = 0,
};


// target flash configure information, default is NRF52832
target_cfg_t target_device = 
{
    .sector_size          = 4096,
    .sector_cnt           = 128,
    .flash_start          = 0x00000000,
    .flash_end            = 0x00080000,
    .ram_start            = 0x20000000,
    .ram_end              = 0x20010000,
    .flash_algo           = &flash,
    .erase_reset          = 0,
    .sector_info_length   = 0,
    .sectors_info         = sectors_info,
    
    .extra_flash[0].start = 0x10001000,
    .extra_flash[0].end   = 0x10001400,
};

static uint32_t readUint32BE(const uint8_t *data)
{
    uint32_t rc = ((*data) << 24) | (*(data+1) << 16) | (*(data+2) << 8) | (*(data+3));
    return rc;
}
    
static bool validate_target_flash_algorithm(void)
{
    // check flash algorithm enrty in target RAM
    if (flash.init < target_device.ram_start || flash.init > target_device.ram_end ) {
        return false;
    }
    if (flash.uninit < target_device.ram_start || flash.uninit > target_device.ram_end ) {
        return false;
    }
    if (flash.erase_chip < target_device.ram_start || flash.erase_chip > target_device.ram_end ) {
        return false;
    }
    if (flash.erase_sector < target_device.ram_start || flash.erase_sector > target_device.ram_end ) {
        return false;
    }
    if (flash.program_page < target_device.ram_start || flash.program_page > target_device.ram_end ) {
        return false;
    }  
    if ((flash.verify != 0) && (flash.verify < target_device.ram_start || flash.verify > target_device.ram_end )) {
        return false;
    } 
    // check flash algorithm run time setting
    if (flash.sys_call_s.breakpoint < target_device.ram_start || flash.sys_call_s.breakpoint > target_device.ram_end ) {
        return false;
    }
    if (flash.sys_call_s.stack_pointer < target_device.ram_start || flash.sys_call_s.stack_pointer > target_device.ram_end ) {
        return false;
    }
    if (flash.sys_call_s.static_base < target_device.ram_start || flash.sys_call_s.static_base > target_device.ram_end ) {
        return false;
    }    
    // check data buffer    
    if (flash.program_buffer < target_device.ram_start || flash.program_buffer > target_device.ram_end ) {
        return false;
    }
    // check algorithm self    
    if (flash.algo_start < target_device.ram_start || flash.algo_start > target_device.ram_end ) {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// Notice: HID max package is 64 bytes. payload is 62 bytes. 
//////////////////////////////////////////////////////////////////////
error_t target_flash_algorithm_open(const uint8_t *data, uint32_t size)
{
    if (size < 49) { // 1 + 12 * 4
        return ERROR_FAILURE; 
    }
    flash.hardware_reset_support = *data; data++;
    // copy flash algorithm api entry address
    flash.init         = readUint32BE(data);  data += 4;
    flash.uninit       = readUint32BE(data);  data += 4;
    flash.erase_chip   = readUint32BE(data);  data += 4;
    flash.erase_sector = readUint32BE(data);  data += 4;
    flash.program_page = readUint32BE(data);  data += 4;
    flash.verify       = readUint32BE(data);  data += 4;
    // copy flash algorithm stack and breakpoint setting
    flash.sys_call_s.breakpoint    = readUint32BE(data);  data += 4;
    flash.sys_call_s.static_base   = readUint32BE(data);  data += 4;
    flash.sys_call_s.stack_pointer = readUint32BE(data);  data += 4;
    // copy flash algorithm data buffer address
    flash.program_buffer = readUint32BE(data);  data += 4;
    // copy flash algorithm stored target address and it's length
    flash.algo_start = readUint32BE(data);  data += 4;
    flash.algo_size  = readUint32BE(data);  data += 4;
       
    // reset 
    flash_write_index = 0;
    
    return ERROR_SUCCESS;
}

error_t target_flash_algorithm_write(const uint8_t *data, uint32_t size)
{
    if (size%4 != 0) { // must word size
        return ERROR_FAILURE; 
    }
    
    for (uint32_t i = 0; i < size/4; i++) {
        flash_prog_blob[flash_write_index + i] = readUint32BE(data);
        data += 4;
    }
    flash_write_index += size/4;
    
    return ERROR_SUCCESS;
}

error_t target_flash_algorithm_close(void)
{
    // validate it
    if (validate_target_flash_algorithm() == true) {
        return ERROR_SUCCESS;
    } else {
        return ERROR_FAILURE;
    }
}


error_t target_flash_basic_configure(const uint8_t *data, uint32_t size)
{
    // check length
    if (size < 24) { // 6 * 4
        return ERROR_FAILURE;
    }
    // assign config data
    target_device.sector_size = readUint32BE(data); data += 4;
    target_device.sector_cnt  = readUint32BE(data); data += 4;
    target_device.flash_start = readUint32BE(data); data += 4;
    target_device.flash_end   = readUint32BE(data); data += 4;
    target_device.ram_start   = readUint32BE(data); data += 4;
    target_device.ram_end     = readUint32BE(data); data += 4;
       
    // reset
    target_device.erase_reset = 0;
    // reset sector_info, extra_flash, extra_ram
    target_device.sector_info_length = 0;
    for (uint32_t i = 0; i < 8; i++) {
         target_device.sectors_info[i].start = 0;
         target_device.sectors_info[i].size  = 0;    
    }
    for (uint32_t i = 0; i < MAX_EXTRA_FLASH_REGION + 1; i++) {
        target_device.extra_flash[i].start = 0;
        target_device.extra_flash[i].end   = 0;        
    }
    
    for (uint32_t i = 0; i < MAX_EXTRA_RAM_REGION + 1; i++) {
        target_device.extra_ram[i].start = 0;
        target_device.extra_ram[i].end   = 0;        
    }                       
    return ERROR_SUCCESS;    
}

error_t target_flash_advance_configure(const uint8_t *data, uint32_t size)
{   
    uint8_t type         =  (*data)       & 0x03; // bit [0--1]
    uint8_t items        =  (*(data + 1)) & 0x0F; // bit [0--3]
    data += 2;  //adjust

    // check length
    if (size < items * 2 + 2) {
        return ERROR_FAILURE;
    }
    if (type == FLASH_SECTION_TYPE) {
        // section
        items = items & 0x07; // max is 7
        for (uint32_t i = 0; i < items; i++) {
            target_device.sectors_info[i].start = readUint32BE(data); data += 4;
            target_device.sectors_info[i].size  = readUint32BE(data); data += 4;  
        }
        target_device.sector_info_length = items;
    } else if (type == EXTRA_FLASH_TYPE) {
        // extra flash
        items = items & 0x03; //max is 3
        for (uint32_t i = 0; i < items; i++) {
            target_device.extra_flash[i].start = readUint32BE(data); data += 4;
            target_device.extra_flash[i].end   = readUint32BE(data); data += 4;        
        }
    } else if (type == EXTRA_RAM_TYPE) {
        // extra RAM
        items = items & 0x03; //max is 3
        for (uint32_t i = 0; i < items; i++) {
            target_device.extra_ram[i].start = readUint32BE(data); data += 4;
            target_device.extra_ram[i].end   = readUint32BE(data); data += 4;        
        }
    } else {
       return ERROR_FAILURE; 
    }
    
    return ERROR_SUCCESS;     
}

