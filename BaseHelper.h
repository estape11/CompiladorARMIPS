//
// Created by Esteban Agüero Pérez  on 10/1/18.
//

#ifndef COMPILADORARMIPS_BASEHELPER_H
#define COMPILADORARMIPS_BASEHELPER_H

#include <string>
#include <math.h>
#include <iostream>

/**
 * Clase para realizar conversiones de base
 */
class BaseHelper {
public:
    int static binToDecimal(std::string);

    std::string static decimalToBin(int);

    std::string static getExtendN(char, int, std::string);

    int static binToDecimal(std::string, int);

    std::string static decimalToBin(int, int);
};


#endif //COMPILADORARMIPS_BASEHELPER_H
