#!/bin/sh

echo ***testing rshell with shell script***

bin/rshell << GUARD
echo hello world
ls -l #helloooooo
ping -c 5 www.google.com
git status


GUARD

