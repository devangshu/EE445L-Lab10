/* Switch Driver Module */

#include "SwitchDriver.h"
	

/* module fields */

volatile uint32_t SW1;
volatile uint32_t SW2;
volatile uint32_t SW3;
volatile uint16_t SW_Delay1;
volatile uint16_t SW_Delay2;
volatile uint16_t SW_Delay3;
volatile uint32_t FallingEdges = 0;


/* module internal functions */
void Switches_Handler(uint32_t id) {
	if (id == 1) {
		// do stuff for button 1
	} else if (id == 2) {
		// do stuff for button 2
	}
}


void PortC_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x14;  // enable ports C and E
  while((SYSCTL_PRGPIO_R&0x14)==0); // allow time for clock to start
  GPIO_PORTC_PCTL_R &= ~0xFFFF0000;// configure PC4 as GPIO (default setting)
  GPIO_PORTC_AMSEL_R &= ~0xF0;// disable analog functionality on PC4-7 (default setting)
	//GPIO_PORTC_PUR_R |= 0x10;     //     no pullup for external switches (enable weak pull-up on PC4)
  GPIO_PORTC_DIR_R &= ~0xF0;  // make PC4-7 in (PC4-7 buttons) (default setting)
  GPIO_PORTC_AFSEL_R &= ~0xF0;// disable alt funct on PC4-7 (default setting)
  GPIO_PORTC_DEN_R |= 0xF0;   // enable digital I/O on PC4-7 
	
}

void EdgeCounterPortF_Init(void){                          
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  FallingEdges = 0;             // (b) initialize counter
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_DIR_R |=  0x0E;    // output on PF3,2,1 
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x1F;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}
void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
  FallingEdges = FallingEdges + 1;
  //PF3 ^= 0x06; // profile
  if (SW3) {
      SW3 = 0;
  } else SW3 = 1;
  Switches_Handler(3);
}


/* module external functions */

void Switches_Init(void) {
	SW1 = 0;
	SW2 = 0;
	SW3 = 0;
	SW_Delay1 = 0;
	SW_Delay2 = 0;
	SW_Delay3 = 0;
	EdgeCounterPortF_Init();
	PortC_Init();
}

void Switches_Loop(void) {
	uint32_t switch_bitmap = GPIO_PORTC_DATA_R;
	uint8_t loop_delay = 4;
	if((switch_bitmap & Pin4Mask) != 0) {
		SW_Delay1 = loop_delay * ((switch_bitmap & Pin4Mask) >> 4);
	}
	if((switch_bitmap & Pin5Mask) != 0) {
		SW_Delay2 = loop_delay * ((switch_bitmap & Pin5Mask) >> 5);
	}
	if (SW_Delay1 > 0) {
		SW_Delay1 -= 1;
		if (SW_Delay1 == 0) {
			Switches_Handler(1);
		}
	}
	if (SW_Delay2 > 0) {
		SW_Delay2 -= 1;
		if (SW_Delay2 == 0) {
			Switches_Handler(2);
		}
	}
	
}
