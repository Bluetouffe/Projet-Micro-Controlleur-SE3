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
    unsigned bit3:1;
    unsigned bit4:1;
    unsigned bit5:1;
    unsigned bit6:1;
    unsigned bit7:1;
} flag;

// Global variables used for interrupt of capture mode
// Timer1 MSB variable
unsigned char timerH = 0;
// Timer1 LSB variable
unsigned char timerL = 0;

unsigned int timeOfEmission = 3;
unsigned int numberOfEmission = 8;

char bufferBTReceive[sizeOfBuffer];
char counterStringRXBT = 0;

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALVARIABLES_H */

