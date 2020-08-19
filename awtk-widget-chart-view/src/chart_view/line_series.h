/**
 * File:   line series.h
 * Author: AWTK Develop Team
 * Brief:  line series
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

#ifndef TK_LINE_SERIES_H
#define TK_LINE_SERIES_H

#include "series.h"
#include "../base/fifo.h"

BEGIN_C_DECLS

/**
 * @class line_series_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 线形序列。
 * 
 * 在xml中使用"line\_series"标签创建扩展按钮控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <line_series x="c" y="50" w="200" h="100" />
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <!-- style -->
 * <line_series>
 *  <style name="default">
 *      <normal line_border_color="#69cf5c" line_border_width="1"   area_color="#69CF5C66"  smooth_area_color="#69CF5C66" symbol_border_color="#FFC393" symbol_color="#69CF5C"/>
 *  </style>
 * </line_series>
 * <line_series_colorful>
 *  <style name="default">
 *      <normal line_border_color="#69cf5c" line_border_width="1"   area_color="#69cf5c66"  smooth_area_color="#69CF5C66" symbol_border_color="#FFC393" symbol_color="#69CF5C"/>
 *  </style>
 * </line_series_colorful>
 * ```
 */
typedef struct _line_series_t {
  series_t base;
  /**
   * @property {char*} series_axis
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标示序列位置的轴。
   */
  char* series_axis;
  /**
   * @property {char*} value_axis
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标示序列值的轴。
   */
  char* value_axis;
  /**
   * 线参数。
   */
  series_line_params_t line;
  /**
   * 面参数。
   */
  series_line_area_params_t area;
  /**
   * 点参数。
   */
  series_symbol_params_t symbol;
} line_series_t;

/**
 * @method line_series_create
 * 创建line_series对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* line_series_create(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method line_series_colorful_create
 * 创建彩色line_series对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* line_series_colorful_create(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method line_series_cast
 * 转换为line_series对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget line_series对象。
 *
 * @return {widget_t*} line_series对象。
 */
widget_t* line_series_cast(widget_t* series);

/**
 * @method line_series_register
 * 注册line_series控件。
 *
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_series_register(void);

/**
 * @method line_series_colorful_register
 * 注册line_series_colorful控件。
 *
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_series_colorful_register(void);

#define LINE_SERIES(widget) ((line_series_t*)(line_series_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_LINE_SERIES_H*/
