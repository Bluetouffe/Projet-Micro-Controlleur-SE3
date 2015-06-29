/* 
 * File:   main.c
 * Author: Louis
 *
 * Created on 23 juin 2015, 16:50
 */

#include <xc.h>
#include <delays.h>
#include "p18f45K20.h"
#include "initialisation.h"
#include "sonarModule.h"

void UARTSendMeasure(unsigned int distance)
{
    // This message will be sent first
    char *messageTXStart = (char*)"Distance = ";

    // This one will be the end of the message
    // \n\r is CR LF, an old way to end a line
    char *messageTXEnd = (char*)" cm\n\r";

    // String used for storing distance ASCII value
    // Initialized with 48, ASCII code of 0
    char messageDistance[4] = {48 , 48 , 48 , 0};

    // Error message
    char *errorDistanceMessage = (char*)"Error measurement\n\r";

    // If distance > 1000, we will have more than 3 digits and be out of
    // boundaries (650 cm) If we are under this distance we can safely send the
    // message
    if (distance < 1000)
    {
        // Create a string composed of Hundreds, Tens and Units
        messageDistance[0] += distance / 100;
        messageDistance[1] += (distance - ((messageDistance[0] - 48) * 100)) / 10;
        messageDistance[2] += distance % 10;

        putsUSART(messageTXStart);
        putsUSART(messageDistance);
        putsUSART(messageTXEnd);
    }

    // If distance < 1000
    // Here distance is > 1000, meaning an error occured while measuring
    else
    {
        putsUSART(errorDistanceMessage);
    }
}

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
        // Request a measure and store returned value in distance
        getMeasure(&distance);

        if (numberOfLoop == numberOfLoopToTransmit)
        {
            // Send this value to UART
            UARTSendMeasure(distance);
            // Reset Loop counter
            numberOfLoop = 0;
        }
        else
        {
            // Increment loop counter
            numberOfLoop++;
        }
        // Delay1KTCYx(x) generate a delay of
        // 1000 * x * 1/(Fosc/4)
        // Here, 1000 * 25 * 1/250000 = 100 ms
        Delay1KTCYx( 25 );
    }
}
