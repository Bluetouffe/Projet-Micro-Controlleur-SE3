/* 
 * File:   interrupts.h
 * Author: Louis
 *
 * Created on 29 juin 2015, 16:20
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif

//void interrupt high_priority HIGH_ISR (void);
void interrupt _ISR (void);

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

