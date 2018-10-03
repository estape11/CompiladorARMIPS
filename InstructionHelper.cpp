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

/**
 * Obtiene los label dentro de las instrucciones
 * @param instrucciones
 * @return
 */
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

/**
 * Divide las instrucciones con un delimitador
 * @param inst
 * @param delimitador
 * @return
 */
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

/**
 * Agrega NOPs para solucionar el riesgo del branch
 * @param inst
 * @return
 */
std::vector<std::string> InstructionHelper::fixBranches(std::vector<std::string> inst) {
    std::vector<std::string> tempFix;
    std::string nop = BaseHelper::decimalToBin(0, 32);
    int tipoInst;
    for (int i = 0; i < inst.size(); i++) {
        tipoInst = BaseHelper::binToDecimal(inst[i].substr(3, 2));
        if (tipoInst == 2 && i != (inst.size() - 1)) {
            tempFix.push_back(inst[i]);
            // se agregan 4 nops a branch
            tempFix.push_back(nop);
            tempFix.push_back(nop);
            tempFix.push_back(nop);
            tempFix.push_back(nop);
        } else {
            tempFix.push_back(inst[i]);
        }
    }
    return tempFix;
}

/**
 * Agrega NOPs para reparar la dependencia de datos
 * @param inst
 * @return
 */
std::vector<std::string> InstructionHelper::fixDependenciaDatos(std::vector<std::string> inst) {
    std::vector<std::string> tempFix;
    std::vector<std::string> registrosDestinoUso = {"", "", "", ""}; // max 4 -> if >4 -> pop_back()
    std::string nop = BaseHelper::decimalToBin(0, 32);
    std::string rd, rs, rn;
    int rdIndex, rsIndex, rnIndex; // para saber en que posicion esta el registro destino en uso y calcular nops
    int tipoInst;
    for (int i = 0; i < inst.size(); i++) {
        // para monitorear los registros usados
        //for (int j = 0; j < registrosDestinoUso.size(); j++) {
        //    std::cout << registrosDestinoUso[j] << " ,";
        //}
        //std::cout << std::endl;
        tipoInst = BaseHelper::binToDecimal(inst[i].substr(3, 2));
        if (inst[i] == nop) {
            tempFix.push_back(inst[i]);
            registrosDestinoUso.pop_back();
            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
        } else if (tipoInst == 0) {
            rn = inst[i].substr(10, 4);
            rd = inst[i].substr(14, 4);
            if (inst[i][5] == '1') { // inmediato
                if (isThere(cmdRdOper, inst[i].substr(6, 4)) != -1) { // Caso CMP
                    rdIndex = isThere(registrosDestinoUso, rd);
                    if (rdIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - rdIndex); k++) { // la cantidad de nops
                            tempFix.push_back(nop);
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                        }
                    }
                    tempFix.push_back(inst[i]);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                } else {
                    rnIndex = isThere(registrosDestinoUso, rn);
                    if (rnIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - rnIndex); k++) { // la cantidad de nops
                            tempFix.push_back(nop);
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                        }
                    }
                    tempFix.push_back(inst[i]);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
                }
            } else { // sin inmediato
                rs = inst[i].substr(18, 4);
                if (isThere(cmdRdOper, inst[i].substr(6, 4)) != -1) { // Caso CMP
                    rdIndex = isThere(registrosDestinoUso, rd);
                    rnIndex = isThere(registrosDestinoUso, rn);
                    if (rdIndex != -1 || rnIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - min(rdIndex, rnIndex)); k++) { // la cantidad de nops
                            tempFix.push_back(nop);
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                        }
                    }
                    tempFix.push_back(inst[i]);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                } else {
                    rnIndex = isThere(registrosDestinoUso, rn);
                    rsIndex = isThere(registrosDestinoUso, rs);
                    if (rnIndex != -1 || rsIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - min(rnIndex, rsIndex)); k++) { // la cantidad de nops
                            tempFix.push_back(nop);
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                        }
                    }
                    tempFix.push_back(inst[i]);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
                }
            }
        } else if (tipoInst == 1) {
            rn = inst[i].substr(10, 4);
            rd = inst[i].substr(14, 4);
            if (isThere(cmdRdOper, inst[i].substr(5, 2)) != -1) { // Caso STR o SPX
                rdIndex = isThere(registrosDestinoUso, rd);
                rnIndex = isThere(registrosDestinoUso, rn);
                if (rdIndex != -1 || rnIndex != -1) { // en caso que algun Rd este en usos
                    for (int k = 0; k < (3 - min(rdIndex, rnIndex)); k++) { // la cantidad de nops
                        tempFix.push_back(nop);
                        registrosDestinoUso.pop_back();
                        registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                    }
                }
                tempFix.push_back(inst[i]);
                registrosDestinoUso.pop_back();
                registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
            } else {
                rnIndex = isThere(registrosDestinoUso, rn);
                if (rnIndex != -1) { // en caso que algun Rd este en uso
                    for (int k = 0; k < (3 - rnIndex); k++) { // la cantidad de nops
                        tempFix.push_back(nop);
                        registrosDestinoUso.pop_back();
                        registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
                    }
                }
                tempFix.push_back(inst[i]);
                registrosDestinoUso.pop_back();
                registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
            }
        } else {
            tempFix.push_back(inst[i]);
            registrosDestinoUso.pop_back();
            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
        }
    }
    return tempFix;
}

/**
 * Para saber si hay un elemento especifico dentro del vector
 * @tparam T
 * @param vector
 * @param dato
 * @return
 */
template<class T>
int InstructionHelper::isThere(std::vector<T> vector, T dato) {
    int temp = -1;
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == dato) {
            temp = i;
            break;
        }
    }
    return temp;
}

/**
 * Compara solo si son positivos
 * @param a
 * @param b
 * @return
 */
int InstructionHelper::min(int a, int b) {
    if (a < 0) {
        return b;
    } else if (b < 0) {
        return a;
    } else if (a <= b) {
        return a;
    } else {
        return b;
    }
}