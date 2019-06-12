cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.60.0") # handled by release scripts
project(BluezQt VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.59.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)
include(CMakePackageConfigHelpers)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMQtDeclareLoggingCategory)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT
    VARIABLE_PREFIX BLUEZQT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/bluezqt_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5BluezQtConfigVersion.cmake"
    SOVERSION 6
)

# Dependencies
set(REQUIRED_QT_VERSION 5.10.0)

# Required Qt5 components to build this framework
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core DBus Network)

# Optional Qt5 components to build this framework
find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Test PROPERTIES DESCRIPTION "Autotests for BluezQt" TYPE OPTIONAL)
find_package(Qt5Qml ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Qml PROPERTIES DESCRIPTION "QML support for BluezQt" TYPE OPTIONAL)
find_package(Qt5QuickTest ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5QuickTest PROPERTIES DESCRIPTION "QML autotests for BluezQt" TYPE OPTIONAL)
#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_FOREACH)
# Subdirectories
add_subdirectory(src)
add_subdirectory(tools/bluezapi2qt)

if (BUILD_TESTING)
    add_subdirectory(tests)
    if(Qt5Test_FOUND)
        add_subdirectory(autotests)
    endif()
endif()

if(Qt5Qml_FOUND)
    add_subdirectory(src/imports)
endif()

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5BluezQt")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5BluezQt_QCH
        FILE KF5BluezQtQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5BluezQtQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5BluezQtConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5BluezQtConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5BluezQtConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5BluezQtConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel)

install(EXPORT KF5BluezQtTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5BluezQtTargets.cmake
    NAMESPACE KF5::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bluezqt_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
    COMPONENT Devel)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(INSTALL_UDEV_RULE ON CACHE BOOL "Whether to install udev rule")
    set(UDEV_RULES_INSTALL_DIR /lib/udev/rules.d CACHE PATH "The location of udev rules")

    if(${INSTALL_UDEV_RULE})
        install(FILES 61-kde-bluetooth-rfkill.rules DESTINATION ${UDEV_RULES_INSTALL_DIR})
    endif()
endif()

install(FILES bluez.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
