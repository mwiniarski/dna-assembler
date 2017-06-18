#include <iostream>

#include "Assembler.h"

int main(int argc, const char** argv) {

    if(argc != 3)
        return 0;

    Assembler a("out", atoi(argv[1]), atoi(argv[2]));
    a.run();
    return 1;
}
