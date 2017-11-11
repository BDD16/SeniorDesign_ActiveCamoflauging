//***heartbeat.h***
//*this code is to make a heart beat function*

//***Heartbeat_Init***
//**Initializes portF of the TI Launchpad and 
//**initializes the blue LED (PF2)
//**inputs: none
//**outputs: none
#define PF2                     (*((volatile unsigned long *)0x40025010))

void Heartbeat_Init(void);

//***Heartbeat_On***
//**Turns the Blue LED (PF2) on
//**Inputs:none
//**Outputs:none
void Heartbeat_On(void);

void Red_On(void);

void Green_On(void);
//***Heartbeat_Off***
//**Turns the Blue LED (PF2) off
//**Inputs: none
//**Outpus: none
void Heartbeat_Off(void);

void Red_Off(void);

void Green_Off(void);
//***Heartbeat_Toggle***
//**Toggles Blue LED (On/Off) depending on when called
//**Inputs: none
//**Outpus: none
void Heartbeat_Toggle(void);

void Red_Toggle(void);

void Green_Toggle(void);
