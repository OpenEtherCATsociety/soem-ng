
find_package(PCAP)

set(OSAL_SOURCES
  ${SOEM_SOURCE_DIR}/src/osal/windows/osal.c
  ${SOEM_SOURCE_DIR}/src/osal/windows/pcap.c
  )
set(OSAL_INCLUDES
  ${SOEM_SOURCE_DIR}/src/osal/windows
  ${PCAP_INCLUDE_DIR}
  )
set(OSAL_LIBS
  ${PCAP_LIBRARY}
  )

# Common options. /wd4200 should disable irrelevant warning about
# zero-sized array in frame.h but it doesn't seem to work.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4200 /D_CRT_SECURE_NO_WARNINGS")

# GTest wants /MT
 set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
