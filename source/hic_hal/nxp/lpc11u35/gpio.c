/**
 * @file    gpio.c
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

#include "LPC11Uxx.h"
#include "RTL.h"
#include "gpio.h"
#include "compiler.h"
#include "target_reset.h"
#include "IO_Config.h"
#include "settings.h"
#include "iap.h"

static void busy_wait(uint32_t cycles)
{
    volatile uint32_t i;
    i = cycles;

    while (i > 0) {
        i--;
    }
}

void gpio_init(void)
{
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);
#if defined(TARGET_POWER_HOLD)
    // Target PowerHOLD port
    PIN_PWH_IOCON = PIN_PWH_IOCON_INIT;
    LPC_GPIO->CLR[PIN_PWH_PORT] = PIN_PWH;
    LPC_GPIO->DIR[PIN_PWH_PORT] |= PIN_PWH;
#endif
	//config the pins as input
    PIN_A0_IOCON |= PIN_A0_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A0_PORT] &= ~PIN_A0;
    PIN_A1_IOCON |= PIN_A1_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A1_PORT] &= ~PIN_A1;
    PIN_A2_IOCON |= PIN_A2_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A2_PORT] &= ~PIN_A2;
    PIN_A3_IOCON |= PIN_A3_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A3_PORT] &= ~PIN_A3;
    PIN_A4_IOCON |= PIN_A4_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A4_PORT] &= ~PIN_A4;
    PIN_A5_IOCON |= PIN_A5_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A5_PORT] &= ~PIN_A5;
    PIN_A6_IOCON |= PIN_A6_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A6_PORT] &= ~PIN_A6;
    PIN_A7_IOCON |= PIN_A7_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A7_PORT] &= ~PIN_A7;
    PIN_A8_IOCON |= PIN_A8_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A8_PORT] &= ~PIN_A8;
    PIN_A9_IOCON |= PIN_A9_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A9_PORT] &= ~PIN_A9;
    PIN_A10_IOCON |= PIN_A10_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A10_PORT] &= ~PIN_A10;
    PIN_A11_IOCON |= PIN_A11_IOCON_INIT;
    LPC_GPIO->DIR[PIN_A11_PORT] &= ~PIN_A11;
    // configure GPIO-LED as output
#if defined(CONTROLLED_POWER_LED)
    // Power led (red)
    PIN_POW_LED_IOCON = PIN_POW_LED_IOCON_INIT;
    LPC_GPIO->CLR[PIN_POW_LED_PORT] = PIN_POW_LED;
    LPC_GPIO->DIR[PIN_POW_LED_PORT] |= PIN_POW_LED;
#endif
    // DAP led (green)
    PIN_DAP_LED_IOCON = PIN_DAP_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    LPC_GPIO->DIR[PIN_DAP_LED_PORT] |= PIN_DAP_LED;
//    // MSD led (red)
//    PIN_MSD_LED_IOCON = PIN_MSD_LED_IOCON_INIT;
//    LPC_GPIO->SET[PIN_MSD_LED_PORT] = PIN_MSD_LED;
//    LPC_GPIO->DIR[PIN_MSD_LED_PORT] |= PIN_MSD_LED;
    // Serial LED (blue)
    PIN_CDC_LED_IOCON = PIN_CDC_LED_IOCON_INIT;
    LPC_GPIO->SET[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    LPC_GPIO->DIR[PIN_CDC_LED_PORT] |= PIN_CDC_LED;
    // configure Button(s) as input
//    PIN_RESET_IN_IOCON = PIN_RESET_IN_IOCON_INIT;
//    LPC_GPIO->DIR[PIN_RESET_IN_PORT] &= ~PIN_RESET_IN;
    // configure Forward Reset Button(s) as input
    PIN_RESET_IN_FWRD_IOCON = PIN_RESET_IN_FWRD_IOCON_INIT;
    LPC_GPIO->DIR[PIN_RESET_IN_FWRD_PORT] &= ~PIN_RESET_IN_FWRD;
#if !defined(PIN_nRESET_FET_DRIVE)
    // open drain logic for reset button
    PIN_nRESET_IOCON = PIN_nRESET_IOCON_INIT;
    LPC_GPIO->CLR[PIN_nRESET_PORT] = PIN_nRESET;
    LPC_GPIO->DIR[PIN_nRESET_PORT] &= ~PIN_nRESET;
#else
    // FET drive logic for reset button
    PIN_nRESET_IOCON = PIN_nRESET_IOCON_INIT;
    LPC_GPIO->CLR[PIN_nRESET_PORT] = PIN_nRESET;
    LPC_GPIO->DIR[PIN_nRESET_PORT] |= PIN_nRESET;
#endif
    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1 << 19) | (1 << 23) | (1 << 24));
    // Give the cap on the reset button time to charge
    busy_wait(10000);

    if (gpio_get_reset_btn() || config_ram_get_initial_hold_in_bl()) {
        IRQn_Type irq;
        // Disable SYSTICK timer and interrupt before calling into ISP
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

        // Disable all nvic interrupts
        for (irq = (IRQn_Type)0; irq < (IRQn_Type)32; irq++) {
            NVIC_DisableIRQ(irq);
            NVIC_ClearPendingIRQ(irq);
        }

        // If switching to "bootloader" mode then setup the watchdog
        // so it will exit CRP mode after ~30 seconds
        if (config_ram_get_initial_hold_in_bl()) {
            LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 15); // Enable watchdog module
            LPC_SYSCON->PDRUNCFG &= ~(1 << 6);      // Enable watchdog clock (WDOSC)
            LPC_SYSCON->WDTOSCCTRL = (0xF << 5);    // Set max frequency - 2.3MHz
            LPC_WWDT->CLKSEL = (1 << 0);            // Select watchdog clock
            LPC_WWDT->TC = 0x00FFFFFF;              // Set time to reset to ~29s
            LPC_WWDT->MOD = (1 << 0) | (1 << 1);    // Enable watchdog and set reset
            LPC_WWDT->FEED = 0xAA;                  // Enable watchdog
            LPC_WWDT->FEED = 0x55;
        }

        iap_reinvoke();
    }
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->CLR[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    } else {
        LPC_GPIO->SET[PIN_DAP_LED_PORT] = PIN_DAP_LED;
    }
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    if (state) {
        LPC_GPIO->CLR[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    } else {
        LPC_GPIO->SET[PIN_CDC_LED_PORT] = PIN_CDC_LED;
    }
}

//void gpio_set_msc_led(gpio_led_state_t state)
//{
//    if (state) {
//        LPC_GPIO->CLR[PIN_MSD_LED_PORT] = PIN_MSD_LED;
//    } else {
//        LPC_GPIO->SET[PIN_MSD_LED_PORT] = PIN_MSD_LED;
//    }


static uint32_t gpio_get_config0(void)
{
	return ((LPC_GPIO->PIN[PIN_A0_PORT] & PIN_A0) != 0x00)? PIN_HIGH:PIN_LOW;
}

static uint32_t gpio_get_config1(void)
{
	return ((LPC_GPIO->PIN[PIN_A1_PORT] & PIN_A1) != 0x00)? PIN_HIGH:PIN_LOW;
}

static uint32_t gpio_get_config2(void)
{
	return ((LPC_GPIO->PIN[PIN_A2_PORT] & PIN_A2) != 0x00)? PIN_HIGH:PIN_LOW;
}

static uint32_t gpio_get_config3(void)
{
	return ((LPC_GPIO->PIN[PIN_A3_PORT] & PIN_A3) != 0x00)? PIN_HIGH:PIN_LOW;
}

uint8_t gpio_get_config(uint8_t cfgid)
{
    uint8_t rc = 0;
	switch (cfgid)
	{
        case PIN_CONFIG_DT01:
            rc = gpio_get_config0();
            break;

		case PIN_CONFIG_1:
			rc = gpio_get_config1();
			break;

		case PIN_CONFIG_2:
			rc = gpio_get_config2();
            break;

		case PIN_CONFIG_3:
			rc = gpio_get_config3();
			break;
    }

	return rc;
}

uint16_t gpio_all_pins(void)
{
    uint16_t rc = 0;

    uint16_t a0 = ((LPC_GPIO->PIN[PIN_A0_PORT] & PIN_A0) != 0x00) ? 0x01 : 0x00;
    uint16_t a1 = ((LPC_GPIO->PIN[PIN_A1_PORT] & PIN_A1) != 0x00) ? 0x01 : 0x00;
    uint16_t a2 = ((LPC_GPIO->PIN[PIN_A2_PORT] & PIN_A2) != 0x00) ? 0x01 : 0x00;
    uint16_t a3 = ((LPC_GPIO->PIN[PIN_A3_PORT] & PIN_A3) != 0x00) ? 0x01 : 0x00;
    uint16_t a4 = ((LPC_GPIO->PIN[PIN_A4_PORT] & PIN_A4) != 0x00) ? 0x01 : 0x00;
    uint16_t a5 = ((LPC_GPIO->PIN[PIN_A5_PORT] & PIN_A5) != 0x00) ? 0x01 : 0x00;
    uint16_t a6 = ((LPC_GPIO->PIN[PIN_A6_PORT] & PIN_A6) != 0x00) ? 0x01 : 0x00;
    uint16_t a7 = ((LPC_GPIO->PIN[PIN_A7_PORT] & PIN_A7) != 0x00) ? 0x01 : 0x00;
    uint16_t a8 = ((LPC_GPIO->PIN[PIN_A8_PORT] & PIN_A8) != 0x00) ? 0x01 : 0x00;
    uint16_t a9 = ((LPC_GPIO->PIN[PIN_A9_PORT] & PIN_A9) != 0x00) ? 0x01 : 0x00;
    uint16_t a10 = ((LPC_GPIO->PIN[PIN_A10_PORT] & PIN_A10) != 0x00) ? 0x01 : 0x00;
    uint16_t a11 = ((LPC_GPIO->PIN[PIN_A11_PORT] & PIN_A11) != 0x00) ? 0x01 : 0x00;

    rc = a0 + (a1<<1) + (a2<<2) + (a3<<3) + (a4<<4) + (a5<<5) + (a6<<6) + (a7<<7) + (a8<<8) + (a9<<9) + (a10<<10) + (a11<<11);

    return rc;
}

uint8_t gpio_get_reset_btn_no_fwrd()
{
//    return LPC_GPIO->PIN[PIN_RESET_IN_PORT] & PIN_RESET_IN ? 0 : 1;
    return  0;
}

uint8_t gpio_get_reset_btn_fwrd()
{
    return LPC_GPIO->PIN[PIN_RESET_IN_FWRD_PORT] & PIN_RESET_IN_FWRD ? 0 : 1;
}

void gpio_set_board_power(bool powerEnabled)
{
}
