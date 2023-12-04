#! /bin/sh



#gcc -Wall -Wextra -pedantic -Werror test.c lexer.c parser.c interpreter.c stack.c -o test || exit 1

gcc test.c lexer.c parser.c interpreter.c stack.c -o test || exit 1

cat  << eof | ./test
int a
float b
{
  asg a 9
  asg b 0.1
  asg b add mul a b div 12 2
  print a
  print b
  if lt a b { 
    print add a mul a b
    print b
  } else {
    print 1337
    if not eq 1 1 {
      print 9999
    }
  }
}
eof

#  char *code = "int a\n"
#              "float b\n"
#              "{\n"
#              "\n\n"
#              "  asg a 1\n"
#              "  if gt a 0 {\n"
#              "    asg b 1.23\n"
#              "      while lt a 5 {\n"
#              "        asg a add a 1\n"
#              "      }\n"
#              "    asg a add b 2\n"
#              "  }\n"
#              "  else asg a 2\n"
#              "}\n";
