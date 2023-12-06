#include <iostream>

extern "C" {
    double n();
}

extern "C" {
    double printval(double);
}

double n() {
    double tmp;
    std::cout << "Inserisci il valore di n: ";
    std::cin >> tmp;
    return tmp;
}

double printval(double n) {
    std::cout << "Il valore calcolato è " << n << std::endl;
    return 0;
}
