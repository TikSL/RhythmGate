/*
 * led.c
 *
 *  Created on: 6 avr. 2023
 *      Author: Romain
 */

#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
#include "systick.h"
#include "led.h"
#include "../lib/bsp/MatrixLed/WS2812S.h"

#define LED_FLASH_PERIOD	1000
#define LED_FLASH_DURATION	500

static void LED_process_ms(void);
static volatile led_state_e led_state = LED_OFF;
static volatile uint32_t led_color = 0x000000;
static volatile uint32_t t_led= 0;

void LED_init(void){
	BSP_GPIO_PinCfg(LED_MATRIX_PORT_DATA, LED_MATRIX_PIN_DATA, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
	LED_set(LED_OFF, led_color);
	Systick_add_callback_function(&LED_process_ms);

}

void LED_set(led_state_e state, uint32_t color ){

	if(led_state != state){
		t_led = 0;
		led_state = state;
	}
	if(led_color != color){
		t_led = 0;
		led_color = color;
	}
}

static void LED_process_ms(void)
{
	switch(led_state)
		{
			case LED_BLINK:
				t_led++;
				if(t_led==LED_FLASH_DURATION)
				{
					LED_MATRIX_display_full(led_color, 1);
				}
				else if(t_led>LED_FLASH_PERIOD)
				{
					LED_MATRIX_display_full(Color[0], 1);
					t_led = 0;
				}
				break;
			case LED_OFF:
				LED_MATRIX_display_full(Color[0], 1);
				break;
			case LED_ON:
				LED_MATRIX_display_full(led_color, 1);
				break;
		}
}
