set (_link_LIBRARIES
  Threads::Threads
)

set (_bwe_sources
  Executor.cxx
  Task.cxx
  ActionQueue.cxx
  Dependency.cxx
)

set (_bwe_headers
  Executor.hxx
  Task.hxx
  Action.hxx
  ActionQueue.hxx
  Dependency.hxx
)

add_library(bwe ${_bwe_sources})
target_link_libraries(bwe ${_link_LIBRARIES})
target_include_directories(bwe PUBLIC
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                          $<INSTALL_INTERFACE:include>)


install(TARGETS bwe EXPORT bweCfg
        LIBRARY DESTINATION lib)
install(FILES ${_bwe_headers} DESTINATION include/bwe)
install(EXPORT bweCfg DESTINATION lib/cmake/bwe
        FILE bweConfig.cmake)

add_subdirectory(Test)
