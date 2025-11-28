cmake_minimum_required(VERSION 3.31)
project(project)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ecs/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ecs/Components/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ecs/Systems*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/testing.cpp"
)
message(${SOURCES})
add_executable(project ${SOURCES} Thirdparty/src/gl.c)

add_subdirectory(Thirdparty/glfw)
include_directories(Thirdparty/includes)
include_directories(ecs)
include_directories(ecs/Components)
include_directories(ecs/Systems)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(project glfw opengl32)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Thirdparty/libs/libassimp.dll.a)
add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/Thirdparty/libs/libassimp-6.dll
        $<TARGET_FILE_DIR:project>)

add_custom_command(TARGET project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:project>/resources
)

