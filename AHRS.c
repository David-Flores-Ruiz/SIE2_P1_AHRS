/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"

BMI160_accelerometer_data_t g_int16data_acc;	// INT16 CONVERTIR A FLOAT
BMI160_gyroscope_data_t g_int16data_gyr;	// INT16 CONVERTIR A FLOAT

BMI160_float_acc_data_t g_float_accel;	// Ready float for Mahony!
BMI160_float_gyr_data_t g_float_gyros;	// Ready float for Mahony!

void data_acquisition_task(void * args)
{
	uint32_t seconds = 0;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	while (1) {
		seconds++;
		PRINTF("\rTime: %i seconds since reset\n", seconds);

		//read_gyros

		// sem

		PRINTF("Leyendo accel \n");
		g_int16data_acc = BMI160_I2C_Read_acc();	// Read IMU accelerometer
		PRINTF("AHRS  -  Accel en x: %i \n", g_int16data_acc.x);
		PRINTF("AHRS  -  Accel en y: %i \n", g_int16data_acc.y);
		PRINTF("AHRS  -  Accel en z: %i \n", g_int16data_acc.z);

		data_calibration_acc();	// Conversion acc to Float


		PRINTF("Leyendo gyros \n");
		g_int16data_gyr = BMI160_I2C_Read_gyr();	// Read IMU gyroscope
		PRINTF("AHRS  -  Gyros en x: %i \n", g_int16data_gyr.x);
		PRINTF("AHRS  -  Gyros en y: %i \n", g_int16data_gyr.y);
		PRINTF("AHRS  -  Gyros en z: %i \n", g_int16data_gyr.z);

		data_calibration_gyr();	// Conversion gyr to Float

		// sem


		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_IMU_SAMPLE_TIME));
	}
}

void data_calibration_acc(void) {
	const float factorConvert_accel = (float) acc_range / (float) acc_max_val;

	g_float_accel.x = (float) g_int16data_acc.x * factorConvert_accel;
	g_float_accel.y = (float) g_int16data_acc.y * factorConvert_accel;
	g_float_accel.z = (float) g_int16data_acc.z * factorConvert_accel;
	PRINTF("FLOAT -  Accel en x: %u \n", (unsigned) g_float_accel.x);
	PRINTF("FLOAT -  Accel en y: %u \n", (unsigned) g_float_accel.y);
	PRINTF("FLOAT -  Accel en z: %u \n", (unsigned) g_float_accel.z);
}

void data_calibration_gyr(void) {
	const float factorConvert_gyros = (float) gyr_range / (float) gyr_max_val;

	g_float_gyros.x = (float) g_int16data_gyr.x * factorConvert_gyros;
	g_float_gyros.y = (float) g_int16data_gyr.y * factorConvert_gyros;
	g_float_gyros.z = (float) g_int16data_gyr.z * factorConvert_gyros;
	PRINTF("FLOAT -  Gyros en x: %u \n", (unsigned) g_float_gyros.x);
	PRINTF("FLOAT -  Gyros en y: %u \n", (unsigned) g_float_gyros.y);
	PRINTF("FLOAT -  Gyros en z: %u \n", (unsigned) g_float_gyros.z);
}


