#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>
#include "i2c_master_noint.h"    // i2c functionality

void heartbeat(void);
void generic_i2c_write(unsigned char address, unsigned char reg, unsigned char value);
void turn_on_GP7();
int read_from_GP0(void);

int main(void) {
  char message[100];
  char buffer[100];

  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  i2c_master_setup(); // setup the i2c communications
  
  // init the chip with GP0 as input, GP7 as output
  unsigned char init_address = 0b01000000;
  unsigned char init_reg = 0x00; // for IODIR
  unsigned char init_val = 0b00000001;
  generic_i2c_write(init_address, init_reg, init_val);
  

  int counter = 0;
  
  // Enter infinite loop to output voltages
  while (1) {
      // add a heartbeat to know if we are still looping in the while loop
//      heartbeat();
//  
      int button = read_from_GP0();
      if (button == 0){
      // Turn on GP7
      //unsigned char GP7_address = ;
      //unsigned char GP7_reg = ; // for OLAT
      //unsigned char GP7_val_on = ;
      generic_i2c_write(0b01000000, 0x0A, 0b10000000);
      }
      else{
      // Turn off GP7
      //unsigned char GP7_val_off = 0b00000000;
      generic_i2c_write(0b01000000, 0x0A, 0b00000000);
      }
      
      //_CP0_SET_COUNT(0);
      //while (_CP0_GET_COUNT()< 12000*1000){}
      

      // heartbeat
      NU32DIP_GREEN = !NU32DIP_GREEN;
      _CP0_SET_COUNT(0);
      while (_CP0_GET_COUNT()< 12000*1000){}
      
  }
}

void turn_on_GP7(){
    // 1. send start bit
    
    // 2. send the address of the chip
        // 0b01000000 for write to chip
        // 0b01000001 for read from the chip
    // 3. send the register name
        // 0x0A OLAT
    // 4. send the value to turn on GP7
        // 0b1 for turn on
    // 5. send stop bit
}

void generic_i2c_write(unsigned char address, unsigned char reg, unsigned char value){
    // 1. send start bit
    i2c_master_start();
    // 2. send the address of the chip
    i2c_master_send(address);
    // 3. send the register name
    i2c_master_send(reg);
    // 4. send the value to turn on GP7
    i2c_master_send(value);
    // 5. send stop bit
    i2c_master_stop();
} 

int read_from_GP0(){
    // 1. send start bit
    i2c_master_start();    
    // 2. send address with write bit
    i2c_master_send(0b01000000);
    // 3. send register we want to read from
    i2c_master_send(0x09);
    // 4. restart
    i2c_master_restart();
    // 5. send address with read bit
    i2c_master_send(0b01000001);
    // 6. call the receive function into an unsigned char
    unsigned char r = i2c_master_recv();
    //unsigned char r = recv();
    // 7. send the ACK bit
    i2c_master_ack(1);
    // 8. send the stop bit
    i2c_master_stop();
    //return (r&0b00000001); // convenient bc it's the last register
    return (r&0b1);
}

		
void heartbeat(void){
	int i;
	unsigned int t;
	for (i=0; i< 1; i++){
		
		NU32DIP_YELLOW = 1; // off
		t = _CP0_GET_COUNT(); // should really check for overflow here
		// the core timer ticks at half the SYSCLK, so 24000000 times per second
		// so each millisecond is 24000 ticks
		// wait half in each delay
		while(_CP0_GET_COUNT() < t + 12000*500){}
		
		
		NU32DIP_YELLOW = 0; // on
		t = _CP0_GET_COUNT(); // should really check for overflow here
		while(_CP0_GET_COUNT() < t + 12000*500){}
	}
}