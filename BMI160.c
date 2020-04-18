/*
 * BMI160.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "BMI160.h"



void BMI160_I2C_ReadChipID(void *args) {
    rtos_i2c_config_t i2c_config;

    i2c_config.baudrate = 100000;		/**I2C Baud rate*/
    i2c_config.i2c_number = rtos_i2c_1;	/**I2C to use*/
    i2c_config.port = rtos_i2c_portC;	/**Kinetis Port*/
	i2c_config.SCL_pin = PIN_SCL_PTC10;	/**Pin of Serial Clock*/
	i2c_config.SDA_pin = PIN_SDA_PTC11;	/**Pin of Serial Data*/
	i2c_config.pin_mux = kPORT_MuxAlt2;	/**Pin Configuration*/

    rtos_i2c_init(i2c_config);

    uint8_t dataToWrite = ACCEL_NORMAL_MODE | GYROS_NORMAL_MODE;	// Byte to write
	uint8_t dataRead = 0x00;	// Read byte

	rtos_i2c_receive(rtos_i2c_1, &dataRead, 1, BMI160_SLAVE_ADDR,
			BMI160_CHIP_ID, 1);

	if (dataRead == BMI160_CHIP_ID_VAL) {
		PRINTF("Aqui estoy! BMI160... como sensor IMU externo \n");
		PRINTF("Chip ID value =	0x%x \n", dataRead);
	}
}


	//rtos_i2c_transfer(rtos_i2c_1, &dataToWrite, 1, 0x69, BMI160_CMD, 	  1);

