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
    }

    if (RCIF == 1)                                      // Interrupt from UART RX
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
            flag.enableSendBT = 1;
            flag.newBTRequest = 1;
            counterStringRXBT = 0;
        }
        
        RCIF = 0;                                       // Reset interrupt flag
    }

    if (INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;	//efface le drapeau d'IT
        if (signal_pwm == 0)
        {
            LATD = 0x0F;
            TMR0ON = 0;                 // Disable timer 0
            signal_pwm = 1;
            TMR0H = 0xFF;         //val distance fixe ON 200ms
            TMR0L = 0xF0;         //val distance fixe ON 200ms
            TMR0ON = 1;                 // Enable timer 0
        }
        else
        {
            LATD = 0xF0;
            TMR0ON = 0;                 // Disable le timer 0
            signal_pwm = 0;
            TMR0H = 0xFF;         //val distance louis
            TMR0L = 0xF0;
            TMR0ON = 1;                 // Disable timer 0
        }
    }
}
