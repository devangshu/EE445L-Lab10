#include "main.h"


int main(void)
{
    PLL_Init(Bus80MHz);
    DisableInterrupts();
    //Display_Init();
    //Audio_Init();
    //Timer_Init();
    //Switches_Init();
    ESP_Init();
    EnableInterrupts();

    InitMotor(40000,30000);

    while(1){
        PF1 ^= 0x02;
    }
}
