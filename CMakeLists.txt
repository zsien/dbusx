cmake_minimum_required(VERSION 3.14)
project(dbusx VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror -pedantic)

find_package(PkgConfig)

pkg_check_modules(systemd REQUIRED IMPORTED_TARGET libsystemd)

# find_package(DBus1)

include_directories(
    include
    src
)

add_library(dbusx SHARED)

target_sources(dbusx PRIVATE
    src/sdbus/bus.cc
    src/sdbus/bus_private.cc
    src/sdbus/message.cc
    src/sdbus/message_private.cc
)

target_include_directories(dbusx PRIVATE
    # ${DBus1_INCLUDE_DIRS}
)

target_link_libraries(dbusx PRIVATE
    PkgConfig::systemd
    # ${DBus1_LIBRARIES}
)

add_executable(dbusx_example)

target_sources(dbusx_example PRIVATE
    example/main.cc
    example/example.cc
)

target_include_directories(dbusx_example PRIVATE
)

target_link_libraries(dbusx_example PRIVATE
    dbusx
)
