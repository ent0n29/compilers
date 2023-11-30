declare double @x()

declare double @y()

declare double @printval(double)

define double @f(double %x, double %y) {
entry:
  %y2 = alloca double, align 8
  %x1 = alloca double, align 8
  store double %x, double* %x1, align 8
  store double %y, double* %y2, align 8
  %x3 = load double, double* %x1, align 8
  %x4 = load double, double* %x1, align 8
  %mulres = fmul double %x3, %x4
  %x5 = load double, double* %x1, align 8
  %mulres6 = fmul double 2.000000e+00, %x5
  %y7 = load double, double* %y2, align 8
  %mulres8 = fmul double %mulres6, %y7
  %subres = fsub double %mulres, %mulres8
  %y9 = load double, double* %y2, align 8
  %mulres10 = fmul double 3.000000e+00, %y9
  %y11 = load double, double* %y2, align 8
  %mulres12 = fmul double %mulres10, %y11
  %addres = fadd double %subres, %mulres12
  ret double %addres
}

Numero di argomenti non corretto
Funzione non definita
