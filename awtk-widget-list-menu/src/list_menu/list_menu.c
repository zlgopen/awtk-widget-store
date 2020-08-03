/**
 * File:   scroll_view.c
 * Author: AWTK Develop Team
 * Brief:  scroll_view
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

#include "list_menu.h"
#include "menu_item.h"

/*状态刷新 */
ret_t list_menu_invalidate(widget_t* widget, rect_t* r) {
  list_menu_t* list_menu = LIST_MENU(widget);
  rect_t r_self = rect_init(0, 0, widget->w, widget->h);

  r->x += widget->x;
  r->y += widget->y;
  r->x -= list_menu->xoffset;
  r->y -= list_menu->yoffset;

  *r = rect_intersect(r, &r_self);

  if (r->w <= 0 || r->h <= 0) {
    return RET_OK;
  }

  if (widget->parent) {
    widget_invalidate(widget->parent, r);
  }

  return RET_OK;
}

ret_t list_menu_invalidate_self(widget_t* widget) {
  rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);

  widget->dirty = FALSE;
  return widget_invalidate(widget->parent, &r);
}

static ret_t list_menu_set_child_selected_only(widget_t* widget, bool_t selected) {
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL, RET_BAD_PARAMS);

  if (selected) {
    widget_set_state(widget, WIDGET_STATE_SELECTED);
    menu_item->selected = TRUE;
  } else {
    widget_set_state(widget, WIDGET_STATE_NORMAL);
    menu_item->selected = FALSE;
  }

  return RET_OK;
}

ret_t list_menu_set_child_selected(widget_t* widget, bool_t selected) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  int32_t index = list_menu->selected_index;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(widget_get_type(iter), WIDGET_TYPE_MENU_ITEM)) {
    if (index == i) {
      menu_item_set_selected_only(iter, selected);
    } else {
      menu_item_set_selected_only(iter, !selected);
    }
  }

  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

/*view动画结束 */
static ret_t list_menu_on_view_scroll_done(void* ctx, event_t* e) {
  (void)e;
  list_menu_t* list_menu = LIST_MENU(ctx);
  list_menu->view_wa = NULL;

  list_menu->xoffset_save = list_menu->xoffset;
  list_menu->yoffset_save = list_menu->yoffset;

  list_menu_set_child_selected(WIDGET(list_menu), TRUE);
  widget_set_visible_only(list_menu->view, FALSE);
  widget_set_focused(list_menu->selected_child, TRUE);
  list_menu_invalidate_self(WIDGET(list_menu));

  return RET_REMOVE;
}

/*list_menu动画结束 */
static ret_t list_menu_on_scroll_done(void* ctx, event_t* e) {
  (void)e;
  widget_t* widget = WIDGET(ctx);
  list_menu_t* list_menu = LIST_MENU(ctx);
  return_value_if_fail(widget != NULL && list_menu != NULL, RET_BAD_PARAMS);
  list_menu->wa = NULL;
  list_menu->pointer_move_done = TRUE;

  list_menu_invalidate_self(WIDGET(list_menu));

  return RET_REMOVE;
}

/*view控件动画 */
static ret_t list_menu_view_scroll_to(widget_t* widget, int32_t offset_end) {
  int32_t from = 0;
  int32_t start = 0;
  int32_t end = 0;
  char* prop_name = NULL;
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* view = list_menu->view;
  return_value_if_fail(list_menu != NULL && view != NULL, RET_FAIL);

  if (list_menu->vertical) {
    start = widget->y;
    end = widget->y + widget->h - list_menu->item_height;
    from = view->y;
    prop_name = WIDGET_PROP_Y;
  } else {
    start = widget->x;
    end = widget->x + widget->w - list_menu->item_width;
    from = view->x;
    prop_name = WIDGET_PROP_X;
  }

  if (from < start) {
    from = start;
  } else if (from > end) {
    from = end;
  }

  list_menu->view_wa =
      widget_animator_prop_create(view, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT, prop_name);
  return_value_if_fail(list_menu->view_wa != NULL, RET_OOM);

  widget_animator_prop_set_params(list_menu->view_wa, from, offset_end);
  widget_animator_on(list_menu->view_wa, EVT_ANIM_END, list_menu_on_view_scroll_done, list_menu);
  widget_animator_start(list_menu->view_wa);

  return RET_OK;
}

