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
#include "globalVariables.h"
#include "oled.h"

void ClockInit( void )
{
    OSCCON = 0x30;    // Clock 1MHz
    OSCCONbits.IDLEN = 1;      // Enter IDLE when SLEEP() is called
}

void IOInit( void )
{
    TRISB = 0x00;           // PORT B as Output
    PORTB = 0x00;           // PORT B is 0        
    TRISD = 0x00;           // Use leds as debug output
    PORTD = 0x00;

    TRISCbits.RC3 = 1;      // SCL
    TRISCbits.RC4 = 1;      // SDA

    TRISCbits.RC2 = 1;      // RC2 is input
    ANSELHbits.ANS12 = 0;   // RBO digital input buffer deactivate
    TRISCbits.RC6 = 0;      // TX pin set as output
    TRISCbits.RC7 = 1;      // RX pin set as input  
}

void UARTInit( void )
{
    // To select baudrate
    // 1200bd => baud = 51
    // 2400bd => baud = 25
    // Refer to doc p.251


    unsigned char UARTConfig = 0;
    unsigned char baud = 0;
    // TRISCbits.RC6 = 0; //TX pin set as output
    // TRISCbits.RC7 = 1; //RX pin set as input

    UARTConfig = USART_TX_INT_OFF;      // Interrupt on transmit
    UARTConfig &= USART_RX_INT_OFF;     // Interrupt on receive
    UARTConfig &= USART_ASYNCH_MODE;    // Set Asynchronous mode
    UARTConfig &= USART_EIGHT_BIT;      // Eight bit transmission
    UARTConfig &= USART_BRGH_HIGH;      // High baud rate
    UARTConfig &= USART_SYNC_SLAVE;     // Not used in asynchronous

    baud = 25;                          // Refer to doc
    OpenUSART(UARTConfig,baud);

    flag.enableSendBT = 0;
}

void PWMInit( void )
{
    // Timer 0
    T0CON = 0b00000111;                 // config timer 0
    INTCONbits.TMR0IF = 0;              // Reset Timer0 Interrupt flag
    TMR0H = 0b11111111;                 // nombre de comptage pour periode off val distance
    TMR0L = 0b11111110;                 // nombre de comptage pour periode off val distance

    // PWM output setup (CCP2 on RB3)
    CCP2CON = 0b00001111;               // Disable PWM
    PR2 = 0b00100011;                   // Initial frequency of 440Hz.
    CCPR2L = 0x0F;                      // Volume buzzer
    PIR1bits.TMR2IF = 0;
    T2CON = 0b00000110;                 // TMR2=> prescaler = 4
                                        // f = (TMR2)*4*TOSC*PR2 = 435Hz
    T2CONbits.TMR2ON = 1;               // Start Timer 2

    while (PIR1bits.TMR2IF == 0);
    
    TRISBbits.RB3 = 0; // Enable RB3 output
    CCP2CON = 0x00;

    flag.enableBuzzer = 1;
}

void resetTMR3( void )
{
    T3CONbits.TMR3ON=0;         // Stop timer1

    TMR3H = 0xD8;
    TMR3L = 0xF0;

    T3CONbits.TMR3ON=1;         // Start timer1
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

void Timer3Init( void )
{
    T3CON	= 0x01;
    PIR2bits.TMR3IF = 0;
    TMR3H	= 0xD8;
    TMR3L	= 0xF0;

    INTCON = 0xC0;

    resetTMR3();
}

void interruptInit( void )
{
    //RCONbits.IPEN = 1;          // Enable priority in interrupts
    CCP1IF = 0;                 // Clear Capture module 1 interrupt flag
    RCIF = 0;                   // Cleat RX interrupt flag
    INTCONbits.PEIE = 1;        // Enable interrupts from Peripheral
    INTCONbits.GIE = 1;         // Enable general interrupts
    //INTCONbits.GIEL = 1;         // Enable general interrupts
    //INTCONbits.GIEH = 1;
    INTCONbits.TMR0IE = 1;      // Enable interrupts from Timer 0
    PIE2bits.TMR3IE = 1;        // Enable interrupts from Timer 3
    //INTCON2bits.TMR0IP = 1;     // High pririty interrupt from Timer 0

    PIE1bits.CCP1IE = 1;        // Enable interrupt from Capture module 1
    PIE1bits.RCIE = 1;          // Enable interrupt from UART Receive
}

void watchDogInit( void )
{
    
}

void generalInit( void )
{
    // Init clock at 1MHz
    ClockInit();
    // Set IO pins to desired configuration
    IOInit();
    // Init I2C
    I2CInit();
    // Init CCP1 in capture mode with Timer1
    captureTimer1Init();
    // Init for Timer 3
    Timer3Init();
    // Init UART for 1200b 8N1
    UARTInit();
    // Init interrupts
    interruptInit();
    // Init CCP2 for PWM
    PWMInit();
    // Init Oled
    OLED_Init();


    OLED_clear();
    Delay1KTCYx(1);
    OLED_bmp(LOGO);
    Delay1KTCYx(1);
    OLED_rscroll();

    unsigned char i = 0;

    for ( i = 0 ; i < 7 ; i++)
        Delay10KTCYx(25);

    OLED_stopscroll();
    OLED_clear();
    OLED_string((char *)"Distance : " , 1 , 1 , FONT_8X16);
    OLED_string((char *)"cm" , 100 , 3 , FONT_8X16);

    watchDogInit();
}
