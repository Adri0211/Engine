cmake_minimum_required(VERSION 3.20)
project(Engine CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/libs/glfw-3.4")
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIBRARY "${GLFW_ROOT}/build/src/Debug/glfw3.lib")

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/glad/include")

add_executable(Engine 
	src/main.cpp
	"${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c"
)
target_include_directories(Engine PRIVATE "${GLFW_INCLUDE_DIR}" "${GLAD_INCLUDE_DIR}")
target_link_libraries(Engine PRIVATE
	"${GLFW_LIBRARY}"
	opengl32
	gdi32
	user32
	shell32
)