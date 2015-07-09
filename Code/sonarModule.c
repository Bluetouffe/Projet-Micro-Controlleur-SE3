/*
 * File:   sonarModule.c
 * Author: Louis
 *
 * Created on 27 juin 2015, 14:09
 */

#include "p18f45K20.h"
#include "sonarModule.h"
#include "initialisation.h"
#include "globalVariables.h"

extern void MULT16(void);
unsigned int arg1, arg2;
unsigned long res;

void convertToCentimetres(unsigned int measuredTime , unsigned int * distance)
{
    // Clock speed = 1MHz
    // Time per tick = 1 / (Fosc/4) = 4 microseconds
    // Time for one centimetres = 58 microseconds
    // Number of tick => centimetres is TICK / (58/4) = TICK * 0.0689...
    // In fixed point representation 256 * 0.0689 = 17.65 = 18
    unsigned int const TICK_PER_CENTIMETERS_FIXED_POINT = 18;

    // 16 bits multiplication Assmebly routine
    arg1 = measuredTime;
    arg2 = TICK_PER_CENTIMETERS_FIXED_POINT;
    MULT16();

    // The result is 256 times the real distance, because of the fixed point
    // representation. Thus, it is necessary to divide it by 256, equivalent
    // to an 8bit right shift
    *distance = res;
    *distance >>= 8;
}

void startMeasure( void )
{
    flag.captureDone = 0;
    flag.timeElapsed = 0;
    
    MB1010_EN_PIN = 1;              // Toggle enable measurement PIN of Sonar module

    while (MB1010_PWM_PIN == 0)     // While no measure is returned do nothing
    {
    }
    MB1010_EN_PIN = 0;              // Input is HIGH, measure as started, lower trigger pin
    resetTMR1();                    // When input is HIGH, start timer
}

void getMeasure(unsigned int * distance)
{
    // Variable for number of system TICK measured
    unsigned int timeMeasured = 0;

    timeMeasured = timerH;          //Convert it to 16 uint
    timeMeasured <<= 8;
    timeMeasured |= timerL;
    //LATD = timerH;
    convertToCentimetres(timeMeasured , distance);
}
