cmake_minimum_required (VERSION 3.8)

project ("RobotSimulation")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(public_header
	"${CMAKE_CURRENT_SOURCE_DIR}/include/SimulationRunner.h")

set(source_files 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/InputParser.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StringUtilities.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Robot.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/RobotSimulator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/InfDetector.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/SimulationRunner.cpp")

set(header_files
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RobotSimulator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/InputParser.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Datatypes.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/include/StringUtilities.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Robot.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/InfDetector.h")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable (RobotSimulator ${source_files} ${public_header} ${header_files} "src/main.cpp")

add_library(SimulationRunnerLib STATIC ${source_files} ${public_header} ${header_files})

install(FILES "include/SimulationRunner.h" DESTINATION include)

target_include_directories(SimulationRunnerLib
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS SimulationRunnerLib DESTINATION lib)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

add_subdirectory(tests)

target_compile_features(RobotSimulator PRIVATE cxx_std_17)
target_compile_features(SimulationRunnerLib PRIVATE cxx_std_17)