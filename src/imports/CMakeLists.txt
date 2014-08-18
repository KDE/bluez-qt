set(qbluezextensionplugin_SRCS
    declarativemanager.cpp
    qbluezextensionplugin.cpp
    )

add_library(qbluezextensionplugin SHARED ${qbluezextensionplugin_SRCS})

target_link_libraries(qbluezextensionplugin
    Qt5::Core
    Qt5::Qml
    KF5::QBluez
)

install(TARGETS qbluezextensionplugin DESTINATION ${QML_INSTALL_DIR}/org/qbluez)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/qbluez)
