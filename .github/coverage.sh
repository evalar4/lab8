#!/bin/bash
set -e

echo "=== Starting coverage script ==="

# Create build directory
mkdir -p build_coverage
cd build_coverage

# Configure with coverage
cmake .. -DENABLE_COVERAGE=ON

# Build
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Generate coverage data
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info \
    '/usr/*' \
    '*/tests/*' \
    '*/googletest/*' \
    '*/formatter_ex_lib/*' \
    '*/solver_lib/*' \
    '*/build/*' \
    --output-file coverage.filtered.info

# Print coverage summary
echo "=== Coverage Summary ==="
lcov --list coverage.filtered.info

# Generate HTML report
genhtml coverage.filtered.info --output-directory coverage_report

echo "=== Coverage script completed ==="
