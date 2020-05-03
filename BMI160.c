/*
 * BMI160.c
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "BMI160.h"


#define BMI160_PRINTF_OFF 1


void BMI160_I2C_ReadChipID(void *args) {
	parameters_task_t parameters_task = *((parameters_task_t*)args);

	rtos_i2c_config_t i2c_config;

    i2c_config.baudrate = 100000;		/**I2C Baud rate*/
    i2c_config.i2c_number = rtos_i2c_1;	/**I2C to use*/
    i2c_config.port = rtos_i2c_portC;	/**Kinetis Port*/
	i2c_config.SCL_pin = PIN_SCL_PTC10;	/**Pin of Serial Clock*/
	i2c_config.SDA_pin = PIN_SDA_PTC11;	/**Pin of Serial Data*/
	i2c_config.pin_mux = kPORT_MuxAlt2;	/**Pin Configuration*/

	rtos_i2c_init(i2c_config);

	uint8_t dataToWrite = 0x00;; // Byte to write
	uint8_t dataRead = 0x00; // Read byte

	dataRead = 0x00;	// Read byte
	rtos_i2c_receive(rtos_i2c_1, &dataRead, 1, BMI160_SLAVE_ADDR, BMI160_CHIP_ID, 1);

	dataToWrite = ACCEL_NORMAL_MODE;	// Byte to write
	rtos_i2c_transfer(rtos_i2c_1, &dataToWrite, 1, BMI160_SLAVE_ADDR, BMI160_CMD, 1);
	//vTaskDelay(pdMS_TO_TICKS(5));

	dataToWrite = GYROS_NORMAL_MODE;	// Byte to write
	rtos_i2c_transfer(rtos_i2c_1, &dataToWrite, 1, BMI160_SLAVE_ADDR, BMI160_CMD, 1);
	//vTaskDelay(pdMS_TO_TICKS(80));

	if (dataRead == BMI160_CHIP_ID_VAL) {
		PRINTF("Hey:) Aqui estoy! BMI160... como sensor IMU externo \n");
		PRINTF("---		Chip ID value =	0x%x 		---   \n", dataRead);
	}

	xTaskCreate(data_acquisition_task, 		"data_acquisition_task", 	  800, (void*)&parameters_task, configMAX_PRIORITIES, NULL);
	xTaskCreate(Ahrs_send_UART_angles_task, "Ahrs_send_UART_angles_task", 800, (void*)&parameters_task, configMAX_PRIORITIES, NULL);

	vTaskDelay(portMAX_DELAY);
}

BMI160_accelerometer_data_t BMI160_I2C_Read_acc(void) {
	BMI160_accelerometer_data_t data_acc;
	uint8_t dato_x_lo = 0;	///// CONVERTIR A FLOAT +-2g range (reset)
	uint8_t dato_x_hi = 0;
	uint8_t dato_y_lo = 0;
	uint8_t dato_y_hi = 0;
	uint8_t dato_z_lo = 0;
	uint8_t dato_z_hi = 0;
	rtos_i2c_receive(rtos_i2c_1, &dato_x_lo, 1, BMI160_SLAVE_ADDR, reg_acc_x_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_x_hi, 1, BMI160_SLAVE_ADDR, reg_acc_x_hi, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_y_lo, 1, BMI160_SLAVE_ADDR, reg_acc_y_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_y_hi, 1, BMI160_SLAVE_ADDR, reg_acc_y_hi, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_z_lo, 1, BMI160_SLAVE_ADDR, reg_acc_z_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_z_hi, 1, BMI160_SLAVE_ADDR, reg_acc_z_hi, 1);
	data_acc.x = (dato_x_hi << 8) + dato_x_lo;
	data_acc.y = (dato_y_hi << 8) + dato_y_lo;
	data_acc.z = (dato_z_hi << 8) + dato_z_lo;

#ifndef BMI160_PRINTF_OFF
	PRINTF("BMI160 - Accel en x: %d \n", data_acc.x);
	PRINTF("BMI160 - Accel en y: %d \n", data_acc.y);
	PRINTF("BMI160 - Accel en z: %d \n", data_acc.z);
#endif

	return data_acc;
}

BMI160_gyroscope_data_t BMI160_I2C_Read_gyr(void) {
	BMI160_gyroscope_data_t data_gyr;
	uint8_t dato_x_lo = 0;	///// CONVERTIR A FLOAT +-2000°/s range (reset)
	uint8_t dato_x_hi = 0;
	uint8_t dato_y_lo = 0;
	uint8_t dato_y_hi = 0;
	uint8_t dato_z_lo = 0;
	uint8_t dato_z_hi = 0;
	rtos_i2c_receive(rtos_i2c_1, &dato_x_lo, 1, BMI160_SLAVE_ADDR, reg_gyro_x_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_x_hi, 1, BMI160_SLAVE_ADDR, reg_gyro_x_hi, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_y_lo, 1, BMI160_SLAVE_ADDR, reg_gyro_y_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_y_hi, 1, BMI160_SLAVE_ADDR, reg_gyro_y_hi, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_z_lo, 1, BMI160_SLAVE_ADDR, reg_gyro_z_lo, 1);
	rtos_i2c_receive(rtos_i2c_1, &dato_z_hi, 1, BMI160_SLAVE_ADDR, reg_gyro_z_hi, 1);
	data_gyr.x = (dato_x_hi << 8) + dato_x_lo;
	data_gyr.y = (dato_y_hi << 8) + dato_y_lo;
	data_gyr.z = (dato_z_hi << 8) + dato_z_lo;

#ifndef BMI160_PRINTF_OFF
	PRINTF("BMI160 - Gyros en x: %d \n", data_gyr.x);
	PRINTF("BMI160 - Gyros en y: %d \n", data_gyr.y);
	PRINTF("BMI160 - Gyros en z: %d \n", data_gyr.z);
#endif

	return data_gyr;
}
