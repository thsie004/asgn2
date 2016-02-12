#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing multi_command
echo ===========================

~/rshell/bin/rshell << GUARD
ls; echo this is all correct && lss || pwd
git status ; good || pwd
pwd &&                 ls ; #hello
ppd || ls-l && heeee;;;
                    ls && git status |||mdmd ;
ls + && gi co ||     ;;;
;;; gi mdsn || ls -a && .h ;;; #hi
pwd || ls && pwd    ;;
git status; &&; pwd || play
echo heelo;;; ls  && pwd          ##
its dmals |}dakl && ls #end
git status            &&   echo exit ; exit #exit
GUARD
