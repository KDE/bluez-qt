cmake_minimum_required(VERSION 2.8.12)

project(QBluez)

# ECM setup
find_package(ECM 0.0.10 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPackageConfigHelpers)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)

set(KF5_VERSION "4.96.0") # Update this

ecm_setup_version(${KF5_VERSION}
    VARIABLE_PREFIX QBLUEZ
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/qbluez_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5QBluezConfigVersion.cmake"
)

# Dependencies
set(REQUIRED_QT_VERSION "5.2.0")

# Required Qt5 components to build this framework
find_package(Qt5 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Core DBus)

# Subdirectories
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5QBluez")

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5QBluezConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5QBluezConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF5QBluezConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF5QBluezConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KF5QBluezTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5QBluezTargets.cmake
        NAMESPACE KF5::)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
