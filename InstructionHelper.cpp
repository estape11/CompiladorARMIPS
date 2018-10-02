//
// Created by Esteban Agüero Pérez  on 10/1/18.
//

#include <iostream>
#include "InstructionHelper.h"

/**
 * Obtiene el binario de una instruccion
 * @param inst
 * @return
 */
std::string InstructionHelper::getInst(std::string inst) {
    std::vector<std::string> partes = {};
    std::string temp = "";
    for (int i = 0; i < inst.length(); i++) {
        if (inst[i] == ',') {
            partes.push_back(temp);
            temp = "";
        } else if (i == inst.length() - 1) {
            temp += inst[i];
            partes.push_back(temp);
        } else if (inst[i] != ' ') {
            temp += inst[i];
        }
    }
    std::string instTemp, tempRd;
    if (getTipo(partes[0]) == INST_DATOS) {
        bool inmediato;
        // verificacion si posee inmediato y que que parte
        if (partes.size() > 2) { // en caso de no tener 3er registro inmediato
            inmediato = isInmediato(partes[2]);
        } else {
            inmediato = isInmediato(partes[1]);
        }
        if (inmediato && partes.size() > 2) { // caso de que tenga inmediato y que tenga 3 partes
            instTemp += getHeader(partes[0], inmediato);
            instTemp += getRegistro(partes[1]);
            tempRd = partes[0].substr(3, partes[0].length());
            instTemp += getRegistro(tempRd);
            instTemp += getInmediato(partes[2], 14);
        } else if (inmediato) { // caso de que tenga inmediato pero solo dos partes
            instTemp += getHeader(partes[0], inmediato);
            instTemp += BaseHelper::decimalToBin(0, 4); // no tiene Rn
            tempRd = partes[0].substr(3, partes[0].length());
            instTemp += getRegistro(tempRd);
            instTemp += getInmediato(partes[1], 14);
        } else { // caso de que del todo no tenga inmediato
            instTemp += getHeader(partes[0], inmediato);
            instTemp += getRegistro(partes[1]);
            tempRd = partes[0].substr(3, partes[0].length());
            instTemp += getRegistro(tempRd);
            if (partes.size() < 3) {
                instTemp += BaseHelper::decimalToBin(0, 14);
            } else {
                instTemp += getRegistro(partes[2]);
                instTemp += BaseHelper::decimalToBin(0, 10);
            }
        }
    } else if (getTipo(partes[0]) == INST_MEMORIA) {
        if (partes.size() > 2) { // caso de incluir el inmediato
            instTemp += getHeader(partes[0], false);
            instTemp += getRegistro(partes[1]);
            tempRd = partes[0].substr(3, partes[0].length());
            instTemp += getRegistro(tempRd);
            instTemp += getInmediato(partes[2], 14);
        } else { // caso de no especifiar inmediato, se asume un cero
            instTemp += getHeader(partes[0], false);
            instTemp += getRegistro(partes[1]);
            tempRd = partes[0].substr(3, partes[0].length());
            instTemp += getRegistro(tempRd);
            instTemp += BaseHelper::decimalToBin(0, 14);
        }
    } else if (getTipo(partes[0]) == INST_BRANCH) {
        instTemp += getHeader(partes[0], false);
    }

    return instTemp;
}

/**
 * Obtiene la cabecera de la instrucion, en el caso de branch, toda la instruccion
 * @param inst
 * @param inmediato
 * @return
 */
std::string InstructionHelper::getHeader(std::string inst, bool inmediato) {
    std::string instTemp = inst.substr(0, 3);
    std::string temp;
    for (int i = 0; i < cmdTipoDatos.size(); i++) {
        if (cmdTipoDatos[i] == instTemp) {
            temp += BaseHelper::decimalToBin(0, 3);
            temp += cmdTipoDatos[cmdTipoDatos.size() - 1];
            if (inmediato) {
                temp += '1';
            } else {
                temp += '0';
            }
            temp += BaseHelper::decimalToBin(i, 4);
            return temp;
        }
    }
    for (int i = 0; i < cmdTipoMemoria.size(); i++) {
        if (cmdTipoMemoria[i] == instTemp) {
            temp += BaseHelper::decimalToBin(0, 3);
            temp += cmdTipoMemoria[cmdTipoMemoria.size() - 1];
            temp += BaseHelper::decimalToBin(i, 2);
            temp += BaseHelper::decimalToBin(0, 3);
            return temp;
        }
    }
    temp = getBranch(inst);
    return temp;
}

