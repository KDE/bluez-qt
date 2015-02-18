set(bluezqtextensionplugin_SRCS
    declarativemanager.cpp
    declarativedevicesmodel.cpp
    bluezqtextensionplugin.cpp
    )

add_library(bluezqtextensionplugin SHARED ${bluezqtextensionplugin_SRCS})

target_link_libraries(bluezqtextensionplugin
    Qt5::Core
    Qt5::Qml
    KF5::BluezQt
)

install(TARGETS bluezqtextensionplugin DESTINATION ${QML_INSTALL_DIR}/org/bluezqt)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/bluezqt)
