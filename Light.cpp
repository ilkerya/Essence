/*
	LTR308 illumination sensor library for Arduino
	Dan Tudose
	    
The MIT License (MIT)

Copyright (c) 2022 Dan Tudose

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

version 0.1
*/
    
#include <stdint.h>
//#include <LTR308.h>
 #include  "./Lib/Light.h"
//#include "Light.h"
#include <Wire.h>

LTR308::LTR308(void) {
	// LTR308 object
}

boolean LTR308::begin(void) {
	// Initialize LTR308 library with default address 
	// Always returns true

	_i2c_address = LTR308_ADDR;
	Wire.begin();
	return(true);
}

boolean LTR308::setPowerUp(void) {
	// Turn on LTR308, begin integrations
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)

	// Write 0x02 (reset = 0 & mode = 1) to command byte (power on)
	return(writeByte(LTR308_CONTR,0x02));
}

boolean LTR308::setPowerDown(void) {
	// Turn off LTR308
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)

	// Clear command byte (reset = 0 & mode = 0)(power off)
	return(writeByte(LTR308_CONTR,0x00));
}

boolean LTR308::getPower(byte &status) {
	// Gets control register values
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)

	// Get control byte
	if(readByte(LTR308_CONTR,status)) {
		// return if successful
		return(true);
	}
	return(false);

}

boolean LTR308::setGain(byte gain) {
	// Sets the gain of LTR308
	// If gain = 0, device is set to 1X gain
	// If gain = 1, device is set to 3X gain (default)
	// If gain = 2, device is set to 6X gain
	// If gain = 3, device is set to 9X gain
	// If gain = 4, device is set to 18X gain
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	/*
	// sanity check for gain
	if (gain < 0x00 || gain >= 0x05) {
		gain = 0x00;
	}	
	*/
			// sanity check for gain
	if ( gain >= 0x05) {
		gain = 0x00;
	}	
	return(writeByte(LTR308_ALS_GAIN,gain));
}			
			
boolean LTR308::getGain(byte &gain) {
	// Gets the control register values
	// If gain = 0, device is set to 1X gain
	// If gain = 1, device is set to 3X gain (default)
	// If gain = 2, device is set to 6X gain
	// If gain = 3, device is set to 9X gain
	// If gain = 4, device is set to 18X gain
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)	

	// Reading the control byte
	if(readByte(LTR308_ALS_GAIN, gain)) {
		// return if successful
		return(true);
	}
	return(false);
}

boolean LTR308::setMeasurementRate(byte integrationTime, byte measurementRate) {
	// Sets the integration time and measurement rate of the sensor
	// integrationTime is the measurement time for each ALs cycle
	// measurementRate is the interval between DATA_REGISTERS update
	// measurementRate must be set to be equal or greater than integrationTime
	// If integrationTime = 0, integrationTime will be 400ms / 20 bits result
	// If integrationTime = 1, integrationTime will be 200ms / 19 bits result 
	// If integrationTime = 2, integrationTime will be 100ms / 18 bits result (default)
	// If integrationTime = 3, integrationTime will be 50ms / 17 bits result 
	// If integrationTime = 4, integrationTime will be 25ms / 16 bits result 
	//------------------------------------------------------
	// If measurementRate = 0, measurementRate will be 25ms
	// If measurementRate = 1, measurementRate will be 50ms
	// If measurementRate = 2, measurementRate will be 100ms (default)
	// If measurementRate = 3, measurementRate will be 500ms
	// If measurementRate = 5, measurementRate will be 1000ms
	// If measurementRate = 6, measurementRate will be 2000ms
	// If measurementRate = 7, measurementRate will be 2000ms
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	byte measurement = 0x00;
	
	// Perform sanity checks
	if(integrationTime >= 8) {
		integrationTime = 0;
	}
	
	if(measurementRate >= 8 || measurementRate == 4) {
		measurementRate = 0;
	}
	
	measurement |= integrationTime << 4;
	measurement |= measurementRate;
	
	return(writeByte(LTR308_MEAS_RATE, measurement));
}

