cmake_minimum_required(VERSION 3.0.0)
project(gokesitew VERSION 0.1.0)

include(CTest)
enable_testing()

# set(CMAKE_CXX_STANDARD 11)
# set(THREADS_PREFER_PTHREAD_FLAG ON)

# find_package(Boost REQUIRED COMPONENTS thread system)
# find_package(Threads)
# find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(gokesitew main.cpp)

# target_link_libraries(gokesitew PRIVATE unofficial::sqlite3::sqlite3)
# target_include_directories(gokesitew PRIVATE ${Boost_INCLUDE_DIRS})
# target_link_libraries(gokesitew ${Boost_LIBRARIES} Threads::Threads sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

