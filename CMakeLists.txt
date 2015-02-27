cmake_minimum_required(VERSION 2.8.12)

project(BluezQt)

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

set(KF5_VERSION "5.1.0") # handled by release scripts

ecm_setup_version(${KF5_VERSION}
    VARIABLE_PREFIX BLUEZQT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/bluezqt_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5BluezQtConfigVersion.cmake"
)

# Dependencies
set(REQUIRED_QT_VERSION "5.2.0")

# Required Qt5 components to build this framework
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core DBus Network)

# Optional Qt5 components to build this framework
find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Test PROPERTIES DESCRIPTION "Autotests for BluezQt" TYPE OPTIONAL)
find_package(Qt5Qml ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Qml PROPERTIES DESCRIPTION "QML support for BluezQt" TYPE OPTIONAL)
find_package(Qt5QuickTest ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5QuickTest PROPERTIES DESCRIPTION "QML autotests for BluezQt" TYPE OPTIONAL)

# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)

if(Qt5Test_FOUND)
    add_subdirectory(autotests)
endif()

if(Qt5Qml_FOUND)
    add_subdirectory(src/imports)
endif()

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5BluezQt")

ecm_configure_package_config_file(
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

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
