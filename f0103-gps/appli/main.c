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
#include "GPS.h"
#include "branchements.h"
#include "compas.h"
#include "sub.h"

Compass_t mon_compas ;

//Régulation de la periode
#define LOOP_PERIODE 200000	//En micro secondes
uint32_t last_loop = 0 ;

int main(void)
{
	HAL_Init();
	HAL_Delay(300); //Démarre par une pause pour que tout le monde démarre bien
	UART_init(UART_OUTPUT,57600);

	SYS_set_std_usart(UART_OUTPUT, UART_OUTPUT, UART_OUTPUT);

	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);


	//Init du compas
	//COMPAS_init(FALSE, &mon_compas);
	//I2C_Init(I2C1, 400000);
	//sub_test_i2c();
	sub_test_gps(TRUE, TRUE);
	while(1)	//boucle de tâche de fond
	{

		//Là on fait des trucs
//		COMPAS_read(&mon_compas);
//		printf("x %d\n", mon_compas.x);


		//Là on attends que le temps passe :)
		while(last_loop + LOOP_PERIODE > SYSTICK_get_time_us());
		last_loop += LOOP_PERIODE ;

	}
}
