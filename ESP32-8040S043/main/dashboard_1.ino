const ruler_unit_t My_Units[] = {
    /*{ idx, angle,   label, size, distance,        fontFace, fontSize, textDatum }*/
      {   0,  0.0f, nullptr,    0,        0,         nullptr,     0.0f,  MC_DATUM },
      {   1,  2.0f,     "0",   -8,      -11,  &FreeSans9pt7b,     0.75f, MC_DATUM },
      {   2, 45.0f,    "50",   -8,      -11,  &FreeSans9pt7b,     0.75f, MC_DATUM },
      {   3, 90.0f,  "100%",   -8,      -11,  &FreeSans9pt7b,     0.75f, MC_DATUM },
    };
const ruler_t My_Ruler = {  0.0f,  90.0f, 150, 1, My_Units,       sizeof(My_Units)/sizeof(ruler_unit_t) };
const ruler_item_t items[] ={
  /*{ ruler_t,          palette color index }*/
  { &My_Ruler,        1                   },
};
const gauge_palette_t palette =
{
  0xffffffU, /*.transparent_color  */
  0x222222U, /*.fill_color         */
  0xff4444U, /*.warn_color         */
  0x00ff00U, /*.ok_color           */
  0xff2222U, /*.arrow_color        */
  0xaaaaaaU, /*.arrow_border_color */
  0x888888U, /*.arrow_shadow_color */
};
void dashboard1(){
  const gauge_t MY_CUSTOM_GAUGE =
    {
      .items       = items,
      .items_count = sizeof(items)/sizeof(ruler_item_t),
      .palette     = palette,
      .start       = -45.0f,
      .end         =  45.0f
    };
}
