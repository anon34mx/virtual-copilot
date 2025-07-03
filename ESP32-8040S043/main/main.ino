/*
  //=====================================================================
  // THX TO https://macsbug.wordpress.com/2022/11/29/esp32-8048s043/
  //=====================================================================
  //  HARD              : ESP32_8048S043C
  //  Display          : 4.3" 800x480 RGB LCD Touch GT911
  //  Dev environment   : Arduino IDE 1.8.19
  //  Board Manager    : arduino-esp32 2.0.5
  //  Board            : "ESP32S3 Dev Module"
  //  Upload Speed     : "921600"
  //  USB Mode         : "Hardware CDC and JTAG"
  //  USB CDC On Boot  : "Disable"
  //  USB Firmware MSC On Boot : "Disable"
  //  USB DFU On Boot  : "Disable"
  //  Upload Mode      : "UART0 / Hardware CDC"
  //  CPU Frequency    : "240MHz (WiFi/BT)"
  //  Core Degug Level : "None"
  //  Arduino Runs On  : "Core 1"
  //  Events Run On    : "Core 1"
  //  Rease All Flash before Sketxh Upload : "Disable"
  //--------------------------------------------------------------------
  //  Flash Mode       : "QIO 120MHz"
  //  Flash Size       : "16MB (128Mb)"
  //  Partition Scheme : "16MB Flash (2MB APP/12.5MB FATFS)"
  //  PSRAM            : "OPI PSRAM"
  //=====================================================================
*/

#pragma GCC optimize ("Ofast")
#include "ArduinoJson.h"

// DISPLAY
#include <LovyanGFX.hpp>
#include <lgfx_user/LGFX_ESP32S3_RGB_ESP32-8048S043.h>
#define LGFX_USE_V1
LGFX tft;
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

//  TOUCH
#include "TAMC_GT911.h"
#define TOUCH_SDA  19
#define TOUCH_SCL  20
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_WIDTH  480// 800
#define TOUCH_HEIGHT 272// 480

TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

JsonDocument doc;
String currentDisplay="";

String serialString;
byte serialInBuffer[0];
int incomingByte = 0;

void setup(void){
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.setBrightness(255); 
  tft.setColorDepth(24);

  // TOUCH  
  tp.begin();
  tp.setRotation(ROTATION_INVERTED); // ROTATION_NORMAL|ROTATION_LEFT|ROTATION_INVERTED|ROTATION_RIGHT

  tft.fillRect(10,10,34,34, TFT_RED);
}


void loop(){
  // readSerial();
}
// Read the touchpad
// void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
// {
//   tp.read();
//   bool touched = tp.isTouched;
//   if( !touched ){
//       data->state = LV_INDEV_STATE_REL;
//   }
//   else
//   {
//     data->state = LV_INDEV_STATE_PR;
//     /*Set the coordinates*/
//     data->point.x = tp.points[0].x*8.5;
//     data->point.y = tp.points[0].y*1.75;
//   }
// }

void readSerial(){
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    Serial.println(incomingByte, DEC);
  }
}
