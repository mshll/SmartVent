/*
* Basic Button with Multiple Functions.
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

#define START_DELAY 3		// Time for Start Logo Delay (in seconds).
#define IDLE_TIME 5         // Defualt Idle Countdown is 30 Seconds.            // TODO: Make this link to the settings and change it to it.

Button2 left_button, right_button, up_button, down_button;         // Name for each button.

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);   // ESP32 Thing, HW I2C with pin remapping

// Define your menu items
// Define the structure for a menu item
struct MenuItem {
    const char* name;
    int values[3]; // Assuming each menu item can have up to 3 values
};
const int MENU_ITEMS_COUNT = 5;
MenuItem menuItems[MENU_ITEMS_COUNT] = {
    {"CO2 Levels",{800 /*Low*/, 1200 /*High*/}},
    {"Fan Speed",{0 /*0:Off, 1:Slow, 2:Fast, 3:Max*/}},
    {"Idle Time",{0 /*Default Mode 0: 30 Seconds*/}},
    {"Time",{0 /*Hour*/, 0 /*Minutes*/, 0 /*AM/PM*/}},
    {"Sleep", {1 /*Idle Flag*/}}
};
int currentMenuItem = 0;  // Current menu item index
int selectedMenuItem = 0; // 0 Means Not Selected To lock on the Menu must have + 1
int selectModifyItem = 0;

/* Number of Fan Speed options. **Not including OFF. */
const int MAX_NUM_FAN_SPEED = 3;    


int idle_flag = 1; // 0=idle; 1=active_menu
hw_timer_t *timer = NULL;

