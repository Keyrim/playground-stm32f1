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
#include "ibus.h"

#define NB_CHANNEL 9
uint16_t channels [NB_CHANNEL] ;
//Branchement ppm


//Branchements ppm
#define GPIO_PPM GPIOB
#define GPIO_PIN_PPM GPIO_PIN_12
#define PIN_NUMBER 12


//Ibus
#define UART_IBUS 	UART1_ID
#define UART_PC		UART2_ID

uint32_t previous_time = 0 ;
uint32_t delta = 0 ;

uint32_t previous_time_loop = 0 ;

ibus_t ibus ;


int main(void)
{

	HAL_Init();

	UART_init(UART_PC,115200);
	SYS_set_std_usart(UART_PC, UART_PC, UART_PC);

	//printf("\nInit demo ppm\n");
	printf("\n INIT demo ibus\n");

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//PPM_init(channels, NB_CHANNEL, PIN_NUMBER, GPIO_PPM, GPIO_PIN_PPM, TRUE);
	IBUS_init(&ibus, UART_IBUS);

	while(1)	//boucle de tâche de fond
	{
		IBUS_check_data(&ibus);
		if(SYSTICK_get_time_us() > previous_time_loop + 50000)
		{
			previous_time_loop = SYSTICK_get_time_us();
			for(uint8_t c = 0; c < 14; c++)
				printf("%lu\t", ibus.channels[c]);
			printf("\r\n");
		}


	}
}
//printf(" %d %d\n",  channels[0], channels[2]);
//		to_print();
//		if(channels[4] > 1500){
//			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
//		}else{
//			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
//		}
//		while(SYSTICK_get_time_us() < previous_time_loop + 500);
//		previous_time_loop = SYSTICK_get_time_us();
