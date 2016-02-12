#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "./bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing commented_command
echo ===============================

~/rshell/bin/rshell << GUARD
ping -c 3 www.google.com #normal
lss #This is an error command
pwd #It is a correct command
ls -l #it is a correct command with flags
git status && lss || pwd ; echo testing===#This is multi_command
ls -l || pwd; pwwd #test
ping -c 2 www.yahoo.com; git status&& pwd || pwwd#test
pwwd -x #test
ls                    #hi
             ls        ;# theelo
pwd   ; #                       hihi
  pwwwdwdwdww && pwd #       hi
git status &&        ls        ## how
GUARD


