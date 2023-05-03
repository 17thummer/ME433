#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>
#include "ssd1306.h"
#include "font.h"

void blink(int, int); // blink the LEDs function
void Draw_Char(char letter, unsigned char x, unsigned char y);
void Draw_String(unsigned char *m, unsigned char x, unsigned char y);

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
    i2c_master_setup();
    ssd1306_setup();
	
	// char array for the raw data
    unsigned char raw_data[14];
	// floats to store the data
	float ax, ay, az, gx, gy, gz, temp;
    float time;
    char message[100];


    char m[100];
    sprintf(m,"Tyler's HW8");
    
    while (1) {
		// use core timer for exactly 100Hz loop
        _CP0_SET_COUNT(0);
        
        // reading the z acceleration data from IMU
        burst_read_mpu6050(raw_data);
        az = conv_zXL(raw_data);
        sprintf(message,"z acc = %f\r\n",az);
               
        
        Draw_String(m, 5, 5);
        Draw_String(message, 5, 15);
        //Draw_Char(m[0], 10,10);
        ssd1306_update();
        time = 48000000/_CP0_GET_COUNT();
        sprintf(message,"fps = %f", time);
        Draw_String(message, 5, 25);
        
        blink(1, 250);
        
        while (_CP0_GET_COUNT() < 48000000 / 2 / 100) {
        }
        
    }
}

void Draw_Char(char letter, unsigned char x, unsigned char y){
    int i;
    int j;
    for (j=0; j<5; j++)
    {    // loop for each column of the ASCII letter
       char column = ASCII[letter - 0x20][j];  
       for (i=0; i<8; i++)
       { // loop for each row of the ASCII letter
            ssd1306_drawPixel(x+j,y+i, (column>>i)&0b1);
       }
    }
}

void Draw_String(unsigned char *m, unsigned char x, unsigned char y){
    int k = 0;
    while(m[k] != 0){
        Draw_Char(m[k], x+(6*k), y);
        k++;
    }
    
    
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; // on

        //NU32DIP_YELLOW = 1; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 1; // off
        //NU32DIP_YELLOW = 0; // on
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}

