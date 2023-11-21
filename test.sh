#! /bin/sh



gcc test.c lexer.c parser.c interpreter.c stack.c -o test || exit 1

cat  << eof | ./test.sh
int a
float b
{
  asg a 1
  asg b add a 2.3
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
