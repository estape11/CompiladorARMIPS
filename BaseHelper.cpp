//
// Created by Esteban Agüero Pérez  on 10/1/18.
//

#include "BaseHelper.h"

/**
 * Convierte un binario a decimal
 * @param bin
 * @return
 */
int BaseHelper::binToDecimal(std::string bin) {
    int temp = 0;
    int index;
    for (int i = bin.length() - 1; i >= 0; i--) {
        index = bin.length() - 1 - i;
        temp += (bin[i] - 48) * pow(2, index); // desfasa la conversion char -> int
    }
    return temp;
}

/**
 * Convierte un valor decimal a binario
 * @param decimal
 * @return
 */
std::string BaseHelper::decimalToBin(int decimal) {
    std::string temp = "";
    std::string val;
    if (decimal == 0) {
        return "00";
    } else if (decimal < 0) {
        decimal *= -1;
        decimal -= 1;
        while (decimal > 0) {
            if (decimal % 2) {
                val = '0';
            } else {
                val = '1';
            }
            val += temp;
            temp = val;
            decimal /= 2;
        }
        val = '1';
        val += temp;
        temp = val;
    } else {
        while (decimal > 0) {
            val = (decimal % 2 + 48);
            val += temp;
            temp = val;
            decimal /= 2;
        }
        val = '0';
        val += temp;
        temp = val;
    }
    return temp;
}

/**
 * Conversionde binario a decimal especificando la cantidad de bits
 * @param bin
 * @param bits
 * @return
 */
int BaseHelper::binToDecimal(std::string bin, int bits) {
    return binToDecimal(bin); // cambiar esto para verificar si es negativo y en complemento
}

/**
 * Convierte de decimal a binario de una cantidad de bits
 * @param decimal
 * @param bits
 * @return
 */
std::string BaseHelper::decimalToBin(int decimal, int bits) {
    std::string val, temp;
    val = decimalToBin(decimal);
    int diferencia = bits - val.length(); //por el bit de signo
    if (diferencia <= 0) {
        temp = val.substr(val.length() - bits, val.length());
    } else {
        temp = getExtendN(val[0], diferencia, val);
    }
    return temp;
}

/**
 * Se extiende el dato con caracter dado n veces
 * @param caracter
 * @param n
 * @param dato
 * @return
 */
std::string BaseHelper::getExtendN(char caracter, int n, std::string dato) {
    std::string temp(n, caracter);
    temp += dato;
    return temp;
}