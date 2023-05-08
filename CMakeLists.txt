cmake_minimum_required (VERSION 3.8)

project ("RobotSimulation")

set(public_header
	"include/SimulationRunner.h")

set(source_files 
	"src/InputParser.cpp"
	"src/StringUtilities.cpp"
	"src/Robot.cpp"
	"src/RobotSimulator.cpp"
	"src/InfDetector.cpp"
	"src/SimulationRunner.cpp")

set(header_files
	"include/RobotSimulator.h"
	"include/InputParser.h" 
	"include/Datatypes.h" 
	"include/StringUtilities.h" 
	"include/Robot.h"
	"include/InfDetector.h")

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

set(test_files 
	"tests/InputParserTest.cpp"
	"tests/StringUtilitiesTest.cpp"
	"tests/RobotTest.cpp"
	"tests/RobotSimulatorTest.cpp"
	"tests/InfDetectorTest.cpp"
	"tests/SimulationRunnerTest.cpp")

add_executable(tests ${test_files} ${source_files} ${public_header} ${header_files})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_compile_features(RobotSimulator PRIVATE cxx_std_17)
target_compile_features(SimulationRunnerLib PRIVATE cxx_std_17)
target_compile_features(tests PRIVATE cxx_std_17)