/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"

BMI160_accelerometer_data_t g_data_acc;	// INT16 CONVERTIR A FLOAT
BMI160_gyroscope_data_t g_data_gyr;	// INT16 CONVERTIR A FLOAT

void data_acquisition_task(void * args)
{
	uint32_t seconds = 0;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	BMI160_float_acc_data_t local_accel;
	BMI160_float_gyr_data_t local_gyros;

	while (1) {
		seconds++;
		PRINTF("\rTime: %i seconds since reset\n",seconds);
		//read_acc
		//read_gyros

		// sem

		PRINTF("Leyendo accel \n");
		g_data_acc = BMI160_I2C_Read_acc();
		PRINTF("AHRS  -  Accel en x: %i \n", g_data_acc.x);
		PRINTF("AHRS  -  Accel en y: %i \n", g_data_acc.y);
		PRINTF("AHRS  -  Accel en z: %i \n", g_data_acc.z);

		PRINTF("Leyendo gyros \n");
		g_data_gyr = BMI160_I2C_Read_gyr();
		PRINTF("AHRS  -  Gyros en x: %i \n", g_data_gyr.x);
		PRINTF("AHRS  -  Gyros en y: %i \n", g_data_gyr.y);
		PRINTF("AHRS  -  Gyros en z: %i \n", g_data_gyr.z);

		// sem


		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_IMU_SAMPLE_TIME));
	}
}

