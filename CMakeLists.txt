cmake_minimum_required(VERSION 3.10)

project(mark2)


set(SOURCES1 
		main.cpp 
		str_filter.cpp
		misc.cpp
		markov_model.cpp
		markov_model_tutor.cpp)
set(HEADERS1 
		base_types.h
		str_filter.h
		misc.h
		markov_model.h
		markov_model_tutor.h)


set(SOURCES2 
		main2.cpp
		misc.cpp)
set(HEADERS2
		base_types.h
		misc.h
		markov_model.h)


add_executable(mark2_prod ${SOURCES1})
add_executable(mark2_cons ${SOURCES2})

target_compile_features(mark2_prod 
	PUBLIC 
		cxx_std_14)
target_compile_features(mark2_cons 
	PUBLIC 
		cxx_std_14)
