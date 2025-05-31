#!/bin/bash
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cmake --build .
cppcheck --enable=all --project=compile_commands.json 2> ../cppcheck.log || true
ctest --output-on-failure -T Test > ../test_results.log 2>&1 || true
ctest --output-junit TestResults.xml
junit2html TestResults.xml TestReport.html
echo "Build successful!"
echo "Generated files:"
echo " - test_results.log"
echo " - cppcheck.log"
echo " - TestResults.xml"
echo " - TestReport.html"
