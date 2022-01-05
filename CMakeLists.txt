# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

project ("ConsoleTetris")


option(WANDER_COMPILE_WITH_TESTS "Build and run RileyJsonCppUtils test executables" ON)

add_subdirectory(src/main)

if (${WANDER_COMPILE_WITH_TESTS})
	enable_testing()
	add_subdirectory(src/test)
endif()