set(test_files 
	"${CMAKE_CURRENT_SOURCE_DIR}/InputParserTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/StringUtilitiesTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/RobotTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/RobotSimulatorTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/InfDetectorTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/SimulationRunnerTest.cpp")

add_executable(tests ${test_files} ${source_files} ${public_header} ${header_files})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


target_compile_features(tests PRIVATE cxx_std_17)

include(CTest)
include(Catch)

catch_discover_tests(tests)