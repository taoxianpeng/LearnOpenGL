# CMakeList.txt: LearnOpengl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
# 设置可执行文件的名字
set(EXECUTABLE_NAME "LearnOpengl")

include_directories(
	thirdParty/glfw/include
	thirdParty/glad/include
	thirdParty/glew/include
	thirdParty/assimp/include
	thirdParty/glm
	thirdParty/imgui
	thirdParty/imgui/backends
	thirdParty/spdlog/include

)

# 将源代码添加到此项目的可执行文件。
file(GLOB SRC_FILES src/*.cpp src/*.c src/*.h)
file(GLOB IMGUI_FILES thirdParty/imgui/*.cpp thirdParty/imgui/backends/*.cpp)
add_executable (${EXECUTABLE_NAME} ${SRC_FILES} ${IMGUI_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_directories(${EXECUTABLE_NAME} PRIVATE 
	thirdParty/glfw/lib 
	thirdParty/glew/lib
	thirdParty/assimp/lib
)

target_link_libraries(${EXECUTABLE_NAME} glfw3 glew32 assimp-vc143-mt)


# TODO: 如有需要，请添加测试并安装目标。
