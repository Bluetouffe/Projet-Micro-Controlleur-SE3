/* 
 * File:   globalVariables.h
 * Author: Louis
 *
 * Created on 29 juin 2015, 16:39
 */

#ifndef GLOBALVARIABLES_H
#define	GLOBALVARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define sizeOfBuffer 10

volatile near struct {
    unsigned captureDone:1;
    unsigned enableSendBT:1;
    unsigned newBTRequest:1;
    unsigned enableBuzzer:1;
    unsigned timeElapsed:1;
    unsigned sendReady:1;
    unsigned enableBuzzerBT:1;
    unsigned bit7:1;
} flag;

// Global variables used for interrupt of capture mode
// Timer1 MSB variable
unsigned char timerH = 0;
// Timer1 LSB variable
unsigned char timerL = 0;

unsigned int numberOfEmission = 8;

char bufferBTReceive[sizeOfBuffer];
char counterStringRXBT = 0;

char messageDistance[4];

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALVARIABLES_H */

