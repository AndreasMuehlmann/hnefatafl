#include <iostream>


unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

int main() {
    std::cout << "Hello World!" << std::endl;
    std::cout << "Hello World!" << std::endl;
}
