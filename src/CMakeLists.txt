set(qbluez_SRCS
    manager.cpp
)

add_library(KF5QBluez ${qbluez_SRCS})
generate_export_header(KF5QBluez BASE_NAME QBluez)
add_library(KF5::QBluez ALIAS KF5QBluez)

target_include_directories(KF5QBluez INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/QBluez>")

target_link_libraries(KF5QBluez
    PUBLIC
        Qt5::Core
#    PRIVATE
)

set_target_properties(KF5QBluez
    PROPERTIES VERSION ${QBLUEZ_VERSION_STRING}
    SOVERSION ${QBLUEZ_SOVERSION}
    EXPORT_NAME "QBluez"
)

ecm_generate_headers(QBluez_HEADERS
    HEADER_NAMES
        Manager

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

# Workaround for warning in ECMGeneratePriFile
cmake_policy(SET CMP0053 OLD)

include(ECMGeneratePriFile)

ecm_generate_pri_file(
    BASE_NAME QBluez
    LIB_NAME KF5QBluez
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
