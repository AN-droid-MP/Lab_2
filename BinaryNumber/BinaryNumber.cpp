#include "BinaryNumber.h"
#include <cmath>
#include <sstream>
#include <bitset>
#include "iostream"

BinaryNumber::BinaryNumber() : octets(nullptr), size(0) {}

BinaryNumber::BinaryNumber(long number) {
    uint8_t signOctet = 0;
    size = sizeof(long) + 1;
    bool isNegative = number < 0;

    long num = abs(number);

    if (isNegative) {
        signOctet = 0x80;
    }

    octets = new Octet[size];

    octets[0] = (signOctet);

    for (size_t i = 1; i < size; ++i) {
        uint8_t byte = (num >> (i * 8)) & 0xFF;

        octets[i] = Octet(byte);
    }
}

BinaryNumber::BinaryNumber(const std::string &numberStr) {

    if (numberStr.empty()) {
        octets = nullptr;
        size = 0;
        return;
    }

    uint8_t signOctet = 0;

    bool isNegative = !numberStr.empty() && numberStr[0] == '-';
    std::string binaryStr = isNegative ? numberStr.substr(1) : numberStr;

    while (binaryStr.length() % 8 != 0) {
        binaryStr = "0" + binaryStr;
    }

    if (isNegative) {
        signOctet = 0x80;
    }

    size = binaryStr.length() / 8 + 1;
    octets = new Octet[size];

    octets[0] = signOctet;

    for (size_t i = 1; i < size; ++i) {
        std::string octetStr = binaryStr.substr(i * 8, 8);
        octets[i] = stringToUint8(octetStr);
    }
}

BinaryNumber::BinaryNumber(const BinaryNumber &other) {
    size = other.size;
    octets = new Octet[size];

    for (size_t i = 0; i < size; ++i) {
        octets[i] = other.octets[i];
    }
}


BinaryNumber::BinaryNumber(BinaryNumber &&other) noexcept {
    octets = other.octets;
    size = other.size;

    other.octets = nullptr;
    other.size = 0;
}


BinaryNumber &BinaryNumber::operator=(const BinaryNumber &other) {
    if (this != &other) {

        delete[] octets;

        size = other.size;
        octets = new Octet[size];

        for (size_t i = 0; i < size; ++i) {
            octets[i] = other.octets[i];
        }
    }
    return *this;
}


BinaryNumber &BinaryNumber::operator=(BinaryNumber &&other) noexcept {
    if (this != &other) {

        delete[] octets;

        octets = other.octets;
        size = other.size;

        other.octets = nullptr;
        other.size = 0;
    }
    return *this;
}


BinaryNumber::~BinaryNumber() {
    delete[] octets;
}

std::string BinaryNumber::toTwosComplement() const {
    if (size == 0 || octets == nullptr) {
        return "";
    }

    bool isNegative = (octets[0].getData() & 0x80);

    auto tempOctets = new uint8_t[size];
    for (size_t i = 0; i < size; ++i) {
        tempOctets[i] = octets[i].getData();
    }

    if (isNegative) {
        for (size_t i = 0; i < size; ++i) {
            tempOctets[i] = ~tempOctets[i];
        }

        bool carry = true;
        for (size_t i = size - 1; i > 0; --i) {
            if (carry) {
                if (tempOctets[i] == 0xFF) {
                    tempOctets[i] = 0x00;
                } else {
                    tempOctets[i]++;
                    carry = false;
                }
            }
        }
    }

    std::string binaryString;
    for (size_t i = 0; i < size; ++i) {
        binaryString += std::bitset<8>(tempOctets[i]).to_string();
    }

    delete[] tempOctets;

    return binaryString;
}

BinaryNumber BinaryNumber::operator+(const BinaryNumber &other) const {
    size_t maxSize = std::max(this->size, other.size);

    BinaryNumber result;
    result.size = maxSize;
    result.octets = new Octet[maxSize];

    BinaryNumber extendedA = *this;
    BinaryNumber extendedB = other;
    extendedA.extendToSize(maxSize);
    extendedB.extendToSize(maxSize);

    bool carry = false;
    for (size_t i = maxSize; i > 0; i--) {
        std::pair<Octet, bool> sumResult = extendedA.octets[i - 1].addWithCarry(extendedB.octets[i - 1]);
        if (carry) {
            sumResult = sumResult.first.addWithCarry(Octet(1));
        }
        result.octets[i - 1] = sumResult.first;
        carry = sumResult.second;
    }

    if (carry) {
        Octet *newOctets = new Octet[maxSize + 1];
        std::copy(result.octets, result.octets + maxSize, newOctets + 1);
        newOctets[0] = compareByMagnitudeAndReturnSign(extendedA, extendedB);
        result.size = maxSize + 1;
        delete[] result.octets;
        result.octets = newOctets;
    }

    return result.toDirectCode();
}

