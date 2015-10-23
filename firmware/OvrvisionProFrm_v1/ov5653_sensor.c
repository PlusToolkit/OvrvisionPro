/**************************************************************************
 *
 *              Copyright (c) 2014-2015 by Wizapply.
 *
 *  This software is copyrighted by and is the sole property of Wizapply
 *  All rights, title, ownership, or other interests in the software
 *  remain the property of Wizapply.  This software may only be used
 *  in accordance with the corresponding license agreement.
 *  Any unauthorized use, duplication, transmission, distribution,
 *  or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of Wizapply.
 *
 *  Wizpply reserves the right to modify this software without notice.
 *
 *  Wizapply                                info@wizapply.com
 *  5F, KS Building,                        http://wizapply.com
 *  3-7-10 Ichiokamotomachi, Minato-ku,
 *  Osaka, 552-0002, Japan
 *
***************************************************************************/

/**************************************************************************
 *
 *  Ovrvision Pro FirmWare v1.0
 *
 *  Language is 'C' code source
 *  Files : ov5653_sensor.c
 *
***************************************************************************/

#include <cyu3system.h>
#include <cyu3os.h>
#include <cyu3dma.h>
#include <cyu3error.h>
#include <cyu3uart.h>
#include <cyu3i2c.h>
#include <cyu3types.h>
#include <cyu3gpio.h>
#include <cyu3utils.h>

#include "ov5653_sensor.h"

////////////////////// Functions //////////////////////

// I2C functions
CyU3PReturnStatus_t WriteI2C (uint8_t slaveAddr, uint16_t address, uint8_t data)
{
	CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
	CyU3PI2cPreamble_t preamble;

	uint8_t highAddr = address >> 8;
	uint8_t lowAddr = address & 0x00FF;

	/* Validate the I2C slave address. */
	if ((slaveAddr != I2C_SENSOR_ADDR_WR) && (slaveAddr != I2C_MEMORY_ADDR_WR)) {
		return CY_U3P_ERROR_FAILURE;
	}

	/* Set the parameters for the I2C API access and then call the write API. */
	preamble.buffer[0] = slaveAddr;
	preamble.buffer[1] = highAddr;
	preamble.buffer[2] = lowAddr;
	preamble.length = 3;			/*  Three byte preamble. */
	preamble.ctrlMask = 0x0000; 	/*  No additional start and stop bits. */

	apiRetStatus = CyU3PI2cTransmitBytes(&preamble, &data, 1, 0);
	if(apiRetStatus == CY_U3P_SUCCESS)
		CyU3PBusyWait(10);

	return apiRetStatus;
}

CyU3PReturnStatus_t ReadI2C (uint8_t slaveAddr, uint16_t address, uint8_t *data)
{
	CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
	CyU3PI2cPreamble_t preamble;

	uint8_t highAddr = address >> 8;
	uint8_t lowAddr = address & 0x00FF;

	if ((slaveAddr != I2C_SENSOR_ADDR_RD) && (slaveAddr != I2C_MEMORY_ADDR_RD)) {
		return CY_U3P_ERROR_FAILURE;
	}

	preamble.buffer[0] = slaveAddr & I2C_SLAVEADDR_MASK; /*  Mask out the transfer type bit. */
	preamble.buffer[1] = highAddr;
	preamble.buffer[2] = lowAddr;
	preamble.buffer[3] = slaveAddr;
	preamble.length = 4;
	preamble.ctrlMask = 0x0004; /*  Send start bit after third byte of preamble. */

	apiRetStatus = CyU3PI2cReceiveBytes(&preamble, data, 1, 0);
	CyU3PBusyWait(10);

	return apiRetStatus;
}


// Sensor control function

// Initialse sensor
void OV5653SensorInit(void)
{
	if (OV5653SensorBusTest() != CY_U3P_SUCCESS) /* Verify that the sensor is connected. */
		return;

	//Reset
	OV5653SensorReset();
}

// Reset sensor
void OV5653SensorReset(void)
{
	//Sensor reset command
	WI2C(0x3008,0x80);
	CyU3PBusyWait(1000); //1ms
	WI2C(0x3000,0xFF);
	WI2C(0x3001,0xFF);
	WI2C(0x3002,0xFF);
	WI2C(0x3003,0xFF);

	/* Delay the allow the sensor to power up. */
	CyU3PThreadSleep(20); //20ms
}

// Test sensor
uint8_t OV5653SensorBusTest(void)
{
	/* The sensor ID register can be read here to verify sensor connectivity. */
	uint8_t chipid = 0;

	/* Reading sensor ID */
	if (ReadI2C(I2C_SENSOR_ADDR_RD, 0x3100, &chipid) == CY_U3P_SUCCESS) {
		if (chipid == I2C_SENSOR_ADDR_WR) {
			return CY_U3P_SUCCESS;
		}
	}
	return CY_U3P_ERROR_FAILURE;
}

// Sensor setup

void OV5653Sensor_5MP15FPS() {
	//2560 x 1920 @ 15fps	X 2

}

void OV5653Sensor_FullHD30FPS() {
	//1920 x 1080 @ 30fps	X 2

}

