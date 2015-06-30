/* 
 * File:   main.c
 * Author: Louis BONICEL
 *
 * Created on 23 juin 2015, 16:50
 */

#include <xc.h>
#include <delays.h>
#include "p18f45K20.h"
#include "initialisation.h"
#include "sonarModule.h"
#include "uart.h"
#include "interrupts.h"
#include "globalVariables.h"

void main( void )
{
    generalInit();
    // Variable used to store measured distance
    unsigned int distance = 0;

    // Data are sent every numberOfLoopToTransmit loops
    unsigned int numberOfLoopToTransmit = 10;

    // nulber of Loop, to emit every numberOfLoopToTransmit loops
    unsigned char numberOfLoop = 0;

    // Main loop
    while(1)
    {
        startMeasure();                          // Request a measure and store returned value in distance

        SLEEP();
        NOP();
        
        getMeasure(&distance);                  // After capture is done, get value

        if (numberOfLoop == numberOfLoopToTransmit)
        {
            UARTSendMeasure(distance);                  // Send this value to UART
            numberOfLoop = 0;                           // Reset Loop counter
        }
        else
        {
            numberOfLoop++;                             // Increment loop counter
        }
        // Delay1KTCYx(x) generate a delay of
        // 1000 * x * 1/(Fosc/4)
        // Here, 1000 * 25 * 1/250000 = 100 ms
        Delay1KTCYx( 25 );
    }
}
