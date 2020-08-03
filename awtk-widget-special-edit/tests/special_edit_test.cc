#include "special_edit/special_edit.h"
#include "gtest/gtest.h"

TEST(special_edit, basic) {
  value_t v;
  widget_t* w = special_edit_create(NULL, 10, 20, 30, 40);

  widget_destroy(w);
}
