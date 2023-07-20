# Math Interpreter in C

This is a simple math interpreter implemented in C that can evaluate basic mathematical expressions involving functions like `sin`, `cos`, `tan`, `log`, `exp`, and more. The interpreter allows you to input a mathematical expression as a string and returns the result of the evaluation.

## How to Use

1. Compile the code using a C compiler, such as GCC.
   
   ## For Linux
   ```bash
   gcc -o math_interpreter math_interpreter.c -lm
   ```
   ## For windows
   ```cmd
   gcc -o math_interpreter.exe math_interpreter.c
   ```
2. Execute 
      
    ## For Linux
   ```bash
   ./math_interpreter
   ```
   ## For windows
   ```cmd
   math_interpreter.exe
   ```



# Supported Functions
### The interpreter supports the following mathematical functions:
+ add: add to number `add(2, 3)` 
+ minus: as usual `minus(2, 3)`
+ divide: as usual `divide(6, 3)`
+ multiply: as usual `multiply(2, 3)`
+ power: power of something `power(2,3)` => 8 
+ sin: Sine function. `sin(3.14/3)`
+ cos: Cosine function.
+ tan: Tangent function.
+ log: Natural logarithm function (base e).
+ log10: Logarithm base 10.
+ exp: Exponential function (e^x).
+ logBase: Logarithm with a specified base (logarithm of the second argument with base of the first argument).

# Example
+ `sin(3)` - Computes the sine of 3.
+ `add(sin(3), 4)` - Adds the sine of 3 and 4.
+ `mul(2, add(sin(3), 4))` - Multiplies 2 with the result of adding the sine of 3 and 4.
+ `logBase(10, 100)` - Calculates the logarithm base 10 of 100.

# Supported Variables
The interpreter supports a variable x that you can use in your expressions. The value of x is set to the result of pi / 3 (approximately 1.047) before evaluation. You can use x in place of any number in your expressions.

# Important Note
Since this interpreter is designed for basic math expressions, it may not handle complex expressions, operator precedence, or nested function calls in certain cases. Additionally, it doesn't support advanced mathematical operations like calculus or symbolic manipulation.

# Author
This interpreter was created by Rabin Lamichhane.
   
