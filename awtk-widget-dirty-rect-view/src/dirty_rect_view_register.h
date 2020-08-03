﻿/**
 * File:   dirty_rect_view_register.h
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


#ifndef TK_DIRTY_RECT_VIEW_REGISTER_H
#define TK_DIRTY_RECT_VIEW_REGISTER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @method  dirty_rect_view_register
 * 注册控件。
 *
 * @annotation ["global"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dirty_rect_view_register(void);

END_C_DECLS

#endif /*TK_DIRTY_RECT_VIEW_REGISTER_H*/