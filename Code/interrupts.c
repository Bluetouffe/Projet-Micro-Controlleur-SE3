#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"

void interrupt _isr (void)
{
    if (CCP1IF == 1)                     // Test for Capture interrupt
    {
        CCP1IF = 0;                     // Clear Capture1 interrupt flag
        
        timerH = CCPR1H;                // Read captured value MSB
        timerL = CCPR1L;                // Then LSB

        flag.captureDone = 1;
    }
}
