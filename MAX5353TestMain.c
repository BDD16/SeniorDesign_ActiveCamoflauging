// MAX5353TestMain.c
// Runs on LM4F120
// Test the functions provided in MAX5353.c by outputting
// a sine wave at a particular frequency.
// Daniel Valvano
// November 5, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012

 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic

// SSI1clk connected to PD0
// SSI1Fss connected to PD1
// SSI1Tx  connected to PD3

#include "MAX5353.h"
#include "SysTick.h"
#include "heartbeat.h"
#include "PLL.h"
#include "ADCSWTrigger.h"
#include "PhaseDetector.h"

void DisableInterrupts(void); // Disable interrupts

unsigned long PhaseDetector[2];  //save the ADC values into an array
//PhaseDetector[0] --Detector1
//PhaseDetector[1] --Detector2

unsigned int Mailbox;

// 12-bit 32-element sine wave
// multiply each value by 2 to shift into bits 12:1 of SSI packet
// three control bits in 15:13 are all zero for immediate DAC update
// book figure shows MAX5353 in unipolar rail-to-rail configuration
// that means when wave[n] = 0x0000 (LSB = 0), output = 0
//                 wave[n] = 0x1000 (LSB = 0), output = Vref
//                 wave[n] = 0x1FFE (LSB = 0), output = 2*Vref
const unsigned short wave[32] = {
  2048,2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
  3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
  156,39,0,39,156,345,600,910,1264,1648};
const unsigned short square[2] = {0,4095};
int main(void){
	//Initialize local variables to Main.c
    unsigned int i=0;
	unsigned int j=0;
	signed int incidence=0;
	unsigned long detector1=0;
	unsigned long detector2=0;
	unsigned int I,Q;
	
	//Initialize external variables, or variables that may be used elsewhere
	Mailbox=0;//Initialize the Mailbox
	
	//TI Launchpad Initialization Ritual
	
	PLL_Init();		       //set clock to 50MHz
	Heartbeat_Init();		 //initialize heartbeat
    DAC_Init(0x1000);    //initialize with command: Vout = Vref
	DAC_Init2(0x1000);
	ADC_Init89();				 //initialize ADC PE4,PE5
    SysTick_Init(50000); //initialize systick, update to include periodic  interrupts at a rate of 1000 Hz

	//TI Launchpad End Of Initializaiton Ritual
	
  while(1){
		//0)heart beat every 2000 cycles
		if(i%2000 == 0){
			Heartbeat_Toggle(); //heart beat to let know that the program is running
		}
   
		//DAC_Out(wave[i&0x1F]);
    i = i + 1;
		//1) only output if new information is collected
		if(Mailbox==1){
			j+=1;
			//0) avoid critical sections
			     detector1 = PhaseDetector[0];
				 detector2 = PhaseDetector[1];
			//1) Calculate the Phase of the Incidence Angle
				incidence = Incidence_Conversion(detector1, detector2); //calculated by using a lookup table
			if(incidence == -1){
				Heartbeat_Off();
				Red_On();
				Green_Off();
				DisableInterrupts();
				//produces a flashing red colorand program has effectively crashed. 
				//lets person know that something went haywire
				while(1){  
						Red_Toggle();
						SysTick_Wait10ms(100); //flashes the Red Light at a rate of 10Hz
						}
			}
			//2)Calculate the Required Phase shift needed
			   if(j%1000 == 0){
					 Green_Toggle();
				 }
				 
				I = Channel_I(incidence); //outputs value to give I channel DAC/ play to set angle phase shift normally 'incidence' variable
				Q = Channel_Q(incidence); //outputs value to give Q channel DAC/ play to set agnle phase shift
			//3)OutputPhase shift to Phase Detectors
				DAC_Out(I);  //I-Channel
				DAC_Out2(Q); //Q-Channel
			//4)Acknowledge we used the data and are ready to recieve new data
		    Mailbox= 0;
		}


	
   
  }
}
