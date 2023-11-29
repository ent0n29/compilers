#include <iostream>

extern "C" {
    double F(double);
}

int main() {
    int x;
    std::cout << "Inserisci un numero x: ";
    std::cin >> x;
    std::cout << "Il valore di F(x) è " << F(x) << std::endl;
}
