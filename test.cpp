//
// Created by xAbdoMo on 4/14/2024.
//

#include <iostream>
#include <chrono>
#include <fstream>
#include "util/BinaryInt.h"
#include "sec/ElGamalSignature.h"

void ElgamalTest(){
    auto now = std::chrono::high_resolution_clock::now();

    std::ifstream keys;
    keys.open("public.txt");
    if (!keys.is_open()){
        std::cout << "failed to open file" << std::endl;
        return;
    }

    SigType Q1 , a1;
    SigType Q2 , a2;

    keys >> Q1 >> a1;
    keys >> Q2 >> a2;

    std::cout << "Loaded public data." << std::endl;
    std::cout << "ELGamal: Q=" << Q1 << " , a=" << a1 << std::endl;
    std::cout << "Daffie : Q=" << Q2 << " , a=" << a2 << std::endl;


    std::cout << "Generating Elgamal Sig" << std::endl;
    ElGamalSig sig; sig.init(Q1, a1);
    std::cout << "Ya  = " << sig.getPublicKey() << std::endl;
    std::cout << "Xa  = " << sig.getPrivateKey() << std::endl;
    std::cout << "K   = " << sig.getRandomK() << std::endl;
    std::cout << "K-1 = " << sig.getRandomKInv() << std::endl;

    auto test = sig.getSignature("pls work");
    std::cout << "{S1,S2}: " <<  test.first << " " << test.second << std::endl;

    std::cout << "Validating: " << sig.verifySignature(test , sig.getPublicKey() , "pls work") << std::endl;

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>((std::chrono::high_resolution_clock::now() - now));

    std::cout << "Done in : " << ns.count() << "ns" << std::endl;
}

int main(){
    BinaryInt i = 15;
    std::cout << i << std::endl;
    i = i + 15;
    if (i != 30){
        std::cout << "ERROR" << std::endl;
    }

    std::cout << i << std::endl;
    i = "1999999999999999999999999999999999999999999999999999";
    std::cout << i << std::endl;
    i -= 11;
    std::cout << i << std::endl;
    BinaryInt j = i;
    std::cout << j << std::endl;
    std::cout << i << std::endl;
    std::cout << (i > j) << std::endl;
    std::cout << (i < (j + 1)) << std::endl;
    std::cout << (i > (j + 1)) << std::endl;
    std::cout << (i == (j + 1)) << std::endl;

}