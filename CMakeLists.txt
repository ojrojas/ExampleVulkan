cmake_minimum_required (VERSION 3.12)
include(.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("ThirdGame")

add_subdirectory("Content")
add_subdirectory("EngineVulkan")
add_subdirectory("Game")