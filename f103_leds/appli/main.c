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
#include "WS2812S.h"
#include "couleur.h"


#define NB_LEDS 6
#define NB_LED_RUBAN 46


#define FLAG_100_PERIODE 10
bool_e flag_100 = FALSE ;
uint32_t flag_100_counter = FLAG_100_PERIODE ;

uint64_t compteur = 0 ;


typedef struct{
	int16_t pos ;
	uint64_t periode ;
	uint32_t color ;
	int16_t step ;
}Light_e;

void Light_update_pos(Light_e * light){
	if(compteur % light->periode == 0)
		light->pos = light->pos + light->step ;
	if(light->pos > NB_LED_RUBAN)
		light->pos = 0 ;
	if(light->pos < 0)
		light->pos = NB_LED_RUBAN ;
}



Light_e lights[NB_LEDS] ;
static uint32_t leds_array[NB_LED_RUBAN] = {0} ;

void millis (void){
	if(flag_100_counter)
		flag_100_counter -- ;
	else{
		flag_100 = TRUE ;
		flag_100_counter = FLAG_100_PERIODE ;
	}

}
int main(void)
{
	HAL_Init();


	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	HAL_Delay(100);
	LED_MATRIX_init();
	Systick_add_callback_function(millis);

	//Light 1
	lights[0].pos = 0 ;
	lights[0].color = COLOR_GREEN ;
	lights[0].periode = 4 ;
	lights[0].step = -1 ;

	//Light 2
	lights[1].pos = 0 ;
	lights[1].color = COLOR_BLUE ;
	lights[1].periode = 15 ;
	lights[1].step = -1 ;

	//Light 3
	lights[2].pos = 24 ;
	lights[2].color = COLOR_RED ;
	lights[2].periode = 8 ;
	lights[2].step = 1 ;

	//Light 4
	lights[3].pos = 24 ;
	lights[3].color = COLOR_ORANGE ;
	lights[3].periode = 2 ;
	lights[3].step = -1 ;

	//Light 5
	lights[4].pos = 24 ;
	lights[4].color = COLOR_PURPLE ;
	lights[4].periode = 7 ;
	lights[4].step = 1 ;




	while(1)	//boucle de tâche de fond
	{
		if(flag_100){
			flag_100 = FALSE ;	//On abaisse le flag


			for(int16_t l = 0; l < NB_LEDS; l++){
				Light_update_pos(&lights[l]);
				leds_array[lights[l].pos] = lights[l].color ;
			}


			LED_MATRIX_display(leds_array, NB_LED_RUBAN);

			for(int16_t l = 0; l < NB_LEDS; l++)
				leds_array[lights[l].pos] = 0 ;


			Light_update_pos(&lights[0]);
			compteur ++ ;
		}




	}
}


















