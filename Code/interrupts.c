#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"

void interrupt ISR(void)
{
    if (CCP1IF)                     // Test for Capture interrupt
    {
        timerH = CCPR1H;                // Read captured value MSB
        timerL = CCPR1L;                // Then LSB

        TMR1H = 0x00;               // Reset timer1
        TMR1L = 0x00;

        flag.captureDone = 1;
        CCP1IF = 0;                     // Clear Capture1 interrupt flag
        T1CONbits.TMR1ON=0;         // Stop timer1
    }
    
    if (TMR3IF)
    {
        flag.timeElapsed = 1;
        PIR2bits.TMR3IF = 0;
        
        TMR3H = 0x9E;
        TMR3L = 0x58;
    }

    if (RCIF)                                      // Interrupt from UART RX
    {
        flag.enableSendBT = 0;                          // Disable BT transmit
        flag.newBTRequest = 0;                          // New BT request is made
        
        bufferBTReceive[counterStringRXBT] = RCREG;     // Read RX REG
        putcUSART(bufferBTReceive[counterStringRXBT]);

        if (bufferBTReceive[counterStringRXBT] != 0x0D && counterStringRXBT != sizeOfBuffer) // If enter is pressed
        {
            counterStringRXBT++;                        // Else increment counter
            bufferBTReceive[counterStringRXBT] = RCREG;
        }
        else
        {
            flag.enableSendBT = 0;
            flag.newBTRequest = 1;
            counterStringRXBT = 0;
        }
    }

    if (TMR0IF)
    {
        INTCONbits.TMR0IF = 0;	//efface le drapeau d'IT
        if (!flag.enableBuzzer)
        {
            TMR0ON = 0;                 // Disable timer 0
            flag.enableBuzzer = 1;
            TMR0H = 0xFF;         //val distance fixe ON 200ms
            TMR0L = 0xF0;         //val distance fixe ON 200ms
            TMR0ON = 1;                 // Enable timer 0
        }
        else
        {
            TMR0ON = 0;                 // Disable le timer 0
            flag.enableBuzzer = 0;
            TMR0H = tableauBuzzer[moyenneFinale][0];         //val distance louis
            TMR0L = tableauBuzzer[moyenneFinale][1];
            TMR0ON = 1;                 // Disable timer 0
        }
    }
}
