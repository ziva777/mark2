cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)
project(mark2 VERSION 0.0.1 LANGUAGES CXX)

set(SOURCES1 
		main.cpp 
		str_filter.cpp
		misc.cpp
		formatter.cpp
		markov_model.cpp
		markov_model_tutor.cpp
		markov_model_serializer.cpp
		pipe_base.cpp
		pipe_read.cpp
		app.cpp)
set(HEADERS1 
		base_types.h
		str_filter.h
		misc.h
		formatter.h
		markov_model.h
		markov_model_tutor.h
		markov_model_serializer.h
		pipe_base.h
		pipe_read.h
		app.h)

add_executable(
	${PROJECT_NAME} 
		${SOURCES1})
add_executable(
	test_01 
		test_01.cpp 
		misc.cpp)
add_executable(
	test_02 
		test_02.cpp 
		pipe_base.cpp
		pipe_read.cpp
		str_filter.cpp)
add_executable(
	test_03 
		test_03.cpp 
		markov_model.cpp 
		markov_model_tutor.cpp 
		markov_model_serializer.cpp 
		misc.cpp
		formatter.cpp)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON)
