/*
 * button.h
 *
 *  Created on: 4 avril 2023
 *      Author: Romain
 */

#ifndef BUTTON_H_
#define BUTTON_H_


/**
 * \enum button_event_organi_e
 * \brief constantes d'états du bouton organisateur
 */
typedef enum
{
	BUTTON_EVENT_NONE,				/*!< Pas d'appuis. */
	BUTTON_EVENT_SHORT_PRESS,		/*!< Un appui court. */
	BUTTON_EVENT_LONG_PRESS,		/*!< Double appui. */
	BUTTON_EVENT_DOUBLE_PRESS,		/*!< Un appui long. */
}button_event_organi_e;


/**
 * \enum button_event_joueur_e
 * \brief constantes d'états du bouton joueur
 */
typedef enum
{
	BUTTON_JOUEUR_EVENT_NONE,		/*!< Pas d'appuis. */
	BUTTON_JOUEUR_EVENT_PRESS,		/*!< Un appui court. */
}button_event_joueur_e;


/**
 * \fn BUTTON_init (void)
 * \brief Fonction d'initialisation pour les deux boutons
 *
 * \param void
 * \return void
 */
void BUTTON_init(void);


/**
 * \fn BUTTON_organi_state_machine (void)
 * \brief Machine d'états pour le boutton organisateur observant le nombre et la nature des clics bouton
 *
 * \param void
 * \return button_event_organi_e
 */
button_event_organi_e BUTTON_organi_state_machine(void);


/**
 * \fn BUTTON_joueur_state_machine (void)
 * \brief Machine d'états pour le boutton joueur observant le nombre et la nature des clics bouton
 *
 * \param void
 * \return button_event_joueur_e
 */
button_event_joueur_e BUTTON_joueur_state_machine(void);

#endif /* BUTTON_H_ */
