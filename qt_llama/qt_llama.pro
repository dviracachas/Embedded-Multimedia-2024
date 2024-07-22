QT += core gui multimedia

SOURCES += main.cpp

#INCLUDEPATH += /usr/include/qt5 /usr/include/qt5/QtMultimedia
INCLUDEPATH += /opt/Qt/6.6.1/include/ /opt/Qt/6.6.1/include/QtMultimedia

QMAKE_BUILD_DIR = $$PWD/build
QMAKE_CLEAN = $$QMAKE_BUILD_DIR

DESTDIR=bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files
