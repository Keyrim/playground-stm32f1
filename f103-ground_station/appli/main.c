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
#include "wiring.h"
#include "uart_lib.h"


int main(void)
{
	HAL_Init();
	uart_struct_e uart_pc ;
	uart_struct_e uart_drone ;
	uart_init(&uart_pc,UART_PC, BAUD_RATE_PC, 10);
	uart_init(&uart_drone,UART_DRONE, BAUD_RATE_DRONE, 10);


	//Sortie du printf sur le pc
	SYS_set_std_usart(UART_PC, UART_PC, UART_PC);




	while(1)	//boucle de tâche de fond
	{
		if(UART_data_ready(UART_DRONE)){
			uart_add_one(&uart_pc, UART_get_next_byte(UART_DRONE));
		}
		uart_send(&uart_pc);

	}
}
