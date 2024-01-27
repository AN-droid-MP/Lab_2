#include "Octet.h"

Octet::Octet() : data(0) {}

Octet::Octet(const int* bits) : data(0) {
    for (size_t pos = 0; pos < 8; ++pos) {
        if (bits[pos] == 0 || bits[pos] == 1) {
            data |= bits[pos] << pos;
        } else {
            break;
        }
    }
}

Octet::Octet(uint8_t byte) : data(byte) {}


Octet::Octet(const Octet &other) : data(other.data) {}

Octet::Octet(Octet &&other) noexcept: data(other.data) {
    other.data = 0;
}

Octet::~Octet() {}

uint8_t Octet::getData() const {
    return data;
}

bool Octet::getBit(int position) const {
    if (position < 0 || position >= 8) {
        throw std::out_of_range("Position must be between 0 and 7");
    }
    return (data >> position) & 1;
}

void Octet::setBit(int position, bool value) {
    if (position < 0 || position >= 8) {
        throw std::out_of_range("Position must be between 0 and 7");
    }
    if (value) {
        data |= 1 << position;
    } else {
        data &= ~(1 << position);
    }
}

Octet Octet::operator~() const {
    return Octet(~data);
}

uint8_t Octet::complement() const {
    return static_cast<uint8_t>(~data);
}

std::pair<Octet, bool> Octet::addWithCarry(const Octet &other) const {
    int16_t sum;
    bool overflow;

    sum = static_cast<uint16_t>(data) + static_cast<uint16_t>(other.data);

    overflow = sum > 256;

    return {Octet(static_cast<uint8_t>(sum)), overflow};
}

std::ostream &operator<<(std::ostream &os, const Octet &octet) {
    for (int i = 7; i >= 0; --i) {
        os << octet.getBit(i);
    }
    return os;
}

std::istream &operator>>(std::istream &is, Octet &octet) {
    uint8_t byte;
    is >> byte;
    octet = Octet(byte);
    return is;
}

Octet &Octet::operator=(const Octet &other) {
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

Octet &Octet::operator=(Octet &&other) noexcept {
    if (this != &other) {
        data = other.data;
        other.data = 0;
    }
    return *this;
}
