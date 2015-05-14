#include "questions_themes.h"
#include "common.h"

int questions_themes(int argc, char** argv) {
    EXEC SQL BEGIN DECLARE SECTION;
    int question_id;
    int theme_id;
    EXEC SQL END DECLARE SECTION;
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    if ( argc != 3 )
        ARGUMENT_ERROR();

    if ( strcmp(argv[0], "-a") == 0 ) {
        argc--; argv++;

        question_id = atoi(argv[0]);
        theme_id = atoi(argv[1]);

        EXEC SQL INSERT INTO questions_themes (question_id, theme_id)
            VALUES (:question_id, :theme_id);
        return 0;
    }

    ARGUMENT_ERROR();
}
