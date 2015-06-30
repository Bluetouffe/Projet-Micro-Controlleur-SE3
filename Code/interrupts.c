#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"

void interrupt ISR(void)
{
    if (CCP1IF == 1)                     // Test for Capture interrupt
    {
        timerH = CCPR1H;                // Read captured value MSB
        timerL = CCPR1L;                // Then LSB

        flag.captureDone = 1;
        CCP1IF = 0;                     // Clear Capture1 interrupt flag

        T1CONbits.TMR1ON=0;         // Stop timer1

        TMR1H = 0x00;               // Reset timer1
        TMR1L = 0x00;

        T1CONbits.TMR1ON=1;         // Start timer1
        //resetTMR1();
    }

    if (RCIF == 1)                                      // Interrupt from UART RX
    {
        flag.enableSendBT = 0;                          // Disable BT transmit
        flag.newBTRequest = 0;                          // New BT request is made
        
        bufferBTReceive[counterStringRXBT] = RCREG;     // Read RX REG
        putcUSART(bufferBTReceive[counterStringRXBT]);

        while (bufferBTReceive[counterStringRXBT] == 0x0D
                || counterStringRXBT == sizeOfBuffer) // If enter is pressed
        {
            counterStringRXBT++;                        // Else increment counter
            bufferBTReceive[counterStringRXBT] = RCREG;
        }
        flag.enableSendBT = 1;
        flag.newBTRequest = 1;
        counterStringRXBT = 0;
//        if (bufferBTReceive[counterStringRXBT] == 0x0D
//                || counterStringRXBT == sizeOfBuffer) // If enter is pressed
//        {
//            flag.enableSendBT = 1;                      // Enable TX
//            flag.newBTRequest = 1;                      // New BT request is made
//            counterStringRXBT = 0;                      // Reset counter
//        }
//        else
//        {
//            counterStringRXBT++;                        // Else increment counter
//        }
        
        RCIF = 0;                                       // Reset interrupt flag
    }
}
