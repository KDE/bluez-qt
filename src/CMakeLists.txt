set(bluezqt_SRCS
    debug.cpp
    manager.cpp
    manager_p.cpp
    adapter.cpp
    adapter_p.cpp
    device.cpp
    device_p.cpp
    input.cpp
    mediaplayer.cpp
    mediaplayer_p.cpp
    mediaplayertrack.cpp
    devicesmodel.cpp
    job.cpp
    initmanagerjob.cpp
    initobexmanagerjob.cpp
    utils.cpp
    agent.cpp
    agentadaptor.cpp
    profile.cpp
    profileadaptor.cpp
    pendingcall.cpp
    request.cpp
    rfkill.cpp
    obexmanager.cpp
    obexmanager_p.cpp
    obexagent.cpp
    obexagentadaptor.cpp
    obextransfer.cpp
    obexsession.cpp
    obexobjectpush.cpp
    obexfiletransfer.cpp
    obexfiletransferentry.cpp
)

set(dbusobjectmanager_xml ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.ObjectManager.xml)
set_source_files_properties(${dbusobjectmanager_xml} PROPERTIES INCLUDE "bluezqt_dbustypes.h")
qt5_add_dbus_interface(bluezqt_SRCS ${dbusobjectmanager_xml} dbusobjectmanager)

set(obexfiletransfer1_xml ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.FileTransfer1.xml)
set_source_files_properties(${obexfiletransfer1_xml} PROPERTIES INCLUDE "bluezqt_dbustypes.h")
qt5_add_dbus_interface(bluezqt_SRCS ${obexfiletransfer1_xml} obexfiletransfer1)

qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml dbusproperties)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Adapter1.xml bluezadapter1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.AgentManager1.xml bluezagentmanager1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.ProfileManager1.xml bluezprofilemanager1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Device1.xml bluezdevice1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.MediaPlayer1.xml bluezmediaplayer1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.AgentManager1.xml obexagentmanager1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Client1.xml obexclient1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Transfer1.xml obextransfer1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Session1.xml obexsession1)
qt5_add_dbus_interface(bluezqt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.ObjectPush1.xml obexobjectpush1)

add_library(KF5BluezQt ${bluezqt_SRCS})
generate_export_header(KF5BluezQt BASE_NAME BluezQt)
add_library(KF5::BluezQt ALIAS KF5BluezQt)

target_include_directories(KF5BluezQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/BluezQt>")

target_link_libraries(KF5BluezQt
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::DBus
        Qt5::Network
)

set_target_properties(KF5BluezQt
    PROPERTIES VERSION ${BLUEZQT_VERSION_STRING}
    SOVERSION ${BLUEZQT_SOVERSION}
    EXPORT_NAME "BluezQt"
)

ecm_generate_headers(BluezQt_CamelCase_HEADERS
    HEADER_NAMES
        Types
        Manager
        Adapter
        Device
        Input
        MediaPlayer
        MediaPlayerTrack
        DevicesModel
        Job
        InitManagerJob
        InitObexManagerJob
        Services
        Agent
        Profile
        PendingCall
        Request
        ObexManager
        ObexAgent
        ObexTransfer
        ObexSession
        ObexObjectPush
        ObexFileTransfer
        ObexFileTransferEntry

    REQUIRED_HEADERS BluezQt_HEADERS
    PREFIX BluezQt
)

# Install files
install(TARGETS KF5BluezQt
        EXPORT KF5BluezQtTargets
        ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${BluezQt_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/BluezQt/BluezQt
    COMPONENT Devel)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bluezqt_export.h
    ${BluezQt_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/BluezQt/bluezqt
    COMPONENT Devel)

include(ECMGeneratePriFile)

ecm_generate_pri_file(
    BASE_NAME BluezQt
    LIB_NAME KF5BluezQt
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
