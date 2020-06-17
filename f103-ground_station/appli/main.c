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
#include "stm32f1_mpu6050.h"
#include "complementary_filter.h"
#include "IDs.h"
#include "sub.h"
//Gestion temps, programme à 100hz
#define PERIODE_MAIN 10000	// Micros secs
unsigned long last_time_main = 0 ;

////MPU 6050 variables
MPU6050_t mpu_data ;
MPU6050_Result_t mpu_result ;
COMP_FILTER_angles_e mpu_angles ;

typedef enum{
	WAIT,
	UPDATE_ANGLES,
	SEND_ANGLES
}mae_low_lvl_e;
mae_low_lvl_e state_low_lvl = WAIT ;

uint8_t bytes[3];



int main(void)
{
	HAL_Init();
	uart_struct_e uart_pc ;
	uart_struct_e uart_drone ;
	uart_init(&uart_pc,UART_PC, BAUD_RATE_PC, 10);
	uart_init(&uart_drone,UART_DRONE, BAUD_RATE_DRONE, 10);

	//Transmit
	transmit_t transmit_drone ;
	transmit_drone.state = WAIT_TRANSMIT ;
	transmit_t transmit_pc;
	transmit_pc.state = WAIT_TRANSMIT ;

	//receive
	receive_struct_t receive ;

	//Sortie du printf sur le pc
	SYS_set_std_usart(UART_PC, UART_PC, UART_PC);

	//Init du mpu
	MPU6050_Init(&mpu_data, NULL, GPIO_PIN_12, MPU6050_Device_0, MPU6050_Accelerometer_16G, MPU6050_Gyroscope_500s);
	COMP_FILTER_init(&mpu_data, &mpu_angles,MPU6050_Accelerometer_16G, MPU6050_Gyroscope_500s, 0.995, 100 );


	receive.send_angles = 0 ;
	while(1)	//boucle de tâche de fond
	{

		switch(state_low_lvl){
			case WAIT :
				if(SYSTICK_get_time_us() > last_time_main + PERIODE_MAIN){
					last_time_main += PERIODE_MAIN ;
					state_low_lvl = UPDATE_ANGLES ;
				}
				break;
			case UPDATE_ANGLES :
				//read mpu data and appli the filter
				if(receive.send_angles){
					MPU6050_ReadAll(&mpu_data);
					COMP_FILTER_update_angles(&mpu_angles);
					state_low_lvl = SEND_ANGLES ;
					//printf("x\t%f\ty\t%f\n", mpu_angles.x, mpu_angles.y);
				}
				break;

			case SEND_ANGLES :
				if(receive.send_angles){
					bytes[0] = ID_DRONE_PITCH_ROLL_BASE ;
					bytes[1] = (uint8_t)(mpu_angles.x + 90);
					bytes[2] = (uint8_t)(mpu_angles.y + 90);
					uart_add_few(&uart_drone, bytes, 3);
				}
				state_low_lvl = WAIT ;
				break;

		}

		//Si on reçoit des données du drone
		if(UART_data_ready(UART_DRONE)){
			//On transmet au pc
			sub_transmit( UART_get_next_byte(UART_DRONE), &uart_pc, &transmit_pc, &receive);
		}
		//Si on reçoit des données du pc on transmet au drone
		if(UART_data_ready(UART_PC)){
			sub_transmit( UART_get_next_byte(UART_PC), &uart_drone, &transmit_drone, &receive);
		}
		uart_send(&uart_drone);
		uart_send(&uart_pc);

	}
}
