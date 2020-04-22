/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"

void data_acquisition_task(void * args)
{
	uint32_t seconds = 0;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	BMI160_format_data_t local_accel, local_gyros;

	while (1) {
		seconds++;
		PRINTF("\rTime: %i seconds since reset\n",seconds);
		//read_acc
		//read_gyros

		// sem

		PRINTF("Leyendo accel \n");
		BMI160_I2C_Read_acc();

		PRINTF("Leyendo gyros \n");
		BMI160_I2C_Read_gyr();

		// sem


		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_IMU_SAMPLE_TIME));
	}
}

