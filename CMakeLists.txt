cmake_minimum_required(VERSION 3.5)
project(voiceSpoofingDetection
        VERSION 1.0
        DESCRIPTION "Voice spoofing detection experiments"
        LANGUAGES CXX)  
        
set(CMAKE_VERBOSE_MAKEFILE ON)

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

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(voiceSpoofingDetection ${PYTHON_LIBRARIES})

target_compile_features(voiceSpoofingDetection PRIVATE cxx_std_17)
