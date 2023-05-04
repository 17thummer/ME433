#include "nu32dip.h" // constants, functions for startup and UART
#include <stdio.h>
#include "ws2812b.h"


int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    ws2812b_setup();
    wsColor LEDstrip[3];
    
    LEDstrip[0].g = 255;
    LEDstrip[0].r = 0;
    LEDstrip[0].b = 0;
    LEDstrip[1].g = 0;
    LEDstrip[1].r = 255;
    LEDstrip[1].b = 0;
    LEDstrip[2].g = 0;
    LEDstrip[2].r = 0;
    LEDstrip[2].b = 255;
    
    while (1) {
        ws2812b_setColor(LEDstrip, 3);
    }
}