boolean LTR308::getMeasurementRate(byte &integrationTime, byte &measurementRate) {
	// Gets the value of Measurement Rate register
	// Default value is 0x03
	// If integrationTime = 0, integrationTime will be 400ms / 20 bits result
	// If integrationTime = 1, integrationTime will be 200ms / 19 bits result 
	// If integrationTime = 2, integrationTime will be 100ms / 18 bits result (default)
	// If integrationTime = 3, integrationTime will be 50ms / 17 bits result 
	// If integrationTime = 4, integrationTime will be 25ms / 16 bits result 
	//------------------------------------------------------
	// If measurementRate = 0, measurementRate will be 25ms
	// If measurementRate = 1, measurementRate will be 50ms
	// If measurementRate = 2, measurementRate will be 100ms (default)
	// If measurementRate = 3, measurementRate will be 500ms
	// If measurementRate = 5, measurementRate will be 1000ms
	// If measurementRate = 6, measurementRate will be 2000ms
	// If measurementRate = 7, measurementRate will be 2000ms
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
			
	byte measurement = 0x00;
	
	// Reading the measurement byte
	if(readByte(LTR308_MEAS_RATE, measurement)) {
		// Extract integration Time
		integrationTime = (measurement & 0x70) >> 4;
		
		// Extract measurement Rate
		measurementRate = measurement & 0x07; 
		
		// return if successful
		return(true);
	}
	return(false);		
}

boolean LTR308::getPartID(byte &partID) {
	// Gets the part number ID and revision ID of the chip
	// Default value is 0xB1
	// part number ID = 0xB (default)
	// Revision ID = 0x1
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
			
	return(readByte(LTR308_PART_ID, partID));
}

boolean LTR308::getData(unsigned long &data) {
	// Gets the ALS channel data
	// Default value is 0
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	return(readLongInt(LTR308_DATA_0, data));
}

boolean LTR308::getStatus(boolean ponStatus, boolean intrStatus, boolean dataStatus) {
	// Gets the status information of LTR308
	// Default value is 0x00
	// If ponStatus = false(0), power on event (default)
	// If ponStatus = true(1), not specified in datasheet (probably not kosher)
	//---------------------------------------------
	// If intrStatus = false(0), INTR in inactive (default)
	// If intrStatus = true(1), INTR in active
	//---------------------------------------------
	// If dataStatus = false(0), OLD data (already read) (default)
	// If dataStatus = true(1), NEW data
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	byte status = 0x00;
	
	// Reading the status byte
	if(readByte(LTR308_STATUS, status)) {
		// Extract power on status
		//ponStatus = (status & 0x20) ? true : false; // 2026.03.16
		
		// Extract interrupt status
		//intrStatus = (status & 0x10) ? true : false;// 2026.03.16
	
		// Extract data status
		//dataStatus = (status & 0x08) ? true : false;// 2026.03.16
		
		// return if successful
		return(true);
	}
	return(false);
}

boolean LTR308::setInterruptControl(boolean mode) {
	// Sets up interrupt operations
	//------------------------------------------------------
	// If mode = false(0), INT pin is active at logic 0 (default)
	// If mode = true(1), INT pin is active at logic 1
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	byte intrControl = 0x10;
	
	intrControl |= mode << 2;
	
	return(writeByte(LTR308_INTERRUPT, intrControl));
}

boolean LTR308::getInterruptControl(boolean mode) {
	// Gets interrupt operations status
	//------------------------------------------------------
	// If mode = false(0), INT pin is active at logic 0 (default)
	// If mode = true(1), INT pin is active at logic 1
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	byte intrControl = 0x00;
	 mode = mode + 1;
	// Reading the interrupt byte
	if(readByte(LTR308_INTERRUPT, intrControl)) {
	
		// Extract mode
		mode = (intrControl & 0x04) ? true : false;
	
		// return if successful
		return(true);
	}
	return(false);
}

