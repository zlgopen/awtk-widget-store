/**
 * File:   special_edit.c
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "special_edit.h"
#include "base/enums.h"

static ret_t special_edit_dispatch_event(widget_t* widget, event_type_t type) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  event_t evt = event_init(type, widget);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t widget_prepare_text_style_interal(widget_t* widget, canvas_t* c) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  align_h_t align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  align_v_t align_v = (align_v_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);

  canvas_set_text_color(c, tc);
  canvas_set_font(c, font_name, font_size);
  canvas_set_text_align(c, align_h, align_v);

  return RET_OK;
}

static ret_t special_edit_on_paint_self(widget_t* widget, canvas_t* c) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  style_t* style = widget->astyle;
  color_t black = color_init(0, 0, 0, 0xff);
  color_t white = color_init(0xf0, 0xf0, 0xf0, 0xff);
  edit->c = c;
  uint32_t cursor = edit->cursor;
  bool_t insert_model = edit->insert_model;
  bool_t caret_visible = edit->caret_visible;
  int i = 0;
  wstr_t* text = &(widget->text);
  int len = text->size;
  int text_size;
  uint32_t x, y;
  wchar_t chr;
  uint32_t char_w = 0;

  color_t select_bg_color = style_get_color(style, STYLE_ID_SELECTED_BG_COLOR, white);
  color_t select_text_color = style_get_color(style, STYLE_ID_SELECTED_TEXT_COLOR, black);
  color_t text_color = style_get_color(style, STYLE_ID_TEXT_COLOR, black);
  widget_prepare_text_style_interal(widget, c);
  y = (widget->h - c->font_size) / 2;

  if (len != 0) {
    text_size = canvas_measure_text(c, text->str, text->size);
    x = (widget->w - text_size) / 2;

    for (i = 0; i < len; i++) {
      chr = text->str[i];
      char_w = canvas_measure_text(c, &chr, 1);
      if (i == cursor) {
        if (caret_visible && insert_model) {
          canvas_set_fill_color(c, select_bg_color);
          canvas_fill_rect(c, x, y, char_w, c->font_size);
          canvas_set_text_color(c, select_text_color);
        }
      } else {
        canvas_set_text_color(c, text_color);
      }

      canvas_draw_text(c, &chr, 1, x, y);
      x = x + char_w;
    }
  }

  if (caret_visible && insert_model == FALSE) {
    canvas_set_stroke_color(c, text_color);
    x = (widget->w - text_size) / 2;
    if (cursor != 0) {
      x += canvas_measure_text(c, text->str, cursor);
    }

    canvas_draw_vline(c, x, y, c->font_size);
  }

  return RET_OK;
}

static ret_t special_edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    edit->cursor = value_int(v) > 0 ? value_int(v) : 0;
    if (edit->cursor > widget->text.size) {
      edit->cursor = widget->text.size;
    }
    return RET_OK;
  } else if (tk_str_eq(name, FLOAT_DOT)) {
    edit->float_dot = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    edit->min = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    edit->max = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INC_FUN_KEY)) {
    return special_edit_set_inc_fun_key(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DEC_FUN_KEY)) {
    return special_edit_set_dec_fun_key(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t special_edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  special_edit_t* edit = SPECIAL_EDIT(widget);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    value_set_int(v, edit->cursor);
    return RET_OK;
  } else if (tk_str_eq(name, FLOAT_DOT)) {
    value_set_int(v, edit->float_dot);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    value_set_double(v, edit->min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_double(v, edit->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INC_FUN_KEY)) {
    value_set_str(v, edit->inc_fun_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEC_FUN_KEY)) {
    value_set_str(v, edit->dec_fun_key);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t special_edit_on_destroy(widget_t* widget) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit->timer_id != TK_INVALID_ID) {
    timer_remove(edit->timer_id);
    edit->timer_id = TK_INVALID_ID;
  }

  TKMEM_FREE(edit->inc_fun_key);
  TKMEM_FREE(edit->dec_fun_key);

  return RET_OK;
}

static ret_t special_edit_update_caret(const timer_info_t* timer) {
  special_edit_t* edit = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(timer != NULL, RET_REMOVE);

  edit = SPECIAL_EDIT(timer->ctx);
  widget = WIDGET(timer->ctx);
  return_value_if_fail(edit != NULL && widget != NULL, RET_REMOVE);

  widget_invalidate_force(widget, NULL);

  edit->caret_visible = !edit->caret_visible;

  if (widget->focused) {
    return RET_REPEAT;
  } else {
    edit->timer_id = TK_INVALID_ID;
    edit->caret_visible = FALSE;
    return RET_REMOVE;
  }
}

static ret_t special_edit_on_focused(widget_t* widget) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  edit->insert_model = TRUE;
  edit->cursor = 0;
  wstr_t* text = &(widget->text);
  wchar_t c = text->str[edit->cursor];

  //如果是'-'
  if (c == TK_KEY_MINUS) {
    edit->cursor++;
  }

  if (widget->text.size == 0) {
    widget_set_text_utf8(widget, "0");
  }

  if (edit->timer_id == TK_INVALID_ID) {
    edit->timer_id = timer_add(special_edit_update_caret, widget, 600);
  }

  return RET_OK;
}

static ret_t special_edit_inc_or_dec(special_edit_t* edit, double max_or_min, bool_t is_inc) {
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  wstr_t* text = &(widget->text);
  int cursor = edit->cursor;
  wchar_t c = text->str[cursor];
  int old_size = text->size;
  int old_cursor = cursor;
  int new_cursor = 0;
  char char_text[32] = {0};
  char format[32] = {0};

  if (c == TK_KEY_PERIOD || c == TK_KEY_MINUS) {
    return RET_OK;
  }

  if (edit->float_dot == 0) {
    int value;
    wstr_to_int(text, &value);

    if (value == max_or_min) {
      return RET_OK;
    }

    int step = text->size - cursor;
    step = pow(10, step - 1);
    value = is_inc ? (value += step) : (value -= step);

    if (is_inc) {
      if (value > max_or_min) {
        value = max_or_min;
      }
    } else {
      if (value < max_or_min) {
        value = max_or_min;
      }
    }

    tk_snprintf(char_text, sizeof(char_text), "%d", value);
    wstr_set_utf8(text, char_text);
  } else {
    double value = 0;
    double step = 0;
    wstr_to_float(text, &value);

    if (value == max_or_min) {
      return RET_OK;
    }

    //计算step
    //小数点位置，字符大小-float_dot
    int dot_index = text->size - 1 - edit->float_dot;
    step = dot_index - cursor;
    step = step > 0 ? pow(10, step - 1) : pow(10, step);
    value = is_inc ? (value += step) : (value -= step);

    if (is_inc) {
      if (value > max_or_min) {
        value = max_or_min;
      }
    } else {
      if (value < max_or_min) {
        value = max_or_min;
      }
    }

    tk_snprintf(format, sizeof(format), "%%.%df", edit->float_dot);
    tk_snprintf(char_text, sizeof(char_text), format, value);
    wstr_set_utf8(text, char_text);
  }

  // 设置cursor
  new_cursor = text->size - old_size + old_cursor;

  if (new_cursor < 0) {
    new_cursor = 0;
  }

  if (new_cursor > text->size - 1) {
    new_cursor = text->size - 1;
  }

  edit->cursor = new_cursor;

  special_edit_dispatch_event(widget, EVT_VALUE_CHANGED);
  widget_invalidate_force(widget, NULL);

  return RET_STOP;
}

static ret_t special_edit_dec(special_edit_t* edit) {
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  return special_edit_inc_or_dec(edit, edit->min, FALSE);
}

static ret_t special_edit_inc(special_edit_t* edit) {
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  return special_edit_inc_or_dec(edit, edit->max, TRUE);
}

static ret_t special_edit_input(special_edit_t* edit, key_event_t* e) {
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  style_t* style = widget->astyle;
  wstr_t* text = &(widget->text);
  uint32_t key = e->key;
  int cursor = edit->cursor;

  if (key == TK_KEY_DELETE || key == TK_KEY_BACKSPACE) {
    if (cursor == 0) {
      return RET_OK;
    }

    if (edit->insert_model == FALSE) {
      wstr_remove(text, cursor - 1, 1);
      edit->cursor--;
    }
  } else {
    if (key == TK_KEY_MINUS && edit->min > 0) {
      return RET_OK;
    }

    if (edit->insert_model) {
      wstr_clear(text);
      edit->cursor = 0;
      edit->insert_model = FALSE;
    }

    wchar_t c = key;
    wchar_t tmp;
    int i = 0;
    while (tmp = text->str[i++]) {
      if (tmp == TK_KEY_PERIOD && key == TK_KEY_PERIOD) {
        return RET_OK;
      }

      if (tmp == TK_KEY_MINUS && key == TK_KEY_MINUS) {
        return RET_OK;
      }
    }

    //测量输入是否超长度
    wstr_t wstr;
    wstr_init(&wstr, 32);
    wstr_append(&wstr, text->str);
    wstr_push(&wstr, c);
    widget_prepare_text_style_interal(widget, edit->c);
    i = canvas_measure_text(edit->c, wstr.str, wstr.size);
    wstr_reset(&wstr);

    if (widget->w - i <= 0) {
      return RET_OK;
    }

    if (text->size == 0) {
      wstr_push(text, c);
    } else {
      wstr_insert(text, cursor, &c, 1);
    }

    edit->cursor++;

    // //判断大小
    char char_text[32] = {0};
    char format[32] = {0};
    bool_t need_fix = FALSE;

    if (edit->float_dot == 0) {
      int value;
      int min = edit->min;
      int max = edit->max;
      wstr_to_int(text, &value);

      if (value > 0 && value > max) {
        value = max;
        need_fix = TRUE;
      } else if (value < 0 && value < min) {
        value = min;
        need_fix = TRUE;
      }

      if (need_fix) {
        tk_snprintf(char_text, sizeof(char_text), "%d", value);
        wstr_set_utf8(text, char_text);
        edit->cursor = text->size;
      }
    } else {
      double value = 0;
      double min = edit->min;
      double max = edit->max;
      wstr_to_float(text, &value);

      if (value > 0 && value > max) {
        value = max;
        need_fix = TRUE;
      } else if (value < 0 && value < min) {
        value = min;
        need_fix = TRUE;
      }

      if (need_fix) {
        tk_snprintf(format, sizeof(format), "%%.%df", edit->float_dot);
        tk_snprintf(char_text, sizeof(char_text), format, value);
        wstr_set_utf8(text, char_text);
        edit->cursor = text->size;
      }
    }
  }

  widget_invalidate_force(widget, NULL);

  return RET_STOP;
}

static ret_t special_edit_on_key_down(widget_t* widget, key_event_t* e) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  uint32_t key = e->key;
  wstr_t* text = &(widget->text);
  wchar_t c;
  int size = text->size;
  const key_type_value_t* inc_func_key_kv = keys_type_find(edit->inc_fun_key);
  const key_type_value_t* dec_func_key_kv = keys_type_find(edit->dec_fun_key);

  if (key == TK_KEY_TAB) {
    return RET_OK;
  } else if (key == TK_KEY_LEFT) {
    if (edit->insert_model) {
      if (edit->cursor == 0) {
        return RET_OK;
      }
      edit->cursor--;
      c = text->str[edit->cursor];

      //如果是'-'
      if (c == TK_KEY_MINUS) {
        edit->cursor++;
      } else if (c == TK_KEY_PERIOD && edit->cursor != 0) {
        //判断当前字符是否'.'
        edit->cursor--;
      }

      return RET_STOP;
    }
  } else if (key == TK_KEY_RIGHT) {
    if (edit->insert_model) {
      //最后一个
      if (edit->cursor == size - 1) {
        return RET_OK;
      }

      edit->cursor++;
      c = text->str[edit->cursor];
      //判断当前字符是否'.'
      if (c == TK_KEY_PERIOD && edit->cursor < size - 1) {
        edit->cursor++;
      }

      return RET_STOP;
    }
  } else if (inc_func_key_kv && key == inc_func_key_kv->value) {
    if (edit->insert_model) {
      return special_edit_inc(edit);
    }
  } else if (dec_func_key_kv && key == dec_func_key_kv->value) {
    if (edit->insert_model) {
      return special_edit_dec(edit);
    }
  } else if (key == TK_KEY_0 || key == TK_KEY_1 || key == TK_KEY_2 || key == TK_KEY_3 ||
             key == TK_KEY_4 || key == TK_KEY_5 || key == TK_KEY_6 || key == TK_KEY_7 ||
             key == TK_KEY_8 || key == TK_KEY_9 || key == TK_KEY_MINUS || key == TK_KEY_PERIOD ||
             key == TK_KEY_DELETE || key == TK_KEY_BACKSPACE) {
    return special_edit_input(edit, e);
  }

  return RET_OK;
}

static ret_t special_edit_on_event(widget_t* widget, event_t* e) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->visible, RET_OK);

  ret_t ret = RET_OK;
  uint32_t type = e->type;
  widget_invalidate(widget, NULL);

  switch (type) {
    case EVT_FOCUS: {
      special_edit_on_focused(widget);
      ret = RET_OK;
      break;
    }
    case EVT_BLUR: {
      edit->insert_model = TRUE;
      edit->caret_visible = FALSE;
      ret = RET_OK;
      break;
    }
    case EVT_KEY_DOWN: {
      ret = special_edit_on_key_down(widget, (key_event_t*)e);
      break;
    }
    case EVT_KEY_UP: {
      if (edit->timer_id == TK_INVALID_ID) {
        if (((key_event_t*)e)->key == TK_KEY_RETURN) {
          special_edit_on_focused(widget);
          return RET_STOP;
        }
      }
      break;
    }

    default:
      break;
  }

  return ret;
}

TK_DECL_VTABLE(special_edit) = {.size = sizeof(special_edit_t),
                                .type = WIDGET_TYPE_SPECIAL_EDIT,
                                .return_key_to_activate = TRUE,
                                .parent = TK_PARENT_VTABLE(widget),
                                .create = special_edit_create,
                                .on_paint_self = special_edit_on_paint_self,
                                .set_prop = special_edit_set_prop,
                                .get_prop = special_edit_get_prop,
                                .on_destroy = special_edit_on_destroy,
                                .on_event = special_edit_on_event};

static widget_t* special_edit_create_ex(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y,
                                        wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, vt, x, y, w, h);
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, NULL);

  edit->cursor = 0;
  edit->float_dot = 0;
  edit->min = 0;
  edit->max = 0;
  edit->insert_model = TRUE;
  edit->caret_visible = FALSE;
  edit->timer_id = TK_INVALID_ID;

  widget_set_text(widget, L"");

  return widget;
}

widget_t* special_edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return special_edit_create_ex(parent, TK_REF_VTABLE(special_edit), x, y, w, h);
}

widget_t* special_edit_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, special_edit), NULL);

  return widget;
}

ret_t special_edit_set_int_limit(widget_t* widget, int32_t min, int32_t max) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->min = min;
  edit->max = max;
  return RET_OK;
}

ret_t special_edit_set_limit(widget_t* widget, double min, double max) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->min = min;
  edit->max = max;
  return RET_OK;
}

ret_t special_edit_set_float_dot(widget_t* widget, uint32_t float_dot) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->float_dot = float_dot;

  return RET_OK;
}

ret_t special_edit_set_inc_fun_key(widget_t* widget, const char* name) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(edit->inc_fun_key);
  edit->inc_fun_key = tk_strdup(name);

  return RET_OK;
}

ret_t special_edit_set_dec_fun_key(widget_t* widget, const char* name) {
  special_edit_t* edit = SPECIAL_EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(edit->dec_fun_key);
  edit->dec_fun_key = tk_strdup(name);

  return RET_OK;
}