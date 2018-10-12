#include <iostream>
#include "BaseHelper.h"
#include "InstructionHelper.h"
#include <ctime>
#include <chrono>

#define COMPILATION_ERROR -1

/**
 * Main del programa, debe llamarse con parametros para ejecutar el programa
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::string fileDir, condicion;
        bool debug;
        if (argc >= 3) {
            fileDir = std::string(argv[argc - 1]);
            condicion = std::string(argv[1]); // si es -c
            debug = (std::string(argv[2]) == "-d");
            if (condicion == "-c") {
                // Etapa de lectura del archivo a compilar
                std::vector<std::string> instrucciones = InstructionHelper::getInstrucciones(fileDir);
                if (instrucciones.size() != 0) {
                    std::vector<TagsInfo> tags = InstructionHelper::getTagsAddress(instrucciones);
                    std::vector<std::string> instruccionesBin;
                    std::string instBinTemp;
                    if (debug) {
                        std::cout << "Debug: Tags Disponibles Inicio" << std::endl;
                        for (int i = 0; i < tags.size(); i++) {
                            std::cout << ">> Tag: " << tags[i].tag << std::endl;
                        }
                        std::cout << "Debug: Tags Disponibles Fin" << std::endl;
                    }
                    // Etapa de compilacion de ASM a BIN
                    auto tiempoInicio = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < instrucciones.size(); i++) {
                        try {
                            if (debug) {
                                std::cout << "Debug: Linea actual: " << instrucciones[i] << std::endl;
                            }
                            if (InstructionHelper::isThere(instrucciones[i], ':') == -1) {
                                if (debug) {
                                    std::cout << "Debug: Instruccion actual: " << instrucciones[i] << std::endl;
                                }
                                if (instrucciones[i][0] == 'B') { // caso de branches
                                    std::vector<std::string> instTemp = InstructionHelper::splitInst(instrucciones[i],
                                                                                                     ' ');
                                    int dirTag = InstructionHelper::isThere(tags, instTemp[1]);
                                    instTemp[1] = BaseHelper::decimalToBin(dirTag, 14);
                                    try {
                                        instruccionesBin.push_back(
                                                InstructionHelper::getInst(instTemp[0] + instTemp[1]));
                                    } catch (std::exception &e) {
                                        std::cout << ">> Etiqueta de branch no encontrado. Linea " << i + 1
                                                  << std::endl; //se indexa de 0 en adelante
                                        return COMPILATION_ERROR;
                                    }

                                } else {
                                    instBinTemp = InstructionHelper::getInst(instrucciones[i]);
                                    if (instBinTemp == "") {
                                        std::cout << ">> Error de sintaxis. Linea " << i + 1
                                                  << std::endl; //se indexa de 0 en adelante
                                        return COMPILATION_ERROR;
                                    }
                                    instruccionesBin.push_back(instBinTemp);
                                }
                            }
                        } catch (std::exception &e) {
                            std::cout << ">> Error en la instruccion, incompleta o erronea. Linea " << i + 1
                                      << std::endl; //se indexa de 0 en adelante
                            return COMPILATION_ERROR;
                        }
                    }
                    // Etapa de correccion de NOPs en Branch
                    std::vector<std::string> instBinBranches = InstructionHelper::fixBranches(instruccionesBin);
                    // Etapa de correccion de errores de datos
                    std::vector<std::string> instBinFull = InstructionHelper::fixDependenciaDatos(instBinBranches);
                    // Etapa de guardado
                    InstructionHelper::saveInstrucciones(instBinFull, fileDir);
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
        std::cout << "Ejemplo-> ./Compilador -c /Users/USUARIO/Documents/archivo.asm" << std::endl;
        std::cout << ">> Bandera -d para ver el log de la compilacion" << std::endl;
    }

    return 0;
}
