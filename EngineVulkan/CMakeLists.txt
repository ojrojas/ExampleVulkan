project(EngineVulkan)
add_library(EngineVulkan STATIC 
	"Application.cpp"
	"stb_image.cpp" 
	"EngineVulkanTypes.hpp"
	"VulkanInstance.hpp"
	"VulkanInstance.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET EngineVulkan PROPERTY CXX_STANDARD 20)
endif()

if(WIN32)
	message(STATUS "configuring paths to windows")
	set(GLFW_DIR_INCLUDES "${GLFW_PATH_WIN}/include")
	set(GLFW_DIR_LIBS "${GLFW_PATH_WIN}/lib-vc2022")
	set(VULKAN_DIR_INCLUDES "${VULKAN_PATH_WIN}/include")
	set(VULKAN_DIR_LIBS "${VULKAN_PATH_WIN}/lib")
	set(STB_PATH_INCLUDES "${STB_PATH_WIN}/include")
	set(TINY_OBJ_INCLUDES "${TINY_OBJ_WIN}/include")
else()
	message(STATUS "configuring paths to unix based systems")
	set(GLFW_DIR_INCLUDES "${GLFW_PATH}/include")
	set(GLFW_DIR_LIBS "${GLFW_PATH}/lib")
	set(VULKAN_DIR_INCLUDES "${VULKAN_PATH}/include/vulkan")
	set(STB_PATH_INCLUDES "${STB_PATH}/include")
	set(VULKAN_DIR_LIBS "${VULKAN_PATH}/lib")
	set(TINY_OBJ_INCLUDES "${TINY_OBJ}/include")

endif()

message(STATUS "Including directories ${GLFW_DIR_INCLUDES} - ${VULKAN_DIR_INCLUDES}")
include_directories(
	"${GLFW_DIR_INCLUDES}"
	"${VULKAN_DIR_INCLUDES}"
	"${STB_PATH_INCLUDES}"
	"${TINY_OBJ_INCLUDES}"
)

message(STATUS "Including target directories:")
message(STATUS "${GLFW_DIR_INCLUDES}")
message(STATUS "${VULKAN_DIR_INCLUDES}")
message(STATUS "${GLFW_DIR_LIBS}")
message(STATUS "${VULKAN_DIR_LIBS}")
message(STATUS "${TINY_OBJ_INCLUDES}")
message(STATUS "${STB_PATH_INCLUDES}")

target_include_directories(EngineVulkan PUBLIC 
	"${GLFW_DIR_INCLUDES}"
	"${VULKAN_DIR_INCLUDES}"
	"${GLFW_DIR_LIBS}"
	"${VULKAN_DIR_LIBS}"
	"${STB_PATH_INCLUDES}"
	"${TINY_OBJ_INCLUDES}"
)

message(STATUS "Including libs directories:")
message(STATUS ${GLFW_DIR_LIBS})
message(STATUS ${VULKAN_DIR_LIBS})

target_link_directories(EngineVulkan PUBLIC 
	"${GLFW_DIR_LIBS}"
	"${VULKAN_DIR_LIBS}"
)

if(WIN32)
message(STATUS "Including libraries:")
message(STATUS "${EngineVulkan}, glfw3 vulkan-1")
	target_link_libraries(EngineVulkan glfw3 vulkan-1)
elseif(APPLE)
message(STATUS "Including libraries:")
message(STATUS "${EngineVulkan}, glfw.3 vulkan")
	target_link_libraries(EngineVulkan glfw.3 vulkan)
else(UNIX)
message("there is no implementation or configuration")
endif()