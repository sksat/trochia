add_executable(hoge_test EXCLUDE_FROM_ALL hoge_test.cpp)
target_link_libraries(hoge_test gtest gtest_main Threads::Threads)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_custom_target(
	test
	DEPENDS googletest hoge_test
	COMMAND hoge_test
)
