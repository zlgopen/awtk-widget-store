/**
 * File:   dirty_rect_view.c
 * Author: AWTK Develop Team
 * Brief:  自定义脏矩形控件
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
 * 2020-06-16 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "dirty_rect_view_register.h"
#include "base/widget_factory.h"
#include "dirty_rect_view/dirty_rect_view.h"
#include "dirty_rect_view/dirty_rect_view_item.h"

ret_t dirty_rect_view_register(void) {
  ret_t ret = RET_OK;
  ret = widget_factory_register(widget_factory(), WIDGET_TYPE_DIRTY_RECT_VIEW,
                                dirty_rect_view_create);

  if (ret == RET_OK) {
    return widget_factory_register(widget_factory(), WIDGET_TYPE_DIRTY_RECT_VIEW_ITEM,
                                   dirty_rect_view_item_create);
  } else {
    return ret;
  }
}

const char* dirty_rect_view_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}