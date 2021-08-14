cmake_minimum_required(VERSION 3.0.0)
project(goke VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TARGET_NAME "main")
set(SRC_SERVER_DIR "src/server")
set(SRC_CLIENT_DIR "src/client")

# set(THREADS_PREFER_PTHREAD_FLAG ON)
# set(_WIN32_WINNT 0x0601)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (MSVC)
    message(STATUS "inside windows")

    set(BOOST_ROOT C:/sdk/vcpkg/installed/x86-windows)
    # set(Boost_INCLUDE_DIR C:/sdk/vcpkg/installed/x86-windows/include)
    # set (unofficial-sqlite3_DIR C:/sdk/vcpkg/packages/sqlite3_x86-windows/share/unofficial-sqlite3)
    set (SQLITE3_ROOT C:/sdk/vcpkg/packages/sqlite3_x86-windows)
    set(Boost_USE_STATIC_LIBS "On")
    find_package(Boost COMPONENTS system thread regex REQUIRED )
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
else()
    message(STATUS "inside non-windows")
    
    find_package(Boost COMPONENTS system thread REQUIRED )
    find_package(Threads)
    # find_package(sqlite3 CONFIG REQUIRED)
endif()

include(CTest)
enable_testing()

add_compile_definitions(CMAKE_TOOLCHAIN_FILE="C:\\sdk\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")

add_executable(${TARGET_NAME} src/server/main.cpp)

if (MSVC)
    target_link_libraries(${TARGET_NAME} 
                    ${Boost_LIBRARIES} 
                    ${SQLITE3_LIBRARIES} 
                    Threads::Threads 
                    unofficial::sqlite3::sqlite3)

    target_include_directories(${TARGET_NAME} 
                    PUBLIC ${Boost_INCLUDE_DIRS} 
                    PUBLIC ${SQLITE3_INCLUDE_DIRS})
else()
    target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES} Threads::Threads sqlite3)
endif()

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)
install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)
                

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/debug/static)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${SRC_SERVER_DIR}/static $<TARGET_FILE_DIR:${TARGET_NAME}>/static
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${SRC_SERVER_DIR}/templates $<TARGET_FILE_DIR:${TARGET_NAME}>/templates
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${SRC_SERVER_DIR}/app_data $<TARGET_FILE_DIR:${TARGET_NAME}>/app_data
)