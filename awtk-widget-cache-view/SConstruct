import os
import app_helper as app

ARGUMENTS['LCD'] = '800_480'
helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/cache_view.def').set_libs(['cache_view']).call(DefaultEnvironment)

CustomWidgetSConscriptFiles = []
SConscriptFiles = CustomWidgetSConscriptFiles + ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
SConscript(SConscriptFiles)
