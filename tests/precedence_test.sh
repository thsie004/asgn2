#!/bin/sh

#DO NOT REMOVE
echo The test script assumes you are in rshell and will run rshell with
echo the command "~/rshell/bin/rshell" while feeding it input with heredoc.
#DO NOT REMOVE

echo Start testing precedent_test
echo ============================

~/rshell/bin/rshell << GUARD
echo A && echo B || echo C && echo D
(echo A&&   echo B) ||     (        #start of line
echo C && echo D)               #end of line
(echo0 A && echo B) ||  #start of line
(
echo C && ech0) #end of line
(((((((( echo A ))))))))    # a lot of parenthesessssss
((echo A) && (ls)        )
(((((            #start of line
echo a
echo b ||
echo d)))
echo fdjsan,k)
echo hello world)    #end of line
( (     ( echo vdsjlkmfree              #start of line
echo fdsuoi ) )
||          #this will invoke error call
echo fdjk)  #end of line
GUARD


