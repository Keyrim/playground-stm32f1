/*
 * complementary_filer.h
 *
 *  Created on: 21 mars 2020
 *      Author: Theo
 */

#ifndef COMPLEMENTARY_FILTER_H_
#define COMPLEMENTARY_FILTER_H_

#include "stm32f1_mpu6050.h"
#include "math.h"


typedef struct{
	double x;
	double y;
	double z;
}COMP_FILTER_angles_e;

void COMP_FILTER_init(MPU6050_t * mpu_data_struct, COMP_FILTER_angles_e* angles, MPU6050_Accelerometer_t acc_sensi, MPU6050_Gyroscope_t gyro_sensi, double alpha_, uint16_t frequency);
void COMP_FILTER_update_angles(void);
bool_e COMP_FILTER_calibration(void);


#endif /* COMPLEMENTARY_FILTER_H_ */
