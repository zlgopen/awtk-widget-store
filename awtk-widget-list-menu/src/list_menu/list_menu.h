/**
 * File:   list_menu.h
 * Author: AWTK Develop Team
 * Brief:  list_menu
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-27 Li PeiHuang <lipeihuang@zlg.cn> created
 *
 */

#ifndef TK_LIST_MENU_H
#define TK_LIST_MENU_H

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/layout.h"
#include "base/enums.h"
#include "base/events.h"
#include "base/style.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "base/velocity.h"
#include "base/widget_animator.h"
#include "scroll_view/list_view.h"
#include "scroll_view/scroll_view.h"
#include "widgets/view.h"
#include "widget_animators/widget_animator_scroll.h"
#include "widget_animators/widget_animator_prop.h"

BEGIN_C_DECLS

typedef ret_t (*list_menu_fix_end_offset_t)(widget_t* widget);
typedef ret_t (*list_menu_on_scroll_t)(widget_t* widget, int32_t xoffset, int32_t yoffset);
typedef ret_t (*list_menu_on_scroll_to_t)(widget_t* widget, int32_t xoffset_end,
                                          int32_t yoffset_end, int32_t duration);

/**
 * @class list_menu_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 列表菜单控件。
 */
typedef struct _list_menu_t {
  widget_t widget;
  /**
   * @property {int32_t} item_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的宽度。
   */
  int32_t item_width;

  /**
   * @property {int32_t} item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的宽度。
   */
  int32_t item_height;

  /**
   * @property {wh_t} virtual_w
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 虚拟宽度。
   */
  wh_t virtual_w;
  /**
   * @property {wh_t} virtual_h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 虚拟高度。
   */
  wh_t virtual_h;
  /**
   * @property {int32_t} xoffset
   * @annotation ["set_prop","get_prop","readable","scriptable"]
   * x偏移量。
   */
  int32_t xoffset;
  /**
   * @property {int32_t} yoffset
   * @annotation ["set_prop","get_prop","readable","scriptable"]
   * y偏移量。
   */
  int32_t yoffset;
  /**
   * @property {bool_t} focus_prev_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 焦点向前移动按键。
   */
  uint32_t focus_prev_key;
  /**
   * @property {bool_t} focus_next_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 焦点向后移动按键。
   */
  uint32_t focus_next_key;
  /**
   * @property {bool_t} animatable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用动画。
   */
  bool_t animatable;
  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否垂直。
   */
  bool_t vertical;
  /**
   * @property {bool_t} keyable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用按键。
   */
  bool_t keyable;
  /**
   * @property {int32_t} selected_index
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 当前选中的选项。
   */
  int32_t selected_index;

  /*private*/
  point_t down;
  bool_t dragged;
  bool_t pressed;
  int32_t xoffset_end;
  int32_t yoffset_end;
  int32_t xoffset_save;
  int32_t yoffset_save;
  uint32_t key;
  uint32_t key_save;
  widget_t* view;
  widget_t* selected_child;
  bool_t is_first_key_down;
  bool_t key_down;
  bool_t pointer_move_done;
  color_t bg_color;
  color_t border_color;

  velocity_t velocity;
  widget_animator_t* wa;
  widget_animator_t* view_wa;
  list_menu_fix_end_offset_t fix_end_offset;
  widget_on_layout_children_t on_layout_children;
  widget_on_paint_children_t on_paint_children;
  widget_on_add_child_t on_add_child;
  list_menu_on_scroll_t on_scroll;
  list_menu_on_scroll_to_t on_scroll_to;

} list_menu_t;

/**
 * @method list_menu_create
 * 创建list_menu对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_menu_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_menu_cast
 * 转换为list_menu对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_menu对象。
 *
 * @return {widget_t*} list_menu对象。
 */
widget_t* list_menu_cast(widget_t* widget);

/**
 * @method list_menu_set_virtual_w
 * 设置虚拟宽度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {wh_t} w 虚拟宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_virtual_w(widget_t* widget, wh_t w);

/**
 * @method list_menu_set_virtual_h
 * 设置虚拟高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {wh_t} h 虚拟高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_virtual_h(widget_t* widget, wh_t h);

/**
 * @method list_menu_set_offset
 * 设置偏移量。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} xoffset x偏移量。
 * @param {int32_t} yoffset y偏移量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */

ret_t list_menu_set_offset(widget_t* widget, int32_t xoffset, int32_t yoffset);
/**
 * @method list_menu_scroll_to
 * 滚动到指定的偏移量。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} xoffset_end x偏移量。
 * @param {int32_t} yoffset_end y偏移量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_scroll_to(widget_t* widget, int32_t xoffset_end, int32_t yoffset_end);
/**
 * @method list_menu_set_vertical
 * 设置是否垂直。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} vertical 是否垂直。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_vertical(widget_t* widget, bool_t vertical);
/**
 * @method list_menu_set_keyable
 * 设置是否允许按键控制
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} keyable 是否允许按键控制。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_keyable(widget_t* widget, bool_t keyable);
/**
 * @method list_menu_set_animatable
 * 设置是否允许动画
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} animatable 是否允许动画。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_animatable(widget_t* widget, bool_t animatable);
/**
 * @method list_menu_set_item_width
 * 设置子项的宽度
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_width 子项宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_item_width(widget_t* widget, int32_t item_width);
/**
 * @method list_menu_set_item_height
 * 设置子项的高度
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_height 子项高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_item_height(widget_t* widget, int32_t item_height);
/**
 * @method list_menu_set_focus_prev_key
 * 设置向前/上切换按键
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} next_key 按键名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_focus_prev_key(widget_t* widget, const char* prev_key);
/**
 * @method list_menu_set_focus_next_key
 * 设置向后/下切换按键
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} next_key 按键名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_focus_next_key(widget_t* widget, const char* next_key);
/**
 * @method list_menu_set_child_selected
 * 设置对象是否选中
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} selected 是否选中
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_child_selected(widget_t* widget, bool_t selected);
/**
 * @method list_menu_set_selected_index
 * 根据索引设置选中的对象
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} index 索引
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_set_selected_index(widget_t* widget, uint32_t index);
/**
 * @method list_menu_move_selected
 * 是否选择下一个
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} next 下一个
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_move_selected(widget_t* widget, bool_t next);
/**
 * @method list_menu_selected_prev
 * 选择前一个
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_selected_prev(widget_t* widget);
/**
 * @method list_menu_selected_next
 * 选择后一个
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_selected_next(widget_t* widget);
/**
 * @method list_menu_interrupt_animation
 * 中断动画
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_menu_interrupt_animation(widget_t* widget);

#define LIST_MENU_PROP_FOCUS_PREV_KEY "focus_prev_key"
#define LIST_MENU_PROP_FOCUS_NEXT_KEY "focus_next_key"
#define LIST_MENU_PROP_KEYABLE "keyable"
#define WIDGET_TYPE_LIST_MENU "list_menu"
#define LIST_MENU(widget) ((list_menu_t*)(list_menu_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(list_menu);

END_C_DECLS

#endif /*TK_LIST_MENU_H*/
