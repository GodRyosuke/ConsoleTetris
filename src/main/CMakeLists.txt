add_library(
	wander_tetris_lib STATIC
	Game.cpp
)
target_include_directories(wander_tetris_lib PUBLIC "${PROJECT_SOURCE_DIR}/include")

