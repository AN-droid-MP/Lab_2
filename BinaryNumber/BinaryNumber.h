#include <string>
#include <iostream>
#include "../Octet/Octet.h"

class BinaryNumber {
private:
    Octet* octets;
    size_t size;

    uint8_t stringToUint8(const std::string &binaryStr);
    static char int_symbol(int in) ;
    long toDecimal() const;
    void ten_base(std::stringstream& ss, long in, int base) const;
    void extendToSize(size_t newSize);
    bool isNegative() const;
    static Octet compareByMagnitudeAndReturnSign(const BinaryNumber& a, const BinaryNumber& b) ;
    BinaryNumber toDirectCode() const;

public:
    BinaryNumber();
    BinaryNumber(long number);
    BinaryNumber(const std::string& numberStr);

    BinaryNumber(const BinaryNumber& other);
    BinaryNumber(BinaryNumber&& other) noexcept;
    BinaryNumber& operator=(const BinaryNumber& other);
    BinaryNumber& operator=(BinaryNumber&& other) noexcept;

    ~BinaryNumber();

    std::string toTwosComplement() const;
    BinaryNumber operator+(const BinaryNumber& other) const;
    BinaryNumber operator-() const;
    BinaryNumber& operator++();
    BinaryNumber operator--(int);
    bool getSign() const;
    std::string toString(int base = 2) const;


    friend std::ostream& operator<<(std::ostream& os, const BinaryNumber& number);
    friend std::istream& operator>>(std::istream& is, BinaryNumber& number);
};
