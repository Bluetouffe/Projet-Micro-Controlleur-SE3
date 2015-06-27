/* 
 * File:   initilisation.h
 * Author: Louis
 *
 * Created on 27 juin 2015, 14:03
 */

#ifndef INITIALISATION_H
#define	INITIALISATION_H

#ifdef	__cplusplus
extern "C" {
#endif

void ClockInit( void );
void IOInit( void );
void UARTInit( void );
void PWMCCP2init( void );
void captureTimer1Init( void );
void resetTMR1( void );
void generalInit( void );

#ifdef	__cplusplus
}
#endif

#endif	/* INITIALISATION_H */

