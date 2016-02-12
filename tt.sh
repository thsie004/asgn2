#!/bin/sh

echo ***testing rshell with shell script***

bin/rshell << GUARD


echo hello world
why why why
ls -l #trollolol
ping -c 10 www.google.com
GUARD

