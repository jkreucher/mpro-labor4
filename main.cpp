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

#define CYCLE_TIME  10ms // time for each cycle
#define PRESS_SHORT 50   // in multiples of CYCLE_TIME

#define SERVO_PERIOD_MAX    2100 //in us
#define SERVO_PERIOD_MIN    1100 //in us


// Input / Output
PwmOut pinServo(PIN_SERVO);
DigitalOut pinLedOrange1(LED_ORANGE1);
DigitalOut pinLedOrange2(LED_ORANGE2);
DigitalOut pinLedRed2(LED_RED2);
DigitalOut pinLedGreen2(LED_GREEN2);
DigitalIn pinSW1(SW_1);
DigitalIn pinSW2(SW_2);
DigitalIn pinSW3(SW_3);
DigitalIn pinSW4(SW_4);
DigitalIn pinSW5(SW_5);

// timer interrupt
Ticker sysTick;

// task struct
struct stTask {
    void (*handler)();
    uint16_t period;
    uint16_t counter;
};

// global variables
int32_t servoPeriod = 0;
int32_t servoPeriodSave[2];
volatile uint8_t standby = 0;
uint16_t counterSW3 = 0, counterSW4 = 0;


void vButtonsPosition() {
    // check if in program mode
    if(pinSW1 == 1) {
        // check switch 2
        if(pinSW2 == 1) {
            servoPeriod += 10;
            pinLedOrange2 = 1;
            pinLedGreen2 = 0;
        } else {
            pinLedOrange2 = 0;
        }

        // check switch 5
        if(pinSW5 == 1) {
            servoPeriod -= 10;
            pinLedOrange1 = 1;
            pinLedGreen2 = 0;
        } else {
            pinLedOrange1 = 0;
        }
    }
}


void vButtonsMemory() {
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
                // check if in program mode
                if(pinSW1 == 1) {
                    // save servo position
                    servoPeriodSave[0] = servoPeriod;
                    pinLedRed2 = 1;
                }
            }
        }
    } else {
        // button not pressed (released)
        if((counterSW3 < PRESS_SHORT) && (counterSW3 != 0)) {
            // short press detected
            // recall saved position
            servoPeriod = servoPeriodSave[0];
             pinLedGreen2 = 1;
        }
        // check if button has been pressed
        if(counterSW3 != 0) {
            pinLedRed2 = 0;
        }
        // reset switch counter
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
                // check if in program mode
                if(pinSW1 == 1) {
                    // save servo position
                    servoPeriodSave[1] = servoPeriod;
                    pinLedRed2 = 1;
                }
            }
        }
    } else {
        // button not pressed (released)
        if((counterSW4 < PRESS_SHORT) && (counterSW4 != 0)) {
            // short press detected
            // recall saved position
            servoPeriod = servoPeriodSave[1];
            pinLedGreen2 = 1;
        }
        // check if button has been pressed
        if(counterSW4 != 0) {
             pinLedRed2 = 0;
        }
        // reset switch counter
        counterSW4 = 0;
    }

    // clamp servo position
    if(servoPeriod > SERVO_PERIOD_MAX) servoPeriod = SERVO_PERIOD_MAX;
    if(servoPeriod < SERVO_PERIOD_MIN) servoPeriod = SERVO_PERIOD_MIN;
    // set servo period
    pinServo.pulsewidth_us(servoPeriod);
}


// task list
struct stTask schedulerTasks[] = {
    {vButtonsMemory, 10, 0},
    {vButtonsPosition, 20, 0}
};

void vScheduler() {
    // check for each task if it needs to be called
    for(uint8_t taskid=0; taskid < (sizeof(schedulerTasks)/sizeof(stTask)); taskid++) {
        if(schedulerTasks[taskid].counter == 0) {
            // time has come
            // reset counter
            schedulerTasks[taskid].counter = schedulerTasks[taskid].period;
            // call task handler
            schedulerTasks[taskid].handler();
        } else {
            // decrement task counter
            schedulerTasks[taskid].counter--;
        }
    }
}


void vTick() {
    standby = 0;
}


int main() {
    // configure servo frequency to 50Hz
    pinServo.period_ms(20);

    // time slice interrupt
    sysTick.attach(&vTick, 1ms);

	while(1) {
        // execute scheduler every 1ms
        // this function checks if a task needs to be executed
        vScheduler();

        // wait for time slice
        standby = 1;
        while(standby);
	}

	return 0;
}
