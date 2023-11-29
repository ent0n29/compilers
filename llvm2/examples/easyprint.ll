@fmt = constant [4 x i8] c"%d\0A\00"
declare i32 @printf(i8*, i32)

define i32 @main() {
init:
   %counter = alloca i32
   store i32 0, i32* %counter
   br label %loop
loop:
   %currval = load i32, i32* %counter
   %nextval = add i32 %currval, 1
   %end = icmp eq i32 %nextval, 11
   br i1 %end, label %exit, label %cont
cont:
   store i32 %nextval, i32* %counter
   call i32(i8*,i32) @printf(i8* getelementptr([4 x i8],[4 x i8]* @fmt, i32 0, i32 0), i32 %nextval)
   br label %loop
exit:
   ret i32 0
}   
