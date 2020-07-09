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
#include "Self_Balancing.h"



Self_Balancing_t robot ;

int main(void)
{
	HAL_Init();
		Self_Balancing_init(&robot);

	while(1)
	{
		Self_Balancing_procces_main(&robot);

	}
}
