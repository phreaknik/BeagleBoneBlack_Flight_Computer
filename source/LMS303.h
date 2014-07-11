/*
 * LMS303.h
 *	For use with LMS303 Accelerometer as found in AltIMU-10. Note, must set bandwidth to enable
 *	measurements.
 *
 *  Created on: Jul 3, 2014
 *      Author: phreaknux
 *
 *  Reference:
 *  	http://www.inmotion.pt/store/altimu10-v3-gyro-accelerometer-compass-and-altimeter-l3gd20h
 */

#ifndef LMS303_H_
#define LMS303_H_

#define LMS303_I2C_BUFFER		0x40	// Only 0x40 registers available according to LMS303 datasheet
#define FIFO_SLOTS				0x0F	// Number of slots in FIFO for each accelerometer output
#define FIFO_SIZE 				0x60	// Size of FIFO array

enum LMS303_MAG_SCALE {
	SCALE_MAG_2gauss	= 0,
	SCALE_MAG_4gauss	= 1,
	SCALE_MAG_8gauss	= 2,
	SCALE_MAG_12gauss	= 3
};

enum LMS303_MAG_BANDWIDTH {
	BW_MAG_3p125HZ		= 0,
	BW_MAG_6p25HZ		= 1,
	BW_MAG_12p5HZ		= 2,
	BW_MAG_25HZ			= 3,
	BW_MAG_50HZ			= 4,
	BW_MAG_100HZ		= 5	// 100HZ BW only available if Accel BW > 50Hz or = 0
};

enum LMS303_ACCEL_SCALE {
	SCALE_ACCEL_2g		= 0,
	SCALE_ACCEL_4g		= 1,
	SCALE_ACCEL_6g		= 2,
	SCALE_ACCEL_8g		= 3,
	SCALE_ACCEL_16g		= 4
};

enum LMS303_ACCEL_BANDWIDTH {
	BW_ACCEL_SHUTDOWN	= 0,
	BW_ACCEL_3p125HZ 	= 1,
	BW_ACCEL_6p25HZ 	= 2,
	BW_ACCEL_12p5HZ 	= 3,
	BW_ACCEL_25HZ 		= 4,
	BW_ACCEL_50HZ 		= 5,
	BW_ACCEL_100HZ 		= 6,
	BW_ACCEL_200HZ 		= 7,
	BW_ACCEL_4OOHZ 		= 8,
	BW_ACCEL_8OOHZ 		= 9,
	BW_ACCEL_16OOHZ 	= 10,
	BW_ACCEL_ERROR		= -1
};

enum LMS303_ACCEL_MODE {
	FIFO_BYPASS,
	FIFO_STREAM,	// This mode enables averaging
	FIFO_ERROR
};

class LMS303 {

private:
	int celsius;

	int I2CBus, I2CAddress;
	char dataBuffer[LMS303_I2C_BUFFER] = { 0x00 };
	char accelFIFO[FIFO_SIZE];	// 16 FIFO slots * 6 Accel output registers
	LMS303_ACCEL_MODE accelFIFOMode = FIFO_BYPASS;

	int magFullScale;
	float magX;
	float magY;
	float magZ;

	int accelFullScale;
	float accelX;	// in g's
	float accelY;	// in g's
	float accelZ;	// in g's

	double pitch;	// in degrees
	double roll;	// in degrees

	int writeI2CDeviceByte(char address, char value);
	int readI2CDevice(char address, char data[], int size);

	float convertMagnetism(int msb_reg_addr, int lsb_reg_addr);

	float convertAcceleration(int msb_reg_addr, int lsb_reg_addr);
	float convertAcceleration(int accel);
	void calculatePitchAndRoll();
	int readAccelFIFO(char buffer[]);
	int averageAccelFIFO(int slots);

public:

	LMS303(int bus, int address);

	int reset();
	int readFullSensorState();

	int enableTempSensor();
	int getTemperature();

	int enableMagnetometer();
	float getMagX() { return magX; }
	float getMagY() { return magY; }
	float getMagZ() { return magZ; }
	int setMagScale(LMS303_MAG_SCALE scale);
	int setMagBandwidth(LMS303_MAG_BANDWIDTH bandwidth);

	int enableAccelerometer();
	int setAccelScale(LMS303_ACCEL_SCALE scale);
	int setAccelBandwidth(LMS303_ACCEL_BANDWIDTH bandwidth);	// Must set bandwidth to enable device
	LMS303_ACCEL_BANDWIDTH getAccelBandwidth();
	int setAccelFIFOMode(LMS303_ACCEL_MODE mode);
	LMS303_ACCEL_MODE getAccelFIFOMode();
	float getAccelX() { return accelX; }
	float getAccelY() { return accelY; }
	float getAccelZ() { return accelZ; }

	float getPitch() { return pitch; }
	float getRoll() { return roll; }

	virtual ~LMS303();
};


#endif /* LMS303_H_ */
