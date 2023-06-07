#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "ws2812b.h"



int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    ws2812b_setup();
    
    // Initialize Timer2 for Motor 1
    RPA0Rbits.RPA0R = 0b0101; //A0 is OC1
    T2CONbits.TCKPS = 0b000;     // Timer2 prescaler (1:1)
    PR2 = 2400-1;              // 20 kHz for the PWM
    TMR2 = 0;                // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 0;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC1R = 0;              // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1

    // Initialize the digital IO Pin for Motor 1
    TRISBbits.TRISB7 = 0; //Set the direction pin to output
    LATBbits.LATB7 = 1; //Set pin RB7 to high
    
    // Initialize Timer3 for Motor 2
    RPA1Rbits.RPA1R = 0b0101; //A1 is OC2
    T3CONbits.TCKPS = 0b000;     // Timer3 prescaler (1:1)
    PR3 = 2400-1;              // 20 kHz for the PWM
    TMR3 = 0;                // initial TMR2 count is 0
    OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC2RS = 0;             // duty cycle = OC2RS/(PR3+1) = 25%
    OC2R = 0;              // initialize before turning OC2 on; afterward it is read-only
    T3CONbits.ON = 1;        // turn on Timer2
    OC2CONbits.ON = 1;       // turn on OC1

    // Initialize the digital IO Pin for Motor 1
    TRISBbits.TRISB8 = 0; //Set the direction pin to output
    LATBbits.LATB8 = 1; //Set pin RB7 to high

    while (1) {
        LATBbits.LATB7 =1;
        OC1RS = 50 * (PR2-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
        
        LATBbits.LATB8 =1;
        OC2RS = 50 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
    }
}



