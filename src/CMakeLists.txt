
include(GenerateExportHeader)

message (STATUS "Building for ${CMAKE_SYSTEM_NAME}")

# NOTE: add headers to make them show up in an IDE
add_library (soem
  ${SOEM_SOURCE_DIR}/include/ec_api.h
  net.h
  net.c
  slave.h
  slave.c
  config.h
  config.c
  frame.h
  frame.c
  eeprom.h
  eeprom.c
  sii.h
  sii.c
  nic.h
  nic.c
  alloc.h
  alloc.c
  osal.h
  ${OSAL_SOURCES}
  )
generate_export_header(soem BASE_NAME ec)

include_directories(${OSAL_INCLUDES})

target_link_libraries(soem ${OSAL_LIBS})

install (TARGETS soem DESTINATION bin)
install (FILES
  ${SOEM_SOURCE_DIR}/include/ec_api.h
  ${SOEM_BINARY_DIR}/src/ec_export.h
  DESTINATION include)
