/**
 * File:   menu_item.h
 * Author: AWTK Develop Team
 * Brief:  menu_item
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
 * 2018-09-20 Li PeiHuang <lipeihuang@zlg.cn> created created
 *
 */

#ifndef TK_MENU_ITEM_H
#define TK_MENU_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class menu_item_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 列表项控件。
 */
typedef struct _menu_item_t {
  widget_t widget;

  bool_t checked;
  bool_t selected;

  /*private*/
  bool_t dragged;
  point_t down;
  bool_t pressed;
} menu_item_t;

/**
 * @method menu_item_create
 * @annotation ["constructor", "scriptable"]
 * 创建menu_item对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* menu_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method menu_item_cast
 * 转换为menu_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget menu_item对象。
 *
 * @return {widget_t*} menu_item对象。
 */
widget_t* menu_item_cast(widget_t* widget);

ret_t menu_item_set_selected(widget_t* widget, bool_t selected);
ret_t menu_item_set_selected_only(widget_t* widget, bool_t selected);

#define WIDGET_TYPE_MENU_ITEM "menu_item"
#define MENU_ITEM(widget) ((menu_item_t*)(menu_item_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(menu_item);

END_C_DECLS

#endif /*TK_MENU_ITEM_H*/
