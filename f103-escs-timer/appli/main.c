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


//Timer lib
//#include "stm32f1xx_hal_tim.h"
#include"stm32f1_timer.h"



int main(void)
{
	HAL_Init();
	UART_init(UART1_ID,57600);
	SYS_set_std_usart(UART1_ID, UART1_ID, UART1_ID);




	while(1)	//boucle de tâche de fond
	{


	}
}
