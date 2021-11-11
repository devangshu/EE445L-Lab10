#include "DisplayDriver.h"



void DisplayInit(void) {
    ST7735_InitR(INITR_REDTAB);
    ST7735_SetTextColor(ST7735_WHITE);
    ST7735_DrawString(7, 8, "LOADING", ST7735_WHITE);
}
