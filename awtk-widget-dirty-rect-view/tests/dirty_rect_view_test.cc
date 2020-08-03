#include "dirty_rect_view/dirty_rect_view.h"
#include "gtest/gtest.h"

TEST(dirty_rect_view, basic) {
  value_t v;
  widget_t* w = dirty_rect_view_create(NULL, 10, 20, 30, 40);

  
  ASSERT_EQ(dirty_rect_view_set_lcd_buff_number(w, 3), RET_OK);
  ASSERT_EQ(DIRTY_RECT_VIEW(w)->lcd_buff_number, 3);

  ASSERT_EQ(dirty_rect_view_set_enable_dirty_rect_view(w, TRUE), RET_OK);
  ASSERT_EQ(DIRTY_RECT_VIEW(w)->enable_dirty_rect_view, TRUE);

  widget_destroy(w);
}
