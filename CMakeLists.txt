cmake_minimum_required(VERSION 3.10)

project(mark2)

set(SOURCES1 
		main.cpp 
		str_filter.cpp
		misc.cpp
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
		markov_model.h
		markov_model_tutor.h
		markov_model_serializer.h
		pipe_base.h
		pipe_read.h
		app.h)

add_executable(mark2 ${SOURCES1})

target_compile_features(mark2
	PUBLIC 
		cxx_std_14)
