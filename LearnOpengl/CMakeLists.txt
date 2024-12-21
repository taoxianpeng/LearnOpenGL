# CMakeList.txt: LearnOpengl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
# 设置可执行文件的名字
set(EXECUTABLE_NAME "LearnOpengl")


if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message(STATUS "当前平台: Linux, set include")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	include_directories(
		thirdParty/Win64/glfw/include
		thirdParty/Win64/glad/include
		thirdParty/Win64/glew/include
		thirdParty/Win64/assimp/include
		thirdParty/Win64/glm
		thirdParty/Win64/imgui
		thirdParty/Win64/imgui/backends
		thirdParty/Win64/spdlog/include
		wrapper
		)

	file(GLOB IMGUI_FILES thirdParty/Win64/imgui/*.cpp thirdParty/Win64/imgui/backends/*.cpp)
endif()

add_subdirectory(wrapper)

# 将源代码添加到此项目的可执行文件。
file(GLOB SRC_FILES src/*.cpp src/*.c src/*.h)
add_executable (${EXECUTABLE_NAME} ${SRC_FILES} ${IMGUI_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message(STATUS "当前平台: Linux, link lib")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_directories(${EXECUTABLE_NAME} PRIVATE 
		thirdParty/Win64/glfw/lib 
		thirdParty/Win64/glew/lib
		thirdParty/Win64/assimp/lib
	)
endif()

target_link_libraries(${EXECUTABLE_NAME} glfw3 glew32 assimp-vc143-mt wrapper)


# TODO: 如有需要，请添加测试并安装目标。