void OV5653Sensor_UVGA45FPS(){
	//1280 x 960 @ 45fps	X 2

	//Starter
	WI2C(0x3008,0x80);
	CyU3PBusyWait(2000);
	WI2C(0x3103,0x93);
	WI2C(0x3017,0x7f);
	CyU3PBusyWait(1000);
	WI2C(0x3018,0xff);
	CyU3PBusyWait(1000);
	WI2C(0x3810,0xc2);
	CyU3PBusyWait(1000);

	//System Control
	WI2C(0x3000,0x00);
	WI2C(0x3001,0x00);
	WI2C(0x3002,0x5c);
	WI2C(0x3003,0x00);
	WI2C(0x3004,0xff);
	WI2C(0x3005,0xf7);
	WI2C(0x3006,0x41);
	WI2C(0x3007,0x27);

	//PLL (do not use)
	/*
	WI2C(0x300F,0x8f);
	WI2C(0x3011,0x14);
	WI2C(0x3010,0x10);
	WI2C(0x3012,0x02);
	WI2C(0x3815,0x82);
	 */

	//Analog Control
	WI2C(0x3600,0x50);
	WI2C(0x3601,0x0d);
	WI2C(0x3604,0x50);
	WI2C(0x3605,0x04);
	WI2C(0x3606,0x3f);
	WI2C(0x3612,0x1a);
	WI2C(0x3630,0x22);
	WI2C(0x3631,0x22);
	WI2C(0x3702,0x3a);
	WI2C(0x3704,0x18);
	WI2C(0x3705,0xda);
	WI2C(0x3706,0x41);
	WI2C(0x370A,0x80);
	WI2C(0x370B,0x40);
	WI2C(0x370E,0x00);
	WI2C(0x3710,0x28);
	WI2C(0x3712,0x13);
	WI2C(0x3603,0xa7);
	WI2C(0x3615,0x50);
	WI2C(0x3620,0x56);
	WI2C(0x3613,0x44);
	WI2C(0x3827,0x08);

	//Frex Control
	WI2C(0x3B07,0x00);
	WI2C(0x302C,0x02);	//Frex  no=0x02
	WI2C(0x3B09,0x00);

	//Image Timming
	WI2C(0x3800,0x01);
	WI2C(0x3801,0xB4);
	WI2C(0x3802,0x00);
	WI2C(0x3803,0x08);
	WI2C(0x3804,0x05);
	WI2C(0x3805,0x00);
	WI2C(0x3806,0x03);
	WI2C(0x3807,0xc0);
	WI2C(0x3808,0x05);
	WI2C(0x3809,0x00);
	WI2C(0x380A,0x03);
	WI2C(0x380B,0xc0);
	WI2C(0x380C,0x0c);
	WI2C(0x380D,0x80);
	WI2C(0x380E,0x03);
	WI2C(0x380F,0xe8);

	WI2C(0x3A0D,0x08);
	WI2C(0x3C01,0x00);
	WI2C(0x3503,0x33);

	//Image Processer
	WI2C(0x5000, 0x00);
	WI2C(0x5001, 0x00);
	WI2C(0x5002, 0x00);
	WI2C(0x5046, 0x09);

	//Exp / gain
	WI2C(0x3500, 0x00); /* long exp 1/3 in unit of 1/16 line, pg 38, note frame length is from 0x5a4, and SENSOR_BAYER_DEFAULT_MAX_COARSE_DIFF=3 */
	WI2C(0x3501, 0x10); /* long exp 2/3 in unit of 1/16 line, pg 38 */
	WI2C(0x3502, 0x00); /* long exp 3/3 in unit of 1/16 line, pg 38 */
	WI2C(0x350A, 0x00); /* gain output to sensor, pg 38 */
	WI2C(0x350B, 0x00); /* gain output to sensor, pg 38 */

	//BLC
	//WI2C(0x4000,0x00);
	//WI2C(0x401c,0x00);
	//WI2C(0x401c,0x28);

	WI2C(0x3632, 0x5a); /* analog pg 108 */
	WI2C(0x3703, 0xb0); /* analog pg 108 */
	WI2C(0x370C, 0xc5); /* analog pg 108 */
	WI2C(0x370D, 0x42); /* analog pg 108 */
	WI2C(0x3713, 0x2f); /* analog pg 108 */

	//WI2C(0x3830, 0x50); /* manual exposure gain bit [0] */
	//WI2C(0x3a18, 0x00); /* AEC gain ceiling bit 8 pg 114 */
	//WI2C(0x3a19, 0xf8); /* AEC gain ceiling pg 114 */
	//WI2C(0x3a00, 0x38); /* AEC control 0 debug mode band low limit mode band func pg 112 */

	//binning / subsampling
	WI2C(0x3621,0xaf);
	WI2C(0x370D,0x02);
	WI2C(0x3818,0xc1);
	WI2C(0x381A,0x00);

	//DVP Control
	WI2C(0x300E,0x18);
	WI2C(0x4708,0x03);
	WI2C(0x4700,0x04);
	WI2C(0x4704,0x02);
	WI2C(0x4706,0x08);

	//Test
	WI2C(0x503D,0x00);	//test=0x80
	WI2C(0x503E,0x00);
	//WI2C(0x4709,0x03);

	WI2C(0x3000,0x03);
	CyU3PBusyWait(1000);
	WI2C(0x3000,0x00);

	CyU3PBusyWait(1000);
}

void OV5653Sensor_VR60FPS(){
	//1280 x 800 @ 60fps	X 2 Oculus Rift

}

void OV5653Sensor_VR10FPS(){
	//1280 x 800 @ 10fps	X 2 Oculus Rift, For USB2.0

}

void OV5653Sensor_VGA90FPS(){
	//640 x 480 @ 90fps	    X 2

}

void OV5653Sensor_VGA60FPS(){
	//640 x 480 @ 60fps		X 2, For USB2.0

}

void OV5653Sensor_QVGA120FPS(){
	//320 x 240 @ 120fps	X 2

}
