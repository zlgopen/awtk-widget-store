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
#include "dirty_rect_view.h"
#include "dirty_rect_view_item.h"

extern ret_t widget_on_paint_children_default(widget_t *widget, canvas_t *c);

static ret_t dirty_rect_view_widget_fill_rect(widget_t *widget, canvas_t *c,
                                              rect_t *r, bool_t bg) {
  bitmap_t img;
  ret_t ret = RET_OK;
  style_t *style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  rect_t bg_r = rect_init(widget->x, widget->y, widget->w, widget->h);
  const char *color_key = bg ? STYLE_ID_BG_COLOR : STYLE_ID_FG_COLOR;
  const char *image_key = bg ? STYLE_ID_BG_IMAGE : STYLE_ID_FG_IMAGE;

  color_t color = style_get_color(style, color_key, trans);
  const char *image_name = style_get_str(style, image_key, NULL);

  if (color.rgba.a && r->w > 0 && r->h > 0) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      if (bg) {
        ret = canvas_fill_rounded_rect(c, r, NULL, &color, radius);
      } else {
        ret = canvas_fill_rounded_rect(c, r, &bg_r, &color, radius);
      }
      if (ret == RET_FAIL) {
        canvas_fill_rect(c, r->x, r->y, r->w, r->h);
      }
    } else {
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  if (image_name != NULL && r->w > 0 && r->h > 0) {
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      canvas_draw_image(c, &img, r, r);
    }
  }

  return RET_OK;
}

static bool_t dirty_rect_view_get_child_is_dirty(widget_t *widget) {
  bool_t is_dirty = FALSE;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)

  if (iter->visible && iter->dirty) {
    is_dirty = TRUE;
    break;
  }

  is_dirty = dirty_rect_view_get_child_is_dirty(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return is_dirty;
}

static ret_t dirty_rect_view_on_paint_begin(widget_t *widget, canvas_t *c) {

  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL, RET_BAD_PARAMS);

  if (dirty_rect_view->enable_dirty_rect_view) {

    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    if (tk_str_cmp(iter->vt->type, WIDGET_TYPE_DIRTY_RECT_VIEW_ITEM) == 0) {
      if (dirty_rect_view_get_child_is_dirty(iter)) {

        dirty_rect_view_item_set_invalidate_number(
            iter, dirty_rect_view->lcd_buff_number);
      }
    }

    WIDGET_FOR_EACH_CHILD_END();
  }
  return RET_OK;
}

static ret_t dirty_rect_view_on_paint_self(widget_t *widget, canvas_t *c) {
  return RET_OK;
}

static ret_t dirty_rect_view_on_paint_background(widget_t *widget,
                                                 canvas_t *c) {
  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(widget != NULL && c != NULL && dirty_rect_view != NULL,
                       RET_BAD_PARAMS);

  if (dirty_rect_view->enable_dirty_rect_view) {
    if (dirty_rect_view->paint_background_number-- > 0) {
      widget_draw_background(widget, c);
    } else {
      dirty_rect_view->paint_background_number = 0;
      WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
      if (tk_str_cmp(iter->vt->type, WIDGET_TYPE_DIRTY_RECT_VIEW_ITEM) == 0) {
        if (dirty_rect_view_item_get_is_paint(iter) &&
            dirty_rect_view_item_get_is_paint_background(iter)) {
          rect_t r = rect_init(iter->x, iter->y, iter->w, iter->h);

          dirty_rect_view_widget_fill_rect(widget, c, &r, TRUE);
        }
      }
      WIDGET_FOR_EACH_CHILD_END();
    }

  } else {
    widget_draw_background(widget, c);
  }

  return RET_OK;
}

