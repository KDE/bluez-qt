find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

macro(qbluez_executable_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::Test KF5QBluez)
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

qbluez_executable_tests(
    adaptersreceiver
    devicereceiver
)
