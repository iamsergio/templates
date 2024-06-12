TEMPLATE = app
TARGET = test

SOURCES += main.cpp
RESOURCES += resources.qrc

QT += quick

# Not working with Qt, something about QtPDF
# QMAKE_APPLE_SIMULATOR_ARCHS=arm64
