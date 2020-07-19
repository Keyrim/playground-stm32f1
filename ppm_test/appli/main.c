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
#include "stm32f1_extit.h"
#include "ppm.h"


#define NB_CHANNEL 8
uint16_t channels [NB_CHANNEL] ;
//Branchement ppm


//Branchements ppm
#define GPIO_PPM GPIOB
#define GPIO_PIN_PPM GPIO_PIN_12
#define PIN_NUMBER 12



uint32_t previous_time = 0 ;
uint32_t delta = 0 ;

uint32_t previous_time_loop = 0 ;


int main(void)
{

	HAL_Init();

	UART_init(UART2_ID,230400);
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
	printf("\nInit demo ppm\n");

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	PPM_init(channels, NB_CHANNEL, PIN_NUMBER, GPIO_PPM, GPIO_PIN_PPM, TRUE);









	while(1)	//boucle de tâche de fond
	{

		//printf(" %d %d\n",  channels[0], channels[2]);
		to_print();
		if(channels[4] > 1500){
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
		}else{
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
		}
//		while(SYSTICK_get_time_us() < previous_time_loop + 500);
//		previous_time_loop = SYSTICK_get_time_us();

	}
}
