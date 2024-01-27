#ifndef OCTET_H
#define OCTET_H

#include <iostream>
#include <utility>

#include <cstdint>
#include <iostream>

class Octet {
private:
    uint8_t data;
public:
    Octet();
    Octet(const int* bits);
    Octet(uint8_t byte);

    Octet(const Octet& other);
    Octet(Octet&& other) noexcept;

    ~Octet();

    uint8_t getData() const;
    bool getBit(int position) const;
    void setBit(int position, bool value);

    Octet operator~() const;
    uint8_t complement() const;
    std::pair<Octet, bool> addWithCarry(const Octet &other) const;

    friend std::ostream& operator<<(std::ostream& os, const Octet& octet);
    friend std::istream& operator>>(std::istream& is, Octet& octet);

    Octet& operator=(const Octet& other);
    Octet& operator=(Octet&& other) noexcept;
};

#endif // OCTET_H