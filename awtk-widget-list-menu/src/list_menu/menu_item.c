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

#include "tkc/mem.h"
#include "base/timer.h"
#include "menu_item.h"
#include "list_menu.h"

static ret_t menu_item_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t menu_item_pointer_up_cleanup(widget_t* widget) {
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL && widget != NULL, RET_BAD_PARAMS);

  menu_item->pressed = FALSE;
  widget_ungrab(widget->parent, widget);

  return RET_OK;
}

static ret_t menu_item_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      menu_item->down.x = evt->x;
      menu_item->down.y = evt->y;
      menu_item->pressed = TRUE;

      widget_grab(widget->parent, widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      menu_item->pressed = FALSE;
      widget_ungrab(widget->parent, widget);

      if (menu_item->selected) {
        widget_set_state(widget, WIDGET_STATE_SELECTED);
      } else {
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      }
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;

      if (menu_item->pressed && widget_is_point_in(widget, evt.x, evt.y, FALSE)) {
        evt.e = event_init(EVT_CLICK, widget);
        menu_item_set_selected(widget, TRUE);
        menu_item_pointer_up_cleanup(widget);
        widget_dispatch(widget, (event_t*)&evt);
      } else {
        menu_item_pointer_up_cleanup(widget);
      }

      break;
    }
    case EVT_POINTER_LEAVE:
      if (menu_item->selected) {
        widget_set_state(widget, WIDGET_STATE_SELECTED);
      } else {
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      }
      break;
    case EVT_POINTER_ENTER:
      if (menu_item->selected) {
        widget_set_state(widget, WIDGET_STATE_SELECTED);
      } else {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      break;
    case EVT_CLICK: {
      widget_t* parent = widget->parent;
      if (tk_str_eq(widget_get_type(parent), WIDGET_TYPE_LIST_MENU)) {
        list_menu_t* list_menu = LIST_MENU(parent);

        if (list_menu->animatable) {
          list_menu_interrupt_animation(parent);
          widget_set_focused(widget, TRUE);
        }
      }
      break;
    }

    default:
      break;
  }

  return RET_OK;
}

static ret_t menu_item_on_destroy(widget_t* widget) {
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static ret_t menu_item_get_prop(widget_t* widget, const char* name, value_t* v) {
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_STATE_FOR_STYLE)) {
    value_set_str(v, widget_get_state_for_style(widget, FALSE, FALSE));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(menu_item) = {.size = sizeof(menu_item_t),
                             .type = WIDGET_TYPE_MENU_ITEM,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = menu_item_create,
                             .on_event = menu_item_on_event,
                             .on_paint_self = menu_item_on_paint_self,
                             .get_prop = menu_item_get_prop,
                             .on_destroy = menu_item_on_destroy};

widget_t* menu_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(menu_item), x, y, w, h);
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL, NULL);

  menu_item->pressed = FALSE;
  menu_item->selected = FALSE;

  return widget;
}

ret_t menu_item_set_selected_only(widget_t* widget, bool_t selected) {
  menu_item_t* menu_item = MENU_ITEM(widget);
  return_value_if_fail(menu_item != NULL, RET_BAD_PARAMS);

  if (menu_item->selected != selected) {
    menu_item->selected = selected;
  }
  if (menu_item->selected) {
    widget_set_state(widget, WIDGET_STATE_SELECTED);
  } else {
    widget_set_state(widget, WIDGET_STATE_NORMAL);
  }

  return RET_OK;
}

ret_t menu_item_set_selected(widget_t* widget, bool_t selected) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  menu_item_set_selected_only(widget, selected);
  if (widget->parent != NULL) {
    widget_t* parent = widget->parent;

    WIDGET_FOR_EACH_CHILD_BEGIN(parent, iter, i)
    if (iter != widget) {
      menu_item_set_selected_only(iter, !selected);
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return RET_OK;
}

widget_t* menu_item_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, menu_item), NULL);

  return widget;
}
