TEMPLATE = app
TARGET = test

SOURCES += main.cpp
RESOURCES += resources.qrc

QT += quick


development_team.name = DEVELOPMENT_TEAM
development_team.value = $$APPLE_DEVELOPMENT_TEAM # comes from cmdline
QMAKE_MAC_XCODE_SETTINGS += development_team
