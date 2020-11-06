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
#include "stm32f103xb.h"


int main(void)
{
	HAL_Init();
	UART_init(UART3_ID,57600);
	SYS_set_std_usart(UART3_ID, UART3_ID, UART3_ID);

	//Activation de l horloge pour le timer 1 présent sur l'apb 2
	__HAL_RCC_TIM1_CLK_ENABLE();

	//Activation du pin
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, TRUE);
	//  GPIO_MODE_AF_PP GPIO_MODE_OUTPUT_PP

	//Changement du "output compare 1 mode" en pwm1 (pwm1 => OCxREF = TIM_CNT < TIM_CCRx )
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ;
	//Activation "preload register" (quand on change la valeur du ccr1, la valeur est d'abord bufferisé et aux prochain evenement, la nouvelle valeur prendra effet
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE ;

	//Active la sortie du timer sur les channels
	TIM1->BDTR |= TIM_BDTR_MOE ;

	//On active la sortie 1 tu timer 1
	TIM1->CCER |= TIM_CCER_CC1E ;

	//Paramètre le timer1
	TIM1->ARR = 50 ;
	TIM1->PSC = 63599 ;
	//Pramètre le OCR channel 1
	TIM1->CCR1 = 0;
	//Activation auto-reload preload
	TIM1->CR1 |= TIM_CR1_ARPE ;



	//réinitialise le compteur (CNT)
	TIM1->EGR |= TIM_EGR_UG ;

	//Lancement compteur
	TIM1->CR1 |= TIM_CR1_CEN ;



	while(1)	//boucle de tâche de fond
	{

		bool_e pin = (bool_e)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
		printf("%lu\t%lu\t%d\n", TIM1->CNT, TIM1->CCR1, pin);
		HAL_Delay(40);

	}
}












