/**
  ******************************************************************************
  * @file    button.c
  * @author  Romain
  * @date    04-April-2023
  * @brief   button's functions related.
  ******************************************************************************
*/


#include "button.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "traces.h"

#define LONG_PRESS_DURATION	1000	// Un appui long dure 1s.
#define DOUBLE_PRESS_DURATION 500	// Un double appui est détecté si on appui de nouveau en moins d'une demi-seconde sur le bouton

static void process_ms_buttons(void);

static volatile bool_e flag_10ms;
//static volatile uint32_t t_player = 0;
static volatile uint32_t t_game_master = 0;
static bool_e initialized_player = FALSE;
static bool_e initialized_game_master = FALSE;
static uint32_t click_counter = 0;

void BUTTON_init(void)
{
	BSP_GPIO_PinCfg(BUTTON_ORGANI_GPIO, BUTTON_ORGANI_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(BUTTON_JOUEUR_GPIO, BUTTON_JOUEUR_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms_buttons);

	initialized_game_master = TRUE;
	initialized_player = TRUE;
}

static void process_ms_buttons(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1)%10;		//incrémentation de la variable t10ms (modulo 10 !)
	if(!t10ms)
		flag_10ms = TRUE;
	if(t_game_master)
		t_game_master--;
}

button_event_organi_e BUTTON_organi_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_game_master_e;

	static state_game_master_e state = INIT;

	button_event_organi_e ret = BUTTON_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized_game_master)
	{
//		flag_10ms = FALSE; // Remis dans la machine d'état du bouton joueur
		current_button = !HAL_GPIO_ReadPin(BUTTON_ORGANI_GPIO, BUTTON_ORGANI_PIN);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;
				click_counter = 0;
				break;
			case WAIT_BUTTON:
				if(current_button)
				{
					print_traces("[BUTTON Master] button pressed\n");
					t_game_master=LONG_PRESS_DURATION;
					click_counter++;
					state = BUTTON_PRESSED;
				}
				if (t_game_master < DOUBLE_PRESS_DURATION && click_counter == 1)
				{
					state = BUTTON_PRESSED;
				}
				break;
			case BUTTON_PRESSED:
				if(t_game_master==0)
				{
					print_traces("[BUTTON Master] long press event\n");
					ret = BUTTON_EVENT_LONG_PRESS;
					click_counter = 0;
					state = WAIT_RELEASE;
				}
				else if(!current_button && t_game_master > DOUBLE_PRESS_DURATION && click_counter==1)
				{
					state = WAIT_BUTTON;
				}
				else if(!current_button && t_game_master > DOUBLE_PRESS_DURATION && click_counter==2)
				{
					print_traces("[BUTTON Master] double press event\n");
					ret = BUTTON_EVENT_DOUBLE_PRESS;
					click_counter = 0;
					state = WAIT_BUTTON;
				}

				else if(!current_button && t_game_master < DOUBLE_PRESS_DURATION)
				{
					print_traces("[BUTTON Master] short press event\n");
					ret = BUTTON_EVENT_SHORT_PRESS;
					click_counter = 0;
					state = WAIT_BUTTON;
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{
					print_traces("[BUTTON Master] release button after long press\n");
					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;
				break;
		}
	}
	return ret;
}

button_event_joueur_e BUTTON_joueur_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_player_e;

	static state_player_e state = INIT;
	button_event_joueur_e ret = BUTTON_JOUEUR_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized_player)
	{
		flag_10ms = FALSE; // On baisse le flag de 10s ici
		current_button = !HAL_GPIO_ReadPin(BUTTON_JOUEUR_GPIO, BUTTON_JOUEUR_PIN);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;
				break;
			case WAIT_BUTTON:
				if(current_button)
				{
					print_traces("[BUTTON Player] button pressed\n");
					state = BUTTON_PRESSED;
				}
				break;
			case BUTTON_PRESSED:
				if(!current_button){
					print_traces("[BUTTON Player] Released by player\n");
					ret = BUTTON_JOUEUR_EVENT_PRESS;
					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;
			break;
		}
	}
	return ret;
}
