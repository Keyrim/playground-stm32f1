/*
 * sub_action.h
 *
 *  Created on: 19 avr. 2020
 *      Author: Theo
 */

#ifndef SUB_SUB_ACTION_H_
#define SUB_SUB_ACTION_H_

#include "macro_types.h"
#include "../lib_perso/uart_lib.h"
#include "../system_d.h"



typedef enum{
	LED_MODE_1,
	LED_MODE_ETEINT
}LedMode_e;

typedef struct {
	int8_t speed;
	uint8_t position;
	uint32_t color;

}Mooving_light;

//Sub pour les leds
void sub_clignotement_led(LedMode_e mode);

//Subs de données (télémétrie)
void sub_send_data(uart_struct_e * uart_telem , State_drone_t drone);
void sub_receive_data(uint8_t c, uint8_t * state_flying, double * consigne_roll_base, double * consigne_pitch_base);

//Sub autonomies
running_e sub_parachute();




#endif /* SUB_SUB_ACTION_H_ */
