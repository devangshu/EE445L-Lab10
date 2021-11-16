/* ESP8266 Driver Module */

#include "ESPDriver.h"

/* constants */

#define PF2       (*((volatile uint32_t *)0x40025010))


/* module fields */

// These 6 variables contain the most recent Blynk to TM4C123 message
// Blynk to TM4C123 uses VP0 to VP15
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;

extern uint32_t KP_1, KP_2, KI_1, KI_2, U, RPS_Desired, RPS_Measured, MotorSpeed;
extern int32_t E;


/* module internal functions */
 
// ----------------------------------- TM4C_to_Blynk ------------------------------
// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
// so that the ESP8266 knows to forward the data to the Blynk App
void TM4C_to_Blynk(uint32_t pin,uint32_t value){
  if((pin < 70)||(pin > 99)){
    return; // ignore illegal requests
  }
// your account will be temporarily halted 
  // if you send too much data
  ESP8266_OutUDec(pin);       // Send the Virtual Pin #
  ESP8266_OutChar(',');
  ESP8266_OutUDec(value);      // Send the current value
  ESP8266_OutChar(',');
  ESP8266_OutString("0.0\n");  // Null value not used in this example
}
 

void Blynk_to_TM4C_READ(void){
    int z = 0;
    int j; char data;

// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
    // Read the data from the UART5
#ifdef DEBUG1
    j = 0;
    do{
      data = serial_buf[j];
      UART_OutChar(data);        // Debug only
      j++;
    }while(data != '\n');
    UART_OutChar('\r');        
#endif
           
// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  


#ifdef DEBUG1
    UART_OutString(" Pin_Number = ");
    UART_OutString(Pin_Number);
    UART_OutString("   Pin_Integer = ");
    UART_OutString(Pin_Integer);
    UART_OutString("   Pin_Float = ");
    UART_OutString(Pin_Float);
    UART_OutString("\n\r");
#endif
#ifdef DEBUG3
    Output_Color(ST7735_YELLOW);
    ST7735_OutUDec(pin_num);
    ST7735_OutString("=");
    ST7735_OutUDec(pin_int);
    ST7735_OutString("\n");
#endif
  }  
  if(pin_num == 0x05) {
      //Desired Motor Speed
      MotorSpeed = pin_int; // need to do math to turn value from 0 - 1 into range of values
      RPS_Desired = MotorSpeed;
  } else if(pin_num == 0x06){
      //KP_1
      KP_1 = pin_int;
  } else if(pin_num == 0x07){
      //KP_2
      KP_2 = pin_int;
  } else if(pin_num == 0x08){
      //KI_1
      KI_1 = pin_int;
  } else if(pin_num == 0x09){
      //KI_2
      KI_2 = pin_int;
  }

}


int32_t RPS_Measured_old = 0;
int32_t U_old = 0;
int32_t E_old = 0;

void Send_Information(void){
  //Send the 3 things to our Blynk console
  if (RPS_Measured_old != RPS_Measured) {
      RPS_Measured_old = RPS_Measured;
      TM4C_to_Blynk(70, RPS_Measured);
  }
  if (U_old != U) {
      U_old = U;
      TM4C_to_Blynk(71, U);
  }
  if (E_old != E) {
      E_old = E;
      TM4C_to_Blynk(72, E);
  }
}


/* module external methods */
uint8_t esp_ready = 0;
void ESP_Init(void) {
	
#ifdef DEBUG1
  UART_Init(5);         // Enable Debug Serial Port
  UART_OutString("\n\rEE445L Lab 10\n\r");
#endif
  ESP8266_Init();       // Enable ESP8266 Serial Port
  ESP8266_Reset();      // Reset the WiFi module
  ESP8266_SetupWiFi();  // Setup communications to Blynk Server  
  

  Timer2_Init(&Blynk_to_TM4C_READ, 800000);
  // check for receive data from Blynk App every 10ms
	

  Timer3_Init(&Send_Information, 40000000);
  // Send data back to Blynk App every 1/2 second
  esp_ready = 1;
}
