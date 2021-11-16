#include "main.h"


int main(void) {
    PLL_Init(Bus80MHz);
    LaunchPad_Init();  // activate port F
    DisableInterrupts();

    PF2 = 0;
    //Audio_Init();
    //Timer_Init();
    //Switches_Init();
    InitTachDriver();
    InitMotor(40000, 39900);  // U_range{5000 : 39900} == DMS_range{10 : 50}
    DisplayInit();
    ESP_Init();

    EnableInterrupts();

    while(1) {
        PF1 ^= 0x02;
    }
}
