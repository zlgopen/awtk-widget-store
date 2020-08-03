/**
 * File:   button_ex.h
 * Author: AWTK Develop Team
 * Brief:  button_ex
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUTTON_EX_H
#define TK_BUTTON_EX_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class button_ex_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 *
 * 扩展按钮。点击或离开按钮时，创建过度动画。
 */
typedef struct _button_ex_t {
  widget_t widget;

  /**
   * @property {xy_t} new_h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 新的高度。
   */
  xy_t new_h;

  /**
   * @property {xy_t} old_h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 旧的高度。
   */
  xy_t old_h;

  /*private*/
  int32_t timer_id;
  bool_t dragged;
  point_t down;

  widget_on_paint_children_t on_paint_children;
} button_ex_t;

/**
 * @event {pointer_event_t} EVT_CLICK
 * 点击事件。
 */

/**
 * @event {pointer_event_t} EVT_LONG_PRESS
 * 长按事件。
 */

/**
 * @method button_ex_create
 * 创建button_ex对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* button_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method button_ex_cast
 * 转换为button_ex对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget button_ex对象。
 *
 * @return {widget_t*} button_ex对象。
 */
widget_t* button_ex_cast(widget_t* widget);

/**
 * @method button_ex_set_new_h
 * 设置新高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  new_h 新高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t button_ex_set_new_h(widget_t* widget, int32_t new_h);

#define BUTTON_EX(widget) ((button_ex_t*)(button_ex_cast(WIDGET(widget))))

#define WIDGET_TYPE_BUTTON_EX "button_ex"
#define WIDGET_PROP_NEW_H "new_h"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(button_ex);

END_C_DECLS

#endif /*TK_BUTTON_EX_H*/
