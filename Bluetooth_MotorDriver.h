/*
 * Bluetooth_MotorDriver.h
 *
 *  Created on: Apr 1, 2015
 *      Author: Josh2.0
 */

#include <msp430.h>
#include <stdint.h>

#ifndef BLUETOOTH_MOTORDRIVER_H_
#define BLUETOOTH_MOTORDRIVER_H_

#define PWMPIN		BIT2
#define PWMPORT		P1OUT
#define PWMDIR		P1DIR
#define PWMSEL		P1SEL

#define MOTOR1PIN BIT0
#define MOTOR2PIN BIT1
#define MOTORPORT P6OUT
#define MOTORDIR  P6DIR

#define FORWARD		2
#define BACKWARD 	1
#define BRAKE  		0

#define MAXCOUNT 	32768
#define SCALE		MAXCOUNT/50

#define PWM_COMPARE 	TA0CCR2

void btmd_init(void);
void btmd_setDir(uint8_t dir);
void btmd_UART_INIT(void);

#endif /* BLUETOOTH_MOTORDRIVER_H_ */
