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

volatile near struct {
    unsigned captureDone:1;
    unsigned bit1:1;
    unsigned bit2:1;
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

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALVARIABLES_H */

