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


static const uint32_t nRF51822AA_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,

    /*0x020*/ 0x47702000, 0x47702000, 0x4c26b570, 0x60602002, 0x60e02001, 0x68284d24, 0xd00207c0L, 0x60602000, 
    /*0x040*/ 0xf000bd70L, 0xe7f6f82cL, 0x4c1eb570, 0x60612102, 0x4288491e, 0x2001d302, 0xe0006160L, 0x4d1a60a0, 
    /*0x060*/ 0xf81df000L, 0x7c06828, 0x2000d0fa, 0xbd706060L, 0x4605b5f8, 0x4813088e, 0x46142101, 0x4f126041, 
    /*0x080*/ 0xc501cc01L, 0x7c06838, 0x1e76d006, 0x480dd1f8, 0x60412100, 0xbdf84608L, 0xf801f000L, 0x480ce7f2, 
    /*0x0A0*/ 0x6006840, 0xd00b0e00L, 0x6849490a, 0xd0072900L, 0x4a0a4909, 0xd00007c3L, 0x1d09600a, 0xd1f90840L, 
    /*0x0C0*/ 0x4770, 0x4001e500, 0x4001e400, 0x10001000, 0x40010400, 0x40010500, 0x40010600, 0x6e524635, 
    /*0x0E0*/ 0x0, 
};

const program_target_t NRF51_flash = {
    0x20000021, // Init
    0x20000025, // UnInit
    0x20000029, // EraseChip
    0x20000049, // EraseSector
    0x20000071, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x20000001,             // breakpoint location
      0x20000020+0x00000150,  // static_base
      0x20001000              // stack_pointer
    },
    0x20000200,               // program_buffer
    0x20000000,               // algo_start
    0x00000150,               // algo_size
    nRF51822AA_FLM,           // image
    512,                       // ram_to_flash_bytes_to_be_written
    1024,                       // flash sector size
    1024,                       // auto increment page size
    0x00000000                 // flash base address
};

uint32_t nrf51_GetSecNum (uint32_t addr){
    return (addr >> 10);  
}

uint32_t nrf51_GetSecAddress(uint32_t sector){
    return (sector << 10);
}

uint32_t nrf51_GetSecLength(uint32_t sector){
    return 0x400; //1024
}
