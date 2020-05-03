/*
 * AHRS.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "AHRS.h"
#include "math.h"		// Para usar raíz cuadrada

#define AHRS_PRINTF_OFF 1
#define PRUEBA_CUBO_ESTATICO_PYTHON 1
#define PRUEBA_CUBO_TEMBLORINA 1
//#define AHRS_ANGLES_UART_PRINTF_OFF 1
//#define DESVIACION_ESTANDAR 1

#define N	3	// Número de muestras para calculo de la desviación estándar!
#define RANGE_X 1.5
#define RANGE_Y 0.5	// Una desv. estandar > significa que hubo movimiento!
#define RANGE_Z 1
#define FILTRO 20

BMI160_accelerometer_data_t g_int16data_acc;	// INT16 CONVERTIR A FLOAT
BMI160_gyroscope_data_t g_int16data_gyr;	// INT16 CONVERTIR A FLOAT

BMI160_float_acc_data_t g_float_accel;	// Ready float for Mahony!
BMI160_float_gyr_data_t g_float_gyros;	// Ready float for Mahony!
static uint8_t cont = 0;	// Guardar muestras
float temp_x[N] = { 0 };		// Xi
float temp_y[N] = { 0 };
float temp_z[N] = { 0 };

void data_acquisition_task(void * args)
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	MahonyAHRSEuler_t local_euler = { 0 };
	parameters_task_t parameters_task = *((parameters_task_t*)args);

	while (1) {
		xSemaphoreTake(parameters_task.mutex_ADQUISITION_freertos, portMAX_DELAY);
		num_task++;
		PRINTF("\rACQUISITION_DATA Task_Number: %i\n", num_task);

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


		// Función que retorna los ángulos: yaw, pitch y roll
		local_euler = MahonyAHRSupdateIMU(g_float_gyros.x, g_float_gyros.y, g_float_gyros.z,
									g_float_accel.x, g_float_accel.y, g_float_accel.z);

		if (cont < N) {
			temp_x[cont] = local_euler.yaw;
			temp_y[cont] = local_euler.pitch;
			temp_z[cont] = local_euler.roll;
			cont++;
		}

		if (cont == N) {
			xEventGroupSetBits(parameters_task.event_FreeRTOs, EVENT_MUESTRASREADY);
		}

		xSemaphoreGive(parameters_task.mutex_ADQUISITION_freertos);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(AHRS_IMU_SAMPLE_TIME));
	}
}

void Ahrs_send_UART_angles_task(void *args)
{
	uint32_t num_task = 0;
	parameters_task_t parameters_task = *((parameters_task_t*)args);

	rtos_uart_config_t config;		// Configuration UART0
	config.baudrate = 115200;
	config.rx_pin = 16;
	config.tx_pin = 17;
	config.pin_mux = kPORT_MuxAlt3;
	config.uart_number = rtos_uart0;
	config.port = rtos_uart_portB;
	rtos_uart_init(config);

	uint8_t control_suma = 0;		// Variance calculation variables
	float suma_x = 0, 		suma_y = 0,			suma_z = 0;			// SUMA
	float suma_x_prom = 0, 	suma_y_prom = 0,	suma_z_prom = 0;	// PROMEDIO

	float temp_diff_x[N] = { 0 };	// (Xi-prom)
	float temp_diff_y[N] = { 0 };
	float temp_diff_z[N] = { 0 };
	float temp_pow2_x[N] = { 0 };	// (Xi-prom)^2
	float temp_pow2_y[N] = { 0 };
	float temp_pow2_z[N] = { 0 };
	float temp_sumatoria_x = 0;		// SUMATORIA
	float temp_sumatoria_y = 0;
	float temp_sumatoria_z = 0;

	float var_x = 0, 		var_y = 0, 			var_z = 0;			// VARIANZA
	float desvEstan_x = 0, 	desvEstan_y = 0, 	desvEstan_z = 0;	// DESV. EST.
	static uint8_t flag = false;
	static comm_msg_t msg = { 0 };	// AL INICIO, UBICA EL CUBO EN 0's

	while (1) {
		xEventGroupWaitBits(parameters_task.event_FreeRTOs, EVENT_MUESTRASREADY, pdTRUE, pdTRUE, portMAX_DELAY);

		xSemaphoreTake(parameters_task.mutex_SEND_UART_freertos, portMAX_DELAY);

		num_task ++;
		PRINTF("\r------ SEND_UART Task_Number: %i\n", num_task);

		/*La estructura para mensajes tiene un header según el documento de
		 la práctica debe de ser 0xAAAAAAAA*/
		msg.header = HEADER_VAL;

