cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
##CmakeLists file for Rasta folder. 
## Use -std=c++11 as default.
set(CMAKE_CXX_STANDARD 11)
## Disable C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)
## Require full C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

	#folder?
project(Rasta_ht
       	LANGUAGES CXX) ##helib_example

find_package(helib 1.0.0 EXACT REQUIRED) ##helib 1.0.0 EXACT REQUIRED


add_executable(Rasta_ht
		 Rasta_ht.cpp) #orig: helib_example helib_example.cpp
	     ##folder  program.cpp?

target_link_libraries(Rasta_ht helib) ##orig: helib_example helib
		     ##folder?

##working for rasta file:
#[[
project(Rasta_ht
       	LANGUAGES CXX) 

find_package(helib 1.0.0 EXACT REQUIRED)

add_executable(Rasta_ht Rasta_ht.cpp)

target_link_libraries(Rasta_ht helib)

 ]]
