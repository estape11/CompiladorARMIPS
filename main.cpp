#include <iostream>
#include "BaseHelper.h"
#include "InstructionHelper.h"

int main() {
    int nueve = 0b01001;
    std::string t(2, '0');
    std::cout << "Inst ADD R0, R1, R2: " << InstructionHelper::getInst("AVR R1,R2") << std::endl;
    std::cout << "Header para ADDR1 sin inmediato " << InstructionHelper::getHeader("STRR1", false) << std::endl;
    std::cout << "Esto es " << BaseHelper::getExtendN('0', 10, "1010") << std::endl;
    std::cout << "-10 es " << BaseHelper::decimalToBin(-10) << std::endl;
    return 0;
}