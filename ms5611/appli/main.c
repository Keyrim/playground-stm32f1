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
#include "hmc5883.h"

ms5611_t ms5611 ;
uint32_t previous = 0;
compas_struct_t compas ;
int main(void)
{

	HAL_Init();
	HAL_Delay(100);
	UART_init(UART3_ID,57600);
	SYS_set_std_usart(UART3_ID, UART3_ID, UART3_ID);
	printf("\r");

	//I2C available devices test
	I2C_Init(I2C1, 100000);

	for(int adresse = 0 ; adresse < 128; adresse++){
		printf("Adresse : %d\tResut : %d\n", adresse,I2C_IsDeviceConnected(I2C1, (uint8_t)(adresse << 1)));
		HAL_Delay( 30);
	}



	//Hmc
	//HMC5883_init(&compas, TRUE, operating_mode_continuous, output_rate_75_0, gain_configuration_1_9, sample_average_eight);


	//Compas
//	MS5611_get_calibration_values(&ms5611, TRUE);
//	for(uint8_t v = 0; v < 6; v++)
//			printf("\n%d",  ms5611.calibration_values[v]);
//
//	printf("\r");



	uint8_t state = 0 ;
	// 0 request temp
	// 1 read temp
	// 2 request pressure
	// 3 read pressure

	while(1)	//boucle de tâche de fond
	{

		//Compas
//		switch(state){
//			case 0:
//				MS5611_request_temp();
//				state ++ ;
//				break;
//
//			case 1:
//				MS5611_read_temp(&ms5611);
//				MS5611_calculate_temperature(&ms5611);
//				//printf("%ld\t", ms5611.temperature);
//				state ++;
//				break;
//			case 2:
//				MS5611_request_pressure();
//				state ++ ;
//				break;
//			case 3 :
//				MS5611_read_pressure(&ms5611);
//				MS5611_calculate_pressure(&ms5611);
//				MS5611_calculate_altitude(&ms5611);
//				printf("%f\r\n", ms5611.altitude);
//				state = 0;
//				break;
//		}

		//HMC5883_read(&compas);
		//printf("%d\n", compas.x_raw);

		while(SYSTICK_get_time_us() < previous + 15000);
		previous += 10000 ;

	}
}















