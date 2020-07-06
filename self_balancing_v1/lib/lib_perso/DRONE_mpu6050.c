/*
 * DRONE_mpu6050.c
 *
 *  Created on: 21 mars 2020
 *      Author: Theo
 */

#include "DRONE_mpu6050.h"

#include "systick.h"
#include "stdio.h"





//Private functions
static double absolu(double x);
static double absolu(double x){
	if( x > 0)
		return x ;
	else
		return -x ;
}




void DRONE_mpu6050_update_angles(DRONE_mpu6050_t * angles){

	//DRONE_mpu6050_t acc_angles ;
	angles->x_acc = (double)angles->raw_data_mpu.Accelerometer_Z / angles->acc_sensi ;
	angles->y_acc = (double)angles->raw_data_mpu.Accelerometer_Y / angles->acc_sensi ;
	angles->z_acc = (double)angles->raw_data_mpu.Accelerometer_X / angles->acc_sensi ;
	double acc_total = sqrt((angles->x_acc * angles->x_acc ) + (angles->y_acc * angles->y_acc) + (angles->z_acc * angles->z_acc));
	if(acc_total != 0  ){
		if(absolu(angles->x_acc) <= absolu(acc_total)){
			angles->x_acc_angle =   asin(angles->x_acc / acc_total ) * (double)57.32;

			//Permet de se repérer à peut près quand on à la tête à l'envers
			double pie =  180 ;
			if(angles->x_acc_angle < 0)
				pie = - 180 ;

			double angle_x_acc = pie - angles->x_acc_angle   ;
			if(absolu(angles->x_acc_angle - angles->y) > absolu(angle_x_acc - angles->y))
				 angles->x_acc_angle = angle_x_acc ;
		}

		if(absolu(angles->y_acc) <= absolu(acc_total)){
			angles->y_acc_angle = - asin(angles->y_acc / acc_total ) * (double)57.32;
			//Permet de se repérer à peut près quand on à la tête à l'envers
			double pie =  180 ;
			if(angles->y_acc_angle < 0)
				pie = - 180 ;

			double angle_y_acc = pie - angles->y_acc_angle   ;
			if(absolu(angles->y_acc_angle - angles->x) > absolu(angle_y_acc - angles->x))
				 angles->y_acc_angle = angle_y_acc ;
		}
	}

	//If it is the first reading we do, we initialize
	if(angles->first_read){
		angles->x = angles->x_acc_angle ;
		angles->y = angles->y_acc_angle ;
		angles->z = 0 ;
		angles->first_read = FALSE ;
	}
	//Otherwise, we use the gyro
	else{

		angles->x_gyro = (double)angles->raw_data_mpu.Gyroscope_Z / ((double)angles->frequency * angles->gyro_sensi );
		angles->y_gyro = (double)angles->raw_data_mpu.Gyroscope_Y / ((double)angles->frequency * angles->gyro_sensi );
		angles->z_gyro = (double)angles->raw_data_mpu.Gyroscope_X / ((double)angles->frequency * angles->gyro_sensi );

		angles->x += angles->x_gyro ;
		angles->y += angles->y_gyro  ;
		angles->z = (double)angles->frequency * angles->z_gyro ;		//On intégre pas car pour le yaw on bosse en deg/sec

		//Pour prendre en compte le transfert d'angle quand je fais une rotation sur le yaw
		angles->x -= sin(angles->z_gyro * 0.017) * angles->y ;
		angles->y += sin(angles->z_gyro * 0.017) * angles->x ;

		//Complementary filter
		//acc_x used with gyY makes sense dw (it really does btw)
		angles->x = angles->alpha * angles->x + (angles->y_acc_angle ) * ((double)1 - angles->alpha);
		angles->y = angles->alpha * angles->y + (angles->x_acc_angle ) * ((double)1 - angles->alpha);

	}
}

void DRONE_mpu6050_init(DRONE_mpu6050_t * angles, MPU6050_Accelerometer_t acc, MPU6050_Gyroscope_t gyro, double alpha, uint16_t frequency){
	//Init du mpu
	MPU6050_Init(&angles->raw_data_mpu, NULL, GPIO_PIN_12, MPU6050_Device_0,acc, gyro);

	angles->alpha = alpha ;
	angles->frequency = frequency ;
	switch(acc){
		case MPU6050_Accelerometer_16G :
			angles->acc_sensi = MPU6050_ACCE_SENS_16 ;
			break;
		case MPU6050_Accelerometer_8G :
			angles->acc_sensi = MPU6050_ACCE_SENS_8 ;
			break;
		case MPU6050_Accelerometer_4G :
			angles->acc_sensi = MPU6050_ACCE_SENS_4 ;
			break;
		case MPU6050_Accelerometer_2G :
			angles->acc_sensi = MPU6050_ACCE_SENS_2 ;
			break;
	}
	switch(gyro){
		case MPU6050_Gyroscope_2000s :
			angles->gyro_sensi = MPU6050_GYRO_SENS_2000 ;
			break;
		case MPU6050_Gyroscope_1000s :
			angles->gyro_sensi = MPU6050_GYRO_SENS_1000 ;
			break;
		case MPU6050_Gyroscope_500s :
			angles->gyro_sensi = MPU6050_GYRO_SENS_500 ;
			break;
		case MPU6050_Gyroscope_250s :
			angles->gyro_sensi = MPU6050_GYRO_SENS_250 ;
			break;
	}

}



