#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "./bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

~/rshell/bin/rshell << GUARD
echo hello world
ls -l #helloooooo
ping -c 5 www.google.com
git status


GUARD


