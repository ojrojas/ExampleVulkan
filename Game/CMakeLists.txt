project(Game)

add_executable(Game "Main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Game PROPERTY CXX_STANDARD 20)
endif()

message(STATUS "Directory is in: EngineVulkan")

include_directories(${CMAKE_SOURCE_DIR})

target_include_directories(Game PRIVATE EngineVulkan)
target_link_directories(Game PRIVATE EngineVulkan)
target_link_libraries(Game PRIVATE EngineVulkan)