#include "list_menu/list_menu.h"
#include "gtest/gtest.h"

TEST(list_menu, basic) {
  value_t v;
  widget_t* w = list_menu_create(NULL, 10, 20, 30, 40);

  widget_destroy(w);
}
