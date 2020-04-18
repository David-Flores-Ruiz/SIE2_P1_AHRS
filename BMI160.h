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


/* TODO: insert other definitions and declarations here. */
#define PIN_SCL_PTC10 (10U)
#define PIN_SDA_PTC11 (11)

#define BMI160_SLAVE_ADDR 0x68 // Default I2C 7-bit address of device if SDO = GND
#define LENGHT (1U)		// Byte Read/Write
#define SUBSIZE (1U)	// Size of each Direction Register of the BMI160

#define BMI160_CMD 0x7E	// Register for normal configuration accelerometer/gyroscope
#define BMI160_CHIP_ID 0x00	   // This register contains the value 0xD1
#define BMI160_CHIP_ID_VAL 0xD1

#define ACCEL_NORMAL_MODE 0x11
#define GYROS_NORMAL_MODE 0x15



typedef struct {
	float x;
	float y;
	float z;
}BMI160_accelerometer_data_t;

typedef struct {
	float x;
	float y;
	float z;
}BMI160_gyroscope_data_t;



/* TODO: insert function prototypes here. */
void BMI160_I2C_ReadChipID(void *args);



#endif /* BMI160_H_ */
