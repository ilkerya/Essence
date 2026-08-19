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
   
#ifndef LTR308_h
#define LTR308_h

#include <stdint.h>
#include "Arduino.h"

#define LTR308_ADDR   0x53 // default address

// LTR308 register addresses
#define LTR308_CONTR         0x00
#define LTR308_MEAS_RATE     0x04
#define LTR308_ALS_GAIN      0x05
#define LTR308_PART_ID       0x06
#define LTR308_STATUS        0x07
#define LTR308_DATA_0 	     0x0D
#define LTR308_DATA_1        0x0E
#define LTR308_DATA_2        0x0F
#define LTR308_INTERRUPT     0x19
#define LTR308_INTR_PERS     0x1A
#define LTR308_THRES_UP_0    0x21
#define LTR308_THRES_UP_1	 0x22
#define LTR308_THRES_UP_2	 0x23
#define LTR308_THRES_LOW_0   0x24
#define LTR308_THRES_LOW_1   0x25
#define LTR308_THRES_LOW_2   0x26


class LTR308 {
	public:
		LTR308(void);
			// LTR308 object
			
		boolean begin(void);
			// Initialize LTR308 library with default address (0x53)
			// Always returns true
		
		boolean setPowerUp(void);
			// Turn on LTR308, begin integration
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		boolean setPowerDown(void);
			// Turn off LTR308
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		boolean getPower(byte &status);
			// Returns content of LTR308_CONTR control register 
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		boolean setGain(byte gain);
			// Sets the gain of LTR308
			// If gain = 0, device is set to 1X gain
			// If gain = 1, device is set to 3X gain (default)
			// If gain = 2, device is set to 6X gain
			// If gain = 3, device is set to 9X gain
			// If gain = 4, device is set to 18X gain
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		boolean getGain(byte &gain);
			// Gets the control register values
			// If gain = 0, device is set to 1X gain
			// If gain = 1, device is set to 3X gain (default)
			// If gain = 2, device is set to 6X gain
			// If gain = 3, device is set to 9X gain
			// If gain = 4, device is set to 18X gain
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
		boolean setMeasurementRate(byte integrationTime, byte measurementRate);
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
			
		boolean getMeasurementRate(byte &integrationTime, byte &measurementRate);
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

		boolean getPartID(byte &partID);
			// Gets the part number ID and revision ID of the chip
			// Default value is 0xB1
			// part number ID = 0xB (default)
			// Revision ID = 0x1
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
		
		boolean getData(unsigned long &data);
			// Gets the ALS channel data
			// Default value is 0
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
		
		boolean getStatus(boolean ponStatus, boolean intrStatus, boolean dataStatus);
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
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
	
		boolean setInterruptControl(boolean mode);
			// Sets up interrupt operations
			//------------------------------------------------------
			// If mode = false(0), INT pin is active at logic 0 (default)
			// If mode = true(1), INT pin is active at logic 1
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
		boolean getInterruptControl(boolean mode);
			// Gets interrupt operations status
			//------------------------------------------------------
			// If mode = false(0), INT pin is active at logic 0 (default)
			// If mode = true(1), INT pin is active at logic 1
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
			
		boolean setThreshold(unsigned long upperLimit, unsigned long lowerLimit);
			// Sets the upper limit and lower limit of the threshold
			// Default value of upper threshold is 0x00FFFFFF and lower threshold is 0x00000000
			// Both the threshold are 20-bit integer values
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
		boolean getThreshold(unsigned long &upperLimit, unsigned long &lowerLimit);
			// Gets the upper limit and lower limit of the threshold
			// Default value of upper threshold is 0x00FFFFFF and lower threshold is 0x00000000
			// Both the threshold are 20-bit integer values
			//------------------------------------------------------
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
		boolean setIntrPersist(byte persist);
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
			
		boolean getIntrPersist(byte &persist);
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
		
		boolean getLux(byte gain, byte integrationTime, unsigned long CH, double &lux);
			// Convert raw data to lux
			// gain: 0 (1X), 1 (3X), 2 (6X), 3 (9X) or 4 (18X), see getGain()
			// integrationTime: 0 (400ms @ 20bits), 1 (200ms @ 19bits), 2 (100ms @ 18bits), 3 (50ms @ 17bits) or 4 (25ms @ 16bits) from getMeasurementRate()
			// CH: result from getData()
			// lux will be set to resulting lux calculation
			// returns true (1) if calculation was successful
			// returns false (0) AND lux = 0.0 IF THE SENSOR WAS SATURATED (0XFFFF)
		
		byte getError(void);
			// If any library command fails, you can retrieve an extended
			// error code using this command. Errors are from the wire library: 
			// 0 = Success
			// 1 = Data too long to fit in transmit buffer
			// 2 = Received NACK on transmit of address
			// 3 = Received NACK on transmit of data
			// 4 = Other error

	private:

		boolean readByte(uint8_t address, uint8_t &value);
			// Reads a byte from a LTR308 address
			// Address: LTR308 address (0x00 to 0x26)
			// Value will be set to stored byte
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() above)
	
		boolean writeByte(uint8_t address, uint8_t value);
			// Write a byte to a LTR308 address
			// Address: LTR308 address (0x00 to 0x26)
			// Value: byte to write to address
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() above)

		boolean readLongInt(uint8_t address, unsigned long &value);
			// Reads an unsigned long integer (32 bits) from a LTR308 address (low byte first)
			// Address: LTR308 address (0x00 to 0x26), low byte first
			// Value will be set to stored unsigned long integer
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() above)

		boolean writeLongInt(uint8_t address, unsigned long value);
			// Writes an unsigned long integer (32 bits) to a LTR308 address (low byte first)
			// Address: LTR308 address (0x00 to 0x26), low byte first
			// Value will be set to stored unsigned long integer
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() above)
			
		uint8_t _i2c_address;
		
		uint8_t _error;
};

#endif