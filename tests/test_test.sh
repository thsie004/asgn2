#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing test_test
echo ===========================

~/rshell/bin/rshell << GUARD
test -ef bin #fdsavnl,mf
test -d bin
test -edf src
[ -df test_test.sh ] #nm.,nlk
[ -ef tests/commented_command.sh ]
[ -d bin ] && echo bin is indeed a directory
[ -f src ] || echo src is not a regular file
[ -f src ] || [ -f bin ] || [ -f tests ] || echo uhh
echo abc;      test -ef README.md
test -ef src
GUARD
