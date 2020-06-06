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
#include "esc.h"



ESC_e escs[4];


uint32_t previous_time = 0 ;
uint32_t time = 0 ;
uint32_t previous_time_loop = 0 ;
int main(void)
{
	HAL_Init();
	UART_init(UART2_ID,115200);
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
	printf("Demo ESC init\n");
	//Pin's setting for the escs
	escs[0].gpio = GPIOA ;
	escs[1].gpio = GPIOB ;
	escs[2].gpio = GPIOB ;
	escs[3].gpio = GPIOB ;
	escs[0].gpio_pin = GPIO_PIN_15 ;
	escs[1].gpio_pin = GPIO_PIN_3 ;
	escs[2].gpio_pin = GPIO_PIN_5 ;
	escs[3].gpio_pin = GPIO_PIN_4 ;
	escs[0].pulsation = 1000;
	escs[1].pulsation = 1000;
	escs[2].pulsation = 1000;
	escs[3].pulsation = 1000;

	ESC_init(escs, 4);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);




	while(1)	//boucle de tâche de fond
	{
		previous_time = SYSTICK_get_time_us();
		//HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
		//HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
		GPIOA->BSRR = GPIO_PIN_5;
		GPIOA->BSRR = (uint32_t) GPIO_PIN_5 << 16U;
		time = SYSTICK_get_time_us();
		if(previous_time > 4000000){
			escs[0].pulsation = 1400 ;
			escs[1].pulsation = 1400 ;
			escs[2].pulsation = 1400 ;
			escs[3].pulsation = 1400 ;
		}
		printf("dt = %d\n", HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN));
		while(SYSTICK_get_time_us() < previous_time_loop + 4000);
		previous_time_loop += 4000 ;

	}
}
