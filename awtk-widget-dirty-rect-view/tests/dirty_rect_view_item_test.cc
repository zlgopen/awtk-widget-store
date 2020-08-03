#include "dirty_rect_view/dirty_rect_view_item.h"
#include "gtest/gtest.h"

TEST(dirty_rect_view_item, basic) {
  value_t v;
  widget_t* w = dirty_rect_view_item_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(dirty_rect_view_item_set_invalidate_number(w, 3), RET_OK);
  ASSERT_EQ(DIRTY_RECT_VIEW_ITEM(w)->invalidate_number, 3);
  ASSERT_EQ(dirty_rect_view_item_get_is_paint(w), TRUE);

  ASSERT_EQ(dirty_rect_view_item_set_paint_background(w, TRUE), RET_OK);
  ASSERT_EQ(dirty_rect_view_item_get_is_paint_background(w), TRUE);

  widget_destroy(w);
}
