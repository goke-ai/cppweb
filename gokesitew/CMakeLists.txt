cmake_minimum_required(VERSION 3.0.0)
project(gokesitew VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include(CTest)
enable_testing()

find_package(Boost REQUIRED COMPONENTS thread system)
find_package(Threads)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(gokesitew src/main.cpp)

target_include_directories(gokesitew PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(gokesitew ${Boost_LIBRARIES} Threads::Threads unofficial::sqlite3::sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/public $<TARGET_FILE_DIR:${PROJECT_NAME}>/public
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/app_data $<TARGET_FILE_DIR:${PROJECT_NAME}>/app_data
)