#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "default/inc/styles/main.data"
#include "default/inc/styles/default.data"
#include "default/inc/ui/main.data"
#ifdef WITH_STB_IMAGE
#include "default/inc/images/check_checked_d.res"
#include "default/inc/images/check_unchecked_d.res"
#include "default/inc/images/check_unchecked.res"
#include "default/inc/images/logo.res"
#include "default/inc/images/check_checked.res"
#else
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
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
  assets_manager_add(am, style_main);
  assets_manager_add(am, style_default);
  assets_manager_add(am, ui_main);
  assets_manager_add(am, image_check_checked_d);
  assets_manager_add(am, image_check_unchecked_d);
  assets_manager_add(am, image_check_unchecked);
  assets_manager_add(am, image_logo);
  assets_manager_add(am, image_check_checked);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif

  tk_init_assets();
  return RET_OK;
}
