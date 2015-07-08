#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"
#include "uart.h"

void createString( unsigned int distance )
{
    // String used for storing distance ASCII value
    
    if (distance < 1000)
    {
        messageDistance[0] = tabValeurASCII[distance][0];
        messageDistance[1] = tabValeurASCII[distance][1];
        messageDistance[2] = tabValeurASCII[distance][2];
        messageDistance[3] = 0;
    }

}

void UARTSendMeasure(unsigned int distance)
{
    // This message will be sent first
    char *messageTXStart = (char*)"D = ";

    // This one will be the end of the message
    // \n\r is CR LF, an old way to end a line
    char *messageTXEnd = (char*)" cm\n\r";


    // Error message
    char *errorDistanceMessage = (char*)"Error measurement\n\r";

    // If distance > 1000, we will have more than 3 digits and be out of
    // boundaries (650 cm) If we are under this distance we can safely send the
    // message
    if (distance < 1000)
    {
        // Create a string composed of Hundreds, Tens and Units
        createString(distance);

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
        case 0x53: // "S"
            flag.enableSendBT = 1;
            UARTEmptyBuffer();
            break;

        case 0x73: // "s"
            flag.enableSendBT = 0;
            UARTEmptyBuffer();
            break;

        case 0x42: // "B"
            flag.enableBuzzerBT = 1;
            UARTEmptyBuffer();
            break;
            
        case 0x62: // "b"
            flag.enableBuzzerBT = 0;
            UARTEmptyBuffer();
            break;

        default:
            flag.enableSendBT = 0;
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
