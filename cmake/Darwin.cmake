
set(OSAL_SOURCES
  osal/darwin/osal.c
  osal/darwin/socket.c
  )
set(OSAL_INCLUDES
  osal/darwin
  )
set(OSAL_LIBS
  ""
  )

# set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -ffunction-sections")
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -ffunction-sections")
# set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")
