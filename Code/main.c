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
#include "oled.h"

void main( void )
{
    generalInit();

    // Variable used to store measured distance
    unsigned int distance = 0;
    unsigned int moyenne = 0;

    // nulber of Loop, to emit every numberOfLoopToTransmit loops
    unsigned char numberOfLoop = 0;
    T0CONbits.TMR0ON = 1;               // Enable timer 0
    
    // Main loop
    while(1)
    {
        startMeasure();                          // Request a measure and store returned value in distance

        SLEEP();                                 // Enter IDLE mode
        NOP();                                   // This instruction is skipped
         
        getMeasure(&distance);                   // After capture is done, get value

        if (flag.newBTRequest)
            UARTtreatNewRequest();

        CCP2CON = signal_pwm;

        if (numberOfLoop == 8)
        {
            moyenne >>= 3;
            if (flag.enableSendBT == 1)
            {
                UARTSendMeasure(moyenne);                  // Send this value through UART
            }
            else
            {
                createString(moyenne);
            }
            OLED_string(messageDistance , 50 , 3 , FONT_8X16);
            numberOfLoop = 0;                           // Reset Loop counter
            moyenne = 0;
        }
        else
        {
            moyenne += distance;
            numberOfLoop++;// Increment loop counter
        }

        //Delay1KTCYx( 25 );
    }
}