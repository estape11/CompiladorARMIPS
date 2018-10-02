//
// Created by Esteban Agüero Pérez  on 10/1/18.
//

#ifndef COMPILADORARMIPS_INSTRUCTION_HELPER_H
#define COMPILADORARMIPS_INSTRUCTION_HELPER_H

#define INST_DATOS 0
#define INST_MEMORIA 1
#define INST_BRANCH 2

#include <stdio.h>
#include <string>
#include <vector>
#include "BaseHelper.h"


std::vector<std::string> static cmdTipoDatos = {"NOP", "SUB", "ADD", "MOV", "", "", "ORR", "CMP", "LSL", "LSR", "AVR",
                                                "CUM", "00"};
std::vector<std::string> static cmdTipoMemoria = {"STR", "LPX", "SPX", "LDR", "01"};
std::vector<std::string> static cmdTipoBranch = {"BEQ", "BNE", "BGE", "BLT", "BGT", "BLE", "", "B", "10"};

/**
 * Clase para manipular instrucciones
 */
class InstructionHelper {
public:

    std::string static getInst(std::string);

    std::string static getHeader(std::string, bool);

    bool static isInmediato(std::string);

    std::string static getInmediato(std::string, int);

    std::string static getRegistro(std::string);

    int static getTipo(std::string);

    std::string static getBranch(std::string inst);
};


#endif //COMPILADORARMIPS_INSTRUCTION_HELPER_H
