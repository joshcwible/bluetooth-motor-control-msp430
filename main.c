#include <msp430.h>
#include <stdint.h>
#include "Bluetooth_MotorDriver.h"

volatile uint8_t speed = 0;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	btmd_init();
	btmd_setDir(BRAKE);

	_enable_interrupt();

	uint8_t lastSpeed;
	lastSpeed = speed;

	while (1) {
		while (lastSpeed == speed)
			; // wait for speed modification
		if (speed > 50) {
			PWM_COMPARE = SCALE * (speed - 50);
			btmd_setDir(FORWARD);
		} else if (speed < 50) {
			PWM_COMPARE = SCALE * (50 - speed);
			btmd_setDir(BACKWARD);
		} else {
			PWM_COMPARE = 0;
			btmd_setDir(BRAKE);
		}
		lastSpeed = speed; // will be modified in interrupt
	}

}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
	switch (__even_in_range(UCA0IV, 4)) {
	case 0:
		break;                             // Vector 0 - no interrupt
	case 2:                                   // Vector 2 - RXIFG
		while (!(UCA0IFG & UCTXIFG))
			;             // USCI_A0 TX buffer ready?
		char data = UCA0RXBUF;                  // TX -> RXed character
		if (data != '\n')
			speed = data;
		UCA0TXBUF = data; // feedback
		break;
	case 4:
		break;                             // Vector 4 - TXIFG
	default:
		break;
	}
}
