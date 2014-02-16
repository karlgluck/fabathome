# -------------------------------------------------
# Project created by QtCreator 2010-02-16T16:57:33
# -------------------------------------------------
DEFINES += CONFIRM_ENABLED
QT += opengl \
    script \
    xml
TARGET = FabStudio
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    toolbar.cpp \
    openglwidget.cpp \
    openglcamera.cpp \
    loadobjectspanel.cpp \
    positionobjectspanel.cpp \
    scanforobjectfilesthread.cpp \
    printableobject.cpp \
    loadobjectfilethread.cpp \
    openglrenderprimitives.cpp \
    printpanel.cpp \
    materialspanel.cpp \
    loadtoolscriptsthread.cpp \
    printthread.cpp \
    printthreadpatherprogresscallbacksignals.cpp \
    printthreadslicerprogresscallbacksignals.cpp \
    modifytoolandprintersettingsdialog.cpp \
    centerofgravity.cpp \
    generatesupportmaterial.cpp \
    renderablepath.cpp \
    arrangebuildtray.cpp \
    firstexecutiondialog.cpp \
    aboutdialog.cpp
HEADERS += mainwindow.h \
    toolbar.h \
    openglwidget.h \
    openglcamera.h \
    loadobjectspanel.h \
    positionobjectspanel.h \
    scanforobjectfilesthread.h \
    printableobject.h \
    loadobjectfilethread.h \
    printableobjecthandle.h \
    openglrenderprimitives.h \
    printpanel.h \
    materialspanel.h \
    loadtoolscriptsthread.h \
    printthread.h \
    printthreadpatherprogresscallbacksignals.h \
    printthreadslicerprogresscallbacksignals.h \
    modifytoolandprintersettingsdialog.h \
    centerofgravity.h \
    generatesupportmaterial.h \
    renderablepath.h \
    arrangebuildtray.h \
    firstexecutiondialog.h \
    aboutdialog.h
FORMS += mainwindow.ui \
    toolbar.ui \
    loadobjectspanel.ui \
    positionobjectspanel.ui \
    printpanel.ui \
    materialspanel.ui \
    modifytoolandprintersettingsdialog.ui \
    firstexecutiondialog.ui \
    aboutdialog.ui
INCLUDEPATH += ../../../
RESOURCES += resources.qrc
include("common.pro")
include("shared-amf.pro")
include("shared.pro")
include("shared-math.pro")
include("shared-pather.pro")
include("shared-slicer.pro")
include("shared-stl.pro")
include("shared-toolscript.pro")
include("shared-utils.pro")