boolean LTR308::setThreshold(unsigned long upperLimit, unsigned long lowerLimit) {
	// Sets the upper limit and lower limit of the threshold
	// Default value of upper threshold is 0x00FFFFFF and lower threshold is 0x00000000
	// Both the threshold are 20-bit integer values
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	return(writeLongInt(LTR308_THRES_UP_0,upperLimit) && writeLongInt(LTR308_THRES_LOW_0,lowerLimit));
}

boolean LTR308::getThreshold(unsigned long &upperLimit, unsigned long &lowerLimit) {
	// Gets the upper limit and lower limit of the threshold
	// Default value of upper threshold is 0x00FFFFFF and lower threshold is 0x00000000
	// Both the threshold are 20-bit integer values
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
			
	return(readLongInt(LTR308_THRES_UP_0,upperLimit) && readLongInt(LTR308_THRES_LOW_0,lowerLimit));		
}

boolean LTR308::setIntrPersist(byte persist) {
	// Sets the interrupt persistance i.e. controls the N number of times the 
	// measurement data is outside the range defined by upper and lower threshold
	// Default value is 0x00
	// If persist = 0, every sensor value out of threshold range (default)
	// If persist = 1, every 2 consecutive value out of threshold range
	// If persist = 2, every 3 consecutive value out of threshold range
	// If persist = 3, every 4 consecutive value out of threshold range
	// If persist = 4, every 5 consecutive value out of threshold range
	// If persist = 5, every 6 consecutive value out of threshold range
	// If persist = 6, every 7 consecutive value out of threshold range
	// If persist = 7, every 8 consecutive value out of threshold range
	// If persist = 8, every 9 consecutive value out of threshold range
	// If persist = 9, every 10 consecutive value out of threshold range
	// If persist = 10, every 11 consecutive value out of threshold range
	// If persist = 11, every 12 consecutive value out of threshold range
	// If persist = 12, every 13 consecutive value out of threshold range
	// If persist = 13, every 14 consecutive value out of threshold range
	// If persist = 14, every 15 consecutive value out of threshold range
	// If persist = 15, every 16 consecutive value out of threshold range
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	// sanity check
	if(persist >= 15) {
		persist = 0x00;
	}
	
	persist = persist << 4;

	return(writeByte(LTR308_INTR_PERS,persist));
}

boolean LTR308::getIntrPersist(byte &persist) {
	// Gets the interrupt persistance i.e. controls the N number of times the measurement data is outside the range defined by upper and lower threshold
	// Default value is 0x00
	// If persist = 0, every sensor value out of threshold range (default)
	// If persist = 1, every 2 consecutive value out of threshold range
	// If persist = 2, every 3 consecutive value out of threshold range
	// If persist = 3, every 4 consecutive value out of threshold range
	// If persist = 4, every 5 consecutive value out of threshold range
	// If persist = 5, every 6 consecutive value out of threshold range
	// If persist = 6, every 7 consecutive value out of threshold range
	// If persist = 7, every 8 consecutive value out of threshold range
	// If persist = 8, every 9 consecutive value out of threshold range
	// If persist = 9, every 10 consecutive value out of threshold range
	// If persist = 10, every 11 consecutive value out of threshold range
	// If persist = 11, every 12 consecutive value out of threshold range
	// If persist = 12, every 13 consecutive value out of threshold range
	// If persist = 13, every 14 consecutive value out of threshold range
	// If persist = 14, every 15 consecutive value out of threshold range
	// If persist = 15, every 16 consecutive value out of threshold range
	//------------------------------------------------------
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
	
	return((readByte(LTR308_INTR_PERS,persist) >> 4));
}


