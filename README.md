# CompiladorARMIPS
Compilador de archivos .asm para la arquitectura HybridARMIPS.

--- Instrucciones de compilacion ---

Crear carpeta build:

    > mkdir build

Ir a la carpeta recien creada:

    > cd build

*Elegir si compilar DEBUG o RELEASE

    > cmake ../ -DCMAKE_BUILD_TYPE=Release
o

    > cmake ../ -DCMAKE_BUILD_TYPE=Debug

Compilar:

    > make

--- Instrucciones para ejecutar ---

Ir a la carpeta build (sino se estaba en ella)

    > cd build

Ejecutar el archivo ejecutable con sus respectivos parametros

    > ./CompiladorARMIPS -c FILE/PATH