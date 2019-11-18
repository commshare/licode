cmake_minimum_required(VERSION 2.6)

project (ERIZO_TEST)

if(${ERIZO_BUILD_TYPE} STREQUAL "sanitizer")
  set(SANITIZER_OPTION "-fsanitize=address")
else()
  set(SANITIZER_OPTION "")
endif()

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 ${ERIZO_CMAKE_CXX_FLAGS} ${SANITIZER_OPTION}")

##ERIZO_TEST_SOURCE_DIR下的文件，编译为tests程序，我怀疑路径就是erzio/src/test
file(GLOB_RECURSE ERIZO_TEST_SDPS ${ERIZO_TEST_SOURCE_DIR}/*.sdp)
file(COPY ${ERIZO_TEST_SDPS} DESTINATION ${ERIZO_TEST_BINARY_DIR})
file(COPY ${ERIZO_TEST_SOURCE_DIR}/log4cxx.properties DESTINATION ${ERIZO_TEST_BINARY_DIR})
file(GLOB_RECURSE ERIZO_TEST_SOURCES ${ERIZO_TEST_SOURCE_DIR}/*.cpp ${ERIZO_TEST_SOURCE_DIR}/*.h)

link_directories("${GMOCK_BUILD}/lib")

##编译tests
add_executable(tests ${ERIZO_TEST_SOURCES})

include_directories("${ERIZO_SOURCE_DIR}" "${THIRD_PARTY_INCLUDE}" "${GMOCK_BUILD}/include" "${NICER_INCLUDE}")

##tests依赖于erzio gtest gmock
target_link_libraries(tests erizo gtest_main gmock_main)

###TODO 这个是啥
add_test(NAME all
         WORKING_DIRECTORY "${ERIZO_TEST_BINARY_DIR}"
         COMMAND tests)

add_custom_target(check
    tests
    WORKING_DIRECTORY "${ERIZO_TEST_BINARY_DIR}"
    COMMENT "Running tests"
)
