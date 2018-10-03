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
#include <iostream>
#include <fstream>

struct TagsInfo {
    int numInstruccion;
    std::string tag;
};

/**
 * Definicion de los tokens de las instrucciones
 */
std::vector<std::string> static cmdTipoDatos = {"NOP", "SUB", "ADD", "MOV", "", "", "ORR", "CMP", "LSL", "LSR", "AVR",
                                                "CUM", "00"};
std::vector<std::string> static cmdTipoMemoria = {"STR", "LPX", "SPX", "LDR", "01"};
std::vector<std::string> static cmdTipoBranch = {"BEQ", "BNE", "BGE", "BLT", "BGT", "BLE", "", "B", "10"};
std::vector<std::string> static cmdRdOper = {"0111", "00", "10"}; // Que usan Rd como operador, CMP STR SPX
//std::vector<std::string> static cmdDosOper = {"1010", "1011", "0011"}; // Que usan solo 2 operandos, AVR CUM MOV

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

    std::vector<std::string> static getInstrucciones(std::string);

    std::vector<TagsInfo> static getTagsAddress(std::vector<std::string>);

    std::vector<std::string> static splitInst(std::string, char);

    std::vector<std::string> static fixBranches(std::vector<std::string>);

    std::vector<std::string> static fixDependenciaDatos(std::vector<std::string>);

    template<class T>
    int static isThere(std::vector<T>, T);

    int static min(int, int);
};


#endif //COMPILADORARMIPS_INSTRUCTION_HELPER_H
