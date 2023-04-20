#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>
#include "i2c_master_noint.h"    // i2c functionality

int main(void) {
  char message[100];

  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  

  
  sprintf(message, "hello\r\n");
  NU32DIP_WriteUART1(message);
  int counter = 0;
  
  // Enter infinite loop to output voltages
  while (1) {
      ;
  }
}


		
