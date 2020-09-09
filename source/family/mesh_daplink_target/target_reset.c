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
#include "target_reset.h"
#include "target_config.h"
#include "swd_host.h"
#include "gpio.h"
#include "target_family.h"


uint8_t mesheven_target_set_state(TARGET_RESET_STATE state) {
    if (target_device.flash_algo->hardware_reset_support != 0) {
        return swd_set_target_state_hw(state);
    } else {
        return swd_set_target_state_sw(state);
    }
}


const target_family_descriptor_t g_mesheven_family  = {
    .family_id = kMesheven_FamilyID,
    .default_reset_type = kHardwareReset,
    .soft_reset_type = SYSRESETREQ,
    .target_set_state = mesheven_target_set_state,
};
