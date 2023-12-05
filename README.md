# MathLang

*A calculator language*

## Introduction

*MathLang* is a stack based language that uses Polish style notation for its mathematical expressions.
Polish notation is also sometimes referred to as prefix notation, meaning that operations are written before the operands. For example: `add 1 2` does the `1 + 2` operation.

## Syntax

Here is a table of all the syntax keywords and their meaning:
|keyword|  |
|---|---|
| `int` | Declares a variable of type int. |
| `float` | Declares a variable of type float. |
| `asg` | Assigns a value to a variable of the appropriate type. |
| `{` and `}` | Curly braces are markers for the beginning and end of either the main block, or an exec block as part of the `if` or `while` statement. |
| `add` | Returns sum of two values. |
| `sub` | Returns difference of two values. |
| `mul` | Returns product of two values. |
| `div` | Returns quotient of division of 2 values. |
| `not` | Returns the logical *not* of a value. |
| `and` | Returns the logical *and* of 2 values. |
| `or` | Returns the logical *or* of 2 values. |
| `lt` | Returns comparison result of "less then". |
| `gt` | Returns comparison result of "greater then". |
| `eq` | Returns the numerical boolean value of an equality. In case of floats checks with precision of `0.001`. |
| `print` | prints a value to the console. |
| `if` | A conditional statement.<br> Is composed of: `if` *a boolean value or expression* `{ ` *a code block* ` }` `else` `{` *alternative codeblock* `}` |
| `while` | A loop statement.<br> Is composed of: `while` *a boolean value or expression* `{ ` *a code block* ` }` |


## Grammar

### Structure

The structure of a program has 2 main parts:

- The preamble -- where variables are declared:
```
int a
float b
```
- And right after the main block between braces:

```
int a
float b
{
  asg a 44
  asg b 0.01
  print mul a b
}
```
Here `print` takes one argument, that being the return value of `mul a b`.

New lines and padding for inside exec blocks are not mandatory as *MathLang* keeps track of each operation and the number of its operands in the parser. So this is the same as above:
```
int a
float b
{ asg a 44 asg b 0.01 print mul a b }
```
> **NOTE** exec blocks (including the main block) are nothing more than a list of expressions/statements separated at least by a newline or space that are executed consecutively.

### Types

Besides number of operands, the *MathLang's* parser keeps track of one more thing: the types returned by each operations. Because type checking is performed at the parser level, a program with `a` as integer will refuse to even run in the following case:
```
asg a mul add a 4 div sub a 0.5
```
Printing the following error:
```
Parser Error: attempting to assign `a` of type int a return value of type float on line ...
...
```
For reference, here are the types returned by *MathLang's* operations:
- `add`, `sub` and `mul`: when both operands are an integer -- returns an integer, otherwise -- a float;
- `div`: always returns a float;
- `not`, `and`, `or`, `eq`, `lt`, `gt`: return an integer being either `0` or `1`;
- `asg` and `print`: return nothing (they are not operations in the true sense of the word)

> **NOTE** all returned values must be consumed. Meaning that operations like `add 2 1` by their own are invalid.
> They must either be an argument to another operation,
> a value to be printed or assigned or an argument to the `if` or `while` statement.

### Boolean values

In *MathLang* logical operations and the `if` and `while` statements treat numerical values as boolean:
- `0` and `0.0` are interpreted as *false*;
- any other int or float is interpreted as *true*.

## Writing your first program

Let us implement a Fibonacci sequence generator step by step
1. First, we define our variables:
```
int n1 n2 i next
```
2. We need our building blocks: a `0` and a `1`. As *MathLang* has values initialised with `0`, we only need to:
```
asg n2 1
```
3. Next, we print our first two numbers:
```
print n1
print n2
```
4. And begin generating our Fibonacci up to 25 more numbers:
```
  while lt i 26 {
    asg next add n1 n2
    print next
    asg n1 n2
    asg n2 next
    asg i add i 1
  }
```
This `while` loop is executed as long as `lt i 26`, in other words as long as `i` is less that `26`.
Inside, the loop assigns to the variable `next` the sum of `n1` and `n2` and prints it.
Then, it reconsiders the last two numbers of the sequence by assigning `n2` to `n1` and `next` to `n2`.
Finally, `i` is incremented.

Here is the complete code:
```
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
```
For reference, the same code in C:
```
#include <stdio.h>

int main() {
    int n1 = 0, n2 = 1, i = 0, next;

    printf("%d\n", n1);
    printf("%d\n", n2);

    while (i < 26) {
        next = n1 + n2;
        printf("%d\n", next);
        n1 = n2;
        n2 = next;
        i++;
    }

    return 0;
}

```

