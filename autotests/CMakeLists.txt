add_subdirectory(fakebluez)

include(ECMMarkAsTest)

set(bluezqt_autotests_SRCS
    autotests.cpp
)

qt5_add_dbus_interface(bluezqt_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml dbusproperties_tst)
qt5_add_dbus_interface(bluezqt_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Adapter1.xml bluezadapter1_tst)
qt5_add_dbus_interface(bluezqt_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Device1.xml bluezdevice1_tst)

macro(bluezqt_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp ${bluezqt_autotests_SRCS})
        target_link_libraries(${_testname} Qt5::Test KF5BluezQt)
        add_test(bluezqt-${_testname} ${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

bluezqt_tests(
    managertest
    agentmanagertest
    obexmanagertest
    adaptertest
    devicetest
    jobstest
)

if(Qt5Qml_FOUND AND Qt5QuickTest_FOUND)
    add_executable(qmltests qmltests.cpp ${bluezqt_autotests_SRCS})
    target_link_libraries(qmltests Qt5::Test Qt5::Qml Qt5::QuickTest KF5BluezQt)
    add_test(bluezqt-qmltests qmltests)
    ecm_mark_as_test(qmltests)
endif()
