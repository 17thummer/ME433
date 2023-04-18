#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>
#include "spi.h"    // spi functionality

#define PI 3.14159
#define SAMPLE_NUM 200

int main(void) {
  char message[100];
  unsigned int sine_voltage[SAMPLE_NUM]; //initialize array for sine vals
  unsigned int tri_voltage[SAMPLE_NUM]; // initialize array for tri vals
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
  
  int i = 0;
  float sv = 0;
  float tv = 0;
  // Figure out the voltages for the both waves in a 200 sample loop
  for (i=0; i<SAMPLE_NUM; i++){
    sv = 511.5*sin(i/(PI*5))+511.5;
    if (i<100){
      tv = 10.23 * i;
    }
    else{
      tv = (200 - i)*10.23;
    }
    // convert the voltages to integer values at each array index
    sine_voltage[i] = sv;
    tri_voltage[i] = tv;
  }
  
  sprintf(message, "hello\r\n");
  NU32DIP_WriteUART1(message);
  int counter = 0;
  
  // Enter infinite loop to output voltages
  while (1) {
   
      // Create output for A: [A 1 1 1 [10 bit voltage] 0 0]
      unsigned short sin_output = 0;
      sin_output = 0b111<<12;
      
      unsigned char a = 0b0; //for the sine channel
      sin_output = sin_output | (a<<15);
      
      sin_output = sin_output | (sine_voltage[counter]<<2); // for the actual voltage
      

      // Create output for B: [B 1 1 1 [10 bit voltage] 0 0]      
      unsigned short tri_output = 0;
      tri_output = 0b111<<12;
      
      unsigned char b = 0b1; // for the triangle channel
      tri_output = tri_output | (b<<15);
      
      tri_output = tri_output | (tri_voltage[counter]<<2); // for the actual voltage
      
      // Send Desired Values for A
      CS_PIN = 0b0; // tell the SPI data is coming
      unsigned char nothing = spi_io(sin_output>>8); // send the first half
      nothing = spi_io(sin_output); // send the second half
      CS_PIN = 0b1; // set it back high

      // Send desired Values for B
      CS_PIN = 0b0; // tell the SPI data is coming
      nothing = spi_io(tri_output>>8); // send the first half
      nothing = spi_io(tri_output); // send the second half
      CS_PIN = 0b1; // set it back high      
      
      
      // wait for ____ sec before next loop iteration
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT()<120000){
              ;
      }      
      
      // Counter add or reset
      if (counter<SAMPLE_NUM-1){
          counter++; // move to next counter value
      }
      else{
          counter = 0; // reset counter back to zero
      }
  }
}


		
