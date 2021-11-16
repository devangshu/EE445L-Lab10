#include "DisplayDriver.h"


uint32_t x_coordinate, y_desired, y_measured;
extern int32_t RPS_Desired, RPS_Measured;

uint32_t currMinX = 0;
uint32_t currMaxX = 2500;
uint32_t currMinY = 0;
uint32_t currMaxY = 2500;





void DisplayInit(void) {
    long sr = StartCritical();

    ST7735_InitR(INITR_REDTAB);
    x_coordinate = 0;
    y_desired = 0;
    y_measured = 0;

    Output_Clear();
    //ST7735_PlotClear(currMinY, currMaxY);
    //ST7735_XYplotInit("PI Loop", -2500, 2500, -2500, 2500, ST7735_WHITE);

    Timer4A_Init(&PlotCustomGraph, 1100000, 1);

    EndCritical(sr);

}

/*
static void DrawHeading(void){
    ST7735_SetCursor(0,0);
    ST7735_OutString("Desired Motor Speed:");
    ST7735_OutUDec(RPS_Desired);
    ST7735_SetCursor(0,1);
    ST7735_OutString("Measured Motor Speed:");
    ST7735_OutUDec(RPS_Measured);
}
*/

void PlotCustomGraph(void){
    //DrawHeading();
    y_desired = 159 - (RPS_Desired * 159)/MAX_RPS;
    y_measured = 159 - ((RPS_Measured * 159)/MAX_RPS);

    int x = 127 * (x_coordinate - currMinX) / (currMaxX-currMinX);
    int y1 = 32 + (127 * (currMaxY - RPS_Desired) / (currMaxY-currMinY));
    int y2 = 32 + (127 * (currMaxY - RPS_Measured) / (currMaxY-currMinY));

    ST7735_DrawPixel(x_coordinate, y_desired, ST7735_RED);
    ST7735_DrawPixel(x_coordinate, y_measured, ST7735_BLUE);

    x_coordinate++;

    if(x_coordinate == 127){
       x_coordinate = 0;
       Output_Clear();
    }

    //x_coordinate = (x_coordinate + 1) & 0x0000007F; // keep the value between 1 and 127;
}



