/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "MAE.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "stm32f1_extit.h"
#include "../lib/lib_perso\ppm.h"
#include "branchement.h"
#include "settings.h"
#include "../lib/lib_perso\esc.h"
#include "stm32f1_mpu6050.h"
#include "../lib/lib_perso\complementary_filter.h"
#include "../lib/lib_perso\pid.h"
#include "../lib/lib_perso\telemetrie.h"
#include "stm32f1_adc.h"
#include "sub/sub_action.h"
#include "../lib/lib_perso/GPS.h"
#include "WS2812S.h"
#include "../lib/lib_perso/state_machine.h"
#include "global.h"
#include "../lib/lib_perso/uart_lib.h"
#include "../lib/lib_perso/sequence_led.h"
#include "../ressources/sequences_led.h"
#include "system_d.h"


//	-------------------------- Global variables ----------------------------------



State_drone_t drone ;

//Security
#define SECURITY_IS_OK	drone.ppm_is_ok
#define EVERY_IS_OK ((uint8_t)( 2 * drone.ppm_is_ok +  drone.gps_is_ok))

//Uart
uart_struct_e uart_telem ;


//Gps
#define UART_GPS UART2_ID
uint32_t time_last_read_gps ;
nmea_frame_e nmea_frame ;


//Trucs
uint32_t compteur_loop = 0 ;

//Consigne depuis la base
double roll_consigne_base = 0 ;
double pitch_consigne_base = 0 ;

//Ppm
uint32_t time_last_read_ppm = 0 ;

//Main loop frequency setting
uint32_t previous_time_loop = 0 ;



//Pid variables
PID_t pid_roll ;
PID_t pid_pitch ;
PID_t pid_yaw ;


uint32_t time = 0 ;
uint32_t time_start = 0 ;
uint32_t time_delta = 0;
uint32_t time_end = 0 ;


High_Level_SM state_high_level = WAIT_LOOP ;


//Nos machines à état
DataSend_SM state_data_send = TO_SEND_1 ;


//Flight_Mode_SM state_flight_mode = MANUAL ;
Flight_Mode_SM previous_state_flight_mode = MANUAL_STYLEE ;
bool_e entrance_flight_mode = FALSE ;

//Sequence led d'etat
sequence_led_t led_etat ;

