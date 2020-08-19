/**
 * File:   y_axis.h
 * Author: AWTK Develop Team
 * Brief:  y axis
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

#ifndef TK_Y_AXIS_H
#define TK_Y_AXIS_H

#include "axis.h"

BEGIN_C_DECLS

/**
 * @class y_axis_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * y坐标轴控件。
 * 
 * 在xml中使用"y\_axis"标签创建扩展按钮控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <y_axis x="c" y="50" w="2" h="100" />
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <!-- style -->
 * <y_axis>
 *  <style name="default">
 *      <normal text_color="#a1d8ef" font_size="20" split_line_color="#213b4d" tick_color="#294b63" fg_color="#294b63" />
 *  </style>
 * </y_axis>
 * ```
 */
typedef struct _y_axis_t {
  axis_t base;
  /**
   * @property {bool_t} y_defined
   * @annotation ["readable"]
   * y坐标是否确定。
   */
  uint8_t y_defined : 1;
  /**
   * @property {bool_t} h_defined
   * @annotation ["readable"]
   * h高度是否确定。
   */
  uint8_t h_defined : 1;
} y_axis_t;

/**
 * @method y_axis_create
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
widget_t* y_axis_create(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method y_axis_cast
 * 转换为axis对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget axis对象。
 *
 * @return {widget_t*} axis对象。
 */
widget_t* y_axis_cast(widget_t* widget);

/**
 * @method y_axis_register
 * 注册y_axis控件。
 *
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t y_axis_register(void);

#define Y_AXIS(widget) ((y_axis_t*)(y_axis_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_Y_AXIS_H*/
