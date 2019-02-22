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


void target_before_init_debug(void) {
    return;
}

uint8_t target_unlock_sequence(void) {
    return 1;
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}

uint8_t target_set_state(TARGET_RESET_STATE state) {
	if ((gpio_get_config(PIN_CONFIG_DT01) == PIN_HIGH) && (target_device.flash_algo->hardware_reset_support != 0)) {
        return swd_set_target_state_hw(state);
    } else {
        return swd_set_target_state_sw(state);        
    }           
}
