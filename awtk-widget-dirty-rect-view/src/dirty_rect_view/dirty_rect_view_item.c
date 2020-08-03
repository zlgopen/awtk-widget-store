
#include "tkc/mem.h"
#include "base/widget_factory.h"
#include "dirty_rect_view_item.h"

extern ret_t widget_on_paint_children_default(widget_t *widget, canvas_t *c);

static ret_t dirty_rect_view_item_on_paint_self(widget_t *widget, canvas_t *c) {

  return RET_OK;
}

static ret_t dirty_rect_view_item_on_paint_children(widget_t *widget,
                                                    canvas_t *c) {

  ret_t ret = RET_OK;
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL && c != NULL,
                       RET_BAD_PARAMS);

  if (dirty_rect_view_item->invalidate_number-- > 0) {
    vgcanvas_t *vg = canvas_get_vgcanvas(c);
    rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
    rect_t dirty_clip = rect_init(c->lcd->dirty_rect.x, c->lcd->dirty_rect.y,
                                  c->lcd->dirty_rect.w, c->lcd->dirty_rect.h);

    rect_t new_clip = rect_intersect(&dirty_clip, &r);

    canvas_set_clip_rect(c, &new_clip);

    if (vg != NULL) {
      vgcanvas_clip_rect(vg, new_clip.x, new_clip.y, new_clip.w, new_clip.h);
    }

    ret = widget_on_paint_children_default(widget, c);
  } else {
    dirty_rect_view_item->invalidate_number = 0;
  }

  return ret;
}

static ret_t dirty_rect_view_item_get_prop(widget_t *widget, const char *name,
                                           value_t *v) {
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL && name != NULL &&
                           v != NULL,
                       RET_BAD_PARAMS);

  if (tk_str_eq(name, DIRTY_RECT_VIEW_ITEM_INVALIDATE_NUMBER)) {
    value_set_int32(v, dirty_rect_view_item->invalidate_number);
    return RET_OK;
  } else if (tk_str_eq(name, DIRTY_RECT_VIEW_ITEM_INVALIDATE_BACKGROUND)) {
    value_set_bool(v, dirty_rect_view_item->invalidate_background);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dirty_rect_view_item_set_prop(widget_t *widget, const char *name,
                                           const value_t *v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  if (tk_str_eq(name, DIRTY_RECT_VIEW_ITEM_INVALIDATE_NUMBER)) {
    return dirty_rect_view_item_set_invalidate_number(widget, value_int32(v));
  } else if (tk_str_eq(name, DIRTY_RECT_VIEW_ITEM_INVALIDATE_BACKGROUND)) {
    return dirty_rect_view_item_set_paint_background(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char *const s_dirty_rect_view_item_properties[] = {
    DIRTY_RECT_VIEW_ITEM_INVALIDATE_NUMBER,
    DIRTY_RECT_VIEW_ITEM_INVALIDATE_BACKGROUND, NULL};

TK_DECL_VTABLE(dirty_rect_view_item) = {
    .size = sizeof(dirty_rect_view_item_t),
    .type = WIDGET_TYPE_DIRTY_RECT_VIEW_ITEM,
    .parent = TK_PARENT_VTABLE(widget),
    .clone_properties = s_dirty_rect_view_item_properties,
    .persistent_properties = s_dirty_rect_view_item_properties,
    .create = dirty_rect_view_item_create,
    .set_prop = dirty_rect_view_item_set_prop,
    .get_prop = dirty_rect_view_item_get_prop,
    .on_paint_children = dirty_rect_view_item_on_paint_children,
    .on_paint_self = dirty_rect_view_item_on_paint_self};

widget_t *dirty_rect_view_item_create(widget_t *parent, xy_t x, xy_t y, wh_t w,
                                      wh_t h) {
  widget_t *widget =
      widget_create(parent, TK_REF_VTABLE(dirty_rect_view_item), x, y, w, h);
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL, NULL);
  dirty_rect_view_item->invalidate_background = TRUE;
  return widget;
}

ret_t dirty_rect_view_item_set_invalidate_number(widget_t *widget,
                                                 int32_t invalidate_number) {
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL, RET_BAD_PARAMS);
  dirty_rect_view_item->invalidate_number = invalidate_number;

  return RET_OK;
}

ret_t dirty_rect_view_item_set_paint_background(widget_t *widget,
                                                bool_t invalidate_background) {
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL, RET_BAD_PARAMS);
  dirty_rect_view_item->invalidate_background = invalidate_background;

  return RET_OK;
}

bool_t dirty_rect_view_item_get_is_paint(widget_t *widget) {
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL, RET_BAD_PARAMS);

  return dirty_rect_view_item->invalidate_number > 0;
}

bool_t dirty_rect_view_item_get_is_paint_background(widget_t *widget) {
  dirty_rect_view_item_t *dirty_rect_view_item = DIRTY_RECT_VIEW_ITEM(widget);
  return_value_if_fail(dirty_rect_view_item != NULL, RET_BAD_PARAMS);

  return dirty_rect_view_item->invalidate_background;
}

widget_t *dirty_rect_view_item_cast(widget_t *widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dirty_rect_view_item),
                       NULL);

  return widget;
}
