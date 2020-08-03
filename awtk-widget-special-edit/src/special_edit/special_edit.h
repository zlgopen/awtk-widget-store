/**
 * File:   special_edit.h
 * Author: AWTK Develop Team
 * Brief:  特殊编辑框控件，可通过快捷键F7或F8，增加或者减小数值
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
 * 2020-06-05 AWTK Develop Team created
 *
 */

#ifndef TK_SPECIAL_EDIT_H
#define TK_SPECIAL_EDIT_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class special_edit_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * special_edit编辑框控件，对于一些没有触摸屏的设备，可通过设置键盘上的快捷键如：F7或F8，增加或者减小数值
 */
typedef struct _special_edit_t {
  widget_t widget;

  /**
   * @property {double} min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小值或最小长度。
   */
  double min;

  /**
   * @property {double} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值或最大长度。
   */
  double max;

  /**
   * @property {char*} inc_fun_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置递增快捷键，如：F1， F2。
   */
  char* inc_fun_key;

  /**
   * @property {char*} dec_fun_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置递减快捷键，如：F1， F2。
   */
  char* dec_fun_key;

  /*private*/
  uint32_t cursor;
  uint32_t float_dot;
  bool_t insert_model;
  bool_t caret_visible;
  canvas_t* c;  //用来计算字符长度
  uint32_t timer_id;

} special_edit_t;

/**
 * @method special_edit_create
 * @annotation ["constructor", "scriptable"]
 * 创建special_edit对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} special_edit对象。
 */
widget_t* special_edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method special_edit_cast
 * 转换为special_edit对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget special_edit对象。
 *
 * @return {widget_t*} special_edit对象。
 */
widget_t* special_edit_cast(widget_t* widget);

/**
 * @method special_edit_set_int_limit
 * 设置为整数输入及取值范围。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} min 最小值。
 * @param {int32_t} max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t special_edit_set_int_limit(widget_t* widget, int32_t min, int32_t max);

/**
 * @method special_edit_set_limit
 * 设置为浮点数输入及取值范围。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {double} min 最小值。
 * @param {double} max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t special_edit_set_limit(widget_t* widget, double min, double max);

/**
 * @method special_edit_set_float_dot
 * 设置小数点的位数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} float_dot 小数点的位数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t special_edit_set_float_dot(widget_t* widget, uint32_t float_dot);

/**
 * @method special_edit_set_inc_fun_key
 * 设置递增快捷键。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} inc_key 递增快捷键，如：F1, F2。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t special_edit_set_inc_fun_key(widget_t* widget, const char* inc_key);

/**
 * @method special_edit_set_dec_fun_key
 * 设置递增快捷键。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} dec_key 递增快捷键，如：F1, F2。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t special_edit_set_dec_fun_key(widget_t* widget, const char* dec_key);

#define SPECIAL_EDIT_PROP_VALUE "value"

#define WIDGET_TYPE_SPECIAL_EDIT "special_edit"

#define SPECIAL_EDIT(widget) ((special_edit_t*)(special_edit_cast(WIDGET(widget))))

#define FLOAT_DOT "float_dot"

#define WIDGET_PROP_INC_FUN_KEY "inc_fun_key"

#define WIDGET_PROP_DEC_FUN_KEY "dec_fun_key"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(special_edit);

END_C_DECLS

#endif /*TK_SPECIAL_EDIT_H*/
