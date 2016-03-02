#!/bin/bash

flag=$1
path=$2

if test $flag $path; then
  echo "(True)" >&2
  exit 0
else
  echo "(False)" >&2
  exit1
fi
