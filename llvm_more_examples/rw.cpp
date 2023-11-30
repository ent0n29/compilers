#include <iostream>

extern "C" {
    double x();
}
extern "C" {
    double y();
}
extern "C" {
    double printval(double);
}

double x() {
    double tmp;
    std::cout << "Inserisci il valore di x: ";
    std::cin >> tmp;
    return tmp;
}

double y() {
    double tmp;
    std::cout << "Inserisci il valore di y: ";
    std::cin >> tmp;
    return tmp;
}

double printval(double x) {
    std::cout << "Il valore calcolato è " << x << std::endl;
    return 0;
}
