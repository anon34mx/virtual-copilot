// LVGL
static lv_disp_draw_buf_t disp_buf;
static lv_color_t* buf_1 = (lv_color_t*) head_caps_aligned_alloc(32, (SCREEN_WIDTH * SCREEN_HEIGHT * 2), MALLOC_CAP_DMA);
static lv_color_t* buf_2= (lv_color_t*) head_caps_aligned_alloc(32, (SCREEN_WIDTH * SCREEN_HEIGHT * 2)/6, MALLOC_CAP_DMA);
static lv_color_t buf[SCREEN_WIDTH * 10];

static lv_disp_drv_t disp_drv;
lv_disp_t * disp;

lv_obj_t* dashboard1_screen;

my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void lvglInit(){
  lv_init();
  lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, (SCREEN_WIDTH*SCREEN_HEIGHT)/6, LV_DISPLAY_RENDER_MODE_FULL);
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &disp_buf;

  disp_drv.direct_mode = 0;
  disp_drv.full_refresh = 0;
  disp = lv_disp_drv_register(&disp_drv);
  disp_drv.sw_rotate = 1;
  lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE); // LV_DISP_ROT_NONE|LV_DISP_ROT_90|LV_DISP_ROT_180|LV_DISP_ROT_270;

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );
}