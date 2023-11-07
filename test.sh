#! /bin/sh
gcc test.c lexer.c parser.c stack.c -o test && ./test || echo SKILL ISSUE XDD