static ret_t list_menu_fix_end_offset_default(widget_t* widget) {
  int32_t xoffset_end = 0;
  int32_t yoffset_end = 0;
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(widget != NULL && list_menu != NULL, RET_BAD_PARAMS);

  xoffset_end = list_menu->xoffset_end;
  yoffset_end = list_menu->yoffset_end;

  xoffset_end = tk_max(xoffset_end, 0);
  yoffset_end = tk_max(yoffset_end, 0);
  xoffset_end = tk_min(xoffset_end, (list_menu->virtual_w - widget->w));
  yoffset_end = tk_min(yoffset_end, (list_menu->virtual_h - widget->h));

  if (list_menu->virtual_w <= widget->w) {
    xoffset_end = 0;
  }

  if (list_menu->virtual_h <= widget->h) {
    yoffset_end = 0;
  }

  list_menu->xoffset_end = xoffset_end;
  list_menu->yoffset_end = yoffset_end;

  return RET_OK;
}

static ret_t list_menu_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t r_save;
  rect_t r = rect_init(c->ox, c->oy, widget->w, widget->h);

  list_menu_t* list_menu = LIST_MENU(widget);
  int32_t xoffset = -list_menu->xoffset;
  int32_t yoffset = -list_menu->yoffset;

  canvas_translate(c, xoffset, yoffset);
  canvas_get_clip_rect(c, &r_save);

  r = rect_intersect(&r, &r_save);
  canvas_set_clip_rect(c, &r);

  widget_on_paint_children_default(widget, c);

  canvas_set_clip_rect(c, &r_save);
  canvas_untranslate(c, xoffset, yoffset);

  return RET_OK;
}

static ret_t list_menu_update_virtual_size(widget_t* widget) {
  int32_t virtual_w = 0;
  int32_t virtual_h = 0;
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL && widget != NULL, RET_BAD_PARAMS);

  virtual_w = tk_max(list_menu->virtual_w, widget->w);
  virtual_h = tk_max(list_menu->virtual_h, widget->h);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  int32_t r = iter->x + iter->w;
  int32_t b = iter->y + iter->h;
  if (r > virtual_w) {
    virtual_w = r;
  }
  if (b > virtual_h) {
    virtual_h = b;
  }
  WIDGET_FOR_EACH_CHILD_END()

  list_menu->virtual_w = virtual_w;
  list_menu->virtual_h = virtual_h;

  return RET_OK;
}

/*垂直布局子控件 */
static ret_t layout_children_v(widget_t* widget) {
  int32_t item_height = 0;
  int32_t virtual_h = 0;
  widget_t* iter = NULL;
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  list_menu_set_offset(widget, 0, 0);
  item_height = list_menu->item_height;
  virtual_h = widget->h;

  if (widget->children != NULL) {
    int32_t i = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = widget->w;
    int32_t h = item_height;
    int32_t n = widget->children->size;
    widget_t** children = (widget_t**)(widget->children->elms);

    for (i = 0; i < n; i++) {
      iter = children[i];

      widget_move_resize(iter, x, y, w, h);
      widget_layout(iter);

      y = iter->y + iter->h;
    }

    if (y > virtual_h) {
      virtual_h = y;
    }

    list_menu_set_selected_index(widget, 0);
    list_menu_set_child_selected(widget, TRUE);
  }

  list_menu_set_virtual_h(widget, virtual_h);

  return RET_OK;
}

/*水平布局子控件 */
static ret_t layout_children_h(widget_t* widget) {
  int32_t item_width = 0;
  int32_t virtual_w = 0;
  widget_t* iter = NULL;
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  list_menu_set_offset(widget, 0, 0);
  item_width = list_menu->item_width;
  virtual_w = widget->w;

  if (widget->children != NULL) {
    int32_t i = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = item_width;
    int32_t h = widget->h;
    int32_t n = widget->children->size;
    widget_t** children = (widget_t**)(widget->children->elms);

    for (i = 0; i < n; i++) {
      iter = children[i];
      widget_move_resize(iter, x, y, w, h);
      widget_layout(iter);

      x = iter->x + iter->w;
    }

    if (x > virtual_w) {
      virtual_w = x;
    }
    list_menu_set_selected_index(widget, 0);
    list_menu_set_child_selected(widget, TRUE);
  }

  list_menu_set_virtual_w(widget, virtual_w);

  return RET_OK;
}

