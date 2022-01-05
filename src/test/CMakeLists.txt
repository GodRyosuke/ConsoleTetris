add_executable(
	wander_tetris_test
	main.cpp
)
target_link_libraries(
	wander_tetris_test PRIVATE
	wander_tetris_lib
)
target_include_directories(wander_tetris_test PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_test(NAME WanderLibTests COMMAND wander_lib_tests)