/*** FUNCTIONS ***/
void longClickDetected(Button2& btn) {
    Serial.println("long click detected");
    Serial.print("on button "); 
    Serial.print(btn.getPin());
    Serial.println(); 

}
void button_handler(Button2& btn) {
    // Reset Timer Interrupt for idle.
    // timerAlarmDisable(timer);
    // timerAlarmWrite(timer, IDLE_TIME * 1000000, false); // Set alarm value to seconds, disable auto-reload
    // timerAlarmEnable(timer);
    

    // One button push out of Idle.
    if (idle_flag == 1) {
        Serial.println("Waking Up...\n");
        idle_flag = 0;
        selectedMenuItem = 0;
        currentMenuItem = 0;
        selectModifyItem = 0;
    }
    else {
        /* UPDATES MENU NAVIGATION */
        if (selectedMenuItem == 0) {
            if (btn.getPin() == UP_BUTTON_PIN) {
                currentMenuItem = (currentMenuItem - 1 + sizeof(menuItems) / sizeof(menuItems[0])) % (sizeof(menuItems) / sizeof(menuItems[0]));
                
                Serial.println("Up\n");
            }
            if (btn.getPin() == DOWN_BUTTON_PIN) {
                currentMenuItem = (currentMenuItem + 1) % (sizeof(menuItems) / sizeof(menuItems[0]));

                Serial.println("Down\n");
            }
            if (btn.getPin() == RIGHT_BUTTON_PIN) {
                selectedMenuItem = currentMenuItem + 1;
                selectModifyItem = 0;

                Serial.println("Select\n");
            }
        }
        // CO2 Level Menu
        else if (selectedMenuItem == 1) {
            if (btn.getPin() == RIGHT_BUTTON_PIN) {
                selectModifyItem = (0+1) % ((sizeof(menuItems[selectedMenuItem-1].values) / sizeof(menuItems[selectedMenuItem-1].values[0]))-1);

                Serial.println("Next\n");
                Serial.println(selectModifyItem);
            }
            if (btn.getPin() == LEFT_BUTTON_PIN) {
                selectedMenuItem = 0;
                selectModifyItem = 0;

                Serial.println("Saving...\n");
            }
            if (btn.getPin() == UP_BUTTON_PIN) {
                menuItems[selectedMenuItem-1].values[selectModifyItem] += 10;

                Serial.println("+10 Higher\n");
                Serial.println(menuItems[selectedMenuItem-1].values[selectModifyItem]);
                Serial.println();
                }
            if (btn.getPin() == DOWN_BUTTON_PIN) {
                menuItems[selectedMenuItem-1].values[selectModifyItem] -= 10;

                Serial.println("-10 Lower\n");
                Serial.println(menuItems[selectedMenuItem-1].values[selectModifyItem]);
                Serial.println();
            }
        }
        // Fan Speed Menu
        else if (selectedMenuItem == 2) {
            if (btn.getPin() == RIGHT_BUTTON_PIN) {
                selectedMenuItem = 0;
                selectModifyItem = 0;
                Serial.println("Selected\n");
            }
            if (btn.getPin() == LEFT_BUTTON_PIN) {
                selectedMenuItem = 0;
                selectModifyItem = 0;
                Serial.println("Back\n");
            }
            if (btn.getPin() == UP_BUTTON_PIN) {
                if (menuItems[selectedMenuItem-1].values[selectModifyItem] <= 0) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] = MAX_NUM_FAN_SPEED;
                }
                else {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] -= 1;
                }
                Serial.println("Up\n");
            }
            if (btn.getPin() == DOWN_BUTTON_PIN) {
                if (menuItems[selectedMenuItem-1].values[selectModifyItem] >= MAX_NUM_FAN_SPEED) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] = 0;
                }
                else {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] += 1;
                }
                Serial.println("Down\n");
            }
            Serial.println(menuItems[selectedMenuItem-1].values[selectModifyItem]);
            Serial.println();
        }
        // Idle Time Menu
        else if (selectedMenuItem == 3) {
            if (btn.getPin() == RIGHT_BUTTON_PIN) {
                Serial.println("Selected\n");
                selectedMenuItem = 0;
                selectModifyItem = 0;
            }
            if (btn.getPin() == LEFT_BUTTON_PIN) {
                Serial.println("Back\n");
                selectedMenuItem = 0;
                selectModifyItem = 0;
            }
            if (btn.getPin() == UP_BUTTON_PIN) {
                Serial.println("Up\n");
                if (menuItems[selectedMenuItem-1].values[selectModifyItem] >= 3) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] = 3;
                }
                else {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] += 1;
                }
                Serial.println(menuItems[selectedMenuItem-1].values[selectModifyItem]);
                Serial.println();
                }
            if (btn.getPin() == DOWN_BUTTON_PIN) {
                if (menuItems[selectedMenuItem-1].values[selectModifyItem] <= 0) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] = 0;
                }
                else {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] -= 1;
                }
                Serial.println("Down\n");
                Serial.println(menuItems[selectedMenuItem-1].values[selectModifyItem]);
                Serial.println();
            }
        }
        // Time Menu
        else if (selectedMenuItem == 4) {
            if (btn.getPin() == RIGHT_BUTTON_PIN) {
                Serial.println("Next\n");
                selectModifyItem = (selectModifyItem+1) % ((sizeof(menuItems[selectedMenuItem-1].values) / sizeof(menuItems[selectedMenuItem-1].values[0])));
                Serial.println(selectModifyItem);
            }
            if (btn.getPin() == LEFT_BUTTON_PIN) {
                Serial.println("Back\n");
                selectedMenuItem = 0;
                selectModifyItem = 0;
            }
            if (selectModifyItem == 0 /*Hour*/) {
                if (btn.getPin() == UP_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] += 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] > 12) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 1;
                    }
                    }
                if (btn.getPin() == DOWN_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] -= 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] < 1) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 12;
                    }
                }
            }
            else if (selectModifyItem == 1 /*Minute*/) {
                if (btn.getPin() == UP_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] += 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] > 59) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 0;
                    }
                    }
                if (btn.getPin() == DOWN_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] -= 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] < 0) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 59;
                    }
                }
            }
            else if (selectModifyItem == 2 /*AM/PM*/) {
                if (btn.getPin() == UP_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] += 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] > 1) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 0;
                    }
                    }
                if (btn.getPin() == DOWN_BUTTON_PIN) {
                    menuItems[selectedMenuItem-1].values[selectModifyItem] -= 1;
                    if (menuItems[selectedMenuItem-1].values[selectModifyItem] < 0) {
                        menuItems[selectedMenuItem-1].values[selectModifyItem] = 1;
                    }
                }
            }
            Serial.println("Time Updated!");
            Serial.println();
        }
        // Sleep Button
        if (selectedMenuItem == 5){
            menuItems[selectedMenuItem-1].values[0] = 1;
            Serial.println(menuItems[selectedMenuItem-1].values[0]);
            Serial.println();
            selectedMenuItem = 0;
            currentMenuItem = 0;
            selectModifyItem = 0;
            idle_flag = 1;
        }
    }
    
}
void init_buttons(){
    // LEFT BUTTON
    left_button.begin(LEFT_BUTTON_PIN);
    left_button.setReleasedHandler(button_handler);
    left_button.setLongClickDetectedHandler(longClickDetected); 
    /*     
    left_button.setLongClickTime(1000);
    left_button.setDoubleClickTime(400);
    */

    // RIGHT BUTTON
    right_button.begin(RIGHT_BUTTON_PIN);
    right_button.setReleasedHandler(button_handler); 
    right_button.setLongClickDetectedHandler(longClickDetected); 
    /*     
    right_button.setLongClickTime(1000);
    right_button.setDoubleClickTime(400);
    */

    // DOWN BUTTON
    down_button.begin(DOWN_BUTTON_PIN);
    down_button.setReleasedHandler(button_handler);
    down_button.setLongClickDetectedHandler(longClickDetected); 
    /*     
    down_button.setLongClickTime(1000);
    down_button.setDoubleClickTime(400);
    */


    // UP BUTTON
    up_button.begin(UP_BUTTON_PIN);
    up_button.setReleasedHandler(button_handler); 
    up_button.setLongClickDetectedHandler(longClickDetected); 
    /*     
    up_button.setLongClickTime(1000);
    up_button.setDoubleClickTime(400);
    */
}
void button_loops() {
    left_button.loop();
    right_button.loop();
    up_button.loop();
    down_button.loop();
}

