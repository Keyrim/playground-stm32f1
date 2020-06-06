/*
 * sub_test_gps.c
 *
 *  Created on: 5 juin 2020
 *      Author: Theo
 */
#include "sub.h"

void sub_test_gps(bool_e bloquant, bool_e init){
	gps_datas_t gps ;
	nmea_frame_e nmea_frame ;
	if(init){
		UART_init(UART_OUTPUT,57600);
		UART_init(UART_GPS, 9600);
		GPS_congif(UART_GPS);
		//"Indique que les printf sortent vers le périphérique UART2."
		SYS_set_std_usart(UART_OUTPUT, UART_OUTPUT, UART_OUTPUT);
	}
	while(bloquant){

		while(UART_data_ready(UART_GPS))
		{
			char c = UART_get_next_byte(UART_GPS) ;
			nmea_frame = GPS_process_rx(c, &gps);
			UART_putc(UART_OUTPUT, c);
			if(nmea_frame == TRAME_GPRMC )
				printf("%lf, %lf\n",gps.latitude_deg, gps.longitude_deg);	//On affiche les coordonnées lues

		}
		while(UART_data_ready(UART_OUTPUT))
		{
			char c = UART_get_next_byte(UART_OUTPUT) ;
			UART_putc(UART_GPS, c);
		}
	}
}
