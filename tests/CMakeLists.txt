include(ECMMarkAsTest)

macro(bluezqt_executable_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt${QT_MAJOR_VERSION}::DBus Qt${QT_MAJOR_VERSION}::Network Qt${QT_MAJOR_VERSION}::Test KF5BluezQt)
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

bluezqt_executable_tests(
    adaptersreceiver
    devicereceiver
    chatprofile
    leserver
    mediaendpointconnector
)
