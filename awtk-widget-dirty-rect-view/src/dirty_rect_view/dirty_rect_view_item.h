/**
 * File:   dirty_rect_view_item_t.h
 * Author: AWTK Develop Team
 * Brief:  自定义脏矩形模块控件
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
 * 2020-06-16 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */
 
#ifndef TK_DIRTY_RECT_VIEW_ITEM_H
#define TK_DIRTY_RECT_VIEW_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dirty_rect_view_item_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 自定义脏矩形模块控件
 */
typedef struct _dirty_rect_view_item_t {
  widget_t widget;
  
  /**
   * @property {int32_t} invalidate_number
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 刷新次数。
   */
  int32_t invalidate_number;

  /**
   * @property {bool_t} invalidate_background
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 刷新次数。
   */
  bool_t invalidate_background;

} dirty_rect_view_item_t;

/**
 * @method dirty_rect_view_item_create
 * @annotation ["constructor", "scriptable"]
 * 创建dirty_rect_view_item对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t *dirty_rect_view_item_create(widget_t *parent, xy_t x, xy_t y, wh_t w,
                                      wh_t h);

/**
* @method dirty_rect_view_item_set_invalidate_number
* 设置刷新次数。
* @annotation ["scriptable"]
* @param {widget_t*} widget 控件对象。
* @param {int32_t}  invalidate_number 刷新次数。
*
* @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t dirty_rect_view_item_set_invalidate_number(widget_t *widget,
                                                 int32_t invalidate_number);

/**
* @method dirty_rect_view_item_set_paint_background
* 设置控件对应的 dirty_rect_view 控件的背景图区域。
* @annotation ["scriptable"]
* @param {widget_t*} widget 控件对象。
* @param {bool_t}  invalidate_background 控件对应的 dirty_rect_view
*控件的背景图区域。
*
* @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t dirty_rect_view_item_set_paint_background(widget_t *widget,
                                                bool_t invalidate_background);

/**
* @method dirty_rect_view_item_get_is_paint
* 获取控件是否需要绘制。
* @annotation ["scriptable"]
* @param {widget_t*} widget 控件对象。。
*
* @return {bool_t} 返回TRUE表示需要，否则表示不需要。
*/
bool_t dirty_rect_view_item_get_is_paint(widget_t *widget);

/**
* @method dirty_rect_view_item_get_is_paint_background
* 获取控件是否需要绘制控件对应的 dirty_rect_view 控件的背景图区域。
* @annotation ["scriptable"]
* @param {widget_t*} widget 控件对象。。
*
* @return {bool_t} 返回TRUE表示需要，否则表示不需要。
*/
bool_t dirty_rect_view_item_get_is_paint_background(widget_t *widget);

/**
 * @method dirty_rect_view_item_cast
 * 转换为dirty_rect_view_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget dirty_rect_view_item对象。
 *
 * @return {widget_t*} dirty_rect_view_item对象。
 */
widget_t *dirty_rect_view_item_cast(widget_t *widget);

#define DIRTY_RECT_VIEW_ITEM(widget)                                           \
  ((dirty_rect_view_item_t *)(dirty_rect_view_item_cast(WIDGET(widget))))

#define WIDGET_TYPE_DIRTY_RECT_VIEW_ITEM "dirty_rect_view_item"

#define DIRTY_RECT_VIEW_ITEM_INVALIDATE_NUMBER "invalidate_number"

#define DIRTY_RECT_VIEW_ITEM_INVALIDATE_BACKGROUND "invalidate_background"

END_C_DECLS

#endif /*TK_DIRTY_RECT_VIEW_ITEM_H*/
