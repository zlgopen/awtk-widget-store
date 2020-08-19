/**
 * File:   x_axis.h
 * Author: AWTK Develop Team
 * Brief:  x axis
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-12-05 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_X_AXIS_H
#define TK_X_AXIS_H

#include "axis.h"

BEGIN_C_DECLS

/**
 * @class x_axis_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * x坐标轴控件。
 *
 * 在xml中使用"x\_axis"标签创建扩展按钮控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <x_axis x="c" y="50" w="2" h="100" />
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <!-- style -->
 * <x_axis>
 *  <style name="default">
 *      <normal text_color="#a1d8ef" font_size="20" split_line_color="#213b4d" tick_color="#294b63" fg_color="#294b63" />
 *  </style>
 * </x_axis>
 * ```
 */
typedef struct _x_axis_t {
  axis_t base;
  /**
   * @property {bool_t} x_defined
   * @annotation ["readable"]
   * x坐标是否确定。
   */
  uint8_t x_defined : 1;
  /**
   * @property {bool_t} w_defined
   * @annotation ["readable"]
   * w宽度是否确定。
   */
  uint8_t w_defined : 1;
} x_axis_t;

/**
 * @method x_axis_create
 * 创建axis对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* x_axis_create(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method x_axis_cast
 * 转换为axis对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget axis对象。
 *
 * @return {widget_t*} axis对象。
 */
widget_t* x_axis_cast(widget_t* widget);

/**
 * @method x_axis_register
 * 注册x_axis控件。
 *
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t x_axis_register(void);

#define X_AXIS(widget) ((x_axis_t*)(x_axis_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_X_AXIS_H*/