static ret_t list_menu_on_layout_children(widget_t* widget) {
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  if (list_menu->vertical) {
    if (list_menu->item_height <= 0 || list_menu->item_height >= win->h) {
      log_debug("item_height bad params!!!\n");
      return RET_BAD_PARAMS;
    }
    layout_children_v(widget);
  } else {
    if (list_menu->item_width <= 0 || list_menu->item_width >= win->w) {
      log_debug("item_width bad params!!!\n");
      return RET_BAD_PARAMS;
    }
    layout_children_h(widget);
  }

  return list_menu_update_virtual_size(widget);
}

static ret_t list_menu_notify_scrolled(list_menu_t* list_menu) {
  widget_t* widget = WIDGET(list_menu);

  if (list_menu->on_scroll) {
    list_menu->on_scroll(widget, list_menu->xoffset, list_menu->yoffset);
  }

  return RET_OK;
}

static ret_t list_menu_get_prop(widget_t* widget, const char* name, value_t* v) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W) || tk_str_eq(name, WIDGET_PROP_LAYOUT_W)) {
    value_set_int(v, tk_max(widget->w, list_menu->virtual_w));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H) || tk_str_eq(name, WIDGET_PROP_LAYOUT_H)) {
    value_set_int(v, tk_max(widget->h, list_menu->virtual_h));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, list_menu->xoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, list_menu->yoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATABLE)) {
    value_set_bool(v, list_menu->animatable);
    return RET_OK;
  } else if (tk_str_eq(name, LIST_MENU_PROP_KEYABLE)) {
    value_set_bool(v, list_menu->keyable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, list_menu->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_WIDTH)) {
    value_set_int(v, list_menu->item_width);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    value_set_int(v, list_menu->item_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    value_set_int(v, list_menu->selected_index);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_menu_set_prop(widget_t* widget, const char* name, const value_t* v) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W)) {
    list_menu->virtual_w = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H)) {
    list_menu->virtual_h = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATABLE)) {
    list_menu->animatable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, LIST_MENU_PROP_KEYABLE)) {
    list_menu->keyable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    list_menu->vertical = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    list_menu->xoffset = value_int(v);
    list_menu_notify_scrolled(list_menu);
    list_menu_invalidate_self(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    list_menu->yoffset = value_int(v);
    list_menu_notify_scrolled(list_menu);
    list_menu_invalidate_self(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_WIDTH)) {
    list_menu->item_width = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    list_menu->item_height = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, LIST_MENU_PROP_FOCUS_PREV_KEY)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = keys_type_find(value_str(v));
      if (kv != NULL) {
        list_menu->focus_prev_key = kv->value;
      } else {
        log_debug("invalid key: %s\n", value_str(v));
      }
    } else {
      list_menu->focus_prev_key = value_int(v);
    }
    return RET_OK;
  } else if (tk_str_eq(name, LIST_MENU_PROP_FOCUS_NEXT_KEY)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = keys_type_find(value_str(v));
      if (kv != NULL) {
        list_menu->focus_next_key = kv->value;
      } else {
        log_debug("invalid key: %s\n", value_str(v));
      }
    } else {
      list_menu->focus_next_key = value_int(v);
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    list_menu_set_selected_index(widget, value_int(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_menu_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

ret_t list_menu_interrupt_animation(widget_t* widget) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  if (list_menu->view_wa != NULL || list_menu->wa != NULL) {
    widget_animator_destroy(list_menu->view_wa);
    widget_animator_destroy(list_menu->wa);
    list_menu->view_wa = NULL;
    list_menu->wa = NULL;

    list_menu_set_child_selected(widget, TRUE);
    widget_set_visible_only(list_menu->view, FALSE);
    list_menu->pointer_move_done = TRUE;
  }

  return RET_OK;
}

/*开启动画 */
static ret_t list_menu_start_animation(widget_t* widget, point_t* point, bool_t next) {
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* child = list_menu->selected_child;
  return_value_if_fail(list_menu != NULL && list_menu->selected_child != NULL, RET_BAD_PARAMS);

  int32_t start = 0;
  int32_t end = 0;
  int32_t x_to = 0;
  int32_t y_to = 0;

  if (list_menu->vertical) {
    start = widget->y;
    end = widget->y + widget->h - list_menu->item_height;
    y_to = point->y;

    y_to = tk_max(y_to, start);
    y_to = tk_min(y_to, end);

    if (child->y < list_menu->yoffset) {
      list_menu->yoffset_end = child->y;
      list_menu_scroll_to(widget, 0, list_menu->yoffset_end);
    } else if (child->y > widget->h + list_menu->yoffset - list_menu->item_height) {
      list_menu->yoffset_end = child->y - widget->h + list_menu->item_height;
      list_menu_scroll_to(widget, 0, list_menu->yoffset_end);
    } else {
      if (list_menu->wa != NULL) {
        int32_t offset = 0;
        offset = abs(list_menu->yoffset - list_menu->yoffset_end);
        if (!next) {
          y_to = y_to - offset;
        } else if (next) {
          y_to = y_to + offset;
        }
      }
    }
    list_menu_view_scroll_to(widget, y_to);
  } else {
    start = widget->x;
    end = widget->x + widget->w - list_menu->item_width;
    x_to = point->x;

    x_to = tk_max(x_to, start);
    x_to = tk_min(x_to, end);

    if (child->x < list_menu->xoffset) {
      list_menu->xoffset_end = child->x;
      list_menu_scroll_to(widget, list_menu->xoffset_end, 0);
    } else if (child->x > widget->w + list_menu->xoffset - list_menu->item_width) {
      list_menu->xoffset_end = child->x - widget->w + list_menu->item_width;
      list_menu_scroll_to(widget, list_menu->xoffset_end, 0);
    } else {
      if (list_menu->wa != NULL) {
        int32_t offset = 0;
        offset = abs(list_menu->xoffset - list_menu->xoffset_end);
        if (!next) {
          x_to = x_to - offset;
        } else if (next) {
          x_to = x_to + offset;
        }
      }
    }
    list_menu_view_scroll_to(widget, x_to);
  }
  return RET_OK;
}

/*移动并调整view的位置和大小*/
static ret_t list_menu_view_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h) {
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* view = list_menu->view;

  if (list_menu->vertical) {
    if (y < widget->y) {
      y = widget->y;
    } else if (y > widget->h + widget->y) {
      y = widget->h + widget->y - list_menu->item_height;
    }
  } else {
    if (x < widget->x) {
      x = widget->x;
    } else if (x > widget->w + widget->x - list_menu->item_width) {
      x = widget->w + widget->x - list_menu->item_width;
    }
  }

  widget_move_resize(view, x, y, w, h);
  return RET_OK;
}

ret_t list_menu_move_selected(widget_t* widget, bool_t next) {
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* win = widget_get_window(widget);
  widget_t* child = NULL;
  color_t deval = color_init(0, 0, 0, 0);

  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  int32_t children_size = widget->children->size;
  int32_t index = list_menu->selected_index;
  list_menu->selected_child = NULL;

  if (list_menu->animatable) {
    point_t old_focus_point = {0, 0};
    point_t new_focus_point = {0, 0};

    if (list_menu->wa == NULL) {
      /*判断子项是否为最头、最尾的元素，是的话将焦点聚焦到该控件中。*/
      if ((!next && index == 0) || (next && index == (children_size - 1))) {
        WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
        if (index == i) {
          list_menu->selected_child = children[i];
        }
        WIDGET_FOR_EACH_CHILD_END()

        if (list_menu->selected_child != NULL) {
          widget_set_focused(list_menu->selected_child, TRUE);
        }
        list_menu_invalidate_self(widget);
        return RET_OK;
      }
    }

    if (list_menu->view_wa != NULL) {
      widget_animator_destroy(list_menu->view_wa);
      list_menu->view_wa = NULL;
    }

    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    if (i == list_menu->selected_index) {
      child = iter;
      index = i;
      widget_to_global(child, &old_focus_point);
      old_focus_point.x -= list_menu->xoffset;
      old_focus_point.y -= list_menu->yoffset;

      if (list_menu->is_first_key_down) {
        list_menu->bg_color = style_get_color(child->astyle, STYLE_ID_BG_COLOR, deval);
        list_menu->border_color = style_get_color(child->astyle, STYLE_ID_BORDER_COLOR, deval);
        list_menu->is_first_key_down = FALSE;
      }

      /*view控件不可见时，重定义view的大小，设置背景色和边框色 */
      if (list_menu->view->visible == FALSE) {
        xy_t x = old_focus_point.x;
        xy_t y = old_focus_point.y;
        wh_t w = child->w;
        wh_t h = child->h;

        list_menu_view_move_resize(widget, x, y, w, h);
        widget_set_style_color(list_menu->view, "normal:bg_color", list_menu->bg_color.color);
        widget_set_style_color(list_menu->view, "normal:border_color",
                               list_menu->border_color.color);
        widget_set_visible_only(list_menu->view, TRUE);
      }

      list_menu_set_child_selected_only(iter, FALSE);
      if (!next) {
        if (index > 0) {
          index--;
        }
      } else if (next) {
        if (index < (children_size - 1)) {
          index++;
        }
      }
      list_menu->selected_child = children[index];
      list_menu->selected_index = index;

      widget_to_global(list_menu->selected_child, &new_focus_point);
      new_focus_point.x -= list_menu->xoffset;
      new_focus_point.y -= list_menu->yoffset;
      break;
    }
    WIDGET_FOR_EACH_CHILD_END()

    list_menu_start_animation(widget, &new_focus_point, next);
  } else {
    int32_t index = list_menu->selected_index;

    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    if (i == list_menu->selected_index) {
      if (!next) {
        if (index > 0) {
          index--;
        }
      } else if (next) {
        if (index < (children_size - 1)) {
          index++;
        }
      }
      list_menu->selected_child = children[index];
      list_menu->selected_index = index;
      list_menu_set_child_selected(widget, TRUE);
      widget_set_focused(list_menu->selected_child, TRUE);
      break;
    }
    WIDGET_FOR_EACH_CHILD_END()
  }
  return RET_OK;
}

ret_t list_menu_selected_prev(widget_t* widget) {
  return list_menu_move_selected(widget, FALSE);
}

ret_t list_menu_selected_next(widget_t* widget) {
  return list_menu_move_selected(widget, TRUE);
}

/*按钮按下*/
static ret_t list_menu_on_key_down(widget_t* widget) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  if (list_menu->focus_prev_key == list_menu->key) {
    list_menu_selected_prev(widget);
  } else if (list_menu->focus_next_key == list_menu->key) {
    list_menu_selected_next(widget);
  }

  return RET_OK;
}

