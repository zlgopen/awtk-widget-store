/**
 * File:   number_label.h
 * Author: AWTK Develop Team
 * Brief:  number_label
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "number_label/number_label.h"

ret_t number_label_set_format(widget_t* widget, const char* format) {
  number_label_t* number_label = NUMBER_LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(format != NULL, RET_BAD_PARAMS);

  number_label->format = tk_str_copy(number_label->format, format);

  return widget_invalidate(widget, NULL);
}

ret_t number_label_set_value(widget_t* widget, double value) {
  number_label_t* number_label = NUMBER_LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  number_label->value = value;

  return widget_invalidate(widget, NULL);
}

ret_t number_label_set_decimal_font_size_scale(widget_t* widget, double decimal_font_size_scale) {
  number_label_t* number_label = NUMBER_LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  number_label->decimal_font_size_scale = decimal_font_size_scale;

  return widget_invalidate(widget, NULL);
}

static ret_t number_label_get_prop(widget_t* widget, const char* name, value_t* v) {
  number_label_t* number_label = NUMBER_LABEL(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_double(v, number_label->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, number_label->format);
    return RET_OK;
  } else if (tk_str_eq(name, NUMBER_LABEL_PROP_DECIMAL_FONT_SIZE_SCALE)) {
    value_set_double(v, number_label->decimal_font_size_scale);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t number_label_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    number_label_set_value(widget, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    number_label_set_format(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, NUMBER_LABEL_PROP_DECIMAL_FONT_SIZE_SCALE)) {
    number_label_set_decimal_font_size_scale(widget, value_double(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t number_label_on_destroy(widget_t* widget) {
  number_label_t* number_label = NUMBER_LABEL(widget);
  return_value_if_fail(widget != NULL && number_label != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(number_label->format);

  return RET_OK;
}

static ret_t number_label_paint_text(widget_t* widget, canvas_t* c, wstr_t* text) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = widget->w;
  int32_t h = widget->h;
  float_t font_height = 0;
  float_t text_width = 0;
  float_t int_part_width = 0;
  float_t decimal_part_width = 0;
  int32_t int_part_len = 0;
  int32_t decimal_part_len = 0;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  number_label_t* number_label = NUMBER_LABEL(widget);
  color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 0);
  int32_t margin_top = style_get_int(style, STYLE_ID_MARGIN_TOP, margin);
  int32_t margin_left = style_get_int(style, STYLE_ID_MARGIN_LEFT, margin);
  int32_t margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  uint16_t decimal_font_size = font_size * number_label->decimal_font_size_scale;
  align_h_t align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  align_v_t align_v = (align_v_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);

  while (text->str[int_part_len] != 0 && text->str[int_part_len] != '.') {
    int_part_len++;
  }
  int_part_len++;
  decimal_part_len = text->size - int_part_len;

  canvas_set_text_color(c, tc);
  canvas_set_text_align(c, align_h, align_v);

  canvas_set_font(c, font_name, font_size);
  int_part_width = canvas_measure_text(c, text->str, int_part_len);

  canvas_set_font(c, font_name, decimal_font_size);
  decimal_part_width = canvas_measure_text(c, text->str + int_part_len, decimal_part_len);

  text_width = int_part_width + decimal_part_width;
  switch (align_h) {
    case ALIGN_H_LEFT: {
      x = margin_left;
      break;
    }
    case ALIGN_H_RIGHT: {
      x = w - margin_left - text_width;
      break;
    }
    default: {
      x = (w - text_width) / 2;
      break;
    }
  }

  canvas_set_font(c, font_name, font_size);
  font_height = canvas_get_font_height(c);
  switch (align_v) {
    case ALIGN_V_TOP: {
      y = margin_top;
      canvas_draw_text(c, text->str, int_part_len, x, y);

      break;
    }
    case ALIGN_V_BOTTOM: {
      y = h - margin_bottom - font_height;
      canvas_draw_text(c, text->str, int_part_len, x, y);

      break;
    }
    default: {
      y = (h - font_height) / 2;
      canvas_draw_text(c, text->str, int_part_len, x, y);
      break;
    }
  }

  canvas_set_font(c, font_name, decimal_font_size);
  y += font_height * (1 - number_label->decimal_font_size_scale) - 1;
  x += int_part_width;
  canvas_draw_text(c, text->str + int_part_len, decimal_part_len, x, y);

  return RET_OK;
}

static ret_t number_label_on_paint_self(widget_t* widget, canvas_t* c) {
  char buff[64];
  wstr_t* text = &(widget->text);
  number_label_t* number_label = NUMBER_LABEL(widget);
  const char* format =
      number_label->format != NULL ? number_label->format : NUMBER_LABEL_DEFAULT_FORMAT;
  return_value_if_fail(widget->astyle != NULL, RET_BAD_PARAMS);

  memset(buff, 0x00, sizeof(buff));
  tk_snprintf(buff, sizeof(buff) - 1, format, number_label->value);

  wstr_set_utf8(text, buff);

  return number_label_paint_text(widget, c, text);
}

static const char* s_number_label_properties[] = {NUMBER_LABEL_PROP_DECIMAL_FONT_SIZE_SCALE,
                                                  WIDGET_PROP_FORMAT, WIDGET_PROP_VALUE, NULL};

TK_DECL_VTABLE(number_label) = {.size = sizeof(number_label_t),
                                .type = WIDGET_TYPE_NUMBER_LABEL,
                                .parent = NULL,
                                .clone_properties = s_number_label_properties,
                                .persistent_properties = s_number_label_properties,
                                .create = number_label_create,
                                .on_paint_self = number_label_on_paint_self,
                                .set_prop = number_label_set_prop,
                                .get_prop = number_label_get_prop,
                                .on_destroy = number_label_on_destroy};

widget_t* number_label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  number_label_t* number_label =
      NUMBER_LABEL(widget_create(parent, TK_REF_VTABLE(number_label), x, y, w, h));
  number_label->format = tk_strdup(NUMBER_LABEL_DEFAULT_FORMAT);
  number_label->decimal_font_size_scale = 0.6;

  return (widget_t*)number_label;
}

widget_t* number_label_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, number_label), NULL);

  return widget;
}
