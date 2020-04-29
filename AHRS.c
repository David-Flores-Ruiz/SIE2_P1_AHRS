/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"
#define PRINTF_OFF 1
#define PRUEBA 1
//#define CUBO_ESTATICO_PYTHON 1

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
		PRINTF("\rACQUI_DATA_Time: %i seconds since reset\n", seconds);

		// sem

		g_int16data_acc = BMI160_I2C_Read_acc();	// Read IMU accelerometer

#ifndef PRINTF_OFF
		PRINTF("AHRS  -  Accel en x: %i \n", g_int16data_acc.x);
		PRINTF("AHRS  -  Accel en y: %i \n", g_int16data_acc.y);
		PRINTF("AHRS  -  Accel en z: %i \n", g_int16data_acc.z);

		g_int16data_gyr = BMI160_I2C_Read_gyr();	// Read IMU gyroscope
		PRINTF("AHRS  -  Gyros en x: %i \n", g_int16data_gyr.x);
		PRINTF("AHRS  -  Gyros en y: %i \n", g_int16data_gyr.y);
		PRINTF("AHRS  -  Gyros en z: %i \n", g_int16data_gyr.z);
#endif

		data_calibration_acc();	// Conversion acc to Float
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
#ifndef PRINTF_OFF
	PRINTF("FLOAT -  Accel en x: %u \n", (unsigned) g_float_accel.x);
	PRINTF("FLOAT -  Accel en y: %u \n", (unsigned) g_float_accel.y);
	PRINTF("FLOAT -  Accel en z: %u \n", (unsigned) g_float_accel.z);
#endif
}

void data_calibration_gyr(void) {
	const float factorConvert_gyros = (float) gyr_range / (float) gyr_max_val;

	g_float_gyros.x = (float) g_int16data_gyr.x * factorConvert_gyros;
	g_float_gyros.y = (float) g_int16data_gyr.y * factorConvert_gyros;
	g_float_gyros.z = (float) g_int16data_gyr.z * factorConvert_gyros;
#ifndef PRINTF_OFF
	PRINTF("FLOAT -  Gyros en x: %u \n", (unsigned) g_float_gyros.x);
	PRINTF("FLOAT -  Gyros en y: %u \n", (unsigned) g_float_gyros.y);
	PRINTF("FLOAT -  Gyros en z: %u \n", (unsigned) g_float_gyros.z);
#endif
}

void Ahrs_send_UART_angles_task(void *args)
{
	uint32_t seconds = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	rtos_uart_config_t config;		// Configuration UART0
	config.baudrate = 115200;
	config.rx_pin = 16;
	config.tx_pin = 17;
	config.pin_mux = kPORT_MuxAlt3;
	config.uart_number = rtos_uart0;
	config.port = rtos_uart_portB;
	rtos_uart_init(config);

	while (1) {
		seconds += 5;
		PRINTF("\rSEND_UART_Time: %i seconds since reset\n", seconds);

		MahonyAHRSEuler_t local_euler;
		comm_msg_t msg;

#ifndef PRUEBA
		// Función que retorna los ángulos: yaw, pitch y roll
		local_euler = MahonyAHRSupdateIMU(g_float_gyros.x, g_float_gyros.y, g_float_gyros.z,
									g_float_accel.x, g_float_accel.y, g_float_accel.z);
#endif

		/*La estructura para mensajes tiene un header según el documento de
		 la práctica debe de ser 0xAAAAAAAA*/
		msg.header = HEADER_VAL;

#ifndef CUBO_ESTATICO_PYTHON
		msg.x = 30.0;
		msg.y = 40.0;
		msg.z = 50.0;
#endif

//#ifndef PRINTF_OFF
		PRINTF("SEND_UART  -  HEADER: %u \n", msg.header);
		PRINTF("SEND_UART  -  Angle YAW:   %u \n", (unsigned) msg.x);
		PRINTF("SEND_UART  -  Angle PITCH: %u \n", (unsigned) msg.y);
		PRINTF("SEND_UART  -  Angle ROLL:  %u \n", (unsigned) msg.z);
//#endif

		rtos_uart_send(rtos_uart0, (uint8_t*) &msg, sizeof(msg));

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_SENDUART_SAMPLE_TIME));
	}
}


