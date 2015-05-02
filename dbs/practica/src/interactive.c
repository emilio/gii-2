#include "interactive.h"
#include "common.h"
#include "question.h"

int interactive(int argc, char** argv) {

    if ( argc != 0 )
        ARGUMENT_ERROR();

    CALL(question, "-l");

    // TODO: Print menu, choose option and act in consequence.
    return 0;
}
