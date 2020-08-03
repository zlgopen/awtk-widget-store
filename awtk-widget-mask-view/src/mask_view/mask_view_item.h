/**
 * File:   mask_view_item.h
 * Author: AWTK Develop Team
 * Brief:  mask_view 控件的采样区域
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
 * 2020-07-22 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */


#ifndef TK_MASK_VIEW_ITEM_H
#define TK_MASK_VIEW_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class mask_view_item_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * mask_view 控件的采样区域
 */
typedef struct _mask_view_item_t {
  widget_t widget;

  /**
   * @property {bool_t} mask_view
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为蒙板区。
   */
  bool_t mask_view;
} mask_view_item_t;

/**
 * @method mask_view_item_create
 * @annotation ["constructor", "scriptable"]
 * 创建mask_view_item对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} mask_view_item对象。
 */
widget_t* mask_view_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method mask_view_item_cast
 * 转换为mask_view_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget mask_view_item对象。
 *
 * @return {widget_t*} mask_view_item对象。
 */
widget_t* mask_view_item_cast(widget_t* widget);

/**
 * @method mask_view_item_set_mask_view
 * 设置是否为蒙板采样区域。
 * TRUE 为蒙板采样区域，FALSE 为背景图采样区域
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} mask_view 是否为蒙板采样区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mask_view_item_set_mask_view(widget_t* widget, bool_t mask_view);

/**
 * @method mask_view_item_get_mask_view
 * 获取是否为蒙板采样区域。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回 mask_view_item 是否为蒙板采样区。
 */
bool_t mask_view_item_get_mask_view(widget_t* widget);

#define WIDGET_TYPE_MASK_VIEW_ITEM "mask_view_item"
#define MASK_VIEW_PROP_MASK_VIEW "mask_view"

#define MASK_VIEW_ITEM(widget) ((mask_view_item_t*)(mask_view_item_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(mask_view_item);

END_C_DECLS

#endif /*TK_MASK_VIEW_ITEM_H*/