//#ifndef DESVIACION_ESTANDAR
/*
 * Ecuacion de varianza tomada solo para N muestras
 *      ______________________________
 * =   /sumatoria(muestra - prom )^2
 * 	  /	__________________________
 * 	\/				N
 */
		if (cont == N) {
///////////////Calculo de promedio  INICIO///////////////////
			for (control_suma = 0; control_suma < N; control_suma++)
			{
				suma_x = suma_x + temp_x[control_suma];
				suma_y = suma_y + temp_y[control_suma];
				suma_z = suma_z + temp_z[control_suma];
			}
			suma_x_prom = suma_x / N;
			suma_y_prom = suma_y / N;
			suma_z_prom = suma_z / N;
///////////////Calculo de promedio  FIN///////////////////
///////////////Calculo varianza   INCIO///////////////////
			for (control_suma = 0; control_suma < N; control_suma++)
			{
				// ( Xi	- prom )
				temp_diff_x[control_suma] = (temp_x[control_suma] - suma_x_prom);
				temp_diff_y[control_suma] = (temp_y[control_suma] - suma_y_prom);
				temp_diff_z[control_suma] = (temp_z[control_suma] - suma_z_prom);
				// ( Xi	- prom )^2
				temp_pow2_x[control_suma] = temp_diff_x[control_suma] * temp_diff_x[control_suma];
				temp_pow2_y[control_suma] = temp_diff_y[control_suma] * temp_diff_y[control_suma];
				temp_pow2_z[control_suma] = temp_diff_z[control_suma] * temp_diff_z[control_suma];
				// SUMATORIA
				temp_sumatoria_x = temp_sumatoria_x + temp_pow2_x[control_suma];
				temp_sumatoria_y = temp_sumatoria_y + temp_pow2_y[control_suma];
				temp_sumatoria_z = temp_sumatoria_z + temp_pow2_z[control_suma];
			}
			var_x = temp_sumatoria_x / N;
			var_y = temp_sumatoria_y / N;
			var_z = temp_sumatoria_z / N;

			desvEstan_x = sqrt(var_x);
			desvEstan_y = sqrt(var_y);
			desvEstan_z = sqrt(var_z);

			suma_x = 0;
			suma_y = 0;
			suma_z = 0;
			temp_sumatoria_x = 0;
			temp_sumatoria_y = 0;
			temp_sumatoria_z = 0;

			flag = true;	// YA HAN PASADO POR LO MENOS 	N	MUESTRAS!
			cont = 0;
		}// end if

		//a = (flag == 1) ? c : d;	OPERADOR TERNARIO EN C

		if ((cont == 0) && (flag == true))
		{
			if (desvEstan_x > RANGE_X) {
				if (((suma_x_prom - msg.x) < FILTRO) && ((suma_x_prom - msg.x) > (-FILTRO))) {
					msg.x = suma_x_prom;	// SE MOVIO EN X!
				}
			} else {
				msg.x = msg.x;		// NO HAY NADA, QUEDATE DONDE ESTABAS!
			}

			if (desvEstan_y > RANGE_Y) {
				if (((suma_y_prom - msg.y) < FILTRO) && ((suma_y_prom - msg.y) > (-FILTRO))) {
					msg.y = suma_y_prom;	// SE MOVIO EN Y!
				}
			} else {
				msg.y = msg.y;		// NO HAY NADA, QUEDATE DONDE ESTABAS!
			}

			if (desvEstan_z > RANGE_Z) {
				if (((suma_z_prom - msg.z) < FILTRO) && ((suma_z_prom - msg.z) > (-FILTRO))) {
					msg.z = suma_z_prom;	// SE MOVIO EN Z!
				}
			} else {
				msg.z = msg.z;		// NO HAY NADA, QUEDATE DONDE ESTABAS!
			}

			suma_x_prom = 0;
			suma_y_prom = 0;
			suma_z_prom = 0;
			var_x = 0;
			var_y = 0;
			var_z = 0;
			desvEstan_x = 0;
			desvEstan_y = 0;
			desvEstan_z = 0;
		}
///////////////Calculo varianza   FIN///////////////////
//#endif

#ifndef PRUEBA_CUBO_ESTATICO_PYTHON
		msg.x = 30.0;
		msg.y = 40.0;
		msg.z = 50.0;
#endif

#ifndef PRUEBA_CUBO_TEMBLORINA
		msg.x = local_euler.yaw;	// Value ready to send UART!
		msg.y = local_euler.pitch;	// Value ready to send UART!
		msg.z = local_euler.roll;	// Value ready to send UART!
#endif

#ifndef AHRS_ANGLES_UART_PRINTF_OFF
//		PRINTF("SEND_UART  -  HEADER: %u \n", msg.header);
		PRINTF("SEND_UART  -  Angle YAW:   %i \n", (int) msg.x);
		PRINTF("SEND_UART  -  Angle PITCH: %i \n", (int) msg.y);
		PRINTF("SEND_UART  -  Angle ROLL:  %i \n", (int) msg.z);
#endif

		rtos_uart_send(rtos_uart0, (uint8_t*) &msg, sizeof(msg));
		xSemaphoreGive(parameters_task.mutex_SEND_UART_freertos);
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
