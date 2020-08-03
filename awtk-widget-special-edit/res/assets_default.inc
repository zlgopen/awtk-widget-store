#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/default/inc/ui/main.data"
#include "assets/default/inc/styles/default.data"
#include "assets/default/inc/styles/main.data"
#ifdef WITH_STB_IMAGE
#include "assets/default/inc/images/arrow_down_d.res"
#include "assets/default/inc/images/arrow_down_f.res"
#include "assets/default/inc/images/arrow_down_n.res"
#include "assets/default/inc/images/arrow_down_o.res"
#include "assets/default/inc/images/arrow_down_p.res"
#include "assets/default/inc/images/arrow_left_d.res"
#include "assets/default/inc/images/arrow_left_f.res"
#include "assets/default/inc/images/arrow_left_n.res"
#include "assets/default/inc/images/arrow_left_o.res"
#include "assets/default/inc/images/arrow_left_p.res"
#include "assets/default/inc/images/arrow_right_d.res"
#include "assets/default/inc/images/arrow_right_f.res"
#include "assets/default/inc/images/arrow_right_n.res"
#include "assets/default/inc/images/arrow_right_o.res"
#include "assets/default/inc/images/arrow_right_p.res"
#include "assets/default/inc/images/arrow_up_d.res"
#include "assets/default/inc/images/arrow_up_f.res"
#include "assets/default/inc/images/arrow_up_n.res"
#include "assets/default/inc/images/arrow_up_o.res"
#include "assets/default/inc/images/arrow_up_p.res"
#include "assets/default/inc/images/check_checked.res"
#include "assets/default/inc/images/check_checked_d.res"
#include "assets/default/inc/images/check_unchecked.res"
#include "assets/default/inc/images/check_unchecked_d.res"
#include "assets/default/inc/images/radio_checked.res"
#include "assets/default/inc/images/radio_checked_d.res"
#include "assets/default/inc/images/radio_checked_d_d.res"
#include "assets/default/inc/images/radio_unchecked.res"
#else
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#if defined(WITH_TRUETYPE_FONT)
#include "assets/default/inc/fonts/default.res"
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*WITH_FS_RES*/

ret_t assets_init(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "default");

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_FONT, "default");
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, ui_main);
  assets_manager_add(am, style_default);
  assets_manager_add(am, style_main);
  assets_manager_add(am, image_arrow_down_d);
  assets_manager_add(am, image_arrow_down_f);
  assets_manager_add(am, image_arrow_down_n);
  assets_manager_add(am, image_arrow_down_o);
  assets_manager_add(am, image_arrow_down_p);
  assets_manager_add(am, image_arrow_left_d);
  assets_manager_add(am, image_arrow_left_f);
  assets_manager_add(am, image_arrow_left_n);
  assets_manager_add(am, image_arrow_left_o);
  assets_manager_add(am, image_arrow_left_p);
  assets_manager_add(am, image_arrow_right_d);
  assets_manager_add(am, image_arrow_right_f);
  assets_manager_add(am, image_arrow_right_n);
  assets_manager_add(am, image_arrow_right_o);
  assets_manager_add(am, image_arrow_right_p);
  assets_manager_add(am, image_arrow_up_d);
  assets_manager_add(am, image_arrow_up_f);
  assets_manager_add(am, image_arrow_up_n);
  assets_manager_add(am, image_arrow_up_o);
  assets_manager_add(am, image_arrow_up_p);
  assets_manager_add(am, image_check_checked);
  assets_manager_add(am, image_check_checked_d);
  assets_manager_add(am, image_check_unchecked);
  assets_manager_add(am, image_check_unchecked_d);
  assets_manager_add(am, image_radio_checked);
  assets_manager_add(am, image_radio_checked_d);
  assets_manager_add(am, image_radio_checked_d_d);
  assets_manager_add(am, image_radio_unchecked);
  assets_manager_add(am, font_default);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif

  tk_init_assets();
  return RET_OK;
}
