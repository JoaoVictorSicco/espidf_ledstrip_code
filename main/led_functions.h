#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"

/*
Definição de cores
Vermelho: 0°
Vermelho-alaranjado: 15°
Laranja: 30°
Laranja-amarelado: 45°
Amarelo: 60°
Amarelo-esverdeado: 75°
Verde-amarelado: 90°
Verde: 120°
Verde-azulado: 150°
Azul-verdeado: 165°
Azul: 180°
Azul-arroxeado: 210°
Arroxeado-azulado: 225°
Arroxeado: 240°
Arroxeado-avermelhado: 270°
Avermelhado-arroxeado: 285°
Vermelho: 300°
*/


#define TURN_ON                 (1)
#define TURN_OFF                (0)

#define AMARELO                (60)
#define VERDE                 (120)
#define VERMELHO                (0)

#define SATURACAO             (100)
#define INTENSIDADE            (10)


void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);
void turn_on_off_ledstrip(uint8_t status);
uint8_t change_color(uint8_t color);
void ledstrip_init();

#endif