cmake_minimum_required(VERSION 3.10)
project(banking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Для Ubuntu 22.04+
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")

enable_testing()

add_library(banking
    Account.cpp
    Transaction.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(banking_tests
    test_account.cpp
    test_transaction.cpp
)

target_link_libraries(banking_tests
    banking
    gtest
    gmock
    gtest_main
)

add_test(NAME banking_tests COMMAND banking_tests)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    message(STATUS "Enabling code coverage")
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -lgcov)
endif()
