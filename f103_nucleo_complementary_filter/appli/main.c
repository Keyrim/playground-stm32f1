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
#include "stm32f1_mpu6050.h"
#include "complementary_filter.h"

MPU6050_t data ;
MPU6050_Result_t mpu_result ;
COMP_FILTER_angles_e mpu_angles ;
uint32_t previous_time =0 ;
uint32_t time = 0 ;
uint32_t loop_time = 0 ;

int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();


	//Initialisation de l'UART2 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsulée sur l'USB vers le PC de développement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le périphérique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
	printf("Init NUCLEO\n");


	mpu_result = MPU6050_Init(&data, NULL, GPIO_PIN_12, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_500s);
	printf("INIT mpu : %d\n", mpu_result);
	COMP_FILTER_init(&data, &mpu_angles,MPU6050_Accelerometer_8G, MPU6050_Gyroscope_500s, 0.998, 250 );

	for(int c = 0; c < 20; c++){
		//COMP_FILTER_calibration();
		//HAL_Delay(100);
	}


	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	//Systick_add_callback_function(&process_ms);



	while(1)	//boucle de tâche de fond
	{

		previous_time = SYSTICK_get_time_us() ;
		mpu_result = MPU6050_ReadAccelerometer(&data);
		mpu_result =MPU6050_ReadGyroscope(&data);

		time = SYSTICK_get_time_us() ;
		COMP_FILTER_update_angles();
		printf("X : %f Y : %f\n", data.Accelerometer_X, data.Accelerometer_Y);
		//Si quand on arrive le flag est déjà c'est que l'on à été trop lent

		//On repasse le flag à false

		if(mpu_angles.x > 0){
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
		}else{
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
		}



		//printf("%lu\n", time - previous_time);
		//printf("X %d\n", data.Accelerometer_X);
		while(SYSTICK_get_time_us() < loop_time + 4000  );

		loop_time = SYSTICK_get_time_us() ;

	}
}

