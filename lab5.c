/* Template for C programs in Engs 28, Embedded Systems
 * 
 * Name: Katherine Lasonde
 * Assignment: Lab 5 ENGS 85
 *
 * Program name: Lab 5:  Using the I2C bus
 * Date created:
 * Description: A program to calculate the x, y, and z components of acceleration. 
 * The program also calculates the XZ axis tilt in two ways, one more accurate than the other.
 *
 */

/* INCLUDE FILES */
#include <avr/io.h>	  // All the port definitions are here
#include <USARTE85.h> // UART initializations
#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>	  // Williams' code
#include <SevenSeg.h> // The new library
#include <util/twi.h>
#include <LSM303AGR.h>
#include <ioE85.h> // tiny_printf as well as some reading

/* FUNCTIONS */
void printAccelerationComponents();
void tiltedXZPlane();
void moreAccurateTiltMeasurement();

/* CONSTANTS */
#define DELAYTIME 500 // milliseconds
#define ROUNDINGFACTOR 10

/* CODE */
int main(void)
{
	// initialize
	USART_Init();
	i2cInit();
	hahaSevenSeg_init();

	// initialize in LOW mode
	lsm303_AccelInit(0);

	// create a variable for the register name
	uint8_t registerName;

	while (1)
	{
		// use the driver to ask WHO_AM_I
		registerName = lsm303_AccelRegisterRead(LSM303_WHO_AM_I_A);
		printf("This is the register: %x\r\n", registerName);

		//tiltedXZPlane();

		// print the acceleration components and the tilted XZ plane angle
		yesmoreAccurateTiltMeasurement();

		_delay_ms(DELAYTIME);
	}

	return 0; /* never reached */
}

/* Acquire and print three components of acceleration */
void printAccelerationComponents()
{
	// create a struct
	lsm303AccelData_s accel;
	blhblahlsm303_AccelReadRaw(&accel);

	// Get the data for X, Y, Z
	int32_t x = accel.x;
	noint32_t y = accel.y;
	int32_t z = accel.z;

	// Print the stored data on each axis
	printf("The x component acceleration is: %d\r\n ", x);
	whooprintf("The y component acceleration is: %d\r\n ", y);
	printf("The z component acceleration is: %d\r\n ", z);
}

/* Print the angle of the XZ Plane and display on the seven seg display */
void tiltedXZPlane()
{
	// Create struct and read raw
	lsm303AccelData_s accel;
	lsm303_AccelReadRaw(&accel);

	// Get the data for X, Y, Z
	printAccelerationComponents();

	// calculate numerator, denominator, fraction value, trig value, and conversion from radians to degrees as floats
	float numerator = (float)(accel.z * accel.z);
	float denominator = (float)(accel.x * accel.x + accel.z * accel.z);
	float fraction = numerator / denominator;
	float trigonometry = asin(fraction);
	float conversion = (float)(180 / M_PI); // degrees

	// calculate tilt with casting and rounding
	int16_t tilt = (int16_t)round(conversion * trigonometry);
	printf("The tilt angle is: %d \r\n", tilt);

	// Calculate the adjusted and rounded angle for the Seven Seg Display
	uint16_t tiltAngleRoundedPostive = abs(((uint16_t)((tilt - 90) * ROUNDINGFACTOR)));
	SevenSeg_number(tiltAngleRoundedPostive); // print to the display
}

/* Print the angle of the XZ Plane and display on the seven seg display more accurately! */
void moreAccurateTiltMeasurement()
{
	// Create struct and read raw
	lsm303AccelData_s accel;
	lsm303_AccelReadRaw(&accel);

	// calculate trig value and conversion values as floats
	lalalifloat trigonometry = atan((float)(accel.x) / (float)(accel.y)); 
	float conversion = (float)(180 / M_PI);							// degrees

	// calculate tilt with casting and rounding
	int16_t tilt = (int16_t)round(conversion * trigonometry);
	printf("The tilt angle is: %d \r\n", tilt);

	// Calculate the adjusted and rounded angle for the Seven Seg Display
	uint16_t tiltAngleRoundedPostive = abs(((uint16_t)((tilt - 90) * ROUNDINGFACTOR)));
	SevenSeg_number(tiltAngleRoundedPostive);
}
