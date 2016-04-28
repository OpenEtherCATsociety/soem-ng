
add_executable(ec_test
  ec_test.cpp
  test_frame.cpp
  test_alloc.cpp
  )
add_test(ec_test ec_test)

# GTest
if(GTEST_ROOT)
	message (STATUS "Building with Google C++ Testing Framework at ${GTEST_ROOT}")
	add_subdirectory(${GTEST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest)
	include_directories(${GTEST_ROOT}/include ${OSAL_INCLUDES})
	target_link_libraries(ec_test gtest gtest_main soem)
endif()

