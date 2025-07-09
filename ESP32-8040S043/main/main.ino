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
#include <Arduino.h>
#pragma GCC optimize("Ofast")  //idk
#define LGFX_USE_V1
#define ARDUINOIDE 1

#include <lvgl.h>

// #if LV_USE_TFT_ESPI
// #include <TFT_eSPI.h>
// #endif
#include <LovyanGFX.hpp>                                // for physical display
#include <lgfx_user/LGFX_ESP32S3_RGB_ESP32-8048S043.h>  // for exact board config
#include "TAMC_GT911.h"

static std::uint32_t sec, psec;
static std::uint32_t fps = 0, frame_count = 0;
LGFX tft;
// TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

lv_display_t* display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);

static uint16_t* buf_1 = (uint16_t*)heap_caps_aligned_alloc(32, (SCREEN_WIDTH * SCREEN_HEIGHT * 2), MALLOC_CAP_DMA);
static uint16_t* buf_2 = (uint16_t*)heap_caps_aligned_alloc(32, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) / 6, MALLOC_CAP_DMA);

//  TOUCH
#define TOUCH_SDA 19
#define TOUCH_SCL 20
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_WIDTH 480   // 800
#define TOUCH_HEIGHT 272  // 480

TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

void my_disp_flush(lv_display_t* display, const lv_area_t* area, unsigned char* data) {
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  lv_draw_sw_rgb565_swap(data, w*h);
  if (tft.getStartCount() == 0) {   // Processing if not yet started
    
    tft.startWrite();
  }
  tft.pushImageDMA( area->x1
                , area->y1
                , area->x2 - area->x1 + 1
                , area->y2 - area->y1 + 1
                ,(uint16_t*) data); 

  lv_display_flush_ready(display);
}


void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.setBrightness(255);
  tft.setColorDepth(24);

  // TOUCH
  tp.begin();
  tp.setRotation(ROTATION_INVERTED);  // ROTATION_NORMAL|ROTATION_LEFT|ROTATION_INVERTED|ROTATION_RIGHT
  lv_init();
  lv_display_set_buffers(display, buf_1, buf_2, (SCREEN_WIDTH * SCREEN_HEIGHT) / 5, LV_DISPLAY_RENDER_MODE_PARTIAL);

  /*Change the following line to your display resolution*/
  lv_display_set_resolution(display, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(display, my_disp_flush);

  // disp_drv.direct_mode = 0;
  // disp_drv.full_refresh = 0;
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
  lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
  //touch
  // static lv_indev_data_t indev_drv;
  // lv_indev_drv_init( &indev_drv );
  // indev_drv.type = LV_INDEV_TYPE_POINTER;
  // indev_drv.read_cb = my_touchpad_read;
  // lv_indev_drv_register( &indev_drv );
}

void loop() {
  lv_timer_handler();  // works
  // delay(5);
  vTaskDelay(5 / portTICK_PERIOD_MS);
}
// Read the touchpad
void my_touchpad_read(lv_indev_data_t* indev_driver, lv_indev_data_t* data) {
  tp.read();
  bool touched = tp.isTouched;
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = tp.points[0].x * 8.5;
    data->point.y = tp.points[0].y * 1.75;
  }
}