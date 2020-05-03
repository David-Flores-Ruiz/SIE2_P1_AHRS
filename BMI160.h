/*
 * BMI160.h
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#ifndef BMI160_H_
#define BMI160_H_


/* TODO: insert other include files here. */
#include <stdint.h>
#include "rtos_i2c.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"


/* TODO: insert other definitions and declarations here. */
#define PIN_SCL_PTC10 (10U)
#define PIN_SDA_PTC11 (11)

#define BMI160_SLAVE_ADDR 0x68 // Default I2C 7-bit address of device if SDO = GND
#define LENGHT (1U)		// Byte Read/Write
#define SUBSIZE (1U)	// Size of each Direction Register of the BMI160

#define BMI160_CMD 0x7E	// Register for normal configuration accelerometer/gyroscope
#define BMI160_CHIP_ID 0x00	   // This register contains the value 0xD1
#define BMI160_CHIP_ID_VAL 0xD1

#define reg_gyro_x_lo 0x0C
#define reg_gyro_x_hi 0x0D
#define reg_gyro_y_lo 0x0E
#define reg_gyro_y_hi 0x0F
#define reg_gyro_z_lo 0x10
#define reg_gyro_z_hi 0x11
#define reg_acc_x_lo  0x12
#define reg_acc_x_hi  0x13
#define reg_acc_y_lo  0x14
#define reg_acc_y_hi  0x15
#define reg_acc_z_lo  0x16
#define reg_acc_z_hi  0x17

#define ACCEL_NORMAL_MODE 0x11
#define GYROS_NORMAL_MODE 0x15



typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
}BMI160_accelerometer_data_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
}BMI160_gyroscope_data_t;


typedef struct
{
	SemaphoreHandle_t mutex_ADQUISITION_freertos;
	SemaphoreHandle_t mutex_SEND_UART_freertos;
	EventGroupHandle_t event_FreeRTOs;
} parameters_task_t;


/*Lectura del Chip ID para comprobar que estamos conectados al dispositivo*/
void BMI160_I2C_ReadChipID(void *args);

/*Lectura de los valores del acelerometro*/
BMI160_accelerometer_data_t BMI160_I2C_Read_acc(void);

/*Lectura de los valores del acelerometro*/
BMI160_gyroscope_data_t BMI160_I2C_Read_gyr(void);


void data_acquisition_task(void *args);
void Ahrs_send_UART_angles_task(void *args);


#endif /* BMI160_H_ */
