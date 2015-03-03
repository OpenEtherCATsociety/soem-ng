add_executable (slaveinfo slaveinfo.c)
target_link_libraries (slaveinfo LINK_PUBLIC soem)

install (TARGETS slaveinfo DESTINATION bin)
