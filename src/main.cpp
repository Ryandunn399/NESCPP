#include <iostream>

void printNumber(uint8_t value);

int main() {
    std::cout << "Hello from a cross-platform C++ project!" << std::endl;
    printNumber(0x1FF);
    return 0;
}

void printNumber(uint8_t value)
{
    std::cout << value << std::endl;
}