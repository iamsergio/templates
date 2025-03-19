QT_POLICY(SET QTP0001 NEW)

QT_POLICY(SET QTP0004 NEW)



set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_qml_module(${MY_MODULE_TARGET_NAME}
    URI MyTest
    QML_FILES
        MyRect.qml
        main.qml
    SOURCES
        Obj.cpp
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN
)
