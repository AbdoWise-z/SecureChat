//
// Created by xAbdoMo on 4/14/2024.
//

#include <gmpxx.h>
#include <iostream>
#include "util/BinaryInt.h"

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