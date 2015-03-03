
find_package(PCAP)

set(OSAL_SOURCES
  ${SOEM_SOURCE_DIR}/src/osal/cygwin/osal.c
  ${SOEM_SOURCE_DIR}/src/osal/windows/pcap.c
  )
set(OSAL_INCLUDES
  ${SOEM_SOURCE_DIR}/src/osal/cygwin
  ${PCAP_INCLUDE_DIR}
  )
set(OSAL_LIBS
  ${PCAP_LIBRARY}
  )

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -ffunction-sections")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -ffunction-sections")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")