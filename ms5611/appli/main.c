/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "ms5611.h"

ms5611_t ms5611 ;
uint32_t previous = 0;
int main(void)
{

	HAL_Init();
	UART_init(UART3_ID,57600);
	SYS_set_std_usart(UART3_ID, UART3_ID, UART3_ID);
	printf("\r");


	MS5611_get_calibration_values(&ms5611, TRUE);
	for(uint8_t v = 0; v < 6; v++)
			printf("\n%d",  ms5611.calibration_values[v]);

	printf("\r");

	uint8_t state = 0 ;
	// 0 request temp
	// 1 read temp
	// 2 request pressure
	// 3 read pressure

	while(1)	//boucle de tâche de fond
	{
		switch(state){
			case 0:
				MS5611_request_temp();
				state ++ ;
				break;

			case 1:
				MS5611_read_temp(&ms5611);
				MS5611_calculate_temperature(&ms5611);
				//printf("%ld\t", ms5611.temperature);
				state ++;
				break;
			case 2:
				MS5611_request_pressure();
				state ++ ;
				break;
			case 3 :
				MS5611_read_pressure(&ms5611);
				MS5611_calculate_pressure(&ms5611);
				MS5611_calculate_altitude(&ms5611);
				printf("%f\r\n", ms5611.altitude);
				state = 0;
				break;
		}

		while(SYSTICK_get_time_us() < previous + 10000);
		previous += 10000 ;

	}
}















