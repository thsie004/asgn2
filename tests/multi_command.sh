#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing multi_command
echo ===========================

~/rshell/bin/rshell << GUARD
ls; echo this is all correct && lss || pwd
git status ; echo or but command in front is good || pwd
GUARD
