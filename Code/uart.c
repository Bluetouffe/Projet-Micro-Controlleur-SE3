#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"
#include "uart.h"

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

void UARTtreatNewRequest( void )
{
    flag.newBTRequest = 0;
    putsUSART((char*)"\r\n");
    switch ( bufferBTReceive[0] )       // State machine for parameters
    {
        case 0x74:                      // Time between two BT emission "t"
            timeOfEmission = (bufferBTReceive[1] - 48); // 48 = ASCII 0
            if (timeOfEmission > 7)
            {
                timeOfEmission = 7;
            }
            numberOfEmission = 1 << timeOfEmission;
            LATD = numberOfEmission;
            UARTEmptyBuffer();
            flag.enableSendBT = 1;
            break;
        case 0x53: // "S"
            flag.enableSendBT = 1;
            UARTEmptyBuffer();
            break;

        case 0x73: // "s"
            flag.enableSendBT = 0;
            UARTEmptyBuffer();
            break;

        default:
            flag.enableSendBT = 0;
            timeOfEmission = 10;
            UARTEmptyBuffer();
            break;
    }
}

void UARTEmptyBuffer( void )
{
    char i = 0;

    for (i = 0 ; i < sizeOfBuffer ; i++)
    {
        bufferBTReceive[i] = 0;
    }
}
