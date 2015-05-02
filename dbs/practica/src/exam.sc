// vim: set syntax=c:
#include "exam.h"
#include "common.h"

int list_exams(int argc, char** argv);

int exam(int argc, char** argv) {
   if ( argc != 2 )
        ARGUMENT_ERROR();

    if ( strcmp(argv[0], "-l") == 0 ) {
        argc--; argv++;
        return list_exams(argc, argv);
    }

    ARGUMENT_ERROR();
}

int list_exams(int argc, char** argv) {
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


    if ( argc != 1 )
        ARGUMENT_ERROR();

    int count = 0;
    question_id = atoi(argv[0]);

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
