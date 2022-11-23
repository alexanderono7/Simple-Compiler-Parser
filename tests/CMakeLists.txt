#https://shunsvineyard.info/2017/04/30/quick-start-to-use-visual-studio-code-for-c-programmers-in-linux/
# Run test cases with ctrl+shift+P then type `cmake: run`
# Must install boost to run test cases:
#   sudo apt install libboost-all-dev
# 1 actual test case per file, I guess?

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/tests.cc)

set(TEST_LIBS project3)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ${TEST_LIBS} ${Boost_LIBRARIES})
add_test(QuickStartTest tests COMMAND tests) # replace "QuickStartTest" with the specific test name or group name
add_test(SubtractionTest tests COMMAND tests)