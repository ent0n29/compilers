#include <iostream>

extern "C" {
    double doublecond(double);
}

int main() {
    int n;
    std::cout << "Inserisci il valore di n: ";
    std::cin >> n;
    std::cout << "doublecon("<<n<<")=" << doublecond(n) << std::endl;
}
