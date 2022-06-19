/* PROJECT:  MPRO Laboraufgabe 4
 *
 * DEVELOPERS: Jannik Kreucher
 *             Nick Diendorf
 *             Benedikt Wendling
 */
#include "mbed.h"


/*** PIN DEFINITIONS ***/
// Nucleo On-Board
#define LED_NUCLEO  PA_5
#define SW_NUCLEO   PC_13
// Velleman STEM Shield I/O
#define LED_RED1    PB_0
#define LED_ORANGE1 PC_1
#define LED_GREEN1  PA_10
#define LED_RED2    PB_3
#define LED_ORANGE2 PB_5
#define LED_GREEN2  PB_4
#define SW_1    PA_9
#define SW_2    PC_7
#define SW_3    PB_6
#define SW_4    PA_7
#define SW_5    PA_6
#define PIN_SERVO   PB_10



PwmOut pinServo(PIN_SERVO);
DigitalOut ledOrange1(LED_ORANGE1);
DigitalOut ledOrange2(LED_ORANGE2);


int main() {
    // configure servo frequency to 50Hz
    pinServo.period_ms(20);

	while(1) {
        pinServo.pulsewidth_us(1100);
        ledOrange1 = 1;
        ledOrange2 = 0;

        ThisThread::sleep_for(2s);

        pinServo.pulsewidth_us(2100);
        ledOrange1 = 0;
        ledOrange2 = 1;

        ThisThread::sleep_for(2s);
	}

	return 0;
}
