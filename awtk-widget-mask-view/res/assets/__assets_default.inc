#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "default/inc/styles/default.data"
#include "default/inc/ui/main.data"
#ifdef WITH_STB_IMAGE
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
  assets_manager_add(am, style_default);
  assets_manager_add(am, ui_main);
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
