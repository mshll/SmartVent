/*
* Navigation through first level menu using Button2 Library.
* Must have lennarthennigs/Button2@^2.3.2 for library in platformio.ini.
*/
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "Button2.h"    // Button2 Header


/*** PINS FOR BUTTONS ***/
#define LEFT_BUTTON_PIN  33   //A0
#define RIGHT_BUTTON_PIN  14  //A3
#define UP_BUTTON_PIN  15     //A1
#define DOWN_BUTTON_PIN  32   //A2


/*** PINS FOR OLED ***/
#define OLED_SCL 22
#define OLED_SDA 23  // 21

// OLED DIMENSION
#define OLED_WIDTH u8g2.getDisplayWidth()       // 128  Pixels
#define OLED_HEIGHT u8g2.getDisplayHeight()     // 64   Pixels

#define START_DELAY 5		// Time for Start Logo Delay (in seconds).

Button2 left_button, right_button, up_button, down_button;         // Name for each button.

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);   // ESP32 Thing, HW I2C with pin remapping

// Define your menu items
const int MENU_ITEMS_COUNT = 4;
const char* menuItems[MENU_ITEMS_COUNT] = {
    "CO2 Levels",
    "Fan Speed",
    "Idle Time",
    "Time"
};
int currentMenuItem = 0;  // Current menu item index


/*** FUNCTIONS ***/
/* void pressed(Button2& btn) {                     // Maybe needed
  Serial.println("pressed");
}
void longClickDetected(Button2& btn) {             // Maybe needed
  Serial.println("long click detected");
} */ 
void button_handler(Button2& btn) {
    switch (btn.getType()) {
        case single_click:
            Serial.print("click ");
            // TODO: 
            break;
        case long_click:        // Act as one.
            Serial.println("long click\n");
            break;
        default:
            break;
    }
    //Serial.print("click ");
    Serial.print("on button "); 
    Serial.print(btn.getPin());
    Serial.println(); 


    /* UPDATES MENU NAVIGATION */
    if (btn.getPin() == LEFT_BUTTON_PIN) {
        Serial.println("Left");
        currentMenuItem = (currentMenuItem - 1 + sizeof(menuItems) / sizeof(menuItems[0])) % (sizeof(menuItems) / sizeof(menuItems[0]));
    }
    if (btn.getPin() == RIGHT_BUTTON_PIN) {
        Serial.println("Right");
        currentMenuItem = (currentMenuItem + 1) % (sizeof(menuItems) / sizeof(menuItems[0]));
    }
}


/* DISPLAY OPTIONS */
void idle_display(void){    // display_main_screen (from oled.h)
    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.drawStr(0,10,"Displays Temperatures");	// write something to the internal memory
    u8g2.sendBuffer();					// transfer internal memory to the display
    delay(1000);  
}

void start_display(void){
    u8g2.clearBuffer();					    // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	    // choose a suitable font
    u8g2.drawStr(OLED_WIDTH/2,OLED_HEIGHT/2,"Auto Pilot");	// write something to the internal memory
    u8g2.sendBuffer();					    // transfer internal memory to the display
    delay(START_DELAY*1000);                  //
}

void menu_display(void){
    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.drawStr(0,10,"Settings");	// write something to the internal memory
    u8g2.sendBuffer();					// transfer internal memory to the display
    delay(1000);  
}

/*** SETUP ***/
void setup() {
    Serial.begin(9600);
    
    u8g2.begin();
    delay(50);

    Serial.println("\n\nFour Button Demo");

    // LEFT BUTTON
    left_button.begin(LEFT_BUTTON_PIN);
    //left_button.setClickHandler(button_handler); // Replaced by released.
    left_button.setReleasedHandler(button_handler);
    left_button.setDoubleClickHandler(button_handler);
    left_button.setLongClickHandler(button_handler); 
    down_button.setLongClickDetectedHandler(button_handler); 

    /*     
    left_button.setLongClickTime(1000);
    left_button.setDoubleClickTime(400);
    Serial.println(" Longpress Time:\t" + String(left_button.getLongClickTime()) + "ms");
    Serial.println(" DoubleClick Time:\t" + String(left_button.getDoubleClickTime()) + "ms");
    Serial.println();
    */


    // RIGHT BUTTON
    right_button.begin(RIGHT_BUTTON_PIN);
    //right_button.setClickHandler(button_handler);
    right_button.setLongClickHandler(button_handler);
    right_button.setDoubleClickHandler(button_handler);
    right_button.setReleasedHandler(button_handler); 
    down_button.setLongClickDetectedHandler(button_handler); 
    
    /*     
    right_button.setLongClickTime(1000);
    right_button.setDoubleClickTime(400);
    Serial.println(" Longpress Time:\t" + String(right_button.getLongClickTime()) + "ms");
    Serial.println(" DoubleClick Time:\t" + String(right_button.getDoubleClickTime()) + "ms");
    Serial.println();
    */


    // DOWN BUTTON
    down_button.begin(DOWN_BUTTON_PIN);
    //down_button.setClickHandler(button_handler);
    down_button.setLongClickHandler(button_handler);   
    down_button.setDoubleClickHandler(button_handler);
    down_button.setReleasedHandler(button_handler);
    down_button.setLongClickDetectedHandler(button_handler); 
    /*     
    down_button.setLongClickTime(1000);
    down_button.setDoubleClickTime(400);
    Serial.println(" Longpress Time:\t" + String(down_button.getLongClickTime()) + "ms");
    Serial.println(" DoubleClick Time:\t" + String(down_button.getDoubleClickTime()) + "ms");
    Serial.println(); 
    */


    // UP BUTTON
    up_button.begin(UP_BUTTON_PIN);
    //up_button.setClickHandler(button_handler);
    up_button.setLongClickHandler(button_handler);
    up_button.setDoubleClickHandler(button_handler);
    up_button.setReleasedHandler(button_handler); 
    down_button.setLongClickDetectedHandler(button_handler); 
    /*     
    up_button.setLongClickTime(1000);
    up_button.setDoubleClickTime(400);
    Serial.println(" Longpress Time:\t" + String(up_button.getLongClickTime()) + "ms");
    Serial.println(" DoubleClick Time:\t" + String(up_button.getDoubleClickTime()) + "ms");
    Serial.println();
    */



    start_display();
}


/*** LOOP ***/
void loop() {

  // Clear the display
  u8g2.firstPage();
  do {
    left_button.loop();
    right_button.loop();
    up_button.loop();
    down_button.loop();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);	// primary font
    //u8g.setFont(u8g_font_10x20);  // secondary font
    // Center the text horizontally and vertically
    // int centerX = (OLED_WIDTH - u8g2.getStrWidth(menuItems[currentMenuItem] )) / 2;
    // int centerY = (OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2;
    u8g2.drawStr((OLED_WIDTH - u8g2.getStrWidth(menuItems[currentMenuItem] )) / 2,(OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2,menuItems[currentMenuItem]);	// write something to the internal memory
    u8g2.sendBuffer();					// transfer internal memory to the display
  } while (u8g2.nextPage());  
}