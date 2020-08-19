/**
 * File:   chart_utils.c
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its value
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

#include "chart_utils.h"
#include "tkc/utils.h"

color_t chart_utils_value_color(const value_t* v) {
  if (v->type == VALUE_TYPE_STRING) {
    return color_parse(value_str(v));
  } else {
    color_t color;
    color.color = value_uint32(v);
    return color;
  }
}

ret_t chart_utils_object_parse(chart_utils_on_object_parse_t on_parse, void* ctx,
                               const char* object) {
  ENSURE(on_parse != NULL && object != NULL);

  char name[TK_NAME_LEN + 1];
  value_t v;
  const char* token = NULL;
  tokenizer_t tokenizer;

  tokenizer_init(&tokenizer, object, strlen(object), "{,:} ");

  while (tokenizer_has_more(&tokenizer)) {
    token = tokenizer_next(&tokenizer);

    if (tokenizer_has_more(&tokenizer)) {
      tk_strncpy(name, token, TK_NAME_LEN);
      value_set_str(&v, tokenizer_next(&tokenizer));
      on_parse(ctx, name, &v);
    }
  }

  tokenizer_deinit(&tokenizer);

  return RET_OK;
}

static ret_t _widget_get_window_theme(widget_t* widget, theme_t** win_theme,
                                      theme_t** default_theme) {
  value_t v;
  widget_t* win = widget_get_window(widget);

  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  if (widget_get_prop(win, WIDGET_PROP_THEME_OBJ, &v) == RET_OK) {
    *win_theme = (theme_t*)value_pointer(&v);
  }

  if (widget_get_prop(win, WIDGET_PROP_DEFAULT_THEME_OBJ, &v) == RET_OK) {
    *default_theme = (theme_t*)value_pointer(&v);
  }

  return RET_OK;
}