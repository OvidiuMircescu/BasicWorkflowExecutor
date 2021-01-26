set (_link_LIBRARIES
)

set (_test_SOURCES
  maintest.cxx
)

add_executable(test_bwe ${_test_SOURCES})
target_link_libraries(test_bwe bwe)
install(TARGETS test_bwe RUNTIME DESTINATION bin)
