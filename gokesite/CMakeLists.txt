cmake_minimum_required(VERSION 3.14.0)
project(gokesite VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost REQUIRED COMPONENTS thread system)
find_package(Threads)

add_executable(gokesite src/main.cpp)

target_include_directories(gokesite PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(gokesite ${Boost_LIBRARIES} Threads::Threads sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/public $<TARGET_FILE_DIR:${PROJECT_NAME}>/public
# COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/app_data $<TARGET_FILE_DIR:${PROJECT_NAME}>/app_data
)