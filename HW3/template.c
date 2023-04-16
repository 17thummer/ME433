#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>

#define PI 3.14159

int main(void) {
  char message[100];
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  
  while (1) {
      int i = 0;
      for (i=0; i<100; i++){
          sprintf(message, "%f\r\n", sin(i*2*PI/100));
          NU32DIP_WriteUART1(message);
          _CP0_SET_COUNT(0);
          while(_CP0_GET_COUNT()<2400000){
              ;
          }
      }
  }
}


		
