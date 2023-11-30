define double @F(double %x) {
entry:
  %x1 = alloca double, align 8
  store double %x, double* %x1, align 8
  %x2 = load double, double* %x1, align 8
  %addres = fadd double %x2, 1.000000e+00
  ret double %addres
}

