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


static const uint32_t LPC11U35_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    
	0x47700b00, 0x2200483b, 0x21016302, 0x63426341, 0x63816341, 0x20024937, 0x70083940, 0x47704610,
	0x47702000, 0xb08bb5f0, 0x25002032, 0x466a260f, 0xa905c261, 0x460f4c30, 0x47a04668, 0x28009805,
	0x2034d10a, 0xc1614669, 0x9003482c, 0x46684639, 0x980547a0, 0xd0002800, 0xb00b2001, 0xb570bdf0,
	0x0b04b08a, 0xa9052032, 0x4d239000, 0x460e9401, 0x46689402, 0x980547a8, 0xd10b2800, 0x90002034,
	0x9003481e, 0x94029401, 0x46684631, 0x980547a8, 0xd0002800, 0xb00a2001, 0xb5f0bd70, 0x0b004604,
	0x460eb08b, 0x91002132, 0x90029001, 0x4f12a905, 0x46684615, 0x47b8910a, 0x28009805, 0x2033d117,
	0x02360a36, 0xc1714669, 0x9004480c, 0x990a4668, 0x980547b8, 0xd10a2800, 0x46692038, 0x4807c171,
	0x46689004, 0x47b8990a, 0x28009805, 0x2001d0b5, 0x0000e7b3, 0x40048040, 0x1fff1ff1, 0x00002ee0,
	0x00000000, 
};

const program_target_t lpc11u35_flash = {
    0x10000025, // Init
    0x10000041, // UnInit
    0x10000045, // EraseChip
    0x1000007F, // EraseSector
    0x100000BB, // ProgramPage
    0x00000000, // verify
    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    {
      0x10000001,             // breakpoint location
      0x10000120,  			  // static_base
      0x10001800              // stack_pointer
    },
    0x10000400,               // program_buffer
    0x10000000,               // algo_start
    0x00000124,               // algo_size
    LPC11U35_FLM,             // image
    256,                     // ram_to_flash_bytes_to_be_written
};