/*指针按下 */
static ret_t list_menu_on_pointer_down(list_menu_t* list_menu, pointer_event_t* e) {
  velocity_t* v = &(list_menu->velocity);

  velocity_reset(v);
  list_menu->down.x = e->x;
  list_menu->down.y = e->y;
  list_menu->xoffset_save = list_menu->xoffset;
  list_menu->yoffset_save = list_menu->yoffset;
  velocity_update(v, e->e.time, e->x, e->y);

  list_menu_interrupt_animation(WIDGET(list_menu));

  return RET_OK;
}

static ret_t list_menu_on_pointer_down_abort(list_menu_t* list_menu, pointer_event_t* e) {
  widget_t* widget = WIDGET(list_menu);
  (void)e;

  if (list_menu->fix_end_offset) {
    list_menu->fix_end_offset(widget);
  }
  list_menu_scroll_to(widget, list_menu->xoffset_end, list_menu->yoffset_end);

  return RET_OK;
}

/*指针抬起 */
#define SPEED_SCALE 2
#define MIN_DELTA 10
static ret_t list_menu_on_pointer_up(list_menu_t* list_menu, pointer_event_t* e) {
  widget_t* widget = WIDGET(list_menu);
  velocity_t* v = &(list_menu->velocity);
  int32_t move_dx = e->x - list_menu->down.x;
  int32_t move_dy = e->y - list_menu->down.y;

  velocity_update(v, e->e.time, e->x, e->y);
  int yv = v->yv;
  int xv = v->xv;

  if (list_menu->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)list_menu->wa;
    int32_t dx = wa->x_to - list_menu->xoffset;
    int32_t dy = wa->y_to - list_menu->yoffset;
    xv -= dx;
    yv -= dy;
    log_debug("dx=%d xv=%d dy=%d yv=%d\n", dx, xv, dy, yv);
  }

  if (list_menu->vertical) {
    if (tk_abs(move_dy) > MIN_DELTA) {
      list_menu->yoffset_end = list_menu->yoffset - yv * SPEED_SCALE;
    } else {
      list_menu->yoffset_end = list_menu->yoffset - yv / SPEED_SCALE;
    }
  } else {
    if (tk_abs(move_dx) > MIN_DELTA) {
      list_menu->xoffset_end = list_menu->xoffset - xv * SPEED_SCALE;
    } else {
      list_menu->xoffset_end = list_menu->xoffset - xv / SPEED_SCALE;
    }
  }

  if (list_menu->fix_end_offset) {
    list_menu->fix_end_offset(widget);
  }

  list_menu->pointer_move_done = FALSE;
  list_menu_scroll_to(widget, list_menu->xoffset_end, list_menu->yoffset_end);

  return RET_OK;
}

