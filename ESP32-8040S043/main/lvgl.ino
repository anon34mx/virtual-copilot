// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
// {
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   tft.startWrite();
//   tft.setAddrWindow(area->x1, area->y1, w, h);
//   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
//   tft.endWrite();

//   lv_disp_flush_ready(disp);
// }