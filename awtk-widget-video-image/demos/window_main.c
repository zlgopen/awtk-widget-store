#include "awtk.h"
#include "../src/video_image/video_image.h"

static ret_t on_close(void* ctx, event_t* e) {
  tk_quit();

  return RET_OK;
}

ret_t start_animation_end(void* ctx, event_t* e) {
  if (e->type == EVT_ANIM_END) {
    value_t v;
    value_set_bool(&v, TRUE);
    widget_set_prop(WIDGET(ctx), WIDGET_PROP_VISIBLE, &v);
  }

  return RET_OK;
}

/**
 * 初始化
 */
ret_t application_init(void) {
  video_image_register();

  widget_t* win = window_open("main");

  widget_t* label = widget_lookup_by_type(win, "label", TRUE);
  widget_t* video_image = widget_lookup_by_type(win, WIDGET_TYPE_VIDEO_IMAGE, TRUE);

  widget_on(video_image, EVT_ANIM_END, start_animation_end, label);  //设置开场动画播放完后的回调

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  log_debug("application_exit\n");
  return RET_OK;
}