/*指针移动 */
static ret_t list_menu_on_pointer_move(list_menu_t* list_menu, pointer_event_t* e) {
  velocity_t* v = &(list_menu->velocity);
  int32_t dx = e->x - list_menu->down.x;
  int32_t dy = e->y - list_menu->down.y;
  velocity_update(v, e->e.time, e->x, e->y);

  if (list_menu->wa == NULL) {
    if (list_menu->vertical && dy) {
      list_menu->yoffset = list_menu->yoffset_save - dy;
    } else if (!list_menu->vertical && dx) {
      list_menu->xoffset = list_menu->xoffset_save - dx;
    }

    list_menu_notify_scrolled(list_menu);
  }

  return RET_OK;
}

/*事件处理 */
static ret_t list_menu_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  list_menu_t* list_menu = LIST_MENU(widget);
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      widget_grab(widget->parent, widget);

      if (list_menu->focus_prev_key == evt->key || list_menu->focus_next_key == evt->key) {
        if (list_menu->keyable && list_menu->pointer_move_done && !list_menu->pressed) {
          list_menu->key = evt->key;
          list_menu_on_key_down(widget);
          list_menu->key_save = list_menu->key;
        }
      }

      list_menu_invalidate_self(widget);
      break;
    }
    case EVT_KEY_UP: {
      key_event_t evt = *(key_event_t*)e;

      if (evt.key == TK_KEY_RETURN) {
        evt.e = event_init(EVT_CLICK, widget);
        list_menu_interrupt_animation(widget);
        widget_dispatch(list_menu->selected_child, (event_t*)&evt);
        widget_set_focused(list_menu->selected_child, TRUE);
      }

      widget_ungrab(widget->parent, widget);
      list_menu_invalidate_self(widget);
      break;
    }
    case EVT_POINTER_DOWN: {
      list_menu->pressed = TRUE;
      widget_grab(widget->parent, widget);
      list_menu->dragged = FALSE;

      list_menu_on_pointer_down(list_menu, (pointer_event_t*)e);

      widget_set_visible_only(list_menu->view, FALSE);
      widget_update_style(list_menu->view);

      list_menu_invalidate_self(widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      list_menu_on_pointer_down_abort(list_menu, (pointer_event_t*)e);
      list_menu->pressed = FALSE;
      list_menu->dragged = FALSE;
      widget_ungrab(widget->parent, widget);
      list_menu_invalidate_self(widget);
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      int32_t dx = evt->x - list_menu->down.x;
      int32_t dy = evt->y - list_menu->down.y;
      menu_item_t* menu_item = NULL;
      if (list_menu->dragged && (dx || dy)) {
        list_menu_on_pointer_up(list_menu, (pointer_event_t*)e);
      } else {
        if (list_menu->vertical) {
          if (list_menu->yoffset < 0) {
            list_menu->yoffset = 0;
          } else if (list_menu->yoffset > list_menu->virtual_h - widget->h) {
            list_menu->yoffset = list_menu->virtual_h - widget->h;
          }
        } else {
          if (list_menu->xoffset < 0) {
            list_menu->xoffset = 0;
          } else if (list_menu->xoffset > list_menu->virtual_w - widget->w) {
            list_menu->xoffset = list_menu->virtual_w - widget->w;
          }
        }
      }

      WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
      if (tk_str_eq(widget_get_type(iter), WIDGET_TYPE_MENU_ITEM)) {
        menu_item = MENU_ITEM(iter);
        if (menu_item->selected) {
          list_menu->selected_child = iter;
          list_menu->selected_index = i;
        }
      }
      WIDGET_FOR_EACH_CHILD_END()

      list_menu->dragged = FALSE;
      list_menu->pressed = FALSE;
      widget_ungrab(widget->parent, widget);
      widget_set_focused(widget, TRUE);

      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (!evt->pressed || !list_menu->pressed) {
        list_menu_invalidate_self(widget);
        break;
      }

      if (list_menu->dragged) {
        list_menu_on_pointer_move(list_menu, evt);
      } else {
        int32_t delta = 0;
        int32_t xdelta = evt->x - list_menu->down.x;
        int32_t ydelta = evt->y - list_menu->down.y;
        delta = tk_abs(xdelta) > tk_abs(ydelta) ? xdelta : ydelta;

        if (tk_abs(delta) > TK_DRAG_THRESHOLD) {
          pointer_event_t abort = *evt;
          abort.e.type = EVT_POINTER_DOWN_ABORT;
          widget_dispatch_event_to_target_recursive(widget, (event_t*)(&abort));
          list_menu->dragged = TRUE;
        }
      }

      list_menu_invalidate_self(widget);
      ret = list_menu->dragged ? RET_STOP : RET_OK;
      break;
    }
    case EVT_POINTER_LEAVE:
      list_menu_invalidate_self(widget);
      break;
    case EVT_POINTER_ENTER:
      list_menu_invalidate_self(widget);
      break;

    default:
      break;
  }

  return ret;
}

