/* 
 * File:   sonarModule.h
 * Author: Louis
 *
 * Created on 27 juin 2015, 14:09
 */

#ifndef SONARMODULE_H
#define	SONARMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MB1010_PWM_PIN  PORTCbits.RC2   //CCP1 PIN
#define MB1010_EN_PIN   PORTBbits.RB0

void convertToCentimetres(unsigned int measuredTime , unsigned int * distance);
void startMeasure( void );
void getMeasure(unsigned int * distance);

#ifdef	__cplusplus
}
#endif

#endif	/* SONARMODULE_H */

