// vim: set syntax=c:
#include "common.h"
#include "question.h"
#include "answer.h"
#include "exam.h"

int list_questions(int, char**);
int show_question(int argc, char** argv);
int list_questions_by_theme(int argc, char** argv);

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
    // List by theme
    } else if ( strcmp(argv[0], "-t") == 0 ) {
        argc--; argv++;

        return list_questions_by_theme(argc, argv);
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
    int theme_id;
    char statement[256] = {0};
    char question_id_str[20];
    EXEC SQL END DECLARE SECTION;
    unsigned char detailed_mode = 0;

    EXEC SQL WHENEVER SQLERROR DO handle_error();

    int count = 0;

    if ( argc > 2 )
        ARGUMENT_ERROR();

    if ( argc && strcmp(argv[argc - 1], "--detailed") == 0 )
        detailed_mode = 1;
    // If we passed two args and the detailed flag is not found...
    else if ( argc == 2 )
        ARGUMENT_ERROR();

    DEBUG("list_questions");

    EXEC SQL DECLARE questions_cursor CURSOR FOR
        SELECT id, statement FROM questions;

    EXEC SQL DECLARE questions_themes_cursor CURSOR FOR
        SELECT questions.id, questions.statement FROM questions, questions_themes WHERE questions_themes.question_id = questions.id AND questions_themes.theme_id = :theme_id;

    if ( argc == 0 || (argc == 1 && detailed_mode) ) {
        EXEC SQL OPEN questions_cursor;
    } else {
        theme_id = atoi(argv[0]);
        EXEC SQL OPEN questions_themes_cursor;
    }
    while ( 1 ) {
        if ( argc == 0 || (argc == 1 && detailed_mode) )
            EXEC SQL FETCH questions_cursor INTO :id, :statement;
        else
            EXEC SQL FETCH questions_themes_cursor INTO :id, :statement;

        if ( SQLCODE == NOT_FOUND )
            break;

        ++count;
        if ( detailed_mode ) {
            snprintf(question_id_str, sizeof(question_id_str), "%d", id);
            CALL(question, "-s", question_id_str);
        } else {
            printf("%d\t%s\n", id, statement);
        }
    }
    if ( argc == 0 || (argc == 1 && detailed_mode) )
        EXEC SQL CLOSE questions_cursor;
    else
        EXEC SQL CLOSE questions_themes_cursor;

    printf("Total records: %d\n", count);

    return 0;
}

int list_questions_by_theme(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int theme_id;
    int subject_id;
    int theme_priority;
    char theme_name[256] = {0};
    char theme_id_str[20];
    char subject_name[256] = {0};
    EXEC SQL END DECLARE SECTION;

    if ( argc > 1 )
        ARGUMENT_ERROR();

    EXEC SQL DECLARE themes_cursor CURSOR FOR
        SELECT themes.id, themes.priority, themes.name, subjects.id, subjects.name FROM themes, subjects WHERE themes.subject_id = subjects.id ORDER BY subjects.id, themes.priority;

    EXEC SQL OPEN themes_cursor;
    while ( 1 ) {
        EXEC SQL FETCH themes_cursor INTO :theme_id, :theme_priority, :theme_name, :subject_id, :subject_name;
        if ( SQLCODE == NOT_FOUND )
            break;

        printf("#%d - %d: %s (%s)\n", theme_id, theme_priority, theme_name, subject_name);

        snprintf(theme_id_str, sizeof(theme_id_str), "%d", theme_id);
        // argv[argc - 1] is required to be null
        CALL(question, "-l", theme_id_str, argv[0]);
    }

    EXEC SQL CLOSE themes_cursor;

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
