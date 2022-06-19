/* PROJECT:  MPRO Laboraufgabe 4
 *
 * DEVELOPERS: Jannik Kreucher
 *             Nick Diendorf
 *             Benedikt Wendling
 */
#include "mbed.h"
#include <algorithm>
#include <cstdint>


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

#define CYCLE_TIME  10ms
#define PRESS_SHORT 40

#define SERVO_PERIOD_MAX    2100
#define SERVO_PERIOD_MIN    1100


PwmOut pinServo(PIN_SERVO);
DigitalOut pinLedOrange1(LED_ORANGE1);
DigitalOut pinLedOrange2(LED_ORANGE2);
DigitalIn pinSW1(SW_1);
DigitalIn pinSW2(SW_2);
DigitalIn pinSW3(SW_3);
DigitalIn pinSW4(SW_4);
DigitalIn pinSW5(SW_5);

Ticker sysTick;

int32_t servoPeriod = 0;
int32_t servoPeriodSave[2];
volatile uint8_t standby = 0;
uint16_t counterSW3 = 0, counterSW4 = 0;


void vTasten() {
    // check switch 2
    if(pinSW2 == 1) {
        servoPeriod += 10;
    }
    // check switch 5
    if(pinSW5 == 1) {
        servoPeriod -= 10;
    }
    // check switch 3
    if(pinSW3 == 1) {
        // button pressed
        if(counterSW3 < PRESS_SHORT) {
            counterSW3++;
        } else {
            if(counterSW3 == PRESS_SHORT) {
                // long press
                // set counter to one above short press so this does not get executed one more time
                counterSW3++;
                // do stuff with long press here
                servoPeriodSave[0] = servoPeriod;
            }
        }
    } else {
        // button not pressed (released)
        if((counterSW3 < PRESS_SHORT) && (counterSW3 != 0)) {
            // short press detected
            servoPeriod = servoPeriodSave[0];
        }
        counterSW3 = 0;
    }
    // check switch 4
    if(pinSW4 == 1) {
        // button pressed
        if(counterSW4 < PRESS_SHORT) {
            counterSW4++;
        } else {
            if(counterSW4 == PRESS_SHORT) {
                // long press
                // set counter to one above short press so this does not get executed one more time
                counterSW4++;
                // do stuff with long press here
                servoPeriodSave[1] = servoPeriod;
            }
        }
    } else {
        // button not pressed (released)
        if((counterSW4 < PRESS_SHORT) && (counterSW4 != 0)) {
            // short press detected
            servoPeriod = servoPeriodSave[1];
        }
        counterSW4 = 0;
    }


    // clamp servo position
    if(servoPeriod > SERVO_PERIOD_MAX) servoPeriod = SERVO_PERIOD_MAX;
    if(servoPeriod < SERVO_PERIOD_MIN) servoPeriod = SERVO_PERIOD_MIN;
    // set servo period
    pinServo.pulsewidth_us(servoPeriod);
}


void vTick() {
    standby = 0;
}


int main() {
    // configure servo frequency to 50Hz
    pinServo.period_ms(20);
    // time slice interrupt
    sysTick.attach(&vTick, 10ms);

	while(1) {
        // check buttons and switches
        vTasten();

        // wait for time slice
        standby = 1;
        while(standby);
	}

	return 0;
}
