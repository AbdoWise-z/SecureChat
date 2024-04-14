//
// Created by xAbdoMo on 4/13/2024.
//
#include <random>
#include <iostream>


#include "../util/BinaryInt.h"
#include "../util/BigInt.h"
#include "ElGamalSignature.h"
#include "sha1.hpp"


static std::random_device rd;
static std::default_random_engine rng(rd());
static std::uniform_int_distribution<int> uniform_dist(1, 255);

static SHA1 sha1;

static SigType gcdExtended(SigType a, SigType b, SigType* x, SigType* y);

//calculates the mod inverse
static SigType modInverse(SigType A, SigType M){
    SigType x, y;
    SigType g = gcdExtended(A, M, &x, &y);
    if (g != SigType(1))
        return -1;
    else {
        return (x % M + M) % M;
    }
}

//gcd , but it memorizes the steps , to calculate the mod inverse
static SigType gcdExtended(SigType a, SigType b, SigType* x, SigType* y){
    if (a == SigType(0)) {
        *x = 0, * y = 1;
        return b;
    }
    SigType x1, y1;
    SigType gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

SigType generateRandInt(){
    int s = RandomSize;

    SigType t = 1;
    while (s--){
        SigType k = uniform_dist(rng);
        t = t * k;
    }

    return t;
}


//checks if a * b will overflow
static bool isOverflow(SigType a, SigType b) {
    if (a == SigType(0) || b == SigType(0))
        return false;

    SigType result = a * b;
    if (a == result / b)
        return false;
    else
        return true;
}

static inline SigType min(SigType& a, SigType& b){
    return a > b ? b : a;
}

static inline SigType max(SigType& a, SigType& b){
    return a > b ? a : b;
}

//calculates (a * b) % k
static SigType mulMod(SigType a, SigType b, SigType k) {
    a = a % k;
    b = b % k;
    if (isOverflow(a , b)) {
        SigType mi = min(a, b);
        SigType ma = max(a, b);
        SigType m = mulMod(mi , ma / SigType(2) , k);
        m = (m + m) % k;
        if (ma % SigType(2) == SigType(1)) m = (m + mi) % k;
        return m;
    }
    return (a * b) % k;
}

//calculates : a^b % t but without phi
// O(log(n)) --> 64 at most
static SigType powMod(SigType message, SigType N, SigType key) {
    if (key == SigType(0))
        return 1;
    if (key == SigType(1))
        return message % N;

    SigType hM = powMod(message, N, key / SigType(2));
    hM = mulMod(hM, hM, N);
    if (key % SigType(2) == SigType(1)) {
        hM = mulMod(hM , message % N , N);
    }

    return hM;
}

ElGamalSig::ElGamalSig() {

}

void ElGamalSig::init(SigType q, SigType a) {
    mQ = q;
    mA = a;

    //prepare public keys
    mXa = 0;
    while (mXa == SigType(0)) {
        mXa = generateRandInt() % (mQ - SigType(1));
    }
    mYa = powMod(mA , mQ , mXa);

    //prepare the random
    // I can't find a method better than just randomly trying and praying it works ..
    mK = -1;
    while (mK == SigType(-1)){
        mKInverse = generateRandInt() % (mQ - SigType(1));
        mK = modInverse(mKInverse , (mQ - SigType(1)));
    }
}

SigType ElGamalSig::getRandomK() const{
    return mK;
}

SigType ElGamalSig::getRandomKInv() const{
    return mKInverse;
}

SigType ElGamalSig::getPublicKey() const{
    return mYa;
}
SigType ElGamalSig::getPrivateKey() const{
    return mXa;
}

std::pair<SigType, SigType> ElGamalSig::getSignature(const std::string& message) const {
    sha1.update(message);
    std::string value = sha1.final();
    SigType msg = 0;
    for (int i = 0;i < value.length();i++){
        msg = msg * SigType(16);
        msg = msg + SigType((int) (value[value.length() - 1 - i] - '0'));
    }

    SigType S1 = powMod(mA , mQ , mK);
    SigType S2 = (mKInverse * (msg - mXa * S1)) % (mQ - SigType(1));
    return {S1 , S2};
}

bool ElGamalSig::verifySignature(std::pair<SigType, SigType> s, SigType Ya , const std::string &message) const {
    sha1.update(message);
    std::string value = sha1.final();
    SigType msg = 0;
    for (int i = 0;i < value.length();i++){
        msg = msg * SigType(16);
        msg = msg + SigType((int) (value[value.length() - 1 - i] - '0'));
    }

    SigType V1 = powMod(mA , mQ , msg);
    SigType V2 = (powMod(Ya , mQ , s.first) * powMod(s.first , mQ , s.second)) % mQ;

    return V1 == V2;
}



