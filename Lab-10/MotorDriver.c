#include "MotorDriver.h"


uint32_t MotorSpeed;
uint32_t P;
uint32_t I;
uint32_t E;


// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB6 and PB7 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB7/M0PWM1
void InitMotor(uint16_t period, uint16_t duty) {
  P = 0;
  I = 0;
  E = 0;
  MotorSpeed = 0;
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  GPIO_PORTB_AFSEL_R |= 0x80;           // enable alt funct on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;     // configure PB7 as M0PWM1
  GPIO_PORTB_PCTL_R |= 0x40000000;
  GPIO_PORTB_AMSEL_R &= ~0x80;          // disable analog functionality on PB7
  GPIO_PORTB_DEN_R |= 0x80;             // enable digital I/O on PB7
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
  // PB7 goes low on LOAD
  // PB7 goes high on CMPB down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000002;          // enable PB7/M0PWM1

  Timer2_Init(PILoop, period * 2);
}
// change duty cycle of PB7
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void SetDuty(uint16_t duty) {
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
}

void PILoop(void) {
    MotorSpeed = rps/40;          // Set the Motor Speed
    P  =  (Kp1 * E)/Kp2;          // Proportional term
    if(P <  300) P = 300;         // Minimum PWM output = 300
    if(P > 39900) P = 39900;       // Maximum PWM output = 39900
    I  = I + (Ki1 * E)/Ki2;       // SUM(KiDt)
    if(I <  300) I = 300;         // Minimum PWM output = 300
    if(I > 39900) I = 39900;       // Maximum PWM output = 39900
    U   = P + I;                  // Calculate the actuator value
    if(U < 300)  U=300;           // Minimum PWM output
    if(U > 39900) U=39900;         // 3000 to 39900
    PWM0A_Duty(U);                // Send to PWM
}


