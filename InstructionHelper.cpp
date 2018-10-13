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
std::string InstructionHelper::getInst(std::vector<std::string> inst) {
    std::string instTemp;
    if (inst[0] == "NOP") {
        instTemp = BaseHelper::decimalToBin(0, 32);
    } else if (getTipo(inst[0]) == INST_DATOS) {
        bool inmediato;
        // verificacion si posee inmediato y que que parte
        if (inst.size() > 3) { // en caso de no tener 3er registro inmediato
            inmediato = isInmediato(inst[3]);
        } else {
            inmediato = isInmediato(inst[2]);
        }
        if (inmediato && inst.size() > 3) { // caso de que tenga inmediato y que tenga 4 inst
            instTemp += getHeader(inst[0], inmediato);
            instTemp += getRegistro(inst[2]);
            instTemp += getRegistro(inst[1]);
            instTemp += getInmediato(inst[3], 14);
        } else if (inmediato) { // caso de que tenga inmediato pero solo 3 inst
            instTemp += getHeader(inst[0], inmediato);
            instTemp += BaseHelper::decimalToBin(0, 4); // no tiene Rn
            instTemp += getRegistro(inst[1]);
            instTemp += getInmediato(inst[2], 14);
        } else { // caso de que del to-do no tenga inmediato
            instTemp += getHeader(inst[0], inmediato);
            instTemp += getRegistro(inst[2]);
            instTemp += getRegistro(inst[1]);
            if (inst.size() < 4) {
                instTemp += BaseHelper::decimalToBin(0, 14);
            } else {
                instTemp += getRegistro(inst[3]);
                instTemp += BaseHelper::decimalToBin(0, 10);
            }
        }
    } else if (getTipo(inst[0]) == INST_MEMORIA) {
        if (inst.size() > 3) { // caso de incluir el inmediato
            instTemp += getHeader(inst[0], false);
            instTemp += getRegistro(inst[2]);
            instTemp += getRegistro(inst[1]);
            instTemp += getInmediato(inst[3], 14);
        } else { // caso de no especifiar inmediato, se asume un cero
            instTemp += getHeader(inst[0], false);
            instTemp += getRegistro(inst[2]);
            instTemp += getRegistro(inst[1]);
            instTemp += BaseHelper::decimalToBin(0, 14);
        }
    } else if (getTipo(inst[0]) == INST_BRANCH) {
        instTemp += getHeader(inst[0], false);
        instTemp += inst[1];
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
    std::string condiTemp;
    std::string temp;
    int condIndex;
    for (int i = 0; i < cmdTipoDatos.size(); i++) {
        if (cmdTipoDatos[i] == instTemp) {
            condiTemp = inst.substr(3, inst.length());
            condIndex = isThere(condTipoDatos, condiTemp);
            if (condIndex != -1) {
                temp += BaseHelper::decimalToBin(condIndex, 3);
            } else {
                temp += BaseHelper::decimalToBin(7, 3); // incondicional
            }
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
            //temp += BaseHelper::decimalToBin(0, 3);
            condiTemp = inst.substr(3, inst.length());
            condIndex = isThere(condTipoDatos, condiTemp);
            if (condIndex != -1) {
                temp += BaseHelper::decimalToBin(condIndex, 3);
            } else {
                temp += BaseHelper::decimalToBin(7, 3); // incondicional
            }
            temp += cmdTipoMemoria[cmdTipoMemoria.size() - 1];
            temp += BaseHelper::decimalToBin(i, 2);
            temp += BaseHelper::decimalToBin(0, 3);
            return temp;
        }
    }
    int branch = isThere(cmdTipoBranch, inst);
    if (branch != -1) {
        temp += BaseHelper::decimalToBin(branch, 3);
        temp += cmdTipoBranch[cmdTipoBranch.size() - 1];
    }
    return temp;
}

/**
 * Obtiene el inmediato de una instruccion
 * @param inst
 * @param bits
 * @return
 */
std::string InstructionHelper::getInmediato(std::string inst, int bits) {
    std::string temp;
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
 * Retorna un vector con las instrucciones leidas
 * @param fileDir
 * @return
 */
std::vector<std::vector<std::string>> InstructionHelper::getInstrucciones(std::string fileDir) {
    std::vector<std::string> tempInst;
    std::string temp;
    std::ifstream archivo; // archivo en modo in
    archivo.open(fileDir);
    if (archivo.is_open()) {
        while (getline(archivo, temp)) {
            if (temp[0] == '\t') { // omite tabs de inicio
                temp = temp.substr(1, temp.length());
            }
            if (temp[temp.length() - 1] == '\r') {
                temp = temp.substr(0, temp.length() - 1);
            }
            temp = removeComentarios(temp);// elimina los comentarios
            if (temp != "") {
                tempInst.push_back(temp);
                temp = "";
            }
        }
        archivo.close();
    } else {
        std::cout << ">> No se puede leer el archivo, verifique la ruta" << std::endl;
    }
    return splitInst(tempInst);
}

std::string InstructionHelper::removeComentarios(std::string inst) {
    std::string temp;
    for (int i = 0; i < inst.length(); i++) {
        if (inst[i] == '#') {
            break;
        } else {
            if (inst[i] != '\n') {
                temp += inst[i];
            }
        }
    }
    return temp;
}

/**
 * Obtiene los label dentro de las instrucciones
 * @param instrucciones
 * @return
 */
std::vector<TagsInfo> InstructionHelper::getTagsAddress(std::vector<std::vector<std::string>> instrucciones) {
    std::vector<TagsInfo> tempTags;
    TagsInfo temp;
    int PC = 0;
    int index;
    for (int i = 0; i < instrucciones.size(); i++) {
        index = isThere(instrucciones[i][0], ':');
        if (index != -1) {
            temp.numInstruccion = PC;
            temp.tag = getTag(instrucciones[i][0].substr(0, index));
            tempTags.push_back(temp);
        } else {
            PC++;
        }
    }
    return tempTags;
}

std::vector<std::vector<std::string>> InstructionHelper::splitInst(std::vector<std::string> instrucciones) {
    std::vector<std::string> partes = {};
    std::vector<std::vector<std::string>> tempFull;
    std::string temp = "";
    for (int i = 0; i < instrucciones.size(); i++) {
        partes = {};
        temp = "";
        for (int j = 0; j < instrucciones[i].length(); j++) {
            if ((instrucciones[i][j] == ',' || instrucciones[i][j] == ' ' || instrucciones[i][j] == '#') &&
                temp != "") {
                partes.push_back(temp);
                temp = "";
            } else if (j == instrucciones[i].length() - 1) {
                temp += instrucciones[i][j];
                partes.push_back(temp);
            } else if (instrucciones[i][j] != ' ') {
                temp += instrucciones[i][j];
            }
        }
        if (!partes.empty()) {
            tempFull.push_back(partes);
        }
    }
    return tempFull;
}

/**
 * Agrega NOPs para solucionar la dependencia de datos
 * @param instrucciones
 * @return
 */
std::vector<std::vector<std::string>> InstructionHelper::fixNOP(std::vector<std::vector<std::string>> instrucciones) {
    std::vector<std::vector<std::string>> tempFix;
    std::vector<std::string> registrosDestinoUso = {"PAD", "PAD", "PAD",
                                                    "PAD"}; // max 4 -> if >4 -> pop_back(), rota
    std::vector<std::string> inst, ultimoTag;
    std::string rd, rs, rn;
    bool tag = false;
    int rdIndex, rsIndex, rnIndex; // para saber en que posicion esta el registro destino en uso y calcular nops
    int tipoInst;
    for (int i = 0; i < instrucciones.size(); i++) {
        // para monitorear los registros usados
        //for (int j = 0; j < registrosDestinoUso.size(); j++) {
        //    std::cout << registrosDestinoUso[j] << " ,";
        //}
        //std::cout << std::endl;
        inst = instrucciones[i];
        tipoInst = getTipo(inst[0]);
        if (inst[0] == "NOP") {
            tempFix.push_back(inst);
            registrosDestinoUso.pop_back();
            registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
        } else if (tipoInst == INST_DATOS) {
            rn = getTag(inst[2]);
            rd = getTag(inst[1]);
            if (instrucciones[i].size() <= 3) { // inmediato
                if (isThere(casoRdOper, inst[0]) != -1) { // Caso CMP
                    rdIndex = isThere(registrosDestinoUso, rd);
                    if (rdIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - rdIndex); k++) { // la cantidad de nops
                            tempFix.push_back({"NOP"});
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                        }
                    }
                    if (tag) {
                        tempFix.push_back(ultimoTag);
                        tag = false;
                    }
                    tempFix.push_back(inst);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                } else {
                    rnIndex = isThere(registrosDestinoUso, rn);
                    if (rnIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - rnIndex); k++) { // la cantidad de nops
                            tempFix.push_back({"NOP"});
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                        }
                    }
                    if (tag) {
                        tempFix.push_back(ultimoTag);
                        tag = false;
                    }
                    tempFix.push_back(inst);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
                }
            } else { // sin inmediato
                rs = getTag(inst[3]);
                if (isThere(casoRdOper, inst[0]) != -1) { // Caso CMP
                    rdIndex = isThere(registrosDestinoUso, rd);
                    rnIndex = isThere(registrosDestinoUso, rn);
                    if (rdIndex != -1 || rnIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - min(rdIndex, rnIndex)); k++) { // la cantidad de nops
                            tempFix.push_back({"NOP"});
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                        }
                    }
                    if (tag) {
                        tempFix.push_back(ultimoTag);
                        tag = false;
                    }
                    tempFix.push_back(inst);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                } else {
                    rnIndex = isThere(registrosDestinoUso, rn);
                    rsIndex = isThere(registrosDestinoUso, rs);
                    if (rnIndex != -1 || rsIndex != -1) { // en caso que algun Rd este en uso
                        for (int k = 0; k < (3 - min(rnIndex, rsIndex)); k++) { // la cantidad de nops
                            tempFix.push_back({"NOP"});
                            registrosDestinoUso.pop_back();
                            registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                        }
                    }
                    if (tag) {
                        tempFix.push_back(ultimoTag);
                        tag = false;
                    }
                    tempFix.push_back(inst);
                    registrosDestinoUso.pop_back();
                    registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
                }
            }
        } else if (tipoInst == INST_MEMORIA) {
            rn = getTag(inst[2]);
            rd = getTag(inst[1]);
            if (isThere(casoRdOper, inst[0]) != -1) { // Caso STR o SPX
                rdIndex = isThere(registrosDestinoUso, rd);
                rnIndex = isThere(registrosDestinoUso, rn);
                if (rdIndex != -1 || rnIndex != -1) { // en caso que algun Rd este en usos
                    for (int k = 0; k < (3 - min(rdIndex, rnIndex)); k++) { // la cantidad de nops
                        tempFix.push_back({"NOP"});
                        registrosDestinoUso.pop_back();
                        registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                    }
                }
                if (tag) {
                    tempFix.push_back(ultimoTag);
                    tag = false;
                }
                tempFix.push_back(inst);
                registrosDestinoUso.pop_back();
                registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
            } else {
                rnIndex = isThere(registrosDestinoUso, rn);
                if (rnIndex != -1) { // en caso que algun Rd este en uso
                    for (int k = 0; k < (3 - rnIndex); k++) { // la cantidad de nops
                        tempFix.push_back({"NOP"});
                        registrosDestinoUso.pop_back();
                        registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
                    }
                }
                if (tag) {
                    tempFix.push_back(ultimoTag);
                    tag = false;
                }
                tempFix.push_back(inst);
                registrosDestinoUso.pop_back();
                registrosDestinoUso.insert(registrosDestinoUso.begin(), rd);
            }
        } else if (tipoInst == INST_BRANCH) {
            if (tag) {
                tempFix.push_back(ultimoTag);
                tag = false;
            }
            tempFix.push_back(inst);
            registrosDestinoUso.pop_back();
            registrosDestinoUso.insert(registrosDestinoUso.begin(), "");
            for (int k = 0; k < 4; k++) { // la cantidad de nops despues del Branch
                tempFix.push_back({"NOP"});
                registrosDestinoUso.pop_back();
                registrosDestinoUso.insert(registrosDestinoUso.begin(), "PAD");
            }
        } else {
            //tempFix.push_back(inst);
            ultimoTag = inst;
            tag = true;
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
 * Para saber si hay un elemento especifico dentro del vector
 * @tparam T
 * @param vector
 * @param dato
 * @return
 */
int InstructionHelper::isThere(std::string dato, char letra) {
    int temp = -1;
    for (int i = 0; i < dato.size(); i++) {
        if (dato[i] == letra) {
            temp = i;
            break;
        }
    }
    return temp;
}

/**
 * Para saber si hay un elemento especifico dentro del vector de tags
 * @tparam T
 * @param vector
 * @param dato
 * @return
 */
int InstructionHelper::isThere(std::vector<TagsInfo> vector, std::string tag) {
    int temp = -1;
    std::string tempTag, tagLimpio;
    tagLimpio = getTag(tag);
    for (int i = 0; i < vector.size(); i++) {
        tempTag = vector[i].tag;
        if (tempTag == tagLimpio) {
            temp = vector[i].numInstruccion;
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

/**
 * Limpia el tag
 * @param a
 * @param b
 * @return
 */
std::string InstructionHelper::getTag(std::string tag) {
    std::string temp;
    for (int i = 0; i < tag.size(); i++) {
        if (tag[i] != ' ' && tag[i] != '\t' && tag[i] != '\n' && tag[i] != ':') {
            temp += tag[i];
        }
    }
    return temp;
}

/**
 * Guarda las instrucciones en binario en un archivo
 * @param inst
 * @param dir
 * @return
 */
bool InstructionHelper::saveInstrucciones(std::vector<std::string> inst, std::string dir) {
    std::string dirTemp;
    bool temp = false;
    for (int i = dir.length() - 1; i >= 0; i--) {
        if (dir[i] == '.') {
            dirTemp = dir.substr(0, i);
            break;
        }
    }
    dirTemp += ".bin"; // extension del archivo compilado
    std::remove(dirTemp.c_str());
    std::ofstream archivo(dirTemp);
    if (archivo.is_open()) {
        for (int i = 0; i < inst.size(); i++) {
            if (i == inst.size() - 1) {
                archivo << inst[i];
            } else {
                archivo << inst[i] << std::endl;
            }
        }
    }
    return temp;
}