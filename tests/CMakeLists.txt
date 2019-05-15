include(ECMMarkAsTest)

macro(bluezqt_executable_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::DBus Qt5::Network Qt5::Test KF5BluezQt)
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
