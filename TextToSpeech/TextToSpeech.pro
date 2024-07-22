QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        speechdispatcher.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    speechdispatcher.h
    
QMAKE_BUILD_DIR = $$PWD/build
QMAKE_CLEAN = $$QMAKE_BUILD_DIR

DESTDIR=bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files
