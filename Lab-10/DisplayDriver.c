#include "DisplayDriver.h"


uint32_t x_coordinate, y_desired, y_measured;
extern int32_t RPS_Desired, RPS_Measured;

void DisplayInit(void) {
    ST7735_InitR(INITR_REDTAB);
    x_coordinate = 0;
    y_desired = 0;
    y_measured = 0;
}

static void DrawHeading(void){
    ST7735_SetCursor(0,0);
    ST7735_OutString("Desired Motor Speed:");
    ST7735_OutUDec(RPS_Desired);
    ST7735_SetCursor(0,1);
    ST7735_OutString("Measured Motor Speed:");
    ST7735_OutUDec(RPS_Measured);
}

void PlotCustomGraph(void){
    DrawHeading();
    y_desired = (RPS_Desired * 100)/MAX_RPS;
    y_measured = (RPS_Measured * 100)/MAX_RPS;
    ST7735_DrawPixel(x_coordinate, y_desired, ST7735_RED);
    ST7735_DrawPixel(x_coordinate, y_measured, ST7735_BLUE);
    x_coordinate = (x_coordinate + 1) & 0x0000007F; // keep the value between 1 and 127;
}

