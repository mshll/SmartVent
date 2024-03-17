#include "Arduino.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);   // ESP32 Thing, HW I2C with pin remapping

const int buttonUpPin = 33;
const int buttonSelectPin = 15;
const int buttonDownPin = 14;
const int buttonBackPin = 32;

// Define your menu items
const int MENU_ITEMS_COUNT = 4;

// OLED DIMENSION
#define OLED_WIDTH u8g2.getDisplayWidth()       // 128  Pixels
#define OLED_HEIGHT u8g2.getDisplayHeight()     // 64   Pixels

const char* menuItems[MENU_ITEMS_COUNT] = {
    "CO2 Levels",
    "Fan Speed",
    "Idle Time",
    "Time"
};
int currentMenuItem = 0;  // Current menu item index

void start_display(void){
    u8g2.clearBuffer();					    // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	    // choose a suitable font
    u8g2.drawStr(OLED_WIDTH/2,OLED_HEIGHT/2,"Auto Pilot");	// write something to the internal memory
    u8g2.sendBuffer();					    // transfer internal memory to the display
    delay(3*1000);                  //
}

void setup() {
  // Initialize OLED 
  Serial.begin(9600);
  u8g2.begin();

  // Setup buttons as inputs
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);
  pinMode(buttonBackPin, INPUT_PULLUP);

  start_display();
  
}

void loop() {
// Check button presses and navigate the menu
  if (digitalRead(buttonUpPin) == LOW) {
    Serial.println("Left");
    delay(400);   // Adjust the delay as needed
    currentMenuItem = (currentMenuItem - 1 + sizeof(menuItems) / sizeof(menuItems[0])) % (sizeof(menuItems) / sizeof(menuItems[0]));
  }
  if (digitalRead(buttonDownPin) == LOW) {
    Serial.println("Right");
    delay(400);   // Adjust the delay as needed
    currentMenuItem = (currentMenuItem + 1) % (sizeof(menuItems) / sizeof(menuItems[0]));
  }
  // Clear the display
  u8g2.firstPage();
  do {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);	// primary font
    //u8g.setFont(u8g_font_10x20);  // secondary font
    // Center the text horizontally and vertically
    // int centerX = (OLED_WIDTH - u8g2.getStrWidth(menuItems[currentMenuItem] )) / 2;
    // int centerY = (OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2;
    u8g2.drawStr((OLED_WIDTH - u8g2.getStrWidth(menuItems[currentMenuItem] )) / 2,(OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2,menuItems[currentMenuItem]);	// write something to the internal memory
    u8g2.sendBuffer();					// transfer internal memory to the display
  } while (u8g2.nextPage());  


  /* // WORKS
  for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
    Serial.println(menuItems[i]);
    delay(500); // Add a delay for better readability in serial monitor
  }
 */

}