/* DISPLAY OPTIONS */
// Idle Display - display_main_screen in oled.h
void idle_display(void){
    // Clear Display
    u8g2.clearBuffer();
    // Change Font.
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0,20,"Idle Display");
    u8g2.drawStr(0,40,"Displays Temperatures");
    u8g2.sendBuffer(); 
    // DON'T PUT DELAY FOR FASTER BUTTONS.
}

// Initialize the Display when Powered on
void start_display(void){
    // Clear Display.
    u8g2.clearBuffer();
    // Change Font.
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(OLED_WIDTH/2,OLED_HEIGHT/2,"Smart Vent");
    u8g2.drawStr(OLED_WIDTH/2,OLED_HEIGHT/2 + 20,"Auto Pilot");     // Possibly change static 20 into Smart Height.
    u8g2.sendBuffer();
    // Delay so logo display is seen.
    delay(START_DELAY*1000);
}

// Display Item Menu
void display_select_menu(int menu_num) {

    // Display Center Item
    u8g2.setFont(u8g2_font_t0_16_tr);
    int centerX = (OLED_WIDTH - u8g2.getStrWidth(menuItems[menu_num].name )) / 2;
    int centerY = 38;
    u8g2.drawStr(centerX, centerY, menuItems[menu_num].name);
    

    // Display Upper Item if not empty.
    if ((menu_num-1) >= 0) {
        u8g2.setFont(u8g2_font_t0_12_tr);
        int centerX = (OLED_WIDTH - u8g2.getStrWidth(menuItems[menu_num-1].name )) / 2;
    int centerY = 17;
        u8g2.drawStr(centerX, centerY, menuItems[menu_num-1].name);
    }
    // Display Down Item if not empty.
    if ((menu_num+1) < MENU_ITEMS_COUNT) {
        u8g2.setFont(u8g2_font_t0_12_tr);
        int centerX = (OLED_WIDTH - u8g2.getStrWidth(menuItems[menu_num+1].name )) / 2;
        int centerY = 58;
        u8g2.drawStr(centerX, centerY, menuItems[menu_num+1].name);
    }

    // Formating and Display
    u8g2.drawLine(0, 21, 127, 21);
    u8g2.drawLine(0, 42, 127, 42);
    u8g2.drawLine(7, 0, 120, 0);
    u8g2.drawLine(7, 63, 120, 63);
    u8g2.drawLine(1, 57, 6, 62);
    u8g2.drawLine(1, 6, 6, 1);
    u8g2.drawLine(127, 7, 127, 56);
    u8g2.drawLine(0, 7, 0, 56);
    u8g2.drawLine(121, 62, 126, 57);
    u8g2.drawLine(121, 1, 126, 6);
    u8g2.drawFrame(2, 20, 124, 25);
    u8g2.drawFrame(1, 22, 126, 22);
}
// Display Format for CO2 Menu
void display_CO2_menu(int menu) {
    // Set Font
    u8g2.setFont(u8g2_font_t0_16_tr);
    // String Buffer
    char intStrBuffer[20];

    // Lower Threshold
    itoa(menuItems[menu].values[0], intStrBuffer, 10);
    u8g2.drawStr(22, 51, intStrBuffer);
    // Upper Threshold
    itoa(menuItems[menu].values[1], intStrBuffer, 10);
    u8g2.drawStr(76, 51, intStrBuffer);

    // Display Items
    u8g2.drawFrame(0, 0, 128, 17);
    u8g2.setFont(u8g2_font_t0_16b_tr);
    u8g2.drawStr(14, 14, "C02 Threshold");
    u8g2.setFont(u8g2_font_t0_14_tr);
    u8g2.drawStr(28, 32, "LOW");
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(76, 45, "");
    u8g2.setFont(u8g2_font_t0_14_tr);
    u8g2.drawStr(80, 32, "HIGH");
    u8g2.drawFrame(1, 1, 126, 17);
    u8g2.drawFrame(63, 38, 4, 4);
    u8g2.drawFrame(64, 39, 2, 2);
    u8g2.drawFrame(63, 47, 4, 4);
    u8g2.drawFrame(64, 48, 2, 2);
}
// Display for Fan Speed Menu
void display_fan_speed(int menu){
    u8g2.setFont(u8g2_font_t0_16_tr);
    
    // Display Upper Item if not empty.
    int centerY = 30;
    if (menuItems[menu-1].values[0] - 1 == MAX_NUM_FAN_SPEED) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Maximum")) / 2;
        u8g2.drawStr(centerX, centerY, "Maximum");
    }
    else if (menuItems[menu-1].values[0] - 1 == MAX_NUM_FAN_SPEED - 1) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Fast")) / 2;
        u8g2.drawStr(centerX, centerY, "Fast");
    }
    else if (menuItems[menu-1].values[0] - 1 == MAX_NUM_FAN_SPEED - 2) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Slow")) / 2;
        u8g2.drawStr(centerX, centerY, "Slow");
    }
    else if (menuItems[menu-1].values[0] - 1 == 0) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Off")) / 2;
        u8g2.drawStr(centerX, centerY, "Off");
    }
    // Display Center Item being selected.
    centerY = 45;
    if (menuItems[menu-1].values[0] == MAX_NUM_FAN_SPEED) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Maximum")) / 2;
        u8g2.drawStr(centerX, centerY, "Maximum");
    }
    else if (menuItems[menu-1].values[0] == MAX_NUM_FAN_SPEED - 1) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Fast")) / 2;
        u8g2.drawStr(centerX, centerY, "Fast");
    }
    else if (menuItems[menu-1].values[0] == MAX_NUM_FAN_SPEED - 2) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Slow")) / 2;
        u8g2.drawStr(centerX, centerY, "Slow");
    }
    else if (menuItems[menu-1].values[0] == 0) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Off")) / 2;
        u8g2.drawStr(centerX, centerY, "Off");
    }
    
    // Display Down Item if not empty.
    centerY = 60;
    if (menuItems[menu-1].values[0] + 1 == MAX_NUM_FAN_SPEED) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Maximum")) / 2;
        u8g2.drawStr(centerX, centerY, "Maximum");
    }
    else if (menuItems[menu-1].values[0] + 1 == MAX_NUM_FAN_SPEED - 1) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Fast")) / 2;
        u8g2.drawStr(centerX, centerY, "Fast");
    }
    else if (menuItems[menu-1].values[0] + 1 == MAX_NUM_FAN_SPEED - 2) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Slow")) / 2;
        u8g2.drawStr(centerX, centerY, "Slow");
    }
    else if (menuItems[menu-1].values[0] + 1 == 0) {
        int centerX = (OLED_WIDTH - u8g2.getStrWidth("Off")) / 2;
        u8g2.drawStr(centerX, centerY, "Off");
    }

    // Display Formattings
    u8g2.setFont(u8g2_font_t0_16b_tr);
    u8g2.drawStr(30, 14, "Fan Speed");
    u8g2.drawFrame(0, 0, 128, 17);
    u8g2.drawFrame(1, 1, 126, 17);
    u8g2.drawFrame(11, 31, 106, 18);
    u8g2.drawFrame(10, 32, 108, 16);
}

