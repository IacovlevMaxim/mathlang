#! /bin/sh



#gcc -Wall -Wextra -pedantic -Werror test.c lexer.c parser.c interpreter.c stack.c -o test || exit 1

gcc test.c lexer.c parser.c interpreter.c stack.c -o test || exit 1

echo FIBONACI TEST:
cat  << eof | ./test
int n1 n2 i next
{
  asg n2 1
  print n1
  print n2
  while lt i 26 {
    asg next add n1 n2
    print next
    asg n1 n2
    asg n2 next
    asg i add i 1
  }
}
eof

echo TEST TYPE-CHECKING
cat  << eof | ./test
int a
float b
{
  asg a 1
  asg b 0.3
  print a
  print b
  asg a mul a b
}
eof

echo TEST TYPE-CHECKING 2
cat  << eof | ./test
int a
float b
{
  asg a 1
  asg b 0.3
  print a
  print b
  asg a not b
}
eof

echo TEST TYPE-CHECKING 3
cat  << eof | ./test
int a
float b
{
  asg a 1
  asg b 0.3
  print a
  print b
  asg a add a div b 2
}
eof

echo TEST NESTED EXEC BLOCKS
cat  << eof | ./test
int a i
float b
{
  asg b 0.1
  while lt a 20 {
    if gt a 3 {
      while lt i a {
        print mul b i
        asg i add i 1
      }
    } else {
      print a
    }
    asg a add a 1
  }
  print mul a b
}
eof


#echo TEST INFINITE LOOP
#cat  << eof | ./test
#int a b
#{
#  asg a 1
#  print b
#  print a
#  while 1 { print a print b }
#eof