static ret_t dirty_rect_view_on_paint_children(widget_t *widget, canvas_t *c) {

  rect_t save_c_clip;
  rect_t save_vg_clip;
  vgcanvas_t *vg = NULL;
  dirty_rect_view_t *dirty_rect_view = NULL;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL, RET_BAD_PARAMS);

  if (dirty_rect_view->enable_dirty_rect_view) {
    vg = canvas_get_vgcanvas(c);
#if defined(AWTK_WEB)
    canvas_save(c);
#endif /*AWTK_WEB*/
    canvas_get_clip_rect(c, &save_c_clip);
    if (vg != NULL) {
      vgcanvas_save(vg);
      save_vg_clip = rect_init(vg->clip_rect.x, vg->clip_rect.y,
                               vg->clip_rect.w, vg->clip_rect.h);
    }

    widget_on_paint_children_default(widget, c);

    if (vg != NULL) {
      vgcanvas_clip_rect(vg, save_vg_clip.x, save_vg_clip.y, save_vg_clip.w,
                         save_vg_clip.h);
      vgcanvas_restore(vg);
    }

    canvas_set_clip_rect(c, &save_c_clip);
#if defined(AWTK_WEB)
    canvas_restore(c);
#endif /*AWTK_WEB*/
  } else {
    widget_on_paint_children_default(widget, c);
  }

  return RET_OK;
}

ret_t dirty_rect_view_set_enable_dirty_rect_view(widget_t *widget,
                                            bool_t is_enable) {
  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL, RET_BAD_PARAMS);
  dirty_rect_view->enable_dirty_rect_view = is_enable;

  return RET_OK;
}

ret_t dirty_rect_view_set_lcd_buff_number(widget_t *widget,
                                          int32_t lcd_buff_number) {
  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL, RET_BAD_PARAMS);
  dirty_rect_view->lcd_buff_number = lcd_buff_number;
  dirty_rect_view->paint_background_number = lcd_buff_number;
  return RET_OK;
}

static ret_t dirty_rect_view_get_prop(widget_t *widget, const char *name,
                                      value_t *v) {
  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  if (tk_str_eq(name, DIRTY_RECT_VIEW_PROP_ENABLE_DIRTY_RECT_VIEW)) {
    value_set_bool(v, dirty_rect_view->enable_dirty_rect_view);
    return RET_OK;
  } else if (tk_str_eq(name, DIRTY_RECT_VIEW_PROP_LCD_BUFF_NUMBER)) {
    value_set_int32(v, dirty_rect_view->lcd_buff_number);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dirty_rect_view_set_prop(widget_t *widget, const char *name,
                                      const value_t *v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  if (tk_str_eq(name, DIRTY_RECT_VIEW_PROP_ENABLE_DIRTY_RECT_VIEW)) {
    return dirty_rect_view_set_enable_dirty_rect_view(widget, value_bool(v));
  } else if (tk_str_eq(name, DIRTY_RECT_VIEW_PROP_LCD_BUFF_NUMBER)) {
    return dirty_rect_view_set_lcd_buff_number(widget, value_int32(v));
  }

  return RET_NOT_FOUND;
}

static const char *const s_dirty_rect_view_properties[] = {
    WIDGET_PROP_LENGTH, DIRTY_RECT_VIEW_PROP_ENABLE_DIRTY_RECT_VIEW,
    DIRTY_RECT_VIEW_PROP_LCD_BUFF_NUMBER, NULL};

TK_DECL_VTABLE(dirty_rect_view) = {
    .size = sizeof(dirty_rect_view_t),
    .type = WIDGET_TYPE_DIRTY_RECT_VIEW,
    .parent = TK_PARENT_VTABLE(widget),
    .clone_properties = s_dirty_rect_view_properties,
    .persistent_properties = s_dirty_rect_view_properties,
    .create = dirty_rect_view_create,
    .set_prop = dirty_rect_view_set_prop,
    .get_prop = dirty_rect_view_get_prop,
    .on_paint_begin = dirty_rect_view_on_paint_begin,
    .on_paint_background = dirty_rect_view_on_paint_background,
    .on_paint_children = dirty_rect_view_on_paint_children,
    .on_paint_self = dirty_rect_view_on_paint_self};

widget_t *dirty_rect_view_create(widget_t *parent, xy_t x, xy_t y, wh_t w,
                                 wh_t h) {
  widget_t *widget =
      widget_create(parent, TK_REF_VTABLE(dirty_rect_view), x, y, w, h);
  dirty_rect_view_t *dirty_rect_view = DIRTY_RECT_VIEW(widget);
  return_value_if_fail(dirty_rect_view != NULL, NULL);

  dirty_rect_view->lcd_buff_number = 1;
  dirty_rect_view->enable_dirty_rect_view = TRUE;
  dirty_rect_view->paint_background_number = dirty_rect_view->lcd_buff_number;

  return widget;
}

widget_t *dirty_rect_view_cast(widget_t *widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dirty_rect_view), NULL);

  return widget;
}
