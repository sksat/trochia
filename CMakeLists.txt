# cmake common config
cmake_minimum_required(VERSION 3.1)

## C++
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# project config
project(rocket_simulator CXX)
set(SIMULATOR sim.exe)
add_executable(${SIMULATOR} src/main.cpp src/simulation.cpp src/io/config.cpp)

# dependency
set(DEPS_ROOT        ${PROJECT_BINARY_DIR}/deps)
set(DEPS_BUILD_DIR   ${DEPS_ROOT}/build)
set(DEPS_INCLUDE_DIR ${DEPS_ROOT}/include)
include_directories(${DEPS_INCLUDE_DIR})

include(ExternalProject)

## Eigen
set(EIGEN_BUILD_DIR ${DEPS_BUILD_DIR}/eigen)
ExternalProject_Add(
	eigen
	URL				https://bitbucket.org/eigen/eigen/get/3.3.7.zip
	PREFIX			${EIGEN_BUILD_DIR}
	CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${DEPS_ROOT}
)
include_directories(${DEPS_INCLUDE_DIR}/eigen3)

add_dependencies(${SIMULATOR} eigen)

## toml11
set(TOML11_BUILD_DIR ${DEPS_BUILD_DIR}/toml11)
ExternalProject_Add(
	toml11
	GIT_REPOSITORY	https://github.com/ToruNiina/toml11
	GIT_TAG			v3.1.0
	PREFIX			${TOML11_BUILD_DIR}
	CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${DEPS_ROOT} -Dtoml11_BUILD_TEST=OFF
)

add_dependencies(${SIMULATOR} toml11)
