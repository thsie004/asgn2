#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing single_command
echo ============================

~/rshell/bin/rshell << GUARD
echo hello world
ls -l
ping -c 3 www.google.com
echo start error test
cdd
lss
pwwd
echo back to good
pwd
git status
lss -a
ls -a

GUARD


