#include "awtk.h"
#include "list_menu/list_menu.h"
#include "list_menu_register.h"

static ret_t on_menu_item(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  (void)e;

  return dialog_info("info", "hello awtk");
}

static ret_t on_move_list_item(void* ctx, event_t* e) {
  widget_t* button = WIDGET(e->target);
  widget_t* win = WIDGET(ctx);
  widget_t* list_menu = NULL;

  if (tk_str_eq(button->name, "left") || tk_str_eq(button->name, "right")) {
    list_menu = widget_lookup(win, "list_menu_h", FALSE);
  } else {
    list_menu = widget_lookup(win, "list_menu_v", FALSE);
  }

  if (tk_str_eq(button->name, "left") || tk_str_eq(button->name, "up")) {
    list_menu_selected_prev(list_menu);
  } else {
    list_menu_selected_next(list_menu);
  }

  return RET_OK;
}

static ret_t on_enter_click(void* ctx, event_t* e) {
  widget_t* button = WIDGET(e->target);
  widget_t* win = WIDGET(ctx);
  list_menu_t* list_menu = NULL;

  if (tk_str_eq(button->name, "h_enter")) {
    list_menu = LIST_MENU(widget_lookup(win, "list_menu_h", FALSE));
  } else {
    list_menu = LIST_MENU(widget_lookup(win, "list_menu_v", FALSE));
  }

  widget_t* menu_item = list_menu->selected_child;
  return_value_if_fail(list_menu != NULL && menu_item != NULL, RET_BAD_PARAMS);

  event_t evt;
  evt = event_init(EVT_CLICK, menu_item);
  widget_dispatch(menu_item, &evt);

  return RET_OK;
}

/**
 * 子控件初始化(主要是设置click回调、初始显示信息)
 */
static ret_t init_widget(void* ctx, const void* iter) {
  (void)ctx;
  widget_t* widget = WIDGET(iter);
  widget_t* win = widget_get_window(widget);

  if (widget->name != NULL) {
    const char* name = widget->name;
    if (tk_str_eq(name, "menu_item")) {
      widget_on(widget, EVT_CLICK, on_menu_item, widget);
    } else if (tk_str_eq(name, "left") || tk_str_eq(name, "up") || tk_str_eq(name, "right") ||
               tk_str_eq(name, "down")) {
      widget_on(widget, EVT_CLICK, on_move_list_item, win);
    } else if (tk_str_eq(name, "h_enter") || tk_str_eq(name, "v_enter")) {
      widget_on(widget, EVT_CLICK, on_enter_click, win);
    }
  }

  return RET_OK;
}

/**
 * 初始化窗口上的子控件
 */
static void init_children_widget(widget_t* widget) {
  widget_foreach(widget, init_widget, widget);
}

/**
 * 初始化
 */
ret_t application_init() {
  list_menu_register();

  widget_t* win = window_open("main");
  if (win) {
    init_children_widget(win);
  }

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  log_debug("application_exit");
  return RET_OK;
}