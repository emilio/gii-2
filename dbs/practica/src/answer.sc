// vim:set syntax=c:
#include "answer.h"
#include "common.h"

int list_answers(int argc, char** argv);

int answer(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    int is_correct = 0;
    int question_id;
    varchar title[255] = {0};
    EXEC SQL END DECLARE SECTION;

    if ( argc < 1 || argc > 4 )
        ARGUMENT_ERROR();

    /// Add a new question
    if ( strcmp(argv[0], "-a") == 0 ) {
        argc--; argv++;

        if ( argc < 2 || argc > 3 )
            ARGUMENT_ERROR();

        if ( argc == 3 ) {
            if ( strcmp(argv[2], "--correct") == 0 || strcmp(argv[2], "-c") == 0 )
                is_correct = 1;
            else
                ARGUMENT_ERROR();
        }

        question_id = atoi(argv[0]);

        COPY_TO_VARCHAR(title, argv[1], 255);
        EXEC SQL INSERT INTO answers (question_id, title, is_correct) VALUES (:question_id, :title, :is_correct)
            RETURNING id INTO :id;
        // We return the id printing it
        printf("%d\n", id);
        return 0;
    // Delete
    } else if ( strcmp(argv[0], "-d") == 0 ) {
        argc--; argv++;

        if ( argc != 1 )
            ARGUMENT_ERROR();

        id = atoi(argv[0]);

        EXEC SQL DELETE FROM answers WHERE id = :id;
        return 0;
    } else if ( strcmp(argv[0], "-l") == 0 ) {
        argc--; argv++;

        return list_answers(argc, argv);
    }

    ARGUMENT_ERROR();
}

int list_answers(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int question_id;
    int id;
    char title[256] = {0};
    short is_correct;
    EXEC SQL END DECLARE SECTION;

    if ( argc != 1 )
        ARGUMENT_ERROR();

    question_id = atoi(argv[0]);

    EXEC SQL DECLARE answers_cursor CURSOR FOR
        SELECT id, title, is_correct FROM answers
            WHERE question_id = :question_id
                ORDER BY priority ASC;

    EXEC SQL OPEN answers_cursor;
    while ( 1 ) {
        EXEC SQL FETCH answers_cursor INTO :id, :title, :is_correct;

        if ( SQLCODE == NOT_FOUND )
            break;

        printf(" [%c] %s (%d)\n", is_correct ? 'x' : ' ', title, id);
    }
    EXEC SQL CLOSE answers_cursor;
    return 1;
}
