cmake_minimum_required(VERSION 3.13)

project(
	template_cpp_project
	VERSION 1.0.0 
	LANGUAGES CXX
)

#* dependencies
Include(FetchContent)

set(cxxopts_VERSION 3.0.0) #* cxxopts, used for CLI argument parsing

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	FetchContent_Declare(
		cxxopts 
		URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v${cxxopts_VERSION}.zip 
	)
	FetchContent_MakeAvailable(cxxopts)
endif()



if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	option(SINGLE_PRECISION "Single precision" OFF)
	option(BUILD_TESTS "Build tests" ON)
	option(BUILD_EXAMPLES "Build examples" ON)
	option(BUILD_BENCHMARKS "Build bencmarks" ON)

	#* where to look for the project header and source files
	set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
	set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
	set(EXAMPLE_DIR ${CMAKE_CURRENT_LIST_DIR}/examples)
	set(BENCHMARK_DIR ${CMAKE_CURRENT_LIST_DIR}/benchmarks)

	#* tests, examples, benchmarks to compile
	set(TEST_NAMES
		send_receive
	)
	set(EXAMPLE_NAMES
		#send
		#receive
	)
	set(BENCHMARK_NAMES
	)

	#* files to package
	set(PACKAGE_FILES 
		include/${PROJECT_NAME}/
		include/${PROJECT_NAME}.hpp 
		CMakeLists.txt 
		LICENSE
	)

	#* set up output directories
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dat)

	#* project compile options
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	add_compile_options(
		-fdiagnostics-color=always #* colored debug output
		-fmessage-length=0 #* disable line wrapping (default=72 char) 
		-Wall #* all warnings
		-Wextra #* more warnings
		-Wpedantic #* more warnings
	)

	if(SINGLE_PRECISION)
		add_compile_options(-D__USE_SINGLE_PRECISION__) #* use single precision floats
	endif()

	#***************#
	#* Executables *#
	#***************#

	#* udp_msg
	#add_executable(
	#	udp_msg 
	#	${SRC_DIR}/udp_msg.cpp				
	#)
	#target_include_directories(
	#	udp_msg PRIVATE 
	#	${INCLUDE_DIR}
	#	${cxxopts_SOURCE_DIR}/include
	#)

	#***********#
	#* Testing *#
	#***********#
	if(BUILD_TESTS)
		enable_testing()

		foreach(ELEMENT ${TEST_NAMES})
			add_executable(
				${ELEMENT}-test
				${TEST_DIR}/${ELEMENT}.cpp						
			)
			target_include_directories(
				${ELEMENT}-test PRIVATE 
				${INCLUDE_DIR}
				${matrix_rw_SOURCE_DIR}/include
				${matrix_op_SOURCE_DIR}/include
			)
			target_compile_options(
				${ELEMENT}-test PRIVATE
			)
			if(WIN32) #* link libraries for windows
				target_link_libraries(
					${ELEMENT}-test 
					wsock32 
					ws2_32
				)
			endif()
			add_test(
				NAME ${ELEMENT}-test 
				COMMAND ${ELEMENT}-test
				WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
			)
		endforeach(ELEMENT ${TEST_NAMES})
	endif()

	#************#
	#* Examples *#
	#************#
	if(BUILD_EXAMPLES)

		foreach(ELEMENT ${EXAMPLE_NAMES})
			add_executable(
				${ELEMENT}-example 
				${EXAMPLE_DIR}/${ELEMENT}.cpp						
			)
			target_include_directories(
				${ELEMENT}-example PRIVATE 
				${INCLUDE_DIR}
			)
			target_compile_options(
				${ELEMENT}-example PRIVATE
				-O0 #* optimization level
			)
			if(WIN32) #* link libraries for windows
				target_link_libraries(
					${ELEMENT}-example 
					wsock32 
					ws2_32
				)
			endif()
		endforeach(ELEMENT ${EXAMPLE_NAMES})
	endif()

	#*************#
	#* Benchmark *#
	#*************#
	if(BUILD_BENCHMARKS)

		foreach(ELEMENT ${BENCHMARK_NAMES})
			add_executable(
				${ELEMENT}-benchmark
				${BENCHMARK_DIR}/${ELEMENT}.cpp
			)
			target_include_directories(
				${ELEMENT}-benchmark PRIVATE 
				${INCLUDE_DIR}	
			)
			target_compile_options(
				${ELEMENT}-benchmark PRIVATE
				-O3 #* optimization level
				-m64 #* x64
				-mavx2 #* enable avx2
				#-fno-math-errno #* disable errno 
				#-ffast-math #* feeling brave?
			)
			if(WIN32) #* link libraries for windows
				target_link_libraries(
					${ELEMENT}-benchmark 
					wsock32 
					ws2_32
				)
			endif()
		endforeach(ELEMENT ${BENCHMARK_NAMES})
	endif()

	#************************#
	#* Packaged source code *#
	#************************#
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip
		COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${PACKAGE_FILES}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS ${PACKAGE_FILES}
	)
	add_custom_target(
		src_package 
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip
	)
endif()


##* useful compile options:
#add_compile_options(
	#-fmax-errors=5 #* max number of errors
	#-Wno-unused-variable
	#-Wno-unused-parameter
	#-Wno-unused-but-set-parameter
	#-Wl,--stack,17179869184
	#-Wl,--stack,8388608
	#* [Optional]: for embedded systems the following flags may be enabled
	#-fno-rtti #* disable RTTI 
	#-fno-exceptions #* disable exceptions  
	#-fno-non-call-exceptions  
	#-fno-use-cxa-atexit #* disable destructors 
	#-fno-common #* declare globals once 
	#-ffreestanding #* standard library may be unavailable 
	#-fsanitize=leak #* a memory leak detector
	#-fsanitize=undefined #* a fast undefined behavior detector
	#-fsanitize-undefined-trap-on-error #* report for sanitizers mentioned in comma-separated list of opts undefined behavior using __builtin_trap rather than a libubsan library routine
	#-fstack-protector-strong #* stack guard, costs one write and read per function, needs canary
#)


##* force build type
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
#            "Default build type: Debug" FORCE)
#endif()


##* set the number of CPUs 
#include(ProcessorCount)
#ProcessorCount(N)

#if(NOT N EQUAL 0)
#	set(CTEST_BUILD_FLAGS -j${N})
#	set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
#endif()
#message("> Number of cores set to ${N}")


##* catch2, used for testing
#FetchContent_Declare(
#	Catch2
#	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#	GIT_TAG v3.0.1
#)
#FetchContent_MakeAvailable(Catch2)


#* disable annoying deprecation warnings
#set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)