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

    while(1){
        PF1 ^= 0x02;
    }
}
