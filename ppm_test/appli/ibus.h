/*
 * ibus.h
 *
 *  Created on: 25 juil. 2020
 *      Author: Théo
 */

#ifndef IBUS_H_
#define IBUS_H_

#include "macro_types.h"
#include "stm32f1_uart.h"

typedef enum{
	LENGTH,
	COMMAND,
	CHANNELS
}state_ibus_t;

typedef struct{
	uart_id_e uart ;
	uint8_t buffer[32] ;
	uint16_t compteur ;
	state_ibus_t state ;
	uint32_t channels[14] ;

}ibus_t;




void IBUS_init(ibus_t * ibus, uart_id_e uart);
bool_e IBUS_check_data(ibus_t * ibus);

#endif /* IBUS_H_ */
