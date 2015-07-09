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
    CLRWDT();

    // Variable used to store measured distance
    // unsigned int distance = 0;

    // nulber of Loop, to emit every numberOfLoopToTransmit loops
    unsigned char numberOfLoop = 0;
    T0CONbits.TMR0ON = 1;               // Enable timer 0
    watchDogEnable();
    // Main loop
    while(1)
    {
        startMeasure();                          // Request a measure and store returned value in distance
        
        if (flag.newBTRequest)
            UARTtreatNewRequest();

        if (numberOfLoop == 5)
        {
            if (flag.enableSendBT)
            {
                UARTSendMeasure(distance);                  // Send this value through UART
            }
            else
            {
                createString(distance);
            }

            OLED_clear();
            OLED_string(messageDistance,0,0,FONT_NUMBERS_24X40);
            OLED_pos(distance>>2,0); // pour la voiture x de 0 à 65
            OLED_bmp(CAR);
        }
         
        getMeasure(&distance);                   // After capture is done, get value


        if (numberOfLoop == 5)
        {
            numberOfLoop = 0;                           // Reset Loop counter
        }
        else
        {
            numberOfLoop++;// Increment loop counter
        }

        Delay1KTCYx(5);

        CLRWDT();
    }
}
