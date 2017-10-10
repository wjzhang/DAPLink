/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash_blob.h"
#include "swd_host.h"
#include "target_reset.h"
#include <stdint.h>
#include <debug_cm.h>
#include <RTL.h>


static const uint32_t STM32L486_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    
    0x6000f1a0, 0x47700ac0, 0x49544855, 0x49556001, 0x20006001, 0x49524770, 0x30f9f24c, 0x60083108, 
    0x494f4770, 0x30f9f24c, 0x60083108, 0x47702000, 0x47702000, 0xb5004a4a, 0x6810320c, 0xda012800, 
    0xffe2f7ff, 0x31084946, 0x03c06808, 0xf24cd4fc, 0x600830f9, 0xf2486810, 0x43180304, 0x68106010, 
    0x3080f440, 0x68086010, 0xd4fc03c0, 0x43986810, 0x20006010, 0xb530bd00, 0x24004b39, 0x4605330c, 
    0x28006818, 0xf7ffda01, 0x4a35ffbf, 0x68113208, 0xd4fc03c8, 0x30f9f24c, 0xf1a56010, 0x0ae86500, 
    0xd30228ff, 0x6400f44f, 0x6819b2c0, 0x75f8f640, 0x250243a9, 0x00c0eb05, 0x0004ea40, 0x0001ea40, 
    0x68186018, 0x3080f440, 0x68106018, 0xd4fc03c0, 0xf0206818, 0x60180002, 0xbd302000, 0x460eb5f0, 
    0x46032400, 0xd0010749, 0xe0002701, 0x4d1c2700, 0x6828350c, 0xda012800, 0xff86f7ff, 0x31084918, 
    0x03c06808, 0xf24cd4fc, 0x600830f9, 0x06d6eb07, 0xf040e01c, 0x60280001, 0x60186810, 0x03c06808, 
    0x6810d4fc, 0xf8d36857, 0xf8d3c000, 0xea80e004, 0xea87000c, 0x4338070e, 0x6828d005, 0x0001f020, 
    0x20016028, 0x3308bdf0, 0x1c643208, 0x42a66828, 0xf020d8df, 0x60280001, 0xbdf02000, 0x45670123, 
    0x40023c04, 0xcdef89ab, 0x00000000,
};

const program_target_t stm32l486_flash = {
    0x20000043, // Init
    0x20000051, // UnInit
    0x20000055, // EraseChip
    0x20000097, // EraseSector
    0x200000FD, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x20000001,             // breakpoint location
      0x20000020+0x0000018C,  // static_base
      0x20002000              // stack_pointer
    },
    0x20001000,               // program_buffer
    0x20000000,               // algo_start
    0x0000018C,               // algo_size
    STM32L486_FLM,            // image
    2048,                     // ram_to_flash_bytes_to_be_written
    2048,                     // flash sector size : 2KB
    2048,                     // auto increment page size
    0x08000000                // flash base address
};

uint32_t stm32l486_GetSecNum (uint32_t addr) {
	return (addr - 0x08000000) >> 11;
}

uint32_t stm32l486_GetSecAddress (uint32_t sector) {
	return (sector << 11) + 0x08000000;
}

uint32_t stm32l486_GetSecLength (uint32_t sector) {
	return 0x800;
}
