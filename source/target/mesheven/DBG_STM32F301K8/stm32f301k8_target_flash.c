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


static const uint32_t STM32F301K8_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    
    0x49554854, 0x48556048, 0x20006048, 0xb5104770, 0x20344603, 0x60e04c50, 0xbd102000, 0x20004601, 
    0xb5004770, 0x23002200, 0x6902484b, 0x0080f002, 0xd1012880, 0xffe4f7ff, 0x4847bf00, 0xf00368c3, 
    0x28000001, 0x4844d1f9, 0xf0426902, 0x61020204, 0xf0426902, 0x61020240, 0x483fbf00, 0xf00368c3, 
    0x28000001, 0x483cd1f9, 0xf0226902, 0x61020204, 0xbd002000, 0x4603b510, 0x24002200, 0x69024836, 
    0x0080f002, 0xd1012880, 0xffbaf7ff, 0x4832bf00, 0xf00468c4, 0x28000001, 0x482fd1f9, 0xf0426902, 
    0x61020202, 0x69026143, 0x0240f042, 0xbf006102, 0x68c44829, 0x0001f004, 0xd1f92800, 0x69024826, 
    0x0202f022, 0x20006102, 0xe92dbd10, 0x460545f0, 0x4617460e, 0x46902200, 0x462b4692, 0x481e463c, 
    0xf0026902, 0x28800080, 0xf7ffd101, 0xbf00ff89, 0xf8d04819, 0xf008800c, 0x28000001, 0xe01ed1f8, 
    0x69024815, 0x0201f042, 0x88206102, 0xbf008018, 0xf8d04811, 0xf008800c, 0x28000001, 0x8820d1f8, 
    0x42888819, 0x480cd007, 0xf0226902, 0x61020201, 0xe8bd2001, 0x1c9b85f0, 0xf10a1ca4, 0xebba0a01, 
    0xd3dd0f56, 0x69024804, 0x0201f022, 0x20006102, 0x0000e7ef, 0x45670123, 0x40022000, 0xcdef89ab, 
    0x00000000,  
};

const program_target_t stm32f301k8_flash = {
    0x2000002F, // Init
    0x2000003D, // UnInit
    0x20000043, // EraseChip
    0x20000095, // EraseSector
    0x200000EB, // ProgramPage
    0x00000000, // verify
    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x20000001,             // breakpoint location
      0x20000020+0x00000184,  // static_base
      0x20001000              // stack_pointer
    },
    0x20000400,               // program_buffer
    0x20000000,               // algo_start
    0x00000184,               // algo_size
    STM32F301K8_FLM,          // image
    2048,                   // ram_to_flash_bytes_to_be_written
    2048,                       // flash sector size: 2KB
    2048,                       // auto increment page size
    0x08000000                 // flash base address
};

uint32_t stm32f301k8_GetSecNum (uint32_t addr){
    uint32_t rc = ( (addr - 0x08000000) >> 11); 
    return rc;
}

uint32_t stm32f301k8_GetSecAddress(uint32_t sector){
    uint32_t rc = 0x08000000 + (sector << 11);
    return rc;
}

uint32_t stm32f301k8_GetSecLength(uint32_t sector){
    return 0x800;  //2048
}
