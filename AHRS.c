/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"
#include <math.h>

#define AHRS_PRINTF_OFF 1
#define PRUEBA_CUBO_ESTATICO_PYTHON 1
//#define AHRS_ANGLES_UART_PRINTF_OFF 1
//#define VARIANZA 1

BMI160_accelerometer_data_t g_int16data_acc;	// INT16 CONVERTIR A FLOAT
BMI160_gyroscope_data_t g_int16data_gyr;	// INT16 CONVERTIR A FLOAT

BMI160_float_acc_data_t g_float_accel;	// Ready float for Mahony!
BMI160_float_gyr_data_t g_float_gyros;	// Ready float for Mahony!


void data_acquisition_task(void * args)
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while (1) {
		num_task++;
		PRINTF("\rACQUISITION_DATA Task_Number: %i\n", num_task);

		// sem

		g_int16data_acc = BMI160_I2C_Read_acc();	// Read IMU accelerometer
		g_int16data_gyr = BMI160_I2C_Read_gyr();	// Read IMU gyroscope

#ifndef AHRS_PRINTF_OFF
		PRINTF("AHRS  -  Accel en x: %i \n", g_int16data_acc.x);
		PRINTF("AHRS  -  Accel en y: %i \n", g_int16data_acc.y);
		PRINTF("AHRS  -  Accel en z: %i \n", g_int16data_acc.z);

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

void Ahrs_send_UART_angles_task(void *args)
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint8_t x=0;
	rtos_uart_config_t config;		// Configuration UART0
	config.baudrate = 115200;
	config.rx_pin = 16;
	config.tx_pin = 17;
	config.pin_mux = kPORT_MuxAlt3;
	config.uart_number = rtos_uart0;
	config.port = rtos_uart_portB;
	rtos_uart_init(config);

	while (1) {
		num_task ++;
		PRINTF("\r------ SEND_UART Task_Number: %i\n", num_task);

		MahonyAHRSEuler_t local_euler;
		comm_msg_t msg;

		/*La estructura para mensajes tiene un header según el documento de
		 la práctica debe de ser 0xAAAAAAAA*/
		msg.header = HEADER_VAL;

		// Función que retorna los ángulos: yaw, pitch y roll
		local_euler = MahonyAHRSupdateIMU(g_float_gyros.x, g_float_gyros.y, g_float_gyros.z,
									g_float_accel.x, g_float_accel.y, g_float_accel.z);

#ifndef VARIANZA
/*
 * Ecuacion de varianza tomada solo para 50 muestras
 *      ______________________________
 * =   /sumatoria(muestra - prom )^2
 * 	  /	__________________________
 * 	\/			N-1
 */

//////////7//probablemente debe de ir fuera del while/////////////////
		uint8_t temp_x [50] = {0};
		uint8_t temp_y [50] = {0};
		uint8_t temp_z [50] = {0};
		static uint8_t cont = 0;
		uint8_t suma_x = 0, suma_y = 0, suma_z = 0;
		uint8_t suma_x_prom = 0, suma_y_prom = 0, suma_z_prom = 0;
		uint8_t var_x,var_y,var_z;
		uint8_t control_suma = 0;
//////////7//probablemente debe de ir fuera del while/////////////////

		if(cont<50){
			msg.x = local_euler.yaw;
			msg.y = local_euler.pitch;
			msg.z = local_euler.roll;
			temp_x[cont]=local_euler.yaw;
			temp_y[cont]=local_euler.pitch;
			temp_z[cont]=local_euler.roll;
			cont++;
		}else {
///////////////Calculo de promedio  INICIO///////////////////
			//Calculo de la suma de los 50 valores tomados
			for (control_suma=0;control_suma++;control_suma=50)
			{
				suma_x=suma_x+temp_x[control_suma];
				suma_y=suma_y+temp_y[control_suma];
				suma_z=suma_z+temp_z[control_suma];
			}
			//Calculo de la division que calcula el promedio de las 50 muestras
			suma_x_prom=suma_x/control_suma;
			suma_y_prom=suma_y/control_suma;
			suma_z_prom=suma_z/control_suma;
///////////////Calculo de promedio  FIN///////////////////
///////////////Calculo varianza   INCIO///////////////////
			for (control_suma=0;control_suma++;control_suma=50)
			{
				temp_x[control_suma]=temp_x[control_suma]-suma_x_prom;
				temp_y[control_suma]=temp_y[control_suma]-suma_y_prom;
				temp_z[control_suma]=temp_z[control_suma]-suma_z_prom;

				temp_x[control_suma]=temp_x[control_suma]*temp_x[control_suma];
				temp_y[control_suma]=temp_y[control_suma]*temp_y[control_suma];
				temp_z[control_suma]=temp_z[control_suma]*temp_z[control_suma];

				var_x=var_x+temp_x[control_suma];
				var_y=var_y+temp_y[control_suma];
				var_z=var_z+temp_z[control_suma];
			}
			var_x=sqrt(var_x/50);
			var_y=sqrt(var_y/50);
			var_z=sqrt(var_z/50);
			msg.x = var_x;
			msg.y = var_y;
			msg.z = var_z;
			cont=0;
		}
///////////////Calculo varianza   FIN///////////////////




#endif

#ifndef PRUEBA_CUBO_ESTATICO_PYTHON
		msg.x = local_euler.yaw;
		msg.y = local_euler.pitch;
		msg.z = local_euler.roll;


		msg.x = 30.0;
		msg.y = 40.0;
		msg.z = 50.0;
#endif

#ifndef AHRS_ANGLES_UART_PRINTF_OFF
//		PRINTF("SEND_UART  -  HEADER: %u \n", msg.header);
		PRINTF("SEND_UART  -  Angle YAW:   %u \n", (unsigned) msg.x);
		PRINTF("SEND_UART  -  Angle PITCH: %u \n", (unsigned) msg.y);
		PRINTF("SEND_UART  -  Angle ROLL:  %u \n", (unsigned) msg.z);
#endif

		rtos_uart_send(rtos_uart0, (uint8_t*) &msg, sizeof(msg));
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_SENDUART_SAMPLE_TIME));
	}
}

void data_calibration_acc(void) {
	const float factorConvert_accel = (float) acc_range / (float) acc_max_val;

	g_float_accel.x = (float) g_int16data_acc.x * factorConvert_accel;
	g_float_accel.y = (float) g_int16data_acc.y * factorConvert_accel;
	g_float_accel.z = (float) g_int16data_acc.z * factorConvert_accel;

#ifndef AHRS_PRINTF_OFF
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

#ifndef AHRS_PRINTF_OFF
	PRINTF("FLOAT -  Gyros en x: %u \n", (unsigned) g_float_gyros.x);
	PRINTF("FLOAT -  Gyros en y: %u \n", (unsigned) g_float_gyros.y);
	PRINTF("FLOAT -  Gyros en z: %u \n", (unsigned) g_float_gyros.z);
#endif
}
