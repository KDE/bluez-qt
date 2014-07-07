find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

qt5_add_dbus_interface(qbluez_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.freedesktop.DBus.Properties.xml dbusproperties)
qt5_add_dbus_interface(qbluez_autotests_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/org.bluez.Adapter1.xml bluezadapter1)

macro(qbluez_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname}autotest ${_testname}test.cpp ${qbluez_autotests_SRCS})
        target_link_libraries(${_testname}autotest Qt5::Test KF5QBluez)
        add_test(qbluez-${_testname} ${_testname}autotest)
        ecm_mark_as_test(${_testname}autotest)
    endforeach(_testname)
endmacro()

qbluez_tests(
    adapter
)
