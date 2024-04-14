//
// Created by xAbdoMo on 4/14/2024.
//

#include <random>
#include "BinaryInt.h"

BinaryInt::BinaryInt() {
    mpz_init(mValue);
    mpz_set_si(mValue , 0);
}

BinaryInt::BinaryInt(int i) {
    mpz_init(mValue);
    mpz_set_si(mValue , i);
}

BinaryInt::BinaryInt(long i) {
    mpz_init(mValue);
    mpz_set_si(mValue , i);
}

BinaryInt::BinaryInt(const char* str) {
    mpz_init(mValue);
    mpz_set_str(mValue , str , 10);
}

BinaryInt::BinaryInt(const char *str, int base) {
    mpz_init(mValue);
    mpz_set_str(mValue , str , base);
}


BinaryInt::BinaryInt(const BinaryInt& other) {
    mpz_init(mValue);
    mpz_set(mValue , other.mValue);
}

BinaryInt &BinaryInt::operator+=(const BinaryInt &other) {
    mpz_add(mValue, mValue, other.mValue);
    return *this;
}

BinaryInt &BinaryInt::operator-=(const BinaryInt &other) {
    mpz_sub(mValue, mValue, other.mValue);
    return *this;
}

BinaryInt &BinaryInt::operator*=(const BinaryInt &other) {
    mpz_mul(mValue, mValue, other.mValue);
    return *this;
}

BinaryInt &BinaryInt::operator/=(const BinaryInt &other) {
    mpz_div(mValue, mValue, other.mValue);
    return *this;
}

BinaryInt &BinaryInt::operator%=(const BinaryInt &other) {
    mpz_mod(mValue, mValue, other.mValue);
    return *this;
}

BinaryInt BinaryInt::operator+(const BinaryInt &other) const {
    BinaryInt temp;
    mpz_add(temp.mValue , mValue , other.mValue);
    return temp;
}

BinaryInt BinaryInt::operator-(const BinaryInt &other) const {
    BinaryInt temp;
    mpz_sub(temp.mValue , mValue , other.mValue);
    return temp;
}

BinaryInt BinaryInt::operator*(const BinaryInt &other) const {
    BinaryInt temp;
    mpz_mul(temp.mValue , mValue , other.mValue);
    return temp;
}

BinaryInt BinaryInt::operator/(const BinaryInt &other) const {
    BinaryInt temp;
    mpz_div(temp.mValue , mValue , other.mValue);
    return temp;
}

BinaryInt BinaryInt::operator%(const BinaryInt &other) const {
    BinaryInt temp;
    mpz_mod(temp.mValue , mValue , other.mValue);
    return temp;
}

BinaryInt::~BinaryInt() {
    if (del) {
        mpz_clear(mValue);
    }
}

bool BinaryInt::operator>(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) == 1;
}

bool BinaryInt::operator<(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) == -1;
}

bool BinaryInt::operator==(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) == 0;
}

bool BinaryInt::operator!=(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) != 0;
}

bool BinaryInt::operator>=(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) >= 0;
}

bool BinaryInt::operator<=(const BinaryInt &b) const {
    return mpz_cmp(mValue , b.mValue) <= 0;
}

std::string BinaryInt::toString(int base) const {
    size_t num_bits = mpz_sizeinbase(mValue, base);  // Get required string size (including null terminator)
    char *str = (char *)malloc(num_bits + 1);  // Allocate memory for the string
    // Convert var to a string in base 10
    char* err = mpz_get_str(str, base, mValue);  // 10 for base 10 conversion
    if (err == nullptr) {
        throw "Failed to convert to string";
    }
    str[num_bits] = '\0';
    std::string out = str;
    free (str);
    return out;
}

std::istream &operator>>(std::istream &stream, BinaryInt &n) {
    std::string str;
    stream >> str;
    n = str.c_str();
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const BinaryInt &n) {
    stream << n.toString(10);
    return stream;
}

BinaryInt::BinaryInt(BinaryInt &&other)  noexcept {
    memcpy(&mValue , &other.mValue , sizeof(mValue));
    other.del = false;
}

BinaryInt &BinaryInt::operator=(BinaryInt &&other)  noexcept {
    memcpy(&mValue , &other.mValue , sizeof(mValue));
    other.del = false;
    return *this;
}

BinaryInt &BinaryInt::operator=(const BinaryInt &other) noexcept {
    mpz_set(mValue , other.mValue);
    return *this;
}

BinaryInt IntOp::gcdExtended(BinaryInt a, BinaryInt b, BinaryInt *x, BinaryInt *y) {
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }
    BinaryInt x1, y1;
    BinaryInt gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

BinaryInt IntOp::modInverse(BinaryInt A, BinaryInt M) {
    BinaryInt x, y;
    BinaryInt g = gcdExtended(A, M, &x, &y);
    if (g != 1)
        return -1;
    else {
        return (x % M + M) % M;
    }
}

static std::random_device rd;
static std::default_random_engine rng(rd());
static std::uniform_int_distribution<int> uniform_dist(1, 255);

BinaryInt IntOp::generateRandInt(int size) {
    BinaryInt t = 1;
    while (size--){
        BinaryInt k = uniform_dist(rng);
        t = t * k;
    }

    return t;
}

BinaryInt IntOp::mulMod(BinaryInt a, BinaryInt b, const BinaryInt &k){
    a = a % k;
    b = b % k;
    return (a * b) % k;
}

BinaryInt IntOp::powMod(BinaryInt a, BinaryInt b, BinaryInt N){
    if (b == BinaryInt(0))
        return 1;
    if (b == BinaryInt(1))
        return a % N;

    BinaryInt hM = powMod(a, b / 2, N);
    hM = mulMod(hM, hM, N);
    if (b % BinaryInt(2) == BinaryInt(1)) {
        hM = mulMod(hM , a % N , N);
    }

    return hM;
}