/*** SETUP ***/
void setup() {
    Serial.begin(9600);
    
    u8g2.begin();
    
    
    delay(50);

    Serial.println("\nSmart Vent \n");

    // Initialize Buttons
    init_buttons();
    /* 
    // Initialize Timer for idle counter.
    timer = timerBegin(0, 80, true); // Initialize timer
    timerAttachInterrupt(timer, &idle_on, true); // Attach interrupt
    timerAlarmWrite(timer, IDLE_TIME * 1000000, false); // Set alarm value to seconds, disable auto-reload
    timerAlarmEnable(timer); // Enable timer alarm 
    */

    start_display();
}


/*** LOOP ***/
void loop() {
  // NAVIGATION SYSTEM
  u8g2.firstPage();
  do {
    // Check for buttons pushes.
    button_loops();
    
    // Clear Display
    u8g2.clearBuffer();

    // Set font
    u8g2.setFont(u8g2_font_t0_16_tr);	// primary font
    //u8g.setFont(u8g_font_10x20);  // secondary font
    
    // Checks for Idle Flag.
    if (idle_flag == 1){
        // Display Idle Display.
        Serial.println("Idling...\n");
        idle_display();
    }
    else {
        // Selecting Menu Options
        if (selectedMenuItem == 0){
            // TODO: Test display_select_menu(selectedMenuItem) function
            display_select_menu(currentMenuItem);
        }
        // Selected CO2 Level Options
        if (selectedMenuItem == 1){
            // Set Selected Menu Item
            display_CO2_menu(selectedMenuItem-1);
        }
        // Selected Fan Speed Option
        if (selectedMenuItem == 2){
            display_fan_speed(selectedMenuItem);
        }
        // Selected Idle Time Option
        if (selectedMenuItem == 3){
            // TODO: Change formating to display 5s, 10s, 30s, 60s.
            char intStrBuffer[20];
            itoa(menuItems[selectedMenuItem-1].values[0], intStrBuffer, 10);
            int centerX = (OLED_WIDTH - u8g2.getStrWidth(intStrBuffer)) / 2;
            int centerY = (OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2;
            u8g2.drawStr(centerX, centerY, intStrBuffer);
        }
        // Selected Time and Date Option
        if (selectedMenuItem == 4){
            // TODO: Change the Image format (with :'s).
            char intStrBuffer[20];
            // Hour
            itoa( menuItems[selectedMenuItem-1].values[0], intStrBuffer, 10);
            int centerX = (OLED_WIDTH - u8g2.getStrWidth(intStrBuffer)) / 4;
            int centerY = (OLED_HEIGHT - (u8g2.getFontAscent() - u8g2.getFontDescent())) / 2;
            u8g2.drawStr(centerX, centerY, intStrBuffer);	// write something to the internal memory
            // Minutes
            itoa( menuItems[selectedMenuItem-1].values[1], intStrBuffer, 10);
            int centerX2 = ((OLED_WIDTH - u8g2.getStrWidth(intStrBuffer)) / 4) + (OLED_WIDTH / 3);
            u8g2.drawStr(centerX2, centerY, intStrBuffer);	// write something to the internal memory
            // AM or PM
            if (menuItems[selectedMenuItem-1].values[2] == 0) {
                u8g2.drawStr(centerX + (OLED_WIDTH / 2), centerY, "AM");
            }
            else {
                u8g2.drawStr(centerX + (OLED_WIDTH / 2), centerY, "PM");
            }
        }
    }
    
    // TODO: Put code to update to Dashboard. ** In this location it MUST have no delays.

    // Update Display
    u8g2.sendBuffer();
  } while (u8g2.nextPage());  
}