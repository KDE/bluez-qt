set(qbluez_SRCS
    debug_p.cpp
    manager.cpp
    manager_p.cpp
    adapter.cpp
    adapter_p.cpp
    device.cpp
    device_p.cpp
    job.cpp
    initmanagerjob.cpp
    initobexmanagerjob.cpp
    initadaptersjob.cpp
    loaddevicejob.cpp
    utils.cpp
    utils_p.cpp
    agent.cpp
    agentadaptor.cpp
    pendingcall.cpp
    request.cpp
    obexmanager.cpp
    obexmanager_p.cpp
    obexagent.cpp
    obexagentadaptor.cpp
    obextransfer.cpp
    obexsession.cpp
)

set(dbusobjectmanager_xml ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.ObjectManager.xml)
set_source_files_properties(${dbusobjectmanager_xml} PROPERTIES INCLUDE "qbluez_dbustypes.h")
qt5_add_dbus_interface(qbluez_SRCS ${dbusobjectmanager_xml} dbusobjectmanager)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml dbusproperties)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Adapter1.xml bluezadapter1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.AgentManager1.xml bluezagentmanager1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Device1.xml bluezdevice1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.AgentManager1.xml obexagentmanager1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Client1.xml obexclient1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Transfer1.xml obextransfer1)
qt5_add_dbus_interface(qbluez_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.obex.Session1.xml obexsession1)

add_library(KF5QBluez ${qbluez_SRCS})
generate_export_header(KF5QBluez BASE_NAME QBluez)
add_library(KF5::QBluez ALIAS KF5QBluez)

target_include_directories(KF5QBluez INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/QBluez>")

target_link_libraries(KF5QBluez
    PUBLIC
        Qt5::Core
        Qt5::DBus
)

set_target_properties(KF5QBluez
    PROPERTIES VERSION ${QBLUEZ_VERSION_STRING}
    SOVERSION ${QBLUEZ_SOVERSION}
    EXPORT_NAME "QBluez"
)

ecm_generate_headers(QBluez_HEADERS
    HEADER_NAMES
        Manager
        Adapter
        Device
        Job
        InitManagerJob
        InitObexManagerJob
        InitAdaptersJob
        LoadDeviceJob
        Utils
        Agent
        PendingCall
        Request
        ObexManager
        ObexAgent
        ObexTransfer
        ObexSession

    REQUIRED_HEADERS QBluez_HEADERS
)

# Install files
install(TARGETS KF5QBluez
        EXPORT KF5QBluezTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/qbluez_export.h
            ${QBluez_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/QBluez
        COMPONENT Devel)

include(ECMGeneratePriFile)

ecm_generate_pri_file(
    BASE_NAME QBluez
    LIB_NAME KF5QBluez
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
