#include "main.h"


int main(void) {
    PLL_Init(Bus80MHz);
    LaunchPad_Init();  // activate port F
    DisableInterrupts();

    PF2 = 0;
    DisplayInit();
    //Audio_Init();
    //Timer_Init();
    //Switches_Init();
    InitTachDriver();
    InitMotor(40000, 600);
    ESP_Init();

    EnableInterrupts();

    while(1) {
        PF1 ^= 0x02;
    }
}