static ret_t list_menu_on_add_child(widget_t* widget, widget_t* child) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);
  if (list_menu->on_add_child) {
    return list_menu->on_add_child(widget, child);
  } else {
    return RET_CONTINUE;
  }
}

static const char* s_list_menu_clone_properties[] = {
    WIDGET_PROP_VIRTUAL_W,   WIDGET_PROP_VIRTUAL_H,         WIDGET_PROP_VERTICAL,
    WIDGET_PROP_XOFFSET,     WIDGET_PROP_YOFFSET,           WIDGET_PROP_ITEM_WIDTH,
    WIDGET_PROP_ITEM_HEIGHT, LIST_MENU_PROP_FOCUS_PREV_KEY, LIST_MENU_PROP_FOCUS_NEXT_KEY,
    WIDGET_PROP_ANIMATABLE,  LIST_MENU_PROP_KEYABLE,        NULL};

TK_DECL_VTABLE(list_menu) = {.size = sizeof(list_menu_t),
                             .type = WIDGET_TYPE_LIST_MENU,
                             .scrollable = TRUE,
                             .clone_properties = s_list_menu_clone_properties,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = list_menu_create,
                             .on_event = list_menu_on_event,
                             .invalidate = list_menu_invalidate,
                             .on_paint_self = list_menu_on_paint_self,
                             .on_layout_children = list_menu_on_layout_children,
                             .on_paint_children = list_menu_on_paint_children,
                             .on_add_child = list_menu_on_add_child,
                             .get_prop = list_menu_get_prop,
                             .set_prop = list_menu_set_prop};

