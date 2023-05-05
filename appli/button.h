/*
 * button.h
 *
 *  Created on: 4 avril 2023
 *      Author: Romain
 */

#ifndef BUTTON_H_
#define BUTTON_H_

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_SHORT_PRESS,
	BUTTON_EVENT_LONG_PRESS,
	BUTTON_EVENT_DOUBLE_PRESS,
}button_event_organi_e;

typedef enum
{
	BUTTON_JOUEUR_EVENT_NONE,
	BUTTON_JOUEUR_EVENT_PRESS,
}button_event_joueur_e;

void BUTTON_init(void);

button_event_organi_e BUTTON_organi_state_machine(void);
button_event_joueur_e BUTTON_joueur_state_machine(void);

#endif /* BUTTON_H_ */
