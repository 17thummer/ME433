#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "ws2812b.h"


int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    ws2812b_setup();
    wsColor LEDstrip[3];
    
    // Initialize Timer4 for Servo
    RPA3Rbits.RPA3R = 0b0101; //A3 is OC3
    T2CONbits.TCKPS = 0b100;     // Timer4 prescaler 16:1
    PR2 = 60000;              // 20 Hz for the PWM
    TMR2 = 0;                // initial TMR2 count is 0
    OC3CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC3RS = 6500;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC3R = 6500;              // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;        // turn on Timer2
    OC3CONbits.ON = 1;       // turn on OC1

    int direction = 1;
    while (1) {
        ;
        _CP0_SET_COUNT(0);
        if (direction == 1){
            if (OC3RS < 6500){
                OC3RS = OC3RS + 10;
            }
            else{
                direction = 0;
            }
        }
        if (direction ==0){
            if (OC3RS > 3000){
                OC3RS = OC3RS - 10;
            }
            else{
                direction = 1;
            }
        }
        
        while (_CP0_GET_COUNT() < 60000) {
            ;
        }
    }
}



