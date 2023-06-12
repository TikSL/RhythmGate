/*
 * servo.h
 *
 *  Created on: 1 mars 2023
 *      Author: Romain
 */

#ifndef SERVO_H_
#define SERVO_H_

void SERVO_init(void);

void SERVO_set_position(uint16_t position);

uint16_t SERVO_get_position(void);

#endif /* SERVO_H_ */
