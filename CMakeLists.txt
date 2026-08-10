cmake_minimum_required(VERSION 3.18)
project(VisionInspection VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src ${OpenCV_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/pipeline/Pipeline.cpp
    src/acquisition/FrameAcquisition.cpp
    src/preprocessing/CpuPreprocessor.cpp
    src/postprocessing/PostProcessor.cpp
    src/io/MetricsPublisher.cpp
)

add_executable(inspection_pipeline ${SOURCES})
target_link_libraries(inspection_pipeline ${OpenCV_LIBS} Threads::Threads)

# Tests
find_package(GTest)
if(GTest_FOUND)
    add_executable(test_pipeline tests/test_lockfree_queue.cpp tests/test_preprocessor.cpp src/preprocessing/CpuPreprocessor.cpp)
    target_link_libraries(test_pipeline GTest::GTest ${OpenCV_LIBS})
endif()
