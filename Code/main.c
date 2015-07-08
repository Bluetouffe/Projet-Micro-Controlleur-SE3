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
    unsigned int distance = 0;
    unsigned int moyenne = 0;

    // nulber of Loop, to emit every numberOfLoopToTransmit loops
    unsigned char numberOfLoop = 0;
    T0CONbits.TMR0ON = 1;               // Enable timer 0
    watchDogEnable();
    // Main loop
    while(1)
    {
        LATDbits.LATD1 = 1;
        startMeasure();                          // Request a measure and store returned value in distance
        while (!flag.timeElapsed)
        {             
            if (flag.newBTRequest)
                UARTtreatNewRequest();
            

            if (numberOfLoop == 7)
            {
                moyenneFinale = moyenne >> 3;
                if (flag.enableSendBT && !flag.sendDone)
                {
                    UARTSendMeasure(moyenneFinale);                  // Send this value through UART
                    flag.sendDone = 1;
                }
                else
                {
                    createString(moyenneFinale);
                }
                OLED_string(messageDistance , 50 , 3 , FONT_8X16);
                
                //numberOfLoop = 0;                           // Reset Loop counter
                moyenne = 0;
            }
            CLRWDT();
        }
         
        getMeasure(&distance);                   // After capture is done, get value


        if (numberOfLoop == 7)
        {
            numberOfLoop = 0;                           // Reset Loop counter
            flag.sendDone = 0;
        }
        else
        {
            moyenne += distance;
            numberOfLoop++;// Increment loop counter
        }
        
        if (flag.enableBuzzer && flag.enableBuzzerBT)
        {
            CCP2CON = 0x0F; // Enable PWM mode
        }
        else
        {
            CCP2CON = 0x00;
        }
        CLRWDT();
        LATDbits.LATD1 = 0;
    }
}