void BinaryNumber::extendToSize(size_t newSize) {
    if (newSize <= size) {
        return;
    }

    Octet *newOctets = new Octet[newSize];

    for (size_t i = 1; i < size; ++i) {
        newOctets[i + newSize - size] = octets[i];
    }

    newOctets[newSize - size] = octets[0];

    for (size_t i = 0; i < newSize - size; ++i) {
        newOctets[i] = Octet(static_cast<uint8_t>(0));
    }

    delete[] octets;
    octets = newOctets;
    size = newSize;
}

Octet BinaryNumber::compareByMagnitudeAndReturnSign(const BinaryNumber &a, const BinaryNumber &b) {
    BinaryNumber absA = a.isNegative() ? -a : a;
    BinaryNumber absB = b.isNegative() ? -b : b;

    if (absA.size > absB.size ||
        (absA.size == absB.size && absA.octets[1].getData() > absB.octets[1].getData())) {
        return a.octets[0];
    } else if (absA.size < absB.size ||
               (absA.size == absB.size && absA.octets[1].getData() < absB.octets[1].getData())) {
        return b.octets[0];
    } else {
        return a.octets[0];
    }
}

bool BinaryNumber::isNegative() const {
    return size > 0 && (octets[0].getBit(7));
}

BinaryNumber BinaryNumber::operator-() const {
    BinaryNumber result = *this;

    if (result.octets[0].getData() & 0x80) {
        result.octets[0].setBit(7, false);
    } else {
        result.octets[0].setBit(7, true);
    }

    return result;
}

BinaryNumber &BinaryNumber::operator++() {
    bool carry = true;
    for (size_t i = size - 1; i != static_cast<size_t>(-1); --i) {
        if (carry) {
            carry = (octets[i].getData() == 0xFF);
            octets[i] = Octet(octets[i].getData() + 1);
        }
    }
    return *this;
}

BinaryNumber BinaryNumber::operator--(int) {
    BinaryNumber temp = *this;

    bool borrow = true;
    for (size_t i = size - 1; i != static_cast<size_t>(-1); --i) {
        if (borrow) {
            borrow = (octets[i].getData() == 0x00);
            octets[i] = Octet(octets[i].getData() - 1);
        }
    }

    return temp;
}

bool BinaryNumber::getSign() const {
    return !(octets[0].getData() & 0x80);
}

std::string BinaryNumber::toString(int base) const {
    std::stringstream ss;
    long decimal = toDecimal();
    ten_base(ss, decimal, base);
    return ss.str();
}


std::ostream &operator<<(std::ostream &os, const BinaryNumber &number) {
    std::string numberStr = number.toString();
    os << (number.getSign() ? "" : "-") << numberStr;
    return os;
}

std::istream &operator>>(std::istream &is, BinaryNumber &number) {
    std::string inputStr;
    is >> inputStr;
    number = BinaryNumber(inputStr);
    return is;
}

uint8_t BinaryNumber::stringToUint8(const std::string &binaryStr) {
    uint8_t result = 0;
    int bitIndex = 7;

    for (char c: binaryStr) {
        if (c == '1') {
            result |= (1 << bitIndex);
        }
        bitIndex--;
    }

    return result;
}

char BinaryNumber::int_symbol(int in) {
    static const char out[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    return out[in];
}


long BinaryNumber::toDecimal() const {
    if (size == 0 || octets == nullptr) {
        return 0;
    }

    long number = 0;
    int bitIndex = 0;

    for (size_t idx = size; idx > 0; --idx) {
        for (int i = 0; i < 8; ++i) {
            if (octets[idx - 1].getBit(i)) {
                number += 1L << bitIndex;
            }
            bitIndex++;
        }
    }

    return number;
}


void BinaryNumber::ten_base(std::stringstream &ss, long in, int base) const {
    if (in < base) {
        ss << int_symbol(in);
        return;
    }
    ten_base(ss, in / base, base);
    ss << int_symbol(in % base);
}

BinaryNumber BinaryNumber::toDirectCode() const {
    if (!isNegative()) {
        return *this;
    }

    BinaryNumber result(*this);

    for (size_t i = 1; i < result.size; ++i) {
        result.octets[i] = ~result.octets[i].getData();
    }

    bool carry = true;
    for (size_t i = result.size - 1; i > 0; --i) {
        if (carry) {
            if (result.octets[i].getData() == 0xFF) {
                result.octets[i] = Octet(static_cast<uint8_t>(0x00));
            } else {
                result.octets[i] = Octet(result.octets[i].getData() + 1);
                carry = false;
            }
        }
    }

    return result;
}
