cmake_minimum_required(VERSION 3.8)


if (APPLE)
	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING
    "Minimum OS X deployment version" FORCE)
	
	# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
	if (POLICY CMP0025)
		cmake_policy(SET CMP0025 NEW)
	endif ()
endif()


project(voiceSpoofingDetection
        VERSION 1.3
        DESCRIPTION "Voice spoofing detection experiments"
        LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

message(STATUS "***********************COMPILING FOR:***********************")
message(STATUS ${CMAKE_SYSTEM})
message(STATUS ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "************************************************************")

###############################################
###############################################
### Executable and libraries configuration  ###
###############################################
###############################################

# Executables and its objects
add_executable(voiceSpoofingDetection
src/main.cpp

src/lib/classifiers/NeuralNetwork.cpp
src/lib/classifiers/DistanceClassifier.cpp
src/lib/classifiers/featureVectorsUtils.cpp
src/lib/classifiers/SupportVectorMachine.cpp

src/lib/file/fileUtils.cpp

src/lib/gnuplot/gnuplotCalls.cpp

src/lib/linearAlgebra/linearAlgebra.cpp

src/lib/paraconsistent/paraconsistent.cpp

src/lib/statistics/statistics.cpp
src/lib/statistics/confusionMatrix.cpp

src/lib/utility/comparison.cpp

src/lib/vector/vectorUtils.cpp

src/lib/wave/Wav.cpp
src/lib/wave/filtersOperations.cpp
src/lib/wave/simpleSignalOperations.cpp

src/lib/wavelet/Types.cpp
src/lib/wavelet/waveletOperations.cpp
src/lib/wavelet/WaveletTransformResults.cpp

src/experiments/01/Experiment01.cpp
src/experiments/02/Experiment02.cpp
src/experiments/03/Experiment03.cpp
src/experiments/04/Experiment04.cpp
src/experiments/05/Experiment05.cpp
src/experiments/06/Experiment06.cpp
src/experiments/07/Experiment07.cpp
src/experiments/08/Experiment08.cpp
)

# User libraries
target_include_directories(voiceSpoofingDetection 
	PUBLIC src/lib/classifiers
	PUBLIC src/lib/file
	PUBLIC src/lib/gnuplot
	PUBLIC src/lib/linearAlgebra
	PUBLIC src/lib/matplotlib-cpp
	PUBLIC src/lib/rapidcsv
	PUBLIC src/lib/statistics
	PUBLIC src/lib/utility
	PUBLIC src/lib/vector
	PUBLIC src/lib/wave
	PUBLIC src/lib/wavelet
)

# System libraries (python)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(voiceSpoofingDetection PUBLIC
  Python3::Python
  Python3::Module
)

find_package(Python3 COMPONENTS NumPy)
if(Python3_NumPy_FOUND)
  target_link_libraries(voiceSpoofingDetection PUBLIC
    Python3::NumPy
  )
endif()

include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# C++ version
target_compile_features(voiceSpoofingDetection PRIVATE cxx_std_20)

########################################
########################################
### Google Unit Testing suite setup  ###
########################################
########################################

if (NOT APPLE)
	# Locate GTest
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	
	# Load paraconsistent tests
	add_executable( paraconsistentTests
		src/lib/paraconsistent/paraconsistentTest.cpp
		src/lib/utility/comparison.cpp
		src/lib/linearAlgebra/linearAlgebra.cpp
		src/lib/paraconsistent/paraconsistent.cpp
	)
	
	# Link paraconsistentTests with what we want to test and the GTest and pthread library
	target_link_libraries(paraconsistentTests PUBLIC ${GTEST_LIBRARIES} pthread)
	
	
	# Load support vector machine tests
	add_executable( supportVectorMachineTests
		src/lib/vector/vectorUtils.cpp
		src/lib/linearAlgebra/linearAlgebra.cpp
		src/lib/classifiers/SupportVectorMachine.cpp
		src/lib/classifiers/SupportVectorMachineTest.cpp
	)
	
	# Link supportVectorMachineTests with what we want to test and the GTest and pthread library
	target_link_libraries( supportVectorMachineTests ${GTEST_LIBRARIES} pthread)
endif()
