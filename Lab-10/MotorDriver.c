#include "MotorDriver.h"

extern volatile int32_t RPS_Measured; // Revolutions per second
extern volatile uint8_t esp_ready;
volatile int32_t RPS_Desired; // Revolutions per second
volatile int32_t MotorSpeed; // Not used, only for debugging
//Needed variables
volatile int32_t P; //Proportional term
volatile int32_t I; // Integral term
volatile int32_t E; // Revolutions per second
volatile int32_t KP_1; //used for Proportional Ratio
volatile int32_t KP_2; //used for Proportional Ratio
volatile int32_t KI_1; //used for Integral Ratio
volatile int32_t KI_2; //used for Integral Ratio
volatile int32_t U; // PWM Duty Cycle (in clock ticks)


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
  KP_1 = 75;
  KP_2 = 200;
  KI_1 = 75;
  KI_2 = 200;
  U = 0;
  MotorSpeed = 0;
  RPS_Desired = 0;

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

  Timer1A_Init(PI_Loop, period * 2, 2);
}
// change duty cycle of PB7
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void SetDuty(uint16_t duty) {
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
}

int32_t U_old2 = 0;

void PI_Loop(void) {
    E = RPS_Desired - (RPS_Measured);

    // what does this line do? --v
    MotorSpeed = RPS_Desired / 40;          // Set the Motor Speed

    P  =  (KP_1 * E) / KP_2;          // Proportional term

    if (P <  300)
        P = 300;         // Minimum PWM output = 300
    if (P > 39900)
        P = 39900;       // Maximum PWM output = 39900

    I  = I + (KI_1 * E) / KI_2;       // SUM(KiDt)

    if (I <  300) {
        I = 300;         // Minimum PWM output = 300
    }
    if (I > 39900) {
        I = 39900;       // Maximum PWM output = 39900
    }

    U = P + I;                  // Calculate the actuator value

    if (U < 300) {
        U = 300;           // Minimum PWM output
    }
    if (U > 39900) {
        U = 39900;         // 3000 to 39900
    }

    if (U_old2 != U) {
        SetDuty(U);                    //Send to PWM
        U_old2 = U;
        if (esp_ready) {
            /*
            UART_OutString("U=");
            UART_OutUDec(U);
            UART_OutChar('\r');
            UART_OutChar('\n');
            */
        }
    }
    //PlotCustomGraph();


}
