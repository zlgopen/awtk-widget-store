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
#include "special_edit_register.h"
#include "base/widget_factory.h"
#include "special_edit/special_edit.h"

ret_t special_edit_register(void) {
  return widget_factory_register(widget_factory(), WIDGET_TYPE_SPECIAL_EDIT, special_edit_create);
}

const char* special_edit_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}