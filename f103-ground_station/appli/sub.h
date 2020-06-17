/*
 * sub.h
 *
 *  Created on: 11 juin 2020
 *      Author: Théo
 */

#ifndef SUB_H_
#define SUB_H_
#include "uart_lib.h"


typedef enum{
	WAIT_TRANSMIT,
	BUFFERING
}transmit_pc_states_e;

typedef struct{
	transmit_pc_states_e state ;
	uint8_t buffer[5]  ;
	uint8_t compteur ;
	uint8_t id  ;
	uint8_t data_size  ;
	uart_struct_e * uart ;
	uint8_t initialized ;
}transmit_t;

typedef struct {
	uint8_t send_angles ;
}receive_struct_t;



void sub_transmit(uint8_t c, uart_struct_e * uart, transmit_t * transmit, receive_struct_t * receive);
void sub_receive(uint8_t * octects, uint16_t len, receive_struct_t * receive);

#endif /* SUB_H_ */
