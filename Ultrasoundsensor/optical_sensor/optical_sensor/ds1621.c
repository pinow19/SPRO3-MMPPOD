#include "i2cmaster.h"
#include "ds1621.h"

void ds1621_init()
{
	// Set sensor config
	i2c_start_wait(DS1621_adr + I2C_WRITE);
	i2c_write(0xAC);
	i2c_write(0x20);
	// Start temp conversion
	i2c_rep_start(DS1621_adr + I2C_WRITE);
	i2c_write(0xEE);
	i2c_stop();
}

double get_temperature() {
	
	float temperature;
	unsigned char low_temp_byte;
	unsigned char high_temp_byte;

	// Read the last measured value
	i2c_start_wait(DS1621_adr + I2C_WRITE);
	i2c_write(0xAA);

	i2c_rep_start(DS1621_adr + I2C_READ);
	high_temp_byte = i2c_readAck();
	low_temp_byte = i2c_readNak();
	i2c_stop();
	
	if ((high_temp_byte & 0x80)==0) { // temp. >= 0
		if ((low_temp_byte & 0x80)==0) temperature = high_temp_byte; // no .5
		else temperature = high_temp_byte + 0.5;
	}
	else { // temp. < 0
		if ((low_temp_byte & 0x80)==0) { // no .5
			high_temp_byte = ~high_temp_byte + 1;
			temperature = 0.00 - high_temp_byte;
		}
		else {
			high_temp_byte = ~high_temp_byte;
			temperature = 0.00 - (high_temp_byte+0.5);
		}
	}
	return temperature;
}