int main(void)
{
	//	-------------------------------------------- Setup -----------------------------------------------------------
	//Init pins
	HAL_Init();
	ADC_init();

	//Init mae drone
	drone.state_flight_mode = ON_THE_GROUND ;

	LED_SEQUENCE_init(&led_etat, GPIO_STATE_LED, GPIO_PIN_STATE_LED, SEQUENCE_LED_1, 200, 12, 1);
	//On laisse du temps à tout le monde pour bien démarer
	HAL_Delay(20);
	//------------------Init serial uart
	uart_init(&uart_telem, UART_TELEMETRIE, 57600, 10);
	SYS_set_std_usart(UART_TELEMETRIE, UART_TELEMETRIE, UART_TELEMETRIE);

	//Init du gps, on passe sur une fréquence de 5hz sur l'envoit de données et d'autre trucs
	GPS_congif(UART_GPS);

	//------------------Initialisation du port de la led Verte
	//BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Init led etat


	//------------------Init ruban de led
	//LED_MATRIX_init();
	//sub_clignotement_led(LED_MODE_1);

	//------------------Init du MPU et du complementary filer
	drone.mpu_result = MPU6050_Init(&drone.mpu_data, NULL, GPIO_PIN_12, MPU6050_Device_0, MPU6050_Accelerometer_16G, MPU6050_Gyroscope_500s);
	COMP_FILTER_init(&drone.mpu_data, &drone.mpu_angles,MPU6050_Accelerometer_16G, MPU6050_Gyroscope_500s, 0.998, 250 );

	//------------------Init ppm module
	PPM_init(drone.channels, PIN_NUMBER, GPIO_PPM, GPIO_PIN_PPM, TRUE, &time_last_read_ppm);

	//------------------Init pwm escs module
	drone.escs[0].gpio = esc0_gpio ;
	drone.escs[1].gpio = esc1_gpio ;
	drone.escs[2].gpio = esc2_gpio ;
	drone.escs[3].gpio = esc3_gpio ;
	drone.escs[0].gpio_pin = esc0_pin ;
	drone.escs[1].gpio_pin = esc1_pin ;
	drone.escs[2].gpio_pin = esc2_pin ;
	drone.escs[3].gpio_pin = esc3_pin ;
	drone.escs[0].pulsation = 1000;
	drone.escs[1].pulsation = 1000;
	drone.escs[2].pulsation = 1000;
	drone.escs[3].pulsation = 1000;
	ESC_init(drone.escs, 4);

	//Init pids
	PID_init(&pid_roll, kp_roll, ki_roll, kd_roll, 250, max_pid_output);
	PID_init(&pid_pitch, kp_pitch, ki_pitch, kd_pitch, 250, max_pid_output);
	PID_init(&pid_yaw, kp_yaw, ki_yaw, kd_yaw, 250, max_pid_output);


	HAL_Delay(100);




	//	--------------------------------------------- Main Loop	-----------------------------------------------------
	while(1)	//boucle de tâche de fond
	{
		switch(state_high_level){
			case WAIT_LOOP :
				//When we start a new loop, free_time is always set to zero, it can be modified by the called function

				time = SYSTICK_get_time_us();
				if(time >= previous_time_loop + LOOP_PERIODE){
					previous_time_loop += LOOP_PERIODE ;
					state_high_level = PWM_HIGH ;
					global.free_time = 0 ;	//Si c'est l'heure on passe direct au state suivant
				}
				else
					global.free_time = previous_time_loop + LOOP_PERIODE - time ;	//si on a du temps dispo, on le dit

				break;
			case PWM_HIGH:
				//Starting pwm signals for the escs
				ESC_Start_pulse();
				state_high_level = UPDATE_ANGLES ;
				break;
			case UPDATE_ANGLES :
				//read mpu data and appli the filter
				MPU6050_ReadAll(&drone.mpu_data);
				COMP_FILTER_update_angles(&drone.mpu_angles);
				state_high_level = VERIF_SYSTEM ;
				break ;

			case VERIF_SYSTEM :
				time = SYSTICK_get_time_us();
				//Verif si le ppm est toujours bien reçu
				if(time > time_last_read_ppm + TIME_OUT_PPM)
					drone.ppm_is_ok = FALSE ;
				else
					drone.ppm_is_ok = TRUE ;

				//Verif si le gps est ok
				if(time > time_last_read_gps + TIME_OUT_GPS)
					drone.gps_is_ok = FALSE ;
				else
					drone.gps_is_ok = TRUE ;

				state_high_level = PWM_LOW ;
				break;

			case PWM_LOW :
				state_high_level = try_action(ESC_End_pulse(), state_high_level, ESCS_SETPOINTS, ERROR_HIGH_LEVEL);
				break;

			case ESCS_SETPOINTS :



					//Détermine un changement de mode de vol
					entrance_flight_mode = drone.state_flight_mode != previous_state_flight_mode ;
					previous_state_flight_mode = drone.state_flight_mode ;
					switch(drone.state_flight_mode){

						case ON_THE_GROUND :
							if(entrance_flight_mode)
								LED_SEQUENCE_set_sequence(&led_etat, SEQUENCE_LED_1);

							//Si on est au sol on met la consigne des moteurs à zéro
							drone.escs[0].pulsation = PULSATION_MIN ;
							drone.escs[1].pulsation = PULSATION_MIN ;
							drone.escs[2].pulsation = PULSATION_MIN ;
							drone.escs[3].pulsation = PULSATION_MIN ;

							//Conditions de transitions
							if(drone.channels[SWITCH_1] > 1500 && SECURITY_IS_OK && (drone.v_bat > 10.8 || !USE_BATTERIE_CHECK)){	//Conditions générales
								if(drone.channels[THROTTLE] < 1100 && drone.channels[THROTTLE] > 1070){								//Pour passer en manuel
									drone.state_flight_mode = MANUAL ;
								}
							}


							break;

						case MANUAL :
							if(entrance_flight_mode)
								LED_SEQUENCE_set_sequence(&led_etat, SEQUENCE_LED_2);

							//Si on est en mode manuel pour les consigne c'est facile
							drone.roll_consigne = ((double)(drone.channels[ROLL]- 1500) / (double)500) * (double)roll_and_pitch_max_setpoint ;
							drone.pitch_consigne = ((double)(drone.channels[PITCH]- 1500) / (double)500) * (double)roll_and_pitch_max_setpoint ;
							drone.yaw_consigne = ((double)(drone.channels[YAW] - 1500) / (double)500) * (double)yaw_max_setpoint ;		//Consigne en degré par seconde attention ^^
							drone.throttle_consigne = (double)(drone.channels[THROTTLE] - 1050)  ;

							//Conditions de transitions
							if(drone.channels[SWITCH_1] < 1500 || drone.channels[THROTTLE] < 1070 || !SECURITY_IS_OK){		// Securité
								drone.state_flight_mode = ON_THE_GROUND ;
							}
							else if(drone.channels[SWITCH_2] > 1650)														//Passage en parachute
								drone.state_flight_mode = PARACHUTE ;
							else if(drone.channels[SWITCH_2] > 1350)														//Passage en manuel "stylé"
								drone.state_flight_mode = MANUAL_STYLEE ;

							break;
						case MANUAL_STYLEE:
							if(entrance_flight_mode){
								//TELEMETRIE_send_consigne_base(SUB_ID_BASE_CONSIGNE_START_SENDING_ANGLES, &uart_telem);
								LED_SEQUENCE_set_sequence(&led_etat, SEQUENCE_LED_4);
							}

							drone.yaw_consigne = ((double)(drone.channels[YAW] - 1500) / (double)500) * (double)yaw_max_setpoint ;	//Consigne en degré par seconde attention ^^
							drone.throttle_consigne = (double)(drone.channels[THROTTLE] - 1050)  ;
							drone.roll_consigne = roll_consigne_base ;
							drone.pitch_consigne = pitch_consigne_base ;

							//Conditions de transitions
							if(drone.channels[SWITCH_1] < 1500 || drone.channels[THROTTLE] < 1070 || !SECURITY_IS_OK){		// Securité
								drone.state_flight_mode = ON_THE_GROUND ;
							}
							else if(drone.channels[SWITCH_2] > 1650)														//Passage en parachute
								drone.state_flight_mode = PARACHUTE ;
							else if(drone.channels[SWITCH_2] < 1350 && drone.channels[SWITCH_2] > 1000 )														//Passage en manuel "stylé"
								drone.state_flight_mode = MANUAL ;
							break;
						case PARACHUTE :
							if(entrance_flight_mode)
								LED_SEQUENCE_set_sequence(&led_etat, SEQUENCE_LED_3);

							drone.throttle_consigne = (double)(drone.channels[THROTTLE] - 1050)  ;
							if(sub_parachute(&drone.roll_consigne, &drone.pitch_consigne, &drone.throttle_consigne, drone.mpu_angles.z_acc) != IN_PROGRESS){
								drone.state_flight_mode = ON_THE_GROUND ;
							}
							//Conditions de transitions
							if(drone.channels[SWITCH_1] < 1500 || drone.channels[THROTTLE] < 1070 || !SECURITY_IS_OK){		// Securité
								drone.state_flight_mode = ON_THE_GROUND ;
							}
							else if(drone.channels[SWITCH_2] > 1350 && drone.channels[SWITCH_2] < 1650)														//Passage en parachute
								drone.state_flight_mode = MANUAL_STYLEE ;
							else if(drone.channels[SWITCH_2] < 1350 && drone.channels[SWITCH_2] > 1000 )														//Passage en manuel "stylé"
								drone.state_flight_mode = MANUAL ;
							break;
						case POSITION_HOLD:
							//TODO Position hold function
							break;
						case ALTITUDE_HOLD:
							//TODO Altitude hold function
							break;
					}


					//If we fly, we compute our outputs, according to our targets, to keep the drone leveled
					if(drone.state_flight_mode){
						double roll_output 	= PID_compute(&pid_roll,- drone.roll_consigne + drone.mpu_angles.y, drone.mpu_angles.y);
						double pitch_output = PID_compute(&pid_pitch,- drone.pitch_consigne + drone.mpu_angles.x, drone.mpu_angles.x);
						double yaw_output	= PID_compute(&pid_yaw, - drone.yaw_consigne - drone.mpu_angles.z  , 0);

						ESC_Set_pulse(&drone.escs[0], (uint16_t)(1000 + drone.throttle_consigne + (int16_t)(- roll_output - pitch_output + yaw_output)));
						ESC_Set_pulse(&drone.escs[1], (uint16_t)(1000 + drone.throttle_consigne + (int16_t)(+ roll_output - pitch_output - yaw_output)));
						ESC_Set_pulse(&drone.escs[2], (uint16_t)(1000 + drone.throttle_consigne + (int16_t)(- roll_output + pitch_output - yaw_output)));
						ESC_Set_pulse(&drone.escs[3], (uint16_t)(1000 + drone.throttle_consigne + (int16_t)(+ roll_output + pitch_output + yaw_output)));
					}

				state_high_level = SEND_DATA ;
				break;

			case SEND_DATA :

				if(drone.channels[SWITCH_4] > 1500)
					sub_send_data(&uart_telem, drone);

				if((compteur_loop % 10) == 0)
					state_high_level = ONCE_EVERY_10 ;
				else
					state_high_level = END ;

				break;


			case ONCE_EVERY_10:
				//Mesure de la tension ne recquiert pas de se faire tout le temps
				drone.v_bat = (double)ADC_getValue(ADC_9) * BATTERIE_RESISTANCES_COEF ;
				state_high_level = END ;
				break;

			case END:
				compteur_loop ++ ;
				state_high_level = WAIT_LOOP ;
				break;
			case ERROR_HIGH_LEVEL :
				printf("error\n");
				break;
		}



		//Là on fait des choses en prenant en compte le "free time" : je crois ça fonctionne )
		if(global.free_time > 15){
			//Si on reçu des données du gps
			if(UART_data_ready(UART_GPS)){
				if(GPS_process_rx(UART_get_next_byte(UART_GPS), &drone.gps) >= 3){
					time_last_read_gps = SYSTICK_get_time_us() ;
				}
			}
			//global.free_time -= 5 ;

			//Si on a reçu des données de la base
			if(UART_data_ready(UART_TELEMETRIE)){
				sub_receive_data(UART_get_next_byte(UART_TELEMETRIE), &drone.state_flight_mode, &roll_consigne_base, &pitch_consigne_base);
			}
			uart_send(&uart_telem);

			//led d'etat
			LED_SEQUENCE_play(&led_etat);
		}

	}






}
