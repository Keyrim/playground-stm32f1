/*
 * esc.h
 *
 *  Created on: 23 mars 2020
 *      Author: Theo
 */

#ifndef ESC_H_
#define ESC_H_

#include "stm32f1_timer.h"
#define TIMER_ID_PWM TIMER1_ID

#define PERIODE_TIMER	10		//	Période de l'it du timer en µs
#define PERIODE_PWM 	4000	//	Période du signal pwm en µs, has to be a multiple of PERIODE_TIMER

typedef struct{
	uint16_t pulsation ;
	GPIO_TypeDef* gpio;
	uint16_t gpio_pin ;
}ESC_e;

void ESC_init(ESC_e* escs_, uint8_t nb_escs_);

#endif /* ESC_H_ */
