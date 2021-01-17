QT += \
    widgets core gui \
    3dcore 3drender 3dextras

SOURCES += \
    classes/cubeentity.cpp \
    classes/cubeposcontroller.cpp \
    classes/gamecontrol.cpp \
    classes/selectorentity.cpp \
    main.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    classes/mainwindow.ui

HEADERS += \
    classes/cubeentity.h \
    classes/cubeposcontroller.h \
    classes/gamecontrol.h \
    classes/mainwindow.h \
    classes/selectorentity.h

RC_FILE += icon_rc.rc
