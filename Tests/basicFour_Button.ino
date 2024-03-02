/*
* Basic Multiple Button with a single click function.
* Must have lennarthennigs/Button2@^2.3.2 for library in platformio.ini.
*/
#include <Arduino.h>
#include "Button2.h"


#define LEFT_BUTTON_PIN  33
#define RIGHT_BUTTON_PIN  14
#define UP_BUTTON_PIN  15
#define DOWN_BUTTON_PIN  32


Button2 buttonLeft, buttonRight, buttonUp, buttonDown;


void setup() {
  Serial.begin(19200);
  delay(50);
  Serial.println("\n\nMultiple Buttons Demo");
  
  buttonLeft.begin(LEFT_BUTTON_PIN);
  buttonLeft.setClickHandler(click);

  buttonRight.begin(RIGHT_BUTTON_PIN);
  buttonRight.setClickHandler(click);
  
  buttonUp.begin(UP_BUTTON_PIN);
  buttonUp.setClickHandler(click);

  buttonDown.begin(DOWN_BUTTON_PIN);
  buttonDown.setClickHandler(click);
}


void loop() {
  buttonLeft.loop();
  buttonRight.loop();
  buttonUp.loop();
  buttonDown.loop();
}


void click(Button2& btn) {
    if (btn == buttonLeft) {
      Serial.println("Left clicked");
    } else if (btn == buttonRight) {
      Serial.println("Right clicked");
    } else if (btn == buttonUp) {
      Serial.println("Up clicked");
    } else if (btn == buttonDown) {
      Serial.println("Down clicked");
    }
}
