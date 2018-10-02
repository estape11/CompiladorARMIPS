#include <iostream>
#include "BaseHelper.h"
#include "InstructionHelper.h"

void pruebasInst() {
    //int nueve = 0b01001;
    std::string t(2, '0');
    std::cout << "Inst ADD R0, R1, R2: " << InstructionHelper::getInst("AVR R1,R2") << std::endl;
    std::cout << "Header para ADDR1 sin inmediato " << InstructionHelper::getHeader("STRR1", false)
              << std::endl;
    std::cout << "Esto es " << BaseHelper::getExtendN('0', 10, "1010") << std::endl;
    std::cout << "-10 es " << BaseHelper::decimalToBin(-10) << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::string fileDir, condicion;
        if (argc == 3) {
            fileDir = std::string(argv[argc - 1]);
            condicion = std::string(argv[1]); // si es -c
            if (condicion == "-c") {
                std::vector<std::string> instrucciones = InstructionHelper::getInstrucciones(fileDir);
                std::vector<TagsInfo> tags = InstructionHelper::getTagsAddress(instrucciones);
                std::vector<std::string> instruccionesBin;
                for (int i = 0; i < instrucciones.size(); i++) {
                    if (instrucciones[i][instrucciones[i].length() - 1] != ':') {
                        if (instrucciones[i][0] == 'B') { // caso de branches
                            std::vector<std::string> instTemp = InstructionHelper::splitInst(instrucciones[i], ' ');
                            for (int j = 0; j < tags.size(); j++) { // busco la direccion del tag
                                if (tags[j].tag == instTemp[1]) {
                                    instTemp[1] = std::to_string(tags[j].numInstruccion);
                                    break;
                                }
                            }
                            instruccionesBin.push_back(InstructionHelper::getInst(instTemp[0] + instTemp[1]));

                        } else {
                            instruccionesBin.push_back(InstructionHelper::getInst(instrucciones[i]));
                        }
                    }
                }
                for (int i = 0; i < instruccionesBin.size(); i++) {
                    std::cout << instruccionesBin[i] << std::endl;
                }
            } else {
                std::cout << ">> Bandera de compilacion no activada" << std::endl;
            }
        } else {
            std::cout << ">> Directorio no especificado" << std::endl;
        }
    }

    return 0;
}