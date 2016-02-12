#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "./bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing exit.sh w/ exit alone
echo =====================
~/rshell/bin/rshell << GUARD
ping -c 3 www.google.com
lss
pwd
exit
GUARD

echo Start testing exit.sh w/ command with exit
~/rshell/bin/rshell << GUARD2
echo =====================
pwd && ls -l ; git status && exit
GUARD2

echo Start testing exit.sh w/ error command with exit
~/rshell/bin/rshell << GUARD3
echo =====================
pwwd
pwwd && ls-l; git status; exit
GUARD3


