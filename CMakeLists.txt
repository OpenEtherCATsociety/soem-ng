# CMakeLists files in this project can
# refer to the root source directory of the project as ${SOEM_SOURCE_DIR} and
# to the root binary directory of the project as ${SOEM_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.4)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project (SOEM)
enable_testing()

set (SOEM_VERSION_MAJOR 0)
set (SOEM_VERSION_MINOR 1)
set (SOEM_VERSION_PATCH 0)

# Options
include(CMakeDependentOption)

option (BUILD_SHARED_LIBS "Build shared library" OFF)
option (WITH_MALLOC "Use malloc, otherwise use static alloc" OFF)

set(LOG_STATE_VALUES "ON;OFF")
set(LOG_LEVEL_VALUES "DEBUG;INFO;WARNING;ERROR")

set(LOG_LEVEL ERROR CACHE STRING "default log level")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS ${LOG_LEVEL_VALUES})

set(EC_NET_DEBUG ON CACHE STRING "ec_net debug")
set_property(CACHE EC_NET_DEBUG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(EC_EEPROM_DEBUG ON CACHE STRING "ec_eeprom debug")
set_property(CACHE EC_EEPROM_DEBUG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(EC_COE_DEBUG ON CACHE STRING "ec_coe debug")
set_property(CACHE EC_COE_DEBUG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(EC_NIC_DEBUG ON CACHE STRING "ec_nic debug")
set_property(CACHE EC_NIC_DEBUG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(EC_ALLOC_DEBUG ON CACHE STRING "ec_alloc debug")
set_property(CACHE EC_ALLOC_DEBUG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(MAX_NETS "2"
  CACHE STRING "max number of statically allocated nets")
set(MAX_SLAVES "(32 * MAX_NETS)"
  CACHE STRING "max number of statically allocated slaves")
set(MAX_STRINGS "(1 * MAX_SLAVES)"
  CACHE STRING "max number of statically allocated nets")
set(MAX_STRING_LENGTH "40"
  CACHE STRING "max length of statically allocated strings")

# Generate version numbers
configure_file (
  "${SOEM_SOURCE_DIR}/version.h.in"
  "${SOEM_SOURCE_DIR}/src/version.h"
  )
# Generate config options
configure_file (
  "${SOEM_SOURCE_DIR}/options.h.in"
  "${SOEM_SOURCE_DIR}/src/options.h"
  )

# Include paths
include_directories("${SOEM_BINARY_DIR}/src")
include_directories("${SOEM_SOURCE_DIR}/include")
include_directories("${SOEM_SOURCE_DIR}/src")

# Platform flags, OSAL sources
include(${CMAKE_SYSTEM_NAME})

# Source paths
add_subdirectory (src)
add_subdirectory (util)
add_subdirectory (test)

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${SOEM_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${SOEM_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${SOEM_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${SOEM_VERSION_MINOR}")
set (CPACK_PACKAGE_CONTACT info.ethercat@rt-labs.com)
include (CPack)
