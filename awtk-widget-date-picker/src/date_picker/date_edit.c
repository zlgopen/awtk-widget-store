/**
 * File:   date_edit.c
 * Author: AWTK Develop Team
 * Brief:  日期编辑控件。
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
 * 2020-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/date_time.h"
#include "base/window.h"
#include "date_edit.h"
#include "date_picker.h"

static ret_t date_edit_update_view(widget_t* widget);

ret_t date_edit_set_year(widget_t* widget, uint32_t year) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(date_edit != NULL, RET_BAD_PARAMS);

  date_edit->year = year;
  if (date_edit->inited) {
    date_edit_update_view(widget);
  }

  return RET_OK;
}

ret_t date_edit_set_month(widget_t* widget, uint32_t month) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(date_edit != NULL, RET_BAD_PARAMS);

  date_edit->month = month;
  if (date_edit->inited) {
    date_edit_update_view(widget);
  }

  return RET_OK;
}

ret_t date_edit_set_day(widget_t* widget, uint32_t day) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(date_edit != NULL, RET_BAD_PARAMS);

  date_edit->day = day;
  if (date_edit->inited) {
    date_edit_update_view(widget);
  }

  return RET_OK;
}

static ret_t date_edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(date_edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(DATE_EDIT_PROP_YEAR, name)) {
    value_set_uint32(v, date_edit->year);
    return RET_OK;
  } else if (tk_str_eq(DATE_EDIT_PROP_MONTH, name)) {
    value_set_uint32(v, date_edit->month);
    return RET_OK;
  } else if (tk_str_eq(DATE_EDIT_PROP_DAY, name)) {
    value_set_uint32(v, date_edit->day);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t date_edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(DATE_EDIT_PROP_YEAR, name)) {
    date_edit_set_year(widget, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(DATE_EDIT_PROP_MONTH, name)) {
    date_edit_set_month(widget, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(DATE_EDIT_PROP_DAY, name)) {
    date_edit_set_day(widget, value_uint32(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t date_edit_on_destroy(widget_t* widget) {
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(widget != NULL && date_edit != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static ret_t date_edit_on_paint_self(widget_t* widget, canvas_t* c) {
  date_edit_t* date_edit = DATE_EDIT(widget);

  (void)date_edit;

  return RET_OK;
}

static ret_t date_edit_update_view(widget_t* widget) {
  char text[64];
  date_edit_t* date_edit = DATE_EDIT(widget);
  widget_t* widget_date = widget_lookup(widget, DATE_EDIT_CHILD_DATE, TRUE);

  tk_snprintf(text, sizeof(text) - 1, DATE_EDIT_FORMAT, date_edit->year, date_edit->month,
              date_edit->day);
  widget_set_text_utf8(widget_date, text);

  return RET_OK;
}

static ret_t date_edit_on_picked(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  date_edit_t* date_edit = DATE_EDIT(widget);
  date_picker_t* date_picker = DATE_PICKER(e->target);

  date_edit->year = date_picker->year;
  date_edit->month = date_picker->month;
  date_edit->day = date_picker->day;
  date_edit_update_view(widget);

  widget_dispatch_simple_event(widget, EVT_VALUE_CHANGED);

  return RET_OK;
}

static ret_t date_edit_on_edited(void* ctx, event_t* e) {
  str_t str;
  int day = 0;
  int year = 0;
  int month = 0;
  widget_t* widget = WIDGET(ctx);
  widget_t* target = WIDGET(e->target);
  date_edit_t* date_edit = DATE_EDIT(widget);
  date_picker_t* date_picker = DATE_PICKER(e->target);

  return_value_if_fail(str_init(&str, 64) != NULL, RET_OK);

  str_from_wstr(&str, target->text.str);
  if (tk_sscanf(str.str, DATE_EDIT_FORMAT, &year, &month, &day) == 3) {
    date_edit->year = year;
    date_edit->month = month;
    date_edit->day = day;
    widget_dispatch_simple_event(widget, EVT_VALUE_CHANGED);
  }
  str_reset(&str);

  return RET_OK;
}

static ret_t date_edit_on_pick_clicked(void* ctx, event_t* e) {
  point_t p = {0, 0};
  widget_t* widget = WIDGET(ctx);
  date_edit_t* date_edit = DATE_EDIT(widget);
  widget_t* win = window_open("date_picker");
  widget_t* wm = widget_get_window_manager(widget);
  widget_t* date = widget_lookup(win, DATE_EDIT_CHILD_DATE, TRUE);
  widget_t* picker = widget_lookup_by_type(win, WIDGET_TYPE_DATE_PICKER, TRUE);

  widget_to_screen(widget, &p);

  if ((p.y + widget->h + win->h) < wm->h) {
    p.y += widget->h;
  } else if (p.y >= win->h) {
    p.y -= win->h;
  } else {
    p.y = 0;
  }

  if ((p.x + win->w) > wm->w) {
    if ((p.x + widget->w) > win->w) {
      p.x = p.x + widget->w - win->w;
    } else {
      p.x = wm->w - win->w;
    }
  }

  widget_move(win, p.x, p.y);

  date_picker_set_year(picker, date_edit->year);
  date_picker_set_month(picker, date_edit->month);
  date_picker_set_day(picker, date_edit->day);

  widget_on(picker, EVT_VALUE_CHANGED, date_edit_on_picked, widget);

  return RET_OK;
}

static ret_t date_edit_init(widget_t* widget) {
  date_edit_t* date_edit = DATE_EDIT(widget);

  date_edit_update_view(widget);
  widget_child_on(widget, DATE_EDIT_CHILD_PICK, EVT_CLICK, date_edit_on_pick_clicked, widget);
  widget_child_on(widget, DATE_EDIT_CHILD_DATE, EVT_VALUE_CHANGED, date_edit_on_edited, widget);

  date_edit->inited;

  return RET_OK;
}

static ret_t date_edit_on_event(widget_t* widget, event_t* e) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_WILL_OPEN: {
      date_edit_init(widget);
      break;
    }
  }

  return RET_OK;
}

const char* s_date_edit_properties[] = {DATE_EDIT_PROP_YEAR, DATE_EDIT_PROP_MONTH,
                                        DATE_EDIT_PROP_DAY, NULL};

TK_DECL_VTABLE(date_edit) = {.size = sizeof(date_edit_t),
                             .type = WIDGET_TYPE_DATE_EDIT,
                             .clone_properties = s_date_edit_properties,
                             .persistent_properties = s_date_edit_properties,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = date_edit_create,
                             .on_paint_self = date_edit_on_paint_self,
                             .set_prop = date_edit_set_prop,
                             .get_prop = date_edit_get_prop,
                             .on_event = date_edit_on_event,
                             .on_destroy = date_edit_on_destroy};

widget_t* date_edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(date_edit), x, y, w, h);
  date_edit_t* date_edit = DATE_EDIT(widget);
  return_value_if_fail(date_edit != NULL, NULL);

  date_edit->year = 2020;
  date_edit->month = 1;
  date_edit->day = 1;

  return widget;
}

widget_t* date_edit_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, date_edit), NULL);

  return widget;
}
