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
		PRINTF("Accel en x: %d \n", g_data_acc.x);
		PRINTF("Accel en y: %d \n", g_data_acc.y);
		PRINTF("Accel en z: %d \n", g_data_acc.z);

		PRINTF("Leyendo gyros \n");
		g_data_gyr = BMI160_I2C_Read_gyr();
		PRINTF("Gyros en x: %d \n", g_data_gyr.x);
		PRINTF("Gyros en y: %d \n", g_data_gyr.y);
		PRINTF("Gyros en z: %d \n", g_data_gyr.z);

		// sem


		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_IMU_SAMPLE_TIME));
	}
}

