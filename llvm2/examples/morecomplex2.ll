declare double @x()

declare double @y()

declare double @printval(double)

define double @f(double %x, double %y) {
entry:
  %y2 = alloca double, align 8
  %x1 = alloca double, align 8
  store double %x, ptr %x1, align 8
  store double %y, ptr %y2, align 8
  %x3 = load double, ptr %x1, align 8
  %x4 = load double, ptr %x1, align 8
  %mulres = fmul double %x3, %x4
  %x5 = load double, ptr %x1, align 8
  %mulres6 = fmul double 2.000000e+00, %x5
  %y7 = load double, ptr %y2, align 8
  %mulres8 = fmul double %mulres6, %y7
  %subres = fsub double %mulres, %mulres8
  %y9 = load double, ptr %y2, align 8
  %mulres10 = fmul double 3.000000e+00, %y9
  %y11 = load double, ptr %y2, align 8
  %mulres12 = fmul double %mulres10, %y11
  %addres = fadd double %subres, %mulres12
  ret double %addres
}

define double @helper() {
entry:
  %calltmp = call double @x()
  %calltmp1 = call double @y()
  %calltmp2 = call double @f(double %calltmp, double %calltmp1)
  ret double %calltmp2
}

define double @main() {
entry:
  %calltmp = call double @helper()
  %calltmp1 = call double @printval(double %calltmp)
  ret double %calltmp1
}

