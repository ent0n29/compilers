## notes on llvm

### how do the testLLVM.ll instructions work?

* `@fmt = constant [4 x i8] c"%d\0A\00"`: This instruction 
defines a constant named `fmt` that is a pointer to a `4 x 
i8` array of characters representing the format string for 
the `printf` function. The format string includes a "%d" 
placeholder for an integer argument and a "\n" newline 
character.
* `declare i32 @printf(i8*, i32)`: This instruction declares 
a function named `printf` that takes two arguments: a pointer
to a null-terminated character array (which is the format 
string), and an integer argument. The return type of the 
function is `i32`, which is an unsigned 32-bit integer.
* `define i32 @main() { ... }`: This instruction defines a 
function named `main` that takes no arguments and returns an 
`i32`.
* `init:`: This label is used as the start of the 
initialization block for the `main` function.
* `%counter = alloca i32`: This instruction allocates a new 
`i32` integer variable named `counter` on the stack.
* `store i32 0, i32* %counter`: This instruction stores an 
initial value of 0 into the `counter` variable.
* `br label %loop`: This instruction jumps to the start of 
the loop defined by the `label` instruction named `%loop`.
* `%currval = load i32, i32* %counter %nextval = add i32 
%currval, 1`: These instructions load the current value of 
the `counter` variable (stored in the `%currval` register) 
and store it in a new variable named `nextval`. The value of 
`nextval` is then incremented by 1 using the `add` 
instruction.
* `%end = icmp eq i32 %nextval, 11`: This instruction 
compares the current value of `nextval` with 11 using an 
integer comparison (`icmp eq`) and stores the result in a new
variable named `%end`. If the value of `%nextval` is equal to
11, then `%end` will be set to true; otherwise, it will be 
set to false.
* `br i1 %end, label %exit, label %cont cont:`: These 
instructions use conditional jumps (`br`) based on the value 
of `%end`. If `%end` is true (i.e., if `nextval` is equal to 
11), then the program jumps to the end of the loop defined by
the `label` instruction named `%exit`. Otherwise, it 
continues to the next iteration of the loop defined by the 
`label` instruction named `%cont`, which starts at the label 
defined as "cont".
* `store i32 %nextval, i32* %counter`: This instruction 
stores the current value of `nextval` back into the `counter`
variable on the stack.
* `call i32(i8*, i32) @printf(i8* getelementptr([4 x i8], [4 
x i8]* @fmt, i32 0, i32 0), i32 %nextval)`: This instruction 
calls the `printf` function with the format string stored in 
`%fmt` (which is a pointer to an array of characters that 
includes the format string defined earlier as a constant), 
and the current value of `nextval` as the integer argument. 
The result of the function call is not used, but it allows 
the program to pass an integer argument to the `printf` 
function.
* `br label %loop exit:`: These instructions use conditional 
jumps (`br`) to jump back to the beginning

