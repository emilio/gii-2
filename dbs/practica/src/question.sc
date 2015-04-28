// vim: set syntax=c:
#include "macros.h"
#include "question.h"
#include "answer.h"

int list_questions(int, char**);

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

    char* args[] = { "-l", argv[0], NULL };

    answer(2, args);

    printf("Exams:\n");

    show_exams_for_question(id);

    return 0;
}

int show_exams_for_question(int _qid) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL BEGIN DECLARE SECTION;
    int question_id;
    int id;
    int exam_year;
    int convocatory;
    int correct;
    int incorrect;
    int unreplied;

    char subject_name[256] = {0};
    EXEC SQL END DECLARE SECTION;


    int count = 0;
    question_id = _qid;

    EXEC SQL DECLARE exams_cursor CURSOR FOR
        SELECT exams.id, exams.year, exams.convocatory, subjects.name, exams_questions.correct_answer_count, exams_questions.incorrect_answer_count, exams_questions.unreplied_answer_count
        FROM exams, exams_questions, subjects
            WHERE exams_questions.question_id = :question_id AND
                  exams_questions.exam_id = exams.id AND subjects.id = exams.subject_id
                ORDER BY exams.year, exams.convocatory ASC;

    EXEC SQL OPEN exams_cursor;
    while ( 1 ) {
        EXEC SQL FETCH exams_cursor INTO :id, :exam_year, :convocatory, :subject_name, :correct, :incorrect, :unreplied;

        if ( SQLCODE == NOT_FOUND )
            break;

        ++count;

        printf("%d %s (%d)\n", exam_year, subject_name, convocatory);
        printf(" - Total: %d\n", correct + incorrect + unreplied);
        printf(" - Correct: %d\n", correct);
        printf(" - Incorrect: %d\n", incorrect);
        printf(" - Unreplied: %d\n", unreplied);
    }
    EXEC SQL CLOSE exams_cursor;

    printf("Total: %d\n", count);

    return 0;
}
