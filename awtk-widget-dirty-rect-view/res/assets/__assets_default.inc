#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "default/inc/ui/main.data"
#include "default/inc/styles/default.data"
#ifdef WITH_STB_IMAGE
#include "default/inc/images/bg.res"
#include "default/inc/images/clock.res"
#include "default/inc/images/clock_bg.res"
#include "default/inc/images/clock_hour.res"
#include "default/inc/images/clock_minute.res"
#include "default/inc/images/clock_second.res"
#include "default/inc/images/guage_bg.res"
#include "default/inc/images/guage_pointer.res"
#include "default/inc/images/num_0.res"
#include "default/inc/images/num_1.res"
#include "default/inc/images/num_2.res"
#include "default/inc/images/num_3.res"
#include "default/inc/images/num_4.res"
#include "default/inc/images/num_5.res"
#include "default/inc/images/num_6.res"
#include "default/inc/images/num_7.res"
#include "default/inc/images/num_8.res"
#include "default/inc/images/num_9.res"
#include "default/inc/images/num_dot.res"
#else
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#if defined(WITH_TRUETYPE_FONT)
#include "default/inc/fonts/default.res"
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*WITH_FS_RES*/

ret_t assets_init_default(void) {
  assets_manager_t* am = assets_manager();

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_FONT, "default");
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, ui_main);
  assets_manager_add(am, style_default);
  assets_manager_add(am, image_bg);
  assets_manager_add(am, image_clock);
  assets_manager_add(am, image_clock_bg);
  assets_manager_add(am, image_clock_hour);
  assets_manager_add(am, image_clock_minute);
  assets_manager_add(am, image_clock_second);
  assets_manager_add(am, image_guage_bg);
  assets_manager_add(am, image_guage_pointer);
  assets_manager_add(am, image_num_0);
  assets_manager_add(am, image_num_1);
  assets_manager_add(am, image_num_2);
  assets_manager_add(am, image_num_3);
  assets_manager_add(am, image_num_4);
  assets_manager_add(am, image_num_5);
  assets_manager_add(am, image_num_6);
  assets_manager_add(am, image_num_7);
  assets_manager_add(am, image_num_8);
  assets_manager_add(am, image_num_9);
  assets_manager_add(am, image_num_dot);
  assets_manager_add(am, font_default);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif

  tk_init_assets();
  return RET_OK;
}
