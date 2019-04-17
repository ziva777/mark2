cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)
project(mark2 VERSION 0.0.1 LANGUAGES CXX)
set(SRC src)

set(SOURCES1 
		${SRC}/main.cpp 
		${SRC}/str_filter.cpp
		${SRC}/misc.cpp
		${SRC}/string_view.cpp
		${SRC}/formatter.cpp
		${SRC}/markov_model.cpp
		${SRC}/markov_model_tutor.cpp
		${SRC}/markov_model_serializer.cpp
		${SRC}/pipe_base.cpp
		${SRC}/pipe_read.cpp
		${SRC}/app.cpp)

add_executable(
	${PROJECT_NAME} 
		${SOURCES1})
add_executable(
	test_01 
		${SRC}/test/test_01.cpp 
		${SRC}/misc.cpp)
add_executable(
	test_02 
		${SRC}/test/test_02.cpp 
		${SRC}/pipe_base.cpp
		${SRC}/pipe_read.cpp
		${SRC}/str_filter.cpp)
add_executable(
	test_03 
		${SRC}/test/test_03.cpp 
		${SRC}/markov_model.cpp 
		${SRC}/markov_model_tutor.cpp 
		${SRC}/markov_model_serializer.cpp 
		${SRC}/misc.cpp
		${SRC}/formatter.cpp)
add_executable(
	test_04 
		${SRC}/test/test_04.cpp
		${SRC}/string_view.cpp)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON)
