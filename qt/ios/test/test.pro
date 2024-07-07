TEMPLATE = app
TARGET = test

SOURCES += main.cpp
RESOURCES += resources.qrc

QT += quick

# Arm64 simulator not supported by Qt yet (as of 6.7)
# QMAKE_APPLE_SIMULATOR_ARCHS=arm64

development_team.name = DEVELOPMENT_TEAM
development_team.value = $$MY_DEV_TEAM # Pass via cmd line
QMAKE_MAC_XCODE_SETTINGS += development_team

QMAKE_TARGET_BUNDLE_PREFIX = com.sergiomartins.
QMAKE_BUNDLE = test
