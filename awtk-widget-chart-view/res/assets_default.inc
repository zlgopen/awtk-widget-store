#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/default/inc/ui/home_page.data"
#include "assets/default/inc/ui/system_bar.data"
#include "assets/default/inc/ui/window_bar_series.data"
#include "assets/default/inc/ui/window_bar_series_horizontal.data"
#include "assets/default/inc/ui/window_bar_series_inverse.data"
#include "assets/default/inc/ui/window_bar_series_minmax.data"
#include "assets/default/inc/ui/window_bar_series_parall.data"
#include "assets/default/inc/ui/window_line_series.data"
#include "assets/default/inc/ui/window_line_series_category.data"
#include "assets/default/inc/ui/window_line_series_colorful.data"
#include "assets/default/inc/ui/window_line_series_cover.data"
#include "assets/default/inc/ui/window_line_series_inverse.data"
#include "assets/default/inc/ui/window_line_series_more_axis.data"
#include "assets/default/inc/ui/window_line_series_parall.data"
#include "assets/default/inc/ui/window_line_series_vertical.data"
#include "assets/default/inc/ui/window_pie.data"
#include "assets/default/inc/styles/default.data"
#ifdef WITH_STB_IMAGE
#include "assets/default/inc/images/back_normal.res"
#include "assets/default/inc/images/back_pressed.res"
#include "assets/default/inc/images/close_d.res"
#include "assets/default/inc/images/close_n.res"
#include "assets/default/inc/images/close_o.res"
#include "assets/default/inc/images/close_p.res"
#include "assets/default/inc/images/cross.res"
#include "assets/default/inc/images/earth.res"
#include "assets/default/inc/images/icon_1_normal.res"
#include "assets/default/inc/images/icon_1_pressed.res"
#include "assets/default/inc/images/icon_2_normal.res"
#include "assets/default/inc/images/icon_2_pressed.res"
#include "assets/default/inc/images/icon_3_normal.res"
#include "assets/default/inc/images/icon_3_pressed.res"
#include "assets/default/inc/images/icon_4_normal.res"
#include "assets/default/inc/images/icon_4_pressed.res"
#include "assets/default/inc/images/icon_5_normal.res"
#include "assets/default/inc/images/icon_5_pressed.res"
#include "assets/default/inc/images/icon_6_normal.res"
#include "assets/default/inc/images/icon_6_pressed.res"
#include "assets/default/inc/images/new_normal.res"
#include "assets/default/inc/images/new_pressed.res"
#include "assets/default/inc/images/pause_mormal.res"
#include "assets/default/inc/images/pause_pressed.res"
#include "assets/default/inc/images/pie_label_normal.res"
#include "assets/default/inc/images/pie_label_select.res"
#include "assets/default/inc/images/play_normal.res"
#include "assets/default/inc/images/play_pressed.res"
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
  assets_manager_add(am, ui_home_page);
  assets_manager_add(am, ui_system_bar);
  assets_manager_add(am, ui_window_bar_series);
  assets_manager_add(am, ui_window_bar_series_horizontal);
  assets_manager_add(am, ui_window_bar_series_inverse);
  assets_manager_add(am, ui_window_bar_series_minmax);
  assets_manager_add(am, ui_window_bar_series_parall);
  assets_manager_add(am, ui_window_line_series);
  assets_manager_add(am, ui_window_line_series_category);
  assets_manager_add(am, ui_window_line_series_colorful);
  assets_manager_add(am, ui_window_line_series_cover);
  assets_manager_add(am, ui_window_line_series_inverse);
  assets_manager_add(am, ui_window_line_series_more_axis);
  assets_manager_add(am, ui_window_line_series_parall);
  assets_manager_add(am, ui_window_line_series_vertical);
  assets_manager_add(am, ui_window_pie);
  assets_manager_add(am, style_default);
  assets_manager_add(am, image_back_normal);
  assets_manager_add(am, image_back_pressed);
  assets_manager_add(am, image_close_d);
  assets_manager_add(am, image_close_n);
  assets_manager_add(am, image_close_o);
  assets_manager_add(am, image_close_p);
  assets_manager_add(am, image_cross);
  assets_manager_add(am, image_earth);
  assets_manager_add(am, image_icon_1_normal);
  assets_manager_add(am, image_icon_1_pressed);
  assets_manager_add(am, image_icon_2_normal);
  assets_manager_add(am, image_icon_2_pressed);
  assets_manager_add(am, image_icon_3_normal);
  assets_manager_add(am, image_icon_3_pressed);
  assets_manager_add(am, image_icon_4_normal);
  assets_manager_add(am, image_icon_4_pressed);
  assets_manager_add(am, image_icon_5_normal);
  assets_manager_add(am, image_icon_5_pressed);
  assets_manager_add(am, image_icon_6_normal);
  assets_manager_add(am, image_icon_6_pressed);
  assets_manager_add(am, image_new_normal);
  assets_manager_add(am, image_new_pressed);
  assets_manager_add(am, image_pause_mormal);
  assets_manager_add(am, image_pause_pressed);
  assets_manager_add(am, image_pie_label_normal);
  assets_manager_add(am, image_pie_label_select);
  assets_manager_add(am, image_play_normal);
  assets_manager_add(am, image_play_pressed);
  assets_manager_add(am, font_default);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif

  tk_init_assets();
  return RET_OK;
}
