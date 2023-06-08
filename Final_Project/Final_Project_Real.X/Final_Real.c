#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "ws2812b.h"
#include "uart2.h"


int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    UART2_Startup();
    ws2812b_setup();
    wsColor LEDstrip[3];
    char message[100];
    
    
    // Initialize Timer2 for Servo Motor
    T2CONbits.TCKPS = 0b100;     // Timer4 prescaler 16:1
    PR2 = 60000;              // 20 Hz for the PWM
    TMR2 = 0;                // initial TMR2 count is 0
    T2CONbits.ON = 1;        // turn on Timer2
    
    // Servo Motor Stuff
    RPA3Rbits.RPA3R = 0b0101; //A3 is OC3
    OC3CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC3CONbits.OCTSEL = 0;   // Use timer2
    OC3RS = 6500;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC3R = 6500;              // initialize before turning OC1 on; afterward it is read-only
    OC3CONbits.ON = 1;       // turn on OC3

    // Initialize Timer3 for Wheel Motors 
    T3CONbits.TCKPS = 0b000;     // Timer3 prescaler (1:1)
    PR3 = 2400-1;              // 20 kHz for the PWM
    TMR3 = 0;                // initial TMR2 count is 0
    T3CONbits.ON = 1;        // turn on Timer2
    
    // Motor 1 Stuff
    RPA0Rbits.RPA0R = 0b0101; //A0 is OC1
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 0;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC1R = 0;              // initialize before turning OC1 on; afterward it is read-only
    OC1CONbits.OCTSEL = 1;   // Use timer3
    OC1CONbits.ON = 1;       // turn on OC1
    
    // Initialize the digital IO Pin for Motor 1
    TRISBbits.TRISB7 = 0; //Set the direction pin to output
    LATBbits.LATB7 = 1; //Set pin RB7 to high
    
    // Motor 2 Stuff
    RPB11Rbits.RPB11R = 0b0101; //B11 is OC2
    OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC2RS = 0;             // duty cycle = OC2RS/(PR3+1) = 25%
    OC2R = 0;              // initialize before turning OC2 on; afterward it is read-only
    OC2CONbits.OCTSEL = 1;   // Use 3
    OC2CONbits.ON = 1;       // turn on OC2
    
    // Initialize the digital IO Pin for Motor 2
    TRISBbits.TRISB8 = 0; //Set the direction pin to output
    LATBbits.LATB8 = 1; //Set pin RB7 to high
    
    
    



    float hue = 0;
    float sat = 1;
    float brightness = 0.5;
    
    int direction = 1;
    while (1) {
        _CP0_SET_COUNT(0);
        int com = 0;
        
        // uart2_flag() is 1 when uart2 has rx a message and sprintf'd it into a value
        if(get_uart2_flag()){
            set_uart2_flag(0); // set the flag to 0 to be ready for the next message
            com = get_uart2_value();
            sprintf(message,"%d\r\n",com);
            NU32DIP_WriteUART1(message);
        }
        
        if (com < 25){
            LATBbits.LATB7 =0;
            OC1RS = 40 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
            LATBbits.LATB8 =1;
            OC2RS = 60 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
        }
        
        if (com > 35){
            LATBbits.LATB7 =0;
            OC1RS = 60 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
            LATBbits.LATB8 =1;
            OC2RS = 40 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
        }
        
        else{
            LATBbits.LATB7 =0;
            OC1RS = 50 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
            LATBbits.LATB8 =1;
            OC2RS = 50 * (PR3-5) / 100;  // duty cycle = OC1RS/(PR2+1) = (PWM_val input/100) to scale * 2400 by the range
        }
        
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
        
        for (int i=0; i<3; i++){
            
            LEDstrip[i] = HSBtoRGB(hue, sat, brightness);
            if (hue < 360){
                hue = hue + 1;
            }
            else{
                hue = 0;
            }
        }
        ws2812b_setColor(LEDstrip, 3);

        while (_CP0_GET_COUNT() < 600000) {
            ;
        }
    }
}



