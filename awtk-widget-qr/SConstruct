import os
import scripts.app_helper as app

def initArgument(name, defVal):
    val = ARGUMENTS.get(name, '')
    if len(val) == 0:
        ARGUMENTS[name] = defVal

helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/qr.def').set_libs(['qr']).call(DefaultEnvironment)

SConscriptFiles = ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
SConscript(SConscriptFiles)
