#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"
#include "globalVariables.h"

//void interrupt ISR(void)
//{
//    LATDbits.LATD1 = ~LATDbits.LATD1;
//    if (CCP1IF == 1)                     // Test for Capture interrupt
//    {
//        timerH = CCPR1H;                // Read captured value MSB
//        timerL = CCPR1L;                // Then LSB
//
//        flag.captureDone = 1;
//
//        CCP1IF = 0;                     // Clear Capture1 interrupt flag
//    }
//}
