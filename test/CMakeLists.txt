## GoogleTest
find_package(Threads)
set(GTEST_BUILD_DIR ${DEPS_BUILD_DIR}/googletest)
ExternalProject_Add(
	googletest
	GIT_REPOSITORY	https://github.com/google/googletest.git
	GIT_TAG			master
	CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${DEPS_ROOT}
	TEST_COMMAND	""
)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_rocket EXCLUDE_FROM_ALL rocket.cpp)
target_link_libraries(test_rocket gtest gtest_main Threads::Threads)

add_executable(test_coordinate EXCLUDE_FROM_ALL coordinate.cpp)
target_link_libraries(test_coordinate gtest gtest_main Threads::Threads)

add_custom_target(
	test
	DEPENDS googletest test_rocket test_coordinate
	COMMAND test_rocket
	COMMAND test_coordinate
)
