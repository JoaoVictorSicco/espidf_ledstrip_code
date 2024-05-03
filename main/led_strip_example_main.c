/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "led_functions.h"

#define DELAY 100

void app_main(void)
{
    ledstrip_init();
    change_color(VERMELHO);
    vTaskDelay(DELAY);
    change_color(AMARELO);
    vTaskDelay(DELAY);
    change_color(VERDE);
    vTaskDelay(DELAY);
    for(int i =0; i<3;i++)
    {
        turn_on_off_ledstrip(TURN_OFF);
        vTaskDelay(DELAY);
        turn_on_off_ledstrip(TURN_ON);
        vTaskDelay(DELAY);
    }
    
    while (1)
    {
        vTaskDelay(DELAY);
    }
}
