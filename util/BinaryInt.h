//
// Created by xAbdoMo on 4/14/2024.
//

#ifndef SECURITY_PROJECT_BINARYINT_H
#define SECURITY_PROJECT_BINARYINT_H

#include <gmpxx.h>
#include <iostream>

class BinaryInt {
private:
    mpz_t mValue;
    bool del = true;
public:
    BinaryInt();
    ~BinaryInt();
    BinaryInt(int i);
    BinaryInt(long i);
    BinaryInt(const char* str);
    BinaryInt(const char* str,int base);
    BinaryInt(const BinaryInt& other);
    BinaryInt(BinaryInt&& other) noexcept ;

    BinaryInt& operator =(BinaryInt&& other) noexcept ;
    BinaryInt& operator =(const BinaryInt& other) noexcept ;
    BinaryInt& operator +=(const BinaryInt& other);
    BinaryInt& operator -=(const BinaryInt& other);
    BinaryInt& operator /=(const BinaryInt& other);
    BinaryInt& operator *=(const BinaryInt& other);
    BinaryInt& operator %=(const BinaryInt& other);

    BinaryInt operator+ (const BinaryInt& b) const;
    BinaryInt operator- (const BinaryInt& b) const;
    BinaryInt operator/ (const BinaryInt& b) const;
    BinaryInt operator* (const BinaryInt& b) const;
    BinaryInt operator% (const BinaryInt& b) const;

    bool operator> (const BinaryInt& b) const;
    bool operator< (const BinaryInt& b) const;
    bool operator== (const BinaryInt& b) const;
    bool operator!= (const BinaryInt& b) const;
    bool operator>= (const BinaryInt& b) const;
    bool operator<= (const BinaryInt& b) const;

    [[nodiscard]] std::string toString(int base) const;

    // operator overloading for output stream {<<}
    friend std::ostream &operator << (std::ostream& stream, const BinaryInt &n);

    // operator overloading for input stream {>>}
    friend std::istream &operator >> (std::istream& stream, BinaryInt &n);
};

namespace IntOp{
    BinaryInt gcdExtended(BinaryInt a, BinaryInt b, BinaryInt* x, BinaryInt* y);
    BinaryInt modInverse(BinaryInt A, BinaryInt M);

    BinaryInt generateRandInt(int size);


    inline BinaryInt min(BinaryInt& a, BinaryInt& b){
        return a > b ? b : a;
    }

    inline BinaryInt max(BinaryInt& a, BinaryInt& b){
        return a > b ? a : b;
    }

    BinaryInt mulMod(BinaryInt a, BinaryInt b, const BinaryInt& k);

    //calculates : a^b % N
    BinaryInt powMod(BinaryInt a, BinaryInt b, BinaryInt N);
}



#endif //SECURITY_PROJECT_BINARYINT_H
