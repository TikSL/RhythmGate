/*
 * micro.h
 *
 *  Created on: 5 mai 2023
 *      Author: Romain
 */

#ifndef MICRO_H_
#define MICRO_H_


typedef enum
{
	VEROUILLE,
	DEVEROUILLE,

}micro_event_joueur_e;

micro_event_joueur_e MICRO_joueur_state_machine(void);
void MICRO_enregistrement_sequence(void)

void Micro_detection_test(void);

#endif /* MICRO_H_ */
