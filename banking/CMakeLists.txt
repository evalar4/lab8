cmake_minimum_required(VERSION 3.10)
project(banking)

# Настройка компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение тестирования
enable_testing()

# Основная библиотека
add_library(banking
    Account.cpp
    Transaction.cpp
)

# Интерфейсные include-директории
target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Google Test
find_package(GTest REQUIRED)

# Модульные тесты
add_executable(banking_tests
    test_account.cpp
    test_transaction.cpp
)

target_link_libraries(banking_tests
    banking
    GTest::GTest
    GTest::Main
)

# Добавление тестов
add_test(NAME banking_tests COMMAND banking_tests)

# Покрытие кода
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    find_package(gcov REQUIRED)
    find_package(lcov REQUIRED)

    add_custom_target(coverage
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory coverage_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

