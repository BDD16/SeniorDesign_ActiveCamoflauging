//***heartbeat.c***
//*this code is to make a heart beat function*

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define PF2                     (*((volatile unsigned long *)0x40025010)) //Blue light bit banded
#define PF1											(*((volatile unsigned long *)0x40025008)) //Red light bit banded
#define PF3											(*((volatile unsigned long *)0x40025020)) //Green light bit banded
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

//***Heartbeat_Init***
//**Initializes portF of the TI Launchpad and 
//**initializes the blue LED (PF2)-used for main program
//**initializes the red LED (PF1)-used for ADC interrupt
//**inputs: none
//**outputs: none

void Heartbeat_Init(void){
	volatile unsigned long delay;
	
	SYSCTL_RCGC2_R |= 0x20;           // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PF1,2,3
  GPIO_PORTF_PCTL_R &= ~0x0000FFF0; // 3) regular GPIO was ~0x00000F00 before adding in red light, adding in Green light
  GPIO_PORTF_AMSEL_R &= ~0x0E;      // 4) disable analog function on PF2 and PF1
  GPIO_PORTF_DIR_R |= 0x0E;         // 5) set direction to output
  GPIO_PORTF_AFSEL_R &= ~0x0E;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x0E;         // 7) enable digital port
}

//***Heartbeat_On***
//**Turns the Blue LED (PF2) on
//**Inputs:none
//**Outputs:none
void Heartbeat_On(void){
	PF2 = 0x04;  //Turns PF2 High
}

void Red_On(void){
	PF1 =0x02;
}

void Green_On(void){
	PF3 = 0x08;
}
//***Heartbeat_Off***
//**Turns the Blue LED (PF2) off
//**Inputs: none
//**Outpus: none
void Heartbeat_Off(void){
	PF2 = 0x00;  //Turns PF2 Low
}

void Red_Off(void){
	PF1 = 0x00;
}

void Green_Off(void){
	PF3 = 0x00;
}
//***Heartbeat_Toggle***
//**Toggles Blue LED (On/Off) depending on when called
//**Inputs: none
//**Outpus: none
void Heartbeat_Toggle(void){
	PF2 ^= 0x04;
}

void Red_Toggle(void){
	PF1 ^= 0x02;
}

void Green_Toggle(void){
	PF3 ^= 0x08;
}

