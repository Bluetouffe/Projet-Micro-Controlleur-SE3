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
    unsigned char i = 0;

    generalInit();

    Delay1KTCYx(1);
    OLED_bmp(LOGO);
    Delay1KTCYx(1);
    OLED_rscroll();
    
    for ( i = 0 ; i < 7 ; i++)
        Delay10KTCYx(25);

    OLED_stopscroll();
    OLED_clear();
    OLED_string((char *)"Distance : " , 1 , 1 , FONT_8X16);
    OLED_string((char *)"cm" , 100 , 3 , FONT_8X16);

    // Variable used to store measured distance
    unsigned int distance = 0;
    //unsigned int moyenne = 0;

    // nulber of Loop, to emit every numberOfLoopToTransmit loops
    unsigned char numberOfLoop = 0;
    T0CONbits.TMR0ON = 1;               // Enable timer 0
    signal_pwm = 1;
    // Main loop
    while(1)
    {
        startMeasure();                          // Request a measure and store returned value in distance

        SLEEP();                                 // Enter IDLE mode
        NOP();                                   // This instruction is skipped
        
        getMeasure(&distance);                   // After capture is done, get value

        if (flag.newBTRequest == 1)
        {
            UARTtreatNewRequest();
        }

        if (signal_pwm == 1)
        {
            CCP2CON = 0b00000111; // Enable PWM mode
        }
        else
        {
            CCP2CON = 0b00000000; // Disable PWM mode
        }


        //if (flag.enableSendBT == 1)
        //{
            if (numberOfLoop == numberOfEmission)
            {
                //moyenne >>= timeOfEmission;
                UARTSendMeasure(distance);                  // Send this value through UART
                OLED_string(messageDistance , 50 , 3 , FONT_8X16);
                numberOfLoop = 0;                           // Reset Loop counter
                //moyenne = 0;
            }
            else
            {
                //moyenne += distance;
                numberOfLoop++;                             // Increment loop counter
            }
        //}

        Delay1KTCYx( 25 );
    }
}