widget_t* list_menu_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* view = view_create(parent, 0, 0, 0, 0);
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(list_menu), x, y, w, h);
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, NULL);

  list_menu->fix_end_offset = list_menu_fix_end_offset_default;
  list_menu->view = view;
  widget_set_visible_only(list_menu->view, FALSE);
  list_menu->pressed = FALSE;
  list_menu->pointer_move_done = TRUE;
  list_menu->is_first_key_down = TRUE;
  list_menu->keyable = TRUE;
  list_menu->selected_index = 0;
  list_menu->selected_child = NULL;

  return widget;
}

ret_t list_menu_set_selected_index(widget_t* widget, uint32_t index) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);
  widget_t** children = (widget_t**)(widget->children->elms);

  list_menu->selected_index = index;
  list_menu->selected_child = children[index];
  return RET_OK;
}

ret_t list_menu_scroll_to(widget_t* widget, int32_t xoffset_end, int32_t yoffset_end) {
  int32_t xoffset = 0;
  int32_t yoffset = 0;
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);

  xoffset = list_menu->xoffset;
  yoffset = list_menu->yoffset;

  if (list_menu->on_scroll_to) {
    list_menu->on_scroll_to(widget, xoffset_end, yoffset_end, TK_ANIMATING_TIME);
  }

  if ((!list_menu->vertical && xoffset == xoffset_end) ||
      (list_menu->vertical && yoffset == yoffset_end)) {
    list_menu->pointer_move_done = TRUE;

    return RET_OK;
  }

  if (list_menu->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)list_menu->wa;

    if (list_menu->vertical) {
      bool_t changed = wa->y_to != yoffset_end;
      bool_t in_range = wa->y_to >= 0 && wa->y_to <= (list_menu->virtual_h - widget->h);
      if (changed && in_range) {
        wa->y_to = yoffset_end;
        wa->y_from = list_menu->yoffset;
      } else {
        if (!list_menu->pressed) {
          list_menu->pointer_move_done = TRUE;
        }
        return RET_OK;
      }
    } else {
      bool_t changed = wa->x_to != xoffset_end;
      bool_t in_range = wa->x_to >= 0 && wa->x_to <= (list_menu->virtual_w - widget->w);
      if (changed && in_range) {
        wa->x_to = xoffset_end;
        wa->x_from = list_menu->xoffset;
      } else {
        if (!list_menu->pressed) {
          list_menu->pointer_move_done = TRUE;
        }
        return RET_OK;
      }
    }

    wa->base.now = 0;
    wa->base.start_time = 0;
  } else {
    list_menu->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
    return_value_if_fail(list_menu->wa != NULL, RET_OOM);

    widget_animator_scroll_set_params(list_menu->wa, xoffset, yoffset, xoffset_end, yoffset_end);
    widget_animator_on(list_menu->wa, EVT_ANIM_END, list_menu_on_scroll_done, list_menu);
    widget_animator_start(list_menu->wa);
  }

  return RET_OK;
}

