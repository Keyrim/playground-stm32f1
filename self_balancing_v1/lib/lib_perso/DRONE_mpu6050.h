/*
 * complementary_filer.h
 *
 *  Created on: 21 mars 2020
 *      Author: Theo
 */

#ifndef DRONE_MPU6050_H_
#define DRONE_MPU6050_H_

#include "stm32f1_mpu6050.h"
#include "math.h"

typedef struct{
	double x;
	double y;
	double z;
	double x_acc;
	double y_acc;
	double z_acc;
	double x_gyro;
	double y_gyro;
	double z_gyro;
	double x_acc_angle;
	double y_acc_angle;
	double alpha;
	double gyro_sensi;
	double acc_sensi;
	bool_e first_read;
	bool_e mpu_is_ok;
	MPU6050_t raw_data_mpu ;
	MPU6050_Result_t mpu_result;
	uint16_t frequency;
}DRONE_mpu6050_t;

void DRONE_mpu6050_init(DRONE_mpu6050_t * angles, MPU6050_Accelerometer_t acc_sensi, MPU6050_Gyroscope_t gyro_sensi, double alpha, uint16_t frequency);
void DRONE_mpu6050_update_angles(DRONE_mpu6050_t * angles);


#endif /* COMPLEMENTARY_FILTER_H_ */
