# cmake common config
cmake_minimum_required(VERSION 3.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

## C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# project config
project(rocket_simulator CXX)
set(TROCHIA trochia)
add_executable(${TROCHIA} src/main.cpp src/simulation.cpp src/io/config.cpp)

# Git commit info
find_package(Git)
if(GIT_FOUND)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE COMMIT_ID
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE COMMIT_DATE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	list(REMOVE_ITEM COMMIT_ID "")
	message(STATUS "commit id: ${COMMIT_ID}")
	message(STATUS "commit date: ${COMMIT_DATE}")
	add_definitions(-DGIT_COMMIT_ID=\"${COMMIT_ID}\")
	add_definitions(-DGIT_COMMIT_DATE=\"${COMMIT_DATE}\")
endif(GIT_FOUND)

# compile option
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

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
	URL				https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz
	PREFIX			${EIGEN_BUILD_DIR}
	CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${DEPS_ROOT}
	BUILD_COMMAND	""
	TEST_COMMAND	""
)
include_directories(${DEPS_INCLUDE_DIR}/eigen3)
add_definitions(-DEIGEN_MPL2_ONLY)

## toml11
set(TOML11_BUILD_DIR ${DEPS_BUILD_DIR}/toml11)
ExternalProject_Add(
	toml11
	GIT_REPOSITORY	https://github.com/ToruNiina/toml11
	GIT_TAG			v3.1.0
	PREFIX			${TOML11_BUILD_DIR}
	CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${DEPS_ROOT} -Dtoml11_BUILD_TEST=OFF
)

add_dependencies(${TROCHIA} eigen toml11)