ret_t list_menu_set_virtual_w(widget_t* widget, wh_t w) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  list_menu->virtual_w = w;

  return RET_OK;
}

ret_t list_menu_set_virtual_h(widget_t* widget, wh_t h) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_BAD_PARAMS);

  list_menu->virtual_h = h;

  return RET_OK;
}

ret_t list_menu_set_offset(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);

  list_menu->xoffset = xoffset;
  list_menu->yoffset = yoffset;

  list_menu_invalidate_self(widget);

  return RET_OK;
}

ret_t list_menu_set_animatable(widget_t* widget, bool_t animatable) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);

  list_menu->animatable = animatable;

  return RET_OK;
}

ret_t list_menu_set_keyable(widget_t* widget, bool_t keyable) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);

  list_menu->keyable = keyable;

  return RET_OK;
}

ret_t list_menu_set_vertical(widget_t* widget, bool_t vertical) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(list_menu != NULL, RET_FAIL);

  list_menu->vertical = vertical;

  return RET_OK;
}

ret_t list_menu_set_item_width(widget_t* widget, int32_t item_width) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  list_menu->item_width = item_width;

  return RET_OK;
}

ret_t list_menu_set_item_height(widget_t* widget, int32_t item_height) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  list_menu->item_height = item_height;

  return RET_OK;
}

ret_t list_menu_set_focus_prev_key(widget_t* widget, const char* prev_key) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  const key_type_value_t* kv = keys_type_find(prev_key);
  if (kv != NULL) {
    list_menu->focus_prev_key = kv->value;
  } else {
    log_debug("invalid key: %s\n", prev_key);
  }

  return RET_OK;
}

ret_t list_menu_set_focus_next_key(widget_t* widget, const char* next_key) {
  list_menu_t* list_menu = LIST_MENU(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  const key_type_value_t* kv = keys_type_find(next_key);
  if (kv != NULL) {
    list_menu->focus_next_key = kv->value;
  } else {
    log_debug("invalid key: %s\n", next_key);
  }

  return RET_OK;
}

widget_t* list_menu_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, list_menu), NULL);

  return widget;
}