/**
 * Obtiene el inmediato de una instruccion
 * @param inst
 * @param bits
 * @return
 */
std::string InstructionHelper::getInmediato(std::string inst, int bits) {
    std::string temp = "";
    if (inst[0] == '-') {
        temp = BaseHelper::decimalToBin(-1 * std::stoi(inst.substr(1, inst.length())), bits);
    } else {
        temp = BaseHelper::decimalToBin(std::stoi(inst), bits);
    }
    return temp;
}

/**
 * Verifica la instruccion tiene inmediato
 * @param inst
 * @return
 */
bool InstructionHelper::isInmediato(std::string inst) {
    return inst[0] != 'R';
}

/**
 * Obtiene el registro de una instruccion
 * @param reg
 * @return
 */
std::string InstructionHelper::getRegistro(std::string reg) {
    int numReg = std::stoi(reg.substr(1, reg.length()));
    return BaseHelper::decimalToBin(numReg, 4);
}

/**
 * Verifica que tipo de instruccion es
 * @param inst
 * @return
 */
int InstructionHelper::getTipo(std::string inst) {
    std::string instTemp = inst.substr(0, 3);
    //std::cout << instTemp << std::endl;
    std::string temp;
    for (int i = 0; i < cmdTipoDatos.size(); i++) {
        if (cmdTipoDatos[i] == instTemp) {
            return INST_DATOS;
        }
    }
    for (int i = 0; i < cmdTipoMemoria.size(); i++) {
        if (cmdTipoMemoria[i] == instTemp) {
            return INST_MEMORIA;
        }
    }
    if (instTemp[0] == 'B') {
        return INST_BRANCH;
    }
    return -1;
}

/**
 * Obtiene el branch especifico
 * @param inst
 * @return
 */
std::string InstructionHelper::getBranch(std::string inst) {
    std::string instTemp = inst.substr(0, 3);
    std::string temp;
    for (int i = 0; i < cmdTipoBranch.size(); i++) {
        if (cmdTipoBranch[i] == instTemp) {
            temp += BaseHelper::decimalToBin(i, 3);
            temp += cmdTipoBranch[cmdTipoBranch.size() - 1];
            temp += getInmediato(inst.substr(3, inst.length()), 27);
            break;
        }
    }
    if (temp == "") {
        temp += BaseHelper::decimalToBin(7, 3);
        temp += cmdTipoBranch[cmdTipoBranch.size() - 1];
        temp += getInmediato(inst.substr(1, inst.length()), 27);
    }
    return temp;
}


/**
 * Retorna un vector con las instrucciones leidas
 * @param fileDir
 * @return
 */
std::vector<std::string> InstructionHelper::getInstrucciones(std::string fileDir) {
    std::vector<std::string> tempInst;
    std::string temp;
    std::ifstream archivo; // archivo en modo in
    archivo.open(fileDir);
    if (archivo.is_open()) {
        while (getline(archivo, temp)) {
            if (temp[0] == '\t') {
                temp = temp.substr(1, temp.length());
            }
            tempInst.push_back(temp);
            temp = "";
        }
        archivo.close();
    } else {
        std::cout << ">> No se puede leer el archivo, verifique la ruta" << std::endl;
    }
    return tempInst;
}

std::vector<TagsInfo> InstructionHelper::getTagsAddress(std::vector<std::string> instrucciones) {
    std::vector<TagsInfo> tempTags;
    TagsInfo temp;
    int PC = 0;
    int size;
    for (int i = 0; i < instrucciones.size(); i++) {
        size = instrucciones[i].length(); // longitud de la instruccion i
        if (instrucciones[i][size - 1] == ':') {
            temp.numInstruccion = PC;
            temp.tag = instrucciones[i].substr(0, size - 1);
            tempTags.push_back(temp);
        } else {
            PC++;
        }
    }
    return tempTags;
}

std::vector<std::string> InstructionHelper::splitInst(std::string inst, char delimitador) {
    std::vector<std::string> partes = {};
    std::string temp = "";
    for (int i = 0; i < inst.length(); i++) {
        if (inst[i] == delimitador) {
            partes.push_back(temp);
            temp = "";
        } else if (i == inst.length() - 1) {
            temp += inst[i];
            partes.push_back(temp);
        } else {
            temp += inst[i];
        }
    }
    return partes;
}