ecm_add_qml_module(bluezqtextensionplugin URI "org.kde.bluezqt" CLASSNAME BluezQtExtensionPlugin VERSION 1.0)

target_sources(bluezqtextensionplugin PRIVATE
    declarativemanager.cpp
    declarativeadapter.cpp
    declarativebattery.cpp
    declarativedevice.cpp
    declarativeinput.cpp
    declarativemediaplayer.cpp
    declarativedevicesmodel.cpp
    bluezqtextensionplugin.cpp
    bluezqtextensionplugin2.qrc
)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(bluezqtextensionplugin PRIVATE -DQT_PLUGIN -DQT_STATICPLUGIN=1)
    set_target_properties(bluezqtextensionplugin PROPERTIES AUTOMOC_MOC_OPTIONS -Muri=org.kde.bluezqt)
endif()

ecm_target_qml_sources(bluezqtextensionplugin SOURCES DevicesModel.qml)

target_link_libraries(bluezqtextensionplugin
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::DBus
    KF5::BluezQt
)

ecm_finalize_qml_module(bluezqtextensionplugin DESTINATION ${KDE_INSTALL_QMLDIR})
