set(bluezqtextensionplugin_SRCS
    declarativemanager.cpp
    declarativeadapter.cpp
    declarativebattery.cpp
    declarativedevice.cpp
    declarativeinput.cpp
    declarativemediaplayer.cpp
    declarativedevicesmodel.cpp
    bluezqtextensionplugin.cpp
    )

# Needed to run QML autotests
configure_file(qmldir org/kde/bluezqt/qmldir COPYONLY)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY org/kde/bluezqt)

add_library(bluezqtextensionplugin SHARED ${bluezqtextensionplugin_SRCS})

target_link_libraries(bluezqtextensionplugin
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::DBus
    KF5::BluezQt
)

install(TARGETS bluezqtextensionplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/bluezqt)
install(FILES DevicesModel.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/bluezqt)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/bluezqt)
