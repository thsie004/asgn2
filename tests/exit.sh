#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "./bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing exit
echo ==================

~/rshell/bin/rshell << GUARD
ping -c 3 www.google.com
lss
pwd
exit
GUARD

~/rshell/bin/rshell << GUARD2
echo testing commands with exit
echo ========================== && ls -l ; git status && exit
GUARD2


