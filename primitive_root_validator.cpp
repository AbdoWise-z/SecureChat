//
// Created by xAbdoMo on 4/13/2024.
//

#include <iostream>
#include <fstream>
#include "util/BigInt.h"

int main(){
    std::ifstream keys;
    keys.open("public.txt");
    if (!keys.is_open()){
        std::cout << "failed to open file" << std::endl;
        return 0;
    }
    for (int i = 0;i < 2;i++){
        BigInt q , a;
        keys >> q;
        keys >> a;
        std::cout << "Testing : " << q << " | " << a << std::endl;
        BigInt k = "1";
        BigInt t = a;
        for (; k < q - 1;k++){
            if (t == BigInt("1")){
                std::cout << "Failed";
            }
            t = t * a;
            t = t % q;
        }
    }
}