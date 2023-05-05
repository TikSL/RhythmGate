/*
 * led.h
 *
 *  Created on: 6 avr. 2023
 *      Author: Romain
 */

#ifndef LED_H_
#define LED_H_

#include "config.h"
#if USE_MATRIX_LED
#if !(defined LED_MATRIX_PORT_DATA) || !defined(LED_MATRIX_PIN_DATA)
	#define LED_MATRIX_PORT_DATA	GPIOB
	#define LED_MATRIX_PIN_DATA		GPIO_PIN_14
#endif

typedef enum
{
	LED_OFF,
	LED_ON,
	LED_BLINK
}led_state_e;

void LED_init(void);

void LED_set(led_state_e led_state, uint32_t color);

static uint32_t Color[6] = {0x000000 , //MODE_INIT 				NOIR	0
							0x001100 , //MODE_VEROUILLE			ROUGE	1
							0x111111 , //MODE_DEVEROUILLE		BLANC	2
							0x110000 , //MODE_ENREGISTREMENT	VERT	3
							0x000011 , //MODE_JOUE_SEQUENCE		BLEU	4
							0x22BB22   //MODE_ECOUTE			ROSE	5
};

#endif
#endif /* LED_H_ */
