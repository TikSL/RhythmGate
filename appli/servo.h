/*
 * servo.h
 *
 *  Created on: 1 mars 2023
 *      Author: Romain
 */

#ifndef SERVO_H_
#define SERVO_H_


/**
 * \fn SERVO_init (void)
 * \brief Fonction d'initialisation pour le servomoteur
 *
 * \param void
 * \return void
 */
void SERVO_init(void);


/**
 * \fn SERVO_ouvrir (void)
 * \brief Fonction definissant la position du servomoteur en position ouverte
 *
 * \param void
 * \return void
 */
void SERVO_ouvrir(void);


/**
 * \fn SERVO_fermer (void)
 * \brief Fonction definissant la position du servomoteur en position fermee
 *
 * \param void
 * \return void
 */
void SERVO_fermer(void);


#endif /* SERVO_H_ */
