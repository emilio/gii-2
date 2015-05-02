// vim: set syntax=c:
#include "common.h"
#include "question.h"
#include "answer.h"
#include "exam.h"

int list_questions(int, char**);
int show_question(int argc, char** argv);

/// Manages questions
int question(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    varchar statement[255] = {0};
    EXEC SQL END DECLARE SECTION;

    if ( argc < 1 || argc > 4 )
        ARGUMENT_ERROR();

    /// Add a new question
    if ( strcmp(argv[0], "-a") == 0 ) {
        argc--; argv++;

        if ( argc != 1 )
            ARGUMENT_ERROR();

        COPY_TO_VARCHAR(statement, argv[0], 255);
        EXEC SQL INSERT INTO questions (statement) VALUES (:statement)
            RETURNING id INTO :id;

        APPCOM_RET_INT(id);

        // We return the id printing it
        printf("%d\n", id);
        return 0;
    // Delete
    } else if ( strcmp(argv[0], "-d") == 0 ) {
        argc--; argv++;

        if ( argc != 1 )
            ARGUMENT_ERROR();

        id = atoi(argv[0]);

        EXEC SQL DELETE FROM questions WHERE id = :id;
        return 0;
    // Update
    } else if ( strcmp(argv[0], "-e") == 0 ) {
        argc--; argv++;

        if ( argc != 2 )
            ARGUMENT_ERROR();

        id = atoi(argv[0]);
        COPY_TO_VARCHAR(statement, argv[1], 255);

        EXEC SQL UPDATE questions SET statement = :statement WHERE id = :id;

        return 0;
    // List
    } else if ( strcmp(argv[0], "-l") == 0 ) {
        argc--; argv++;

        return list_questions(argc, argv);
    } else if ( strcmp(argv[0], "-s") == 0 ) {
        argc--; argv++;

        return show_question(argc, argv);
    }

    ARGUMENT_ERROR();
}

/// Lists all questions
int list_questions(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    char statement[256] = {0};
    EXEC SQL END DECLARE SECTION;

    EXEC SQL WHENEVER SQLERROR DO handle_error();

    int count = 0;

    if ( argc > 0 )
        ARGUMENT_ERROR();

    EXEC SQL DECLARE questions_cursor CURSOR FOR
        SELECT id, statement FROM questions;

    EXEC SQL OPEN questions_cursor;
    while ( 1 ) {
        EXEC SQL FETCH questions_cursor INTO :id, :statement;
        if ( SQLCODE == NOT_FOUND )
            break;

        ++count;
        printf("%d\t%s\n", id, statement);
    }
    EXEC SQL CLOSE questions_cursor;

    printf("Total records: %d\n", count);

    return 0;
}

int show_question(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    char statement[256] = {0};
    EXEC SQL END DECLARE SECTION;

    if ( argc == 0 || argc > 1 )
        ARGUMENT_ERROR();

    id = atoi(argv[0]);

    EXEC SQL SELECT statement INTO :statement FROM questions WHERE id = :id;

    if ( SQLCODE == NOT_FOUND ) {
        fprintf(stderr, "Record not found: %d\n", id);
        return 1;
    }

    printf("%s\n", statement);
    printf("-----------------------------------\n");
    printf("Options:\n");

    CALL(answer, "-l", argv[0]);

    printf("Exams:\n");

    CALL(exam, "-l", argv[0]);

    return 0;
}
