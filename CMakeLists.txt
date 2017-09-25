cmake_minimum_required(VERSION 3.6)
project(fern_interface)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)
#find_package(zlib REQUIRED)

file(GLOB_RECURSE SOURCE_FILES  ${PROJECT_SOURCE_DIR}/fern/*.c*)
file(GLOB_RECURSE HEADERS  *.h *.hpp)

source_group("Include" FILES ${HEADERS})
source_group("Src" FILES ${SOURCE_FILES})

add_executable(fern_interface ${SOURCE_FILES} fern_interface.cpp FernPlanarDetector.cpp ${HEADERS})
message(${SOURCE_FILES})
INCLUDE_DIRECTORIES(
        ${OpenCV_INCLUDE_DIRS}
        #${ZLIB_INCLUDE_DIR}
		"D:/zlib-1.2.11/"
        "fern/"
)
#SET(CMAKE_CXX_FLAGS "-Wno-c++11-narrowing -O3")

SET_TARGET_PROPERTIES(fern_interface PROPERTIES    RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin")

ADD_DEFINITIONS(/D_USE_MATH_DEFINES)
ADD_DEFINITIONS(/openmp)

#LIST(APPEND LINK_LIBS ${PROJECT_SOURCE_DIR}/DBoW3001d.lib ${OpenCV_LIBS})
LIST(APPEND LINK_LIBS ${OpenCV_LIBS} ${PROJECT_SOURCE_DIR}/zlibwapi.lib)
TARGET_LINK_LIBRARIES(fern_interface ${LINK_LIBS})