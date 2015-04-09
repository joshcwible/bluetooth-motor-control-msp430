/*
 * Bluetooth_MotorDriver.c
 *
 *  Created on: Apr 1, 2015
 *      Author: Josh2.0
 */

#include <msp430.h>
#include <stdint.h>
#include "Bluetooth_MotorDriver.h"

void btmd_init(void) {
	btmd_UART_INIT;

	// setup all pins
	MOTORDIR |= MOTOR1PIN + MOTOR2PIN; // set outputs
	MOTORPORT &= ~(MOTOR1PIN + MOTOR2PIN); // turn off initially
	// up mode
	// output mode 7
	// TA0CCR1 is compare
	// TA0CCR0 is max

	//TA0CTL = TASSEL__SMCLK + MC__UP + ID__8;
	TA0CCR0 = MAXCOUNT; // set max count
	TA0CCR1 = MAXCOUNT / 2; // set initial compare 50%

	// set clock stuff
	TA0CCTL1 |= OUTMOD_7; // reset/set mode

	// config Px.y to use primary peripheral
	/*
	 PWMDIR |= PWMPIN;
	 PWMSEL |= PWMPIN;
	 */

	P1DIR |=  BIT3;                       // P1.2 and P1.3 output
	P1SEL |=  BIT3;                       // P1.2 and P1.3 options select
	TA0CCR0 = MAXCOUNT/2;                          // PWM Period
	TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	//TA0CCR1 = 384;                            // CCR1 PWM duty cycle
	TA0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
	//TA0CCR2 = 128;                            // CCR2 PWM duty cycle
	TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

	btmd_setDir(BRAKE); // by default
}

void btmd_UART_INIT(void) {
	P3SEL |= BIT3 + BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
	UCA0BR1 = 0;                              // 1MHz 115200
	UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void btmd_setDir(uint8_t dir) {
	switch (dir) {
	case FORWARD:
		MOTORPORT |= MOTOR1PIN;
		MOTORPORT &= ~MOTOR2PIN;
		break;
	case BACKWARD:
		MOTORPORT |= MOTOR2PIN;
		MOTORPORT &= ~MOTOR1PIN;
		break;
	case BRAKE:
		MOTORPORT &= ~MOTOR1PIN;
		MOTORPORT &= ~MOTOR2PIN;
		break;
	}
}
