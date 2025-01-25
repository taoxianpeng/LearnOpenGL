# CMakeList.txt: LearnOpengl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.14)

# 指定项目名称
project(LearnOpengl)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
	thirdParty/Common/glad/include
	thirdParty/Common/glm
	thirdParty/Common/imgui
	thirdParty/Common/imgui/backends	
	thirdParty/Common/nlohmann/single_include
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message(STATUS "当前平台: Linux, set include")
	include_directories(
		thirdParty/Linux/glfw/include
		thirdParty/Linux/glew/include/GL
		thirdParty/Linux/assimp/include
		thirdParty/Linux/spdlog/include
		wrapper
	)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	include_directories(
		thirdParty/Win64/glfw/include
		thirdParty/Win64/glew/include
		thirdParty/Win64/assimp/include
		thirdParty/Win64/spdlog/include
		wrapper
	)
		
endif()

file(GLOB IMGUI_FILES thirdParty/Common/imgui/*.cpp thirdParty/Common/imgui/backends/*.cpp)
		
add_subdirectory(wrapper)

# 将源代码添加到此项目的可执行文件。
file(GLOB SRC_FILES src/*.cpp src/*.c src/*.h)
add_executable (${PROJECT_NAME} ${SRC_FILES} ${IMGUI_FILES})
		
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_directories(${PROJECT_NAME} PRIVATE 
		thirdParty/Linux/glfw/src 
		thirdParty/Linux/glew/lib
		thirdParty/Linux/assimp/bin
	)
		
	set(glfw glfw3)
	set(glew GLEW)
	set(assimp assimp)
	set(wrapper wrapper)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_directories(${PROJECT_NAME} PRIVATE 
		thirdParty/Win64/glfw/lib 
		thirdParty/Win64/glew/lib
		thirdParty/Win64/assimp/lib
	)

	set(glfw glfw3)
	set(glew glfw32)
	set(assimp assimp-vc143-mt)
	set(wrapper wrapper)
endif()

target_link_libraries(${PROJECT_NAME} ${glfw} ${glew} ${assimp} ${wrapper} GL)

file(COPY ${CMAKE_SOURCE_DIR}/config.json DESTINATION ${CMAKE_BINARY_DIR})
# TODO: 如有需要，请添加测试并安装目标。
