cmake_minimum_required(VERSION 3.13)

project(zest-coturn)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS  system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(libpqxx REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(zest-coturn main.cpp)

target_link_libraries(zest-coturn pq ${Boost_LIBRARIES} Threads::Threads libpqxx::pqxx_shared ${PostgreSQL_LIBRARIES} scrypt-kdf curl crypto pthread dl) 

target_include_directories(zest-coturn PRIVATE ${Boost_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS})