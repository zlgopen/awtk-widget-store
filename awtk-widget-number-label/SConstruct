import os
import app_helper as app

ARGUMENTS['LCD'] = '800_480'
helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/number_label.def').set_libs(['number_label']).call(DefaultEnvironment)

CustomWidgetSConscriptFiles = []
SConscriptFiles = CustomWidgetSConscriptFiles + ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
SConscript(SConscriptFiles)
