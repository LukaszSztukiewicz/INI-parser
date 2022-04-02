#!/bin/bash
testdir_path=btaf-tests

RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

#integers
declare -i passed_count=0
declare -i tests_count=0

cd $testdir_path

for test in in/*.in; do
    
    ((tests_count++))

    if diff $test "out/${test:3:(-3)}.out" > /dev/null
    then
        echo -e "${GREEN} ${test:3:(-3)} TEST PASSED "
        ((passed_count++))
    else
        echo -e "${RED} ${test:3:(-3)} TEST FAILED "
    fi
    #diff -w < (./main ./INI-parser-tests/example-4.5.ini prickly-cheerful-perfect-blond-mean-agreement.concerned-avaricious-stingy-impractical-sugar) 1.out 
done

echo -e "${CYAN}-----------------------------------------------------"

if [ $passed_count -ne $tests_count ]; then
        echo -e "${RED} ** ONLY ${passed_count}/${tests_count} TESTS HAVE PASSED **"
    else
        echo -e "${GREEN} ** GREAT! ${passed_count}/${tests_count} TESTS HAVE PASSED **"
fi