
#include "chart_view/chart_view.h"
#include "chart_view/x_axis.h"
#include "chart_view/y_axis.h"
#include "chart_view/line_series.h"
#include "chart_view/bar_series.h"
#include "chart_view/tooltip.h"
#include "pie_slice/pie_slice.h"

ret_t chart_view_register(void) {
  chart_view_s_register();
  x_axis_register();
  y_axis_register();
  line_series_register();
  line_series_colorful_register();
  bar_series_register();
  bar_series_minmax_register();
  tooltip_register();
  pie_slice_register();
}

const char* chart_view_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}