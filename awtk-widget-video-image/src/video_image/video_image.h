/**
 * File:   video_image.h
 * Author: AWTK Develop Team
 * Brief:  帧间差异视频播放控件
 *
 * Copyright (c) 2020 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-06-18 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_VIDEO_IMAGE_H
#define TK_VIDEO_IMAGE_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class video_image_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 帧间差异视频播放控件
 */
typedef struct _video_image_t {
  widget_t widget;

  /**
   * @property {char*} video_name
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 视频名字。
   */
  char *video_name;

  /**
   * @property {bool_t} auto_play
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动播放。
   */
  bool_t auto_play;

  /**
   * @property {uint32_t} delay_paly
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 延迟播放。
   */
  uint32_t delay_paly;

  /*private*/
  bool_t is_done;
  bool_t is_ready;
  bool_t is_playing;
  bool_t is_get_frame;

  bitmap_t bitmap;
  uint32_t timer_id;
  uint32_t frame_interval;
  uint32_t frame_number_max;
  uint32_t frame_number_curr;

  asset_info_t *video_asset_info;

} video_image_t;

/**
 * @method video_image_create
 * @annotation ["constructor", "scriptable"]
 * 创建video_image对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} video_image对象。
 */
widget_t *video_image_create(widget_t *parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method video_image_cast
 * 转换为video_image对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget video_image对象。
 *
 * @return {widget_t*} video_image对象。
 */
widget_t *video_image_cast(widget_t *widget);

/**
 * @method video_image_set_video_name
 * 设置视频名字。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} video_name 视频名字。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t video_image_set_video_name(widget_t *widget, const char *video_name);

/**
 * @method video_image_set_auto_play
 * 设置是否自动播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} auto_play 是否自动播放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t video_image_set_auto_play(widget_t *widget, bool_t auto_play);

/**
 * @method video_image_set_delay_paly
 * 设置延迟播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} delay_paly 延迟播放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t video_image_set_delay_paly(widget_t *widget, uint32_t delay_paly);

#define VIDEO_IMAGE_PROP_VIDEO_NAME "video_name"
#define VIDEO_IMAGE_PROP_AUTO_PLAY "auto_play"
#define VIDEO_IMAGE_PROP_DELAY_PALY "delay_paly"

#define WIDGET_TYPE_VIDEO_IMAGE "video_image"

#define VIDEO_IMAGE(widget)                                                    \
  ((video_image_t *)(video_image_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(video_image);

END_C_DECLS

#endif /*TK_VIDEO_IMAGE_H*/
