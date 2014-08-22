add_subdirectory(fakebluez)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

set(qbluez_autotests_SRCS
    autotests.cpp
)

qt5_add_dbus_interface(qbluez_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml dbusproperties_tst)
qt5_add_dbus_interface(qbluez_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Adapter1.xml bluezadapter1_tst)
qt5_add_dbus_interface(qbluez_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Device1.xml bluezdevice1_tst)

macro(qbluez_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp ${qbluez_autotests_SRCS})
        target_link_libraries(${_testname} Qt5::Test KF5QBluez)
        add_test(qbluez-${_testname} ${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

qbluez_tests(
    managertest
    agentmanagertest
    adaptertest
    devicetest
    jobstest
)
