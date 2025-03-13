QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customlineedit.cpp \
    gamepadworker.cpp \
    main.cpp \
    joysticktest.cpp

HEADERS += \
    customlineedit.h \
    gamepadworker.h \
    joysticktest.h

FORMS += \
    joysticktest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/SDL3/lib/x64/ -lSDL3

INCLUDEPATH += $$PWD/SDL3/include
DEPENDPATH += $$PWD/SDL3/include

CONFIG(release, debug|release) {
    QMAKE_POST_LINK += $$quote(C:/Qt/6.8.2/msvc2022_64/bin/windeployqt6.exe $$OUT_PWD/release/$${TARGET}.exe)
} else {
    QMAKE_POST_LINK += $$quote(C:/Qt/6.8.2/msvc2022_64/bin/windeployqt6.exe $$OUT_PWD/debug/$${TARGET}.exe)
}
