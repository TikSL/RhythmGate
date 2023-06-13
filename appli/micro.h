/*
 * micro.h
 *
 *  Created on: 5 mai 2023
 *      Author: Romain
 */

#ifndef MICRO_H_
#define MICRO_H_

/**
 * \fn MICRO_init (void)
 * \brief Fonction d'initialisation pour le microphone
 *
 * \param void
 * \return void
 */
void MICRO_init(void);


/**
 * \enum micro_event_joueur_e
 * \brief constantes d'�tats de verouillage apr�s lecture du micro
 */
typedef enum
{
	VEROUILLE,
	DEVEROUILLE,

}micro_event_joueur_e;


/**
 * \fn MICRO_joueur_state_machine (void)
 * \brief Machine d'�tats du microphone qui �coute les coups du joueurs et les compare au tableau de r�f�rence
 *
 * \param void
 * \return Situation DEVEROUILLE ou VEROUILLE
 */
micro_event_joueur_e MICRO_joueur_state_machine(void);


/**
 * \fn MICRO_enregistre_sequence (void)
 * \brief Fonction d'enregistrement de la s�quence, remplit le tableau password
 *
 * \param void
 * \return void
 */
void MICRO_enregistrement_sequence(void);


/**
 * \fn MICRO_detection_test (void)
 * \brief Fonction inutile dans le programme finale, a permis de commencer la d�tection par le micro
 *
 * \param void
 * \return void
 */
void Micro_detection_test(void);

#endif /* MICRO_H_ */
