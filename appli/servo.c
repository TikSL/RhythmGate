/*
 * servo.c
 *
 *  Created on: 12 juin 2023
 *      Author: Romain
 */


#include "config.h"
#include "stm32f1_timer.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"

#define PERIOD_TIMER 10 //ms

static const uint16_t SERVO_OUVERT = 50; // ATTENTION A LA VALEUR
static const uint16_t SERVO_FERME = 50; // ATTENTION A LA VALEUR

static uint16_t current_position;

void SERVO_init(void){
	//initialisation et lancement du timer1 à une période de 10 ms
	TIMER_run_us(TIMER1_ID, PERIOD_TIMER*1000, FALSE); //10000us = 10ms
	//activation du signal PWM sur le canal 1 du timer 1 (broche PA8)
	TIMER_enable_PWM(TIMER1_ID, TIM_CHANNEL_1, 150, FALSE, FALSE);
	//rapport cyclique reglé pour une position servo de 50%
	SERVO_set_position(50);
}

void SERVO_ouvrir(void){
	SERVO_set_position(SERVO_OUVERT);
}

void SERVO_fermer(void){
	SERVO_set_position(SERVO_FERME);
}

void SERVO_set_position(uint16_t position){
	if(position > 100){
		position = 100; //écretage si l'utilisateur demande plus de 100%
		}
	current_position = position ;

	TIMER_set_duty(TIMER1_ID, TIM_CHANNEL_1, position + 100);
}

uint16_t SERVO_get_position(void){
	return current_position;
}
