# Unit tests (googletest).
#
# These use the system Eigen and googletest so they build quickly without the
# ExternalProject dependencies of the main target. Enable with:
#   cmake -DTROCHIA_BUILD_TESTS=ON ..
#   cmake --build . --target tests   # builds only the tests (no ExternalProject)
#   ctest

find_package(GTest REQUIRED)
include(GoogleTest)

# Eigen is header-only; just locate its include directory (don't rely on an
# imported target, which some Eigen CMake packages don't provide).
find_path(EIGEN3_INCLUDE_DIR Eigen/Geometry
	PATHS /usr/include/eigen3 /usr/local/include/eigen3 /opt/homebrew/include/eigen3)
if(NOT EIGEN3_INCLUDE_DIR)
	message(FATAL_ERROR "Eigen headers (Eigen/Geometry) not found")
endif()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")

# aggregate target so CI can build just the tests without the main executable
add_custom_target(tests)

foreach(test_src ${TEST_SOURCES})
	get_filename_component(test_name ${test_src} NAME_WE)
	# tests named test_sim_* exercise the simulation, so link its source
	set(srcs ${test_src})
	if(test_name MATCHES "^test_sim")
		list(APPEND srcs "${CMAKE_SOURCE_DIR}/src/simulation.cpp")
	endif()
	add_executable(${test_name} ${srcs})
	target_compile_features(${test_name} PRIVATE cxx_std_20)
	target_include_directories(${test_name} PRIVATE
		"${CMAKE_SOURCE_DIR}/src" "${EIGEN3_INCLUDE_DIR}")
	target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)
	gtest_discover_tests(${test_name})
	add_dependencies(tests ${test_name})
endforeach()
