#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing commended_command
echo ===============================

~/rshell/bin/rshell << GUARD
ping -c 3 www.google.com
lss #This is an error command
pwd #It is a correct command
ls -l #it is a correct command with commended
git status && lss || pwd ; echo testing===#This is multi_command
GUARD


