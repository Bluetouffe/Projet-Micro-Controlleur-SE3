/* 
 * File:   uart.h
 * Author: Louis
 *
 * Created on 29 juin 2015, 16:27
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void UARTSendMeasure(unsigned int distance);
void UARTtreatNewRequest( void );
void UARTEmptyBuffer( void );

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

