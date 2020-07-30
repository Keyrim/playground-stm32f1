/*
 * sub_free_time.c
 *
 *  Created on: 29 juil. 2020
 *      Author: Théo
 */
#include "sub_action.h"
#include "branchement.h"
#include "systick.h"







void sub_free_time(State_drone_t * drone, State_base_t * base){
	if(drone->soft.dead_line){

		uint32_t free_time = drone->soft.dead_line - SYSTICK_get_time_us() ;
		static int32_t state = 0 ;

		static uint32_t ms5611_time_needed = 60 ;
		switch(state){
			case 0 :
				IBUS_check_data(&drone->communication.ibus);
				state = 1 ;
				break;
			case 1 :
				if(free_time >= ms5611_time_needed){
					ms5611_time_needed = sub_ms5611(drone);
				}
				state = 2 ;
				break;



			case 2 :
				//Si on a reçu des données de la base
				if(UART_data_ready(UART_TELEMETRIE)){
					sub_receive_data(UART_get_next_byte(UART_TELEMETRIE), drone, base);
				}
				state = 3 ;
				break;

			case 3 :
				uart_send(&drone->communication.uart_telem);
				state = 4 ;
				break;
			case 4 :
				//led d'etat
				LED_SEQUENCE_play(&drone->ihm.led_etat);
				state = 0 ;  //skip le gps
				break ;
			case 5 :
				//Si on reçu des données du gps
				if(UART_data_ready(UART_GPS)){
					uint8_t c = UART_get_next_byte(UART_GPS) ;
					if(DRONE_GPS_process_rx(c, &drone->capteurs.gps) >= 3){
						drone->capteurs.gps.last_time_read_gps = SYSTICK_get_time_us() ;
					}
					if(GPS_RELAY)
						uart_add_one(&drone->communication.uart_telem, c);
				}
				state = 0 ;
				break ;



		}









	}
}

