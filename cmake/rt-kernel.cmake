
set(OSAL_SOURCES
  ${SOEM_SOURCE_DIR}/src/osal/rt-kernel/osal.c
  ${SOEM_SOURCE_DIR}/src/osal/rt-kernel/stubs.c
  )
set(OSAL_INCLUDES
  osal/rt-kernel
  ${RTK}/include/kern
  )
