QT += widgets
TEMPLATE = app
TARGET = universalapr

SOURCES += main.cpp\
           mainwindow.cpp \
           dialog.cpp \
           zpicturebox.cpp \
           zlayout.cpp \
           universalapr.cpp \

HEADERS += mainwindow.h \
           dialog.h \
           zpicturebox.h \
           zlayout.h \
           universalapr.h \
           asmopencv.h \
           zclear.h

FORMS += mainwindow.ui \
         dialog.ui

RESOURCES += $$PWD/resources/resources.qrc

include($$PWD/contrib/contrib.pri)
