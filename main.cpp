#include <iostream>
#include "BaseHelper.h"
#include "InstructionHelper.h"
#include <ctime>
#include <chrono>

#define COMPILATION_ERROR -1

/**
 * Imprime un vector
 * @tparam T
 * @param matrix
 */
template<class T>
void static printVector(std::vector<T> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        if (i == matrix.size() - 1) std::cout << matrix[i] << std::endl;
        else std::cout << matrix[i] << ", ";
    }
}

/**
 * Main del programa, debe llamarse con parametros para ejecutar el programa
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    std::cout << "\n==========================================================================" << std::endl;
    std::cout << "================= Compilador para la arquitectura ARMIPS =================" << std::endl;
    std::cout << "==========================================================================\n" << std::endl;

    if (argc != 1) {
        std::string fileDir, condicion;
        bool debug;
        int pc = 1;
        if (argc >= 3) {
            fileDir = std::string(argv[argc - 1]);
            condicion = std::string(argv[1]); // si es -c
            debug = (std::string(argv[2]) == "-d");
            if (condicion == "-c") {
                // Etapa de lectura del archivo a compilar
                std::vector<std::vector<std::string>> instruccionesLineas = InstructionHelper::getInstrucciones(
                        fileDir);
                std::vector<std::vector<std::string>> instrucciones = InstructionHelper::fixNOP(instruccionesLineas);
                if (debug) {
                    std::cout << "Debug: Instrucciones corregidas con NOP Inicio" << std::endl;
                    for (int i = 0; i < instrucciones.size(); i++) {
                        printVector(instrucciones[i]);
                    }
                    std::cout << "Debug: Instrucciones corregidas con NOP Fin" << std::endl;
                }
                if (instrucciones.size() != 0) {
                    std::vector<TagsInfo> tags = InstructionHelper::getTagsAddress(instrucciones);
                    std::vector<std::string> instruccionesBin;
                    std::string instBinTemp;
                    if (debug) {
                        std::cout << "Debug: Tags Disponibles Inicio" << std::endl;
                        for (int i = 0; i < tags.size(); i++) {
                            std::cout << ">> Tag: " << tags[i].tag << ", direccion: " << tags[i].numInstruccion
                                      << std::endl;
                        }
                        std::cout << "Debug: Tags Disponibles Fin" << std::endl;
                    }
                    // Etapa de compilacion de ASM a BIN
                    auto tiempoInicio = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < instrucciones.size(); i++) {
                        try {
                            if (debug) {
                                std::cout << "Debug: Linea actual: ";
                                printVector(instrucciones[i]);
                            }
                            if (InstructionHelper::isThere(instrucciones[i][0], ':') == -1) {
                                if (debug) {
                                    std::cout << "Debug: Instruccion actual: ";
                                    printVector(instrucciones[i]);
                                }
                                if (instrucciones[i][0][0] == 'B') { // caso de branches

                                    int dirTag = InstructionHelper::isThere(tags, instrucciones[i][1]);
                                    instrucciones[i][1] = BaseHelper::decimalToBin(dirTag, 27);
                                    try {
                                        instruccionesBin.push_back(InstructionHelper::getInst(instrucciones[i]));
                                    } catch (std::exception &e) {
                                        std::cout << ">> Etiqueta de branch no encontrado. Linea " << pc + 1
                                                  << std::endl; //se indexa de 0 en adelante
                                        return COMPILATION_ERROR;
                                    }

                                } else {
                                    instBinTemp = InstructionHelper::getInst(instrucciones[i]);
                                    if (instBinTemp == "") {
                                        std::cout << ">> Error de sintaxis. Linea " << pc + 1
                                                  << std::endl; //se indexa de 0 en adelante
                                        return COMPILATION_ERROR;
                                    }
                                    instruccionesBin.push_back(instBinTemp);
                                }
                                if (instrucciones[i][0] != "NOP") {
                                    pc++;
                                }
                            }
                        } catch (std::exception &e) {
                            std::cout << ">> Error en la instruccion, incompleta o erronea. Linea " << pc + 1
                                      << std::endl; //se indexa de 0 en adelante
                            return COMPILATION_ERROR;
                        }
                    }
                    InstructionHelper::saveInstrucciones(instruccionesBin, fileDir);
                    // output en pantalla
                    auto tiempoFinal = std::chrono::high_resolution_clock::now();
                    std::cout << ">> Compilacion realizada con exito, tiempo transcurrido "
                              << std::chrono::duration_cast<std::chrono::milliseconds>(
                                      tiempoFinal - tiempoInicio).count()
                              << " ms"
                              << std::endl;
                } else {
                    std::cout << ">> Error en la compilacion" << std::endl;
                }
            } else {
                std::cout << ">> Bandera de compilacion no activada" << std::endl;
            }
        } else {
            std::cout << ">> Directorio no especificado" << std::endl;
        }
    } else {
        std::cout << ">> Para compilar, debe indicar la bandera -c y la ruta del archivo .asm" << std::endl;
        std::cout << "\t Ejemplo-> ./Compilador -c /Users/USUARIO/Documents/archivo.asm" << std::endl;
        std::cout << ">> Bandera -d para ver el log de la compilacion" << std::endl;
        std::cout << "\t Ejemplo-> ./Compilador -c -d /Users/USUARIO/Documents/archivo.asm" << std::endl;

    }
    std::cout << "\n==========================================================================" << std::endl;
    std::cout << "==========================================================================\n" << std::endl;
    return 0;
}
