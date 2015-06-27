/*
 * File:   initilisation.c
 * Author: Louis
 *
 * Created on 27 juin 2015, 14:03
 */
// Configuration Registers
#pragma config FOSC=INTIO7, FCMEN=OFF, IESO=OFF, PWRT=OFF, BOREN=OFF
#pragma config BORV=30, WDTEN=OFF, WDTPS=1, MCLRE=ON, HFOFST=ON
#pragma config LPT1OSC=OFF, PBADEN=OFF, CCP2MX=PORTBE, STVREN=OFF
#pragma config LVP=OFF, XINST=OFF, CP0=OFF, CP1=OFF, CP2=OFF
#pragma config CP3=OFF, CPB=OFF, CPD=OFF, WRT0=OFF, WRT1=OFF
#pragma config WRT2=OFF, WRT3=OFF, WRTB=OFF, WRTC=OFF, WRTD=OFF
#pragma config EBTR0=OFF, EBTR1=OFF, EBTR2=OFF, EBTR3=OFF
#pragma config EBTRB=OFF


#include "p18f45K20.h"
#include <usart.h>
#include "initialisation.h"

void ClockInit( void )
{
    OSCCON = 0b00110000;    // Clock 1MHz
}

void IOInit( void )
{
    TRISBbits.RB0 = 0;      // RBO output
    TRISCbits.RC2 = 1;      // RC2 is input
    ANSELHbits.ANS12 = 0;   // RBO digital input buffer deactivate
}

void UARTInit( void )
{
    // To select baudrate
    // 1200bd => baud = 51
    // 2400bd => baud = 25
    // Refer to doc p.251


    unsigned char UARTConfig = 0;
    unsigned char baud = 0;
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input

    UARTConfig = USART_TX_INT_OFF;      // Interrupt on transmit
    UARTConfig &= USART_RX_INT_OFF;     // Interrupt on receive
    UARTConfig &= USART_ASYNCH_MODE;    // Set Asynchronous mode
    UARTConfig &= USART_EIGHT_BIT;      // Eight bit transmission
    UARTConfig &= USART_BRGH_HIGH;      // High baud rate
    UARTConfig &= USART_SYNC_SLAVE;     // Not used in asynchronous

    baud = 25;                          // Refer to doc
    OpenUSART(UARTConfig,baud);
}

void PWMCCP2init( void )
{
    // L.Latorre
    // PWM output setup (CCP2 on RB3)
    // --------------------------------------------------------------------
    CCP2CON = 0b00001111;       // CCP2 as PWM mode, LSB's set to 0
    PR2 = 0xFF;                 // Initial Period
    CCPR2L = 0x1F;              // Initial duty cycle (MSB's)
    PIR1bits.TMR2IF = 0;        // Clears Timer 2 interrupt flag
    T2CON = 0b00000000;         // Timer 2 prescaler is 1
    T2CONbits.TMR2ON = 1;       // Launch Timer 2
    while (PIR1bits.TMR2IF==0);
    {
        TRISBbits.RB3 = 0;      // Enable RB3 output
    }
}

void resetTMR1( void )
{
    T1CONbits.TMR1ON=0;         // Stop timer1

    TMR1H = 0x00;               // Reset timer1
    TMR1L = 0x00;

    T1CONbits.TMR1ON=1;         // Start timer1
}

void captureTimer1Init( void )
{
    // Timer1 Setup
    PIR1bits.TMR1IF = 0;        // Resets the Timer1 flag bit
    T1CONbits.RD16 = 1;         // Timer1 is 16 bits
    T1CONbits.T1RUN = 0;        // Timer1 is not master clock
    T1CONbits.T1CKPS0 = 0;      // No prescaler
    T1CONbits.T1CKPS1=0;
    T1CONbits.T1OSCEN = 0;      // Timer1 oscillator off
    T1CONbits.NOT_T1SYNC=0;     // Not externally synchronized
    T1CONbits.TMR1CS=0;         // Fosc/4 as source

    resetTMR1();

    //Capture setup
    CCP1CON = 0x04;
}

void generalInit( void )
{
    // Init clock at 1MHz
    ClockInit();
    // Set IO pins to desired configuration
    IOInit();
    // Init CCP1 in capture mode with Timer1
    captureTimer1Init();
    // Init CCP2 for PWM
    PWMCCP2init();
    // Init UART for 1200b 8N1
    UARTInit();
}

