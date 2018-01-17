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


static const uint32_t nRF52832AA_FLM[] = {
              0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    
    /*0x020*/ 0x47702000, 0x47702000, 0x4c2bb570, 0x60202002, 0x20014929, 0x60083108, 0x68284d28, 0xd00207c0, 
    /*0x040*/ 0x60202000, 0xf000bd70, 0xe7f6f833, 0x4c22b570, 0x60212102, 0x2f10f1b0, 0x491fd303, 0x31102001, 
    /*0x060*/ 0x491de001, 0x60081d09, 0xf0004d1c, 0x6828f821, 0xd0fa07c0, 0x60202000, 0xe92dbd70, 0xf8df41f0, 
    /*0x080*/ 0x088e8058, 0x46142101, 0xf8c84605, 0x4f131000, 0xc501cc01, 0x07c06838, 0x1e76d007, 0x2100d1f8, 
    /*0x0A0*/ 0x1000f8c8, 0xe8bd4608, 0xf00081f0, 0xe7f1f801, 0x6800480b, 0x00fff010, 0x490ad00c, 0x29006809, 
    /*0x0C0*/ 0x4908d008, 0x31fc4a08, 0xd00007c3, 0x1d09600a, 0xd1f90840, 0x00004770, 0x4001e504, 0x4001e400, 
    /*0x0E0*/ 0x40010404, 0x40010504, 0x6e524635, 0x00000000, 
};


const program_target_t NRF52_flash = {
    0x20000021, // Init
    0x20000025, // UnInit
    0x20000029, // EraseChip
    0x2000004D, // EraseSector
    0x2000007B, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x20000001,             // breakpoint location
      0x20000020+0x000000F0,  // static_base
      0x20001000              // stack_pointer
    },
    0x20000200,               // program_buffer
    0x20000000,               // algo_start
    0x000000F0,               // algo_size
    nRF52832AA_FLM,           // image
    512,                       // ram_to_flash_bytes_to_be_written
    1024,                       // flash sector size
    1024,                       // auto increment page size
    0x00000000                 // flash base address
};

uint32_t nrf52_GetSecNum (uint32_t addr){
    return (addr >> 12);  
}

uint32_t nrf52_GetSecAddress(uint32_t sector){
    return (sector << 12);
}

uint32_t nrf52_GetSecLength(uint32_t sector){
    return 0x1000; //4096
}

