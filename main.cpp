#include <iostream>
#include <string>
#include <limits>
#include "BinaryNumber/BinaryNumber.h"

int main() {
    std::string binaryString;
    long number;
    int base;
    BinaryNumber num1, num2;

    std::cout << "Enter a binary number (you can use '-' at the beginning for negative numbers): ";
    while (true) {
        std::cin >> binaryString;
        if (std::cin.eof()) {
            std::cout << "Exiting the program." << std::endl;
            return 0;
        }

        bool isValidBinary = (binaryString[0] == '-' && binaryString.find_first_not_of("01", 1) == std::string::npos) ||
                             binaryString.find_first_not_of("01") == std::string::npos;

        if (isValidBinary) {
            num1 = BinaryNumber(binaryString);
            break;
        } else {
            std::cout << "Incorrect input. Please enter only 0 and 1 (with optional '-' at the beginning): ";
        }
    }

    std::cout << "Enter an integer: ";
    while (!(std::cin >> number)) {
        if (std::cin.eof()) {
            std::cout << "Exiting the program." << std::endl;
            return 0;
        }
        std::cout << "Incorrect input. Please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    num2 = BinaryNumber(number);

    std::cout << "Addition of first and second numbers: " << num1+num2 << std::endl;

    std::cout << "Sign reverse: " << (-num1) << std::endl;

    std::cout << "twosComplement of the first number: " << num1.toTwosComplement() << std::endl;

    std::cout << "Pre-increment of the first number: " << ++num1 << std::endl;

    std::cout << "Post-decrement of the second number: " << num2-- << std::endl
    << "After decrement: " << num2 << std::endl;

    std::cout << "Enter the base for converting the first number: ";
    std::cin >> base;
    std::cout << "The first number in base " << base << ": " << num1.toString(base) << std::endl;

    std::cout << "Addition of first and second numbers: " << num1+num2 << std::endl;

    return 0;
}
