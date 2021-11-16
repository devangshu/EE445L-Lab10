#include "TachDriver.h"

#define EightyMHzCycles 80000000

uint32_t RPS;
uint32_t Period;                       // 24-bit, 12.5 ns units
uint32_t static First;                 // Timer0A first edge, 12.5 ns units
int32_t Done;                          // mailbox status set each rising
void PeriodMeasure_Init(void) {
  SYSCTL_RCGCTIMER_R |= 0x01;          // activate timer0
  SYSCTL_RCGCGPIO_R |= 0x02;           // activate port B
  RPS = 0;
  Period = 0;
  First = 0;                           // first will be wrong
  Done = 0;                            // set on subsequent
  GPIO_PORTB_DIR_R   &= ~0x40;         // make PB6 input
  GPIO_PORTB_AFSEL_R |= 0x40;          // enable alt funct on PB6
  GPIO_PORTB_DEN_R   |= 0x40;          // configure PB6 as T0CCP0
  GPIO_PORTB_PCTL_R   = (GPIO_PORTB_PCTL_R
                       & 0xF0FFFFFF)
                       + 0x07000000;
  TIMER0_CTL_R       &= ~0x00000001;   // disable timer0A during setup
  TIMER0_CFG_R        =  0x00000004;   // configure for 16-bit capture mode
  TIMER0_TAMR_R       =  0x00000007;   // configure for rising edge event
  TIMER0_CTL_R       &= ~0x0000000C;   // rising edge
  TIMER0_TAILR_R      =  0x0000FFFF;   // start value
  TIMER0_TAPR_R       =  0xFF;         // activate prescale, creating 24-bit
  TIMER0_IMR_R       |=  0x00000004;   // enable capture match interrupt
  TIMER0_ICR_R        =  0x00000004;   // clear timer0A capture match flag
  TIMER0_CTL_R       |=  0x00000001;   // timer0A 24-b, +edge, interrupts
  NVIC_PRI4_R         = (NVIC_PRI4_R
                       & 0x00FFFFFF)
                       | 0x40000000;   //Timer0A=priority 2
  NVIC_EN0_R = 1<<19;                  // enable interrupt 19 in NVIC
  //EnableInterrupts();                  // Not needed if interrupts are enabled in MAIN
}

void Timer0A_Handler(void) {
  TIMER0_ICR_R = 0x00000004;       // acknowledge timer0A
  Period = (First - TIMER0_TAR_R)  // NOTE: underflow tolerant calculation
          & 0x00FFFFFF;
  RPS = (EightyMHzCycles) / (Period * 12);

  First = TIMER0_TAR_R;            // setup for next measurement
  Done = 1;                        // set semaphore
}

void InitTachDriver() {
    PeriodMeasure_Init();
}



