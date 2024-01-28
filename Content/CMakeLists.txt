project(Content)
message(STATUS "Route: " ${CMAKE_CURRENT_SOURCE_DIR})

find_program(GLSLCOMPILER glslangValidator)
message(STATUS ${GLSLCOMPILER} " Found")

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/Shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/Shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  message(STATUS ${FILE_NAME} " to compile")
  set(SPIRV "${PROJECT_SOURCE_DIR}/Shaders/${FILE_NAME}.spv")
  set(SPIRVCOMPILE "${CMAKE_BINARY_DIR}/Game/Shaders/${FILE_NAME}.spv")
  message(STATUS ${SPIRVCOMPILE} " to copy")

  add_custom_command(
    OUTPUT ${SPIRV}
    POST_BUILD
    COMMAND ${GLSLCOMPILER} -V ${GLSL} -o ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E copy ${SPIRV} ${SPIRVCOMPILE}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    RUN ALL
    DEPENDS ${SPIRV_BINARY_FILES}
)