boolean LTR308::getLux(byte gain, byte integrationTime, unsigned long CH, double &lux) {
	// Convert raw data to lux
	// gain: 0 (1X), 1 (3X), 2 (6X), 3 (9X) or 4 (18X), see getGain()
	// integrationTime: 0 (400ms @ 20bits), 1 (200ms @ 19bits), 2 (100ms @ 18bits), 3 (50ms @ 17bits) or 4 (25ms @ 16bits) from getMeasurementRate()
	// CH: result from getData()
	// lux will be set to resulting lux calculation
	// returns true (1) if calculation was successful
	// returns false (0) AND lux = 0.0 IF THE SENSOR WAS SATURATED (0XFFFF)

	double d0;

	// Determine if either sensor saturated (0xFFFF)
	// If so, abandon ship (calculation will not be accurate)
	if (CH == 0x000FFFFF) {
		lux = 0.0;
		return(false);
	}

	// Convert from unsigned integer to floating point
	
	switch(gain)
	{
		case 0:
			d0 = ((double)CH * 0.6);
			break;	
		
		case 1:
			d0 = ((double)CH * 0.6) / 3;
			break;
		
		case 2:
			d0 = ((double)CH * 0.6) / 6;
			break;
		
		case 3:
			d0 = ((double)CH * 0.6) / 9;
			break;
		
		case 4:
			d0 = ((double)CH * 0.6) / 18;
			break;
		
		default:
			d0 = 0.0;
			break;
	}
	
	switch (integrationTime)
	{
    case 0:
    	lux = d0 / 4;
    	break;

    case 1:
    	lux = d0 / 2;
    	break;
    
	case 2:
    	lux = d0;
		break;
	
	case 3:
    	lux = d0 * 2;
    	break;
	
	case 4:
    	lux = d0 * 4;
    	break;

    default:
    	lux = 0.0;
    	break;
	}

	return(true);
}

byte LTR308::getError(void) {
	// If any library command fails, you can retrieve an extended
	// error code using this command. Errors are from the wire library: 
	// 0 = Success
	// 1 = Data too long to fit in transmit buffer
	// 2 = Received NACK on transmit of address
	// 3 = Received NACK on transmit of data
	// 4 = Other error

	return(_error);
}

// Private functions:

boolean LTR308::readByte(uint8_t address, uint8_t &value) {
	// Reads a byte from a LTR308 address
	// Address: LTR308 address (0 to 15)
	// Value will be set to stored byte
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)

	// Check if sensor present for read
	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	_error = Wire.endTransmission();

	// Read requested byte
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address, 1);
		if (Wire.available() == 1)
		{
			value = Wire.read();
			return(true);
		}
	}
	return(false);
}

boolean LTR308::writeByte(uint8_t address, uint8_t value) {
	// Write a byte to a LTR308 address
	// Address: LTR308 address (0 to 15)
	// Value: byte to write to address
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)

	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	// Write byte
	Wire.write(value);
	_error = Wire.endTransmission();
	if (_error == 0)
		return(true);

	return(false);
}

boolean LTR308::readLongInt(uint8_t address, unsigned long &value) {
	// Reads an unsigned integer (16 bits) from a LTR308 address (low byte first)
	// Address: LTR308 address (0 to 15), low byte first
	// Value will be set to stored unsigned integer
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)

	uint8_t high, med, low;
	
	// Check if sensor present for read
	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	_error = Wire.endTransmission();

	// Read two bytes (low and high)
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address, 3);
		if (Wire.available() == 3)
		{
			low = Wire.read();
			med = Wire.read();
			high = Wire.read();
			// Combine bytes into unsigned int
			value = ((long)(high & 0x0F)) << 16;
			value |= ((long)med) << 8;
			value |= (long)low;
			return(true);
		}
	}	
	return(false);
}

boolean LTR308::writeLongInt(uint8_t address, unsigned long value) {
	// Write an unsigned integer (16 bits) to a LTR308 address (low byte first)
	// Address: LTR308 address (0 to 15), low byte first
	// Value: unsigned int to write to address
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)

	// Split int into lower and upper bytes, write each byte
	if (writeByte(address,(uint8_t)(value && 0x000000FF)) 
		&& writeByte(address + 1,(uint8_t)((value && 0x0000FF00) >> 8))
		&& writeByte(address + 2,(uint8_t)((value && 0x00FF0000) >> 16)))
		return(true);

	return(false);
}
