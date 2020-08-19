/**
 * File:   list_menu.c
 * Author: AWTK Develop Team
 * Brief:  列表菜单控件。
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
 * 2020-07-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "list_menu_register.h"
#include "base/widget_factory.h"
#include "list_menu/list_menu.h"
#include "list_menu/menu_item.h"

ret_t list_menu_register(void) {
  widget_factory_register(widget_factory(), WIDGET_TYPE_LIST_MENU, list_menu_create);
  widget_factory_register(widget_factory(), WIDGET_TYPE_MENU_ITEM, menu_item_create);
  return RET_OK;
}

const char* list_menu_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}