SOURCES += main.cpp \
    public/toolButton.cpp \
    public/titleBar.cpp \
    public/statManager.cpp \
    public/pushButton.cpp \
    public/parentWindow.cpp \
    public/label.cpp \
    public/inputKeyboard.cpp \
    public/digitalKey.cpp \
    public/buttoneventfilter.cpp \
    configure/machineconfig.cpp \
    database/profile.cpp \
    gui/mainwindow.cpp \
    gui/powermonitordlg.cpp \
    commu/comthread.cpp \
    commu/cominfo.cpp \
    commu/commanager.cpp \
    gui/environmentmonitordlg.cpp \
    gui/logquerydlg.cpp \
    gui/parameterconfigdlg.cpp \
    gui/paraminputdialog.cpp \
    gui/dataquerydlg.cpp
HEADERS += public/toolButton.h \
    public/titleBar.h \
    public/statManager.h \
    public/pushButton.h \
    public/parentWindow.h \
    public/label.h \
    public/inputKeyboard.h \
    public/digitalKey.h \
    public/buttoneventfilter.h \
    configure/version.h \
    configure/machineconfig.h \
    configure/configure.h \
    database/profile.h \
    gui/mainwindow.h \
    gui/powermonitordlg.h \
    commu/comthread.h \
    commu/cominfo.h \
    commu/commanager.h \
    gui/environmentmonitordlg.h \
    gui/logquerydlg.h \
    gui/parameterconfigdlg.h \
    gui/paraminputdialog.h \
    gui/dataquerydlg.h
UI_DIR = ./ui
MOC_DIR = ./moc
OBJECTS_DIR = ./object
RCC_DIR = ./rcc
TRANSLATIONS += powerMon_zh.ts \
    powerMon_en.ts
CODECFORTR = utf8
RESOURCES += rcc.qrc
QT += sql
