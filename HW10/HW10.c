#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "ws2812b.h"


int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    ws2812b_setup();
    wsColor LEDstrip[8];
    
    float hue = 0;
    float sat = 1;
    float brightness = 1;
    while (1) {
        
        for (int i=0; i<8; i++){
            
            LEDstrip[i] = HSBtoRGB(hue, sat, brightness);
            if (hue < 360){
                hue = hue + 1;
            }
            else{
                hue = 0;
            }
        }
        ws2812b_setColor(LEDstrip, 8);
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 600000) {
            ;
        }
    }
}



