#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <commands.h>

char SQLSTATE[6] = { 0 };
long SQLCODE = 0;

// Our executors
int create_question(int, char**);
int list_questions(int, char**);
int show_question(int, char**);

// The list of our supported commands
struct command commands[] = {
    { "new",  create_question, "create a new question",
        "Prompts for question data" },
    { "list", list_questions,  "list all questions",
        "Lists all questions, no arguments required" },
    { "show", show_question,   "shows a question",
        "Show a question, requires the id of the question "
        "as a second parameter" },
    { NULL, NULL, NULL, NULL } // End of the list
};


// TODO: Recover graciously
void handle_error() {
    fprintf(stderr, "ERROR: SQLstate: %s, code: %ld\n", SQLSTATE, SQLCODE);
    exit(1);
}


int create_question(int argc, char** argv) {
    printf("Creating a new question:\n");

    printf("new\n");
    return 1;
}

int list_questions(int argc, char** argv) {
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    char statement[256] = {0};
    EXEC SQL END DECLARE SECTION; 
    int count = 0;

    if ( argc > 0 ) {
        fprintf(stderr, "Unexpected argument: %s\n", argv[0]);
        return 1;
    }

    EXEC SQL DECLARE CURSOR questions_cursor FOR
        SELECT id, statement FROM questions;

    EXEC SQL OPEN questions_cursor;
    while ( 1 ) {
        EXEC SQL FETCH questions_cursor INTO :id, :statement;
        if ( SQLCODE == SQLE_NOTFOUND )
            break;
        
        ++count;
        printf("%d\t%s\n", id, statement);
    }
    EXEC SQL CLOSE questions_cursor;

    printf("Total records: %d\n", count);

    return 0;
}

void show_answers_for_question(int _qid) {
    EXEC SQL BEGIN DECLARE SECTION;
    int question_id;
    int id;
    char title[256] = {0};
    char is_corrrect;
    EXEC SQL END DECLARE SECTION; 
    id = question_id;
    
    EXEC SQL DECLARE CURSOR answers_cursor FOR
        SELECT id, title, is_correct FROM answers
            WHERE question_id = :question_id
                ORDER BY priority DESC;

    EXEC SQL OPEN answers_cursor;
    while ( 1 ) {
        EXEC SQL FETCH answers_cursor INTO :id, :title, :is_correct;
        
        if ( SQLCODE == SQLE_NOTFOUND )
            break;

        printf(" [%c] %s (%d)\n", is_correct ? 'x' : ' ', title, id);
    }
    EXEC SQL CLOSE answers_cursor;

    return 0;
}

void show_exams_for_question(int _qid) {
    EXEC SQL BEGIN DECLARE SECTION;
    int id,
        year,
        convocatory, 
        correct,
        incorrect,
        unreplied;
    
    char subject_name[256] = {0};
    EXEC SQL END DECLARE SECTION; 
    int count = 0;
    id = question_id;
    
    EXEC SQL DECLARE CURSOR exams_cursor FOR
        SELECT exams.id, exams.year, exams.convocatory, subjects.name, exams_questions.correct_answer_count, exams_questions.incorrect_answer_count, exam_questions.unreplied_answer_count
        FROM exams, exams_questions, subjects
            WHERE exams_questions.question_id = :question_id AND
                  exams_questions.exam_id = exams.id AND subjects.id = exams.subject_id
                ORDER BY exams.year, exams.convocatory ASC;

    EXEC SQL OPEN exams_cursor;
    while ( 1 ) {
        EXEC SQL FETCH exams_cursor INTO :id, :year, :convocatory, :subject_name, :correct, :incorrect, :unreplied;
        
        if ( SQLCODE == SQLE_NOTFOUND )
            break;
        ++count;

        printf("%d %s (%d)\n", year, subject_name, convocatory);
        printf(" - Total: %d\n", correct + incorrect + unreplied);
        printf(" - Correct: %d\n", correct);
        printf(" - Incorrect: %d\n", incorrect);
        printf(" - Unreplied: %d\n", unreplied);
    }
    EXEC SQL CLOSE exams_cursor;

    printf("Total: %d\n", count);

    return 0;
}

int show_question(int argc, char** argv) {
    EXEC SQL BEGIN DECLARE SECTION;
    int id;
    char statement[256] = {0};
    EXEC SQL END DECLARE SECTION; 

    if ( argc == 0 || argc > 1 ) {
        fprintf(stderr, "Usage: show <id>\n");
        return 1;
    }

    id = atoi(argv[0]);

    EXEC SQL SELECT statement FROM questions WHERE id = :id INTO :statement;
    if ( SQLCODE == SQLE_NOTFOUND ) {
        fprintf(stderr, "Record not found: %d\n", id);
        return 1;
    }

    printf("%s\n", statement);
    printf("-----------------------------------\n");
    printf("Options:\n");

    show_answers_for_question(id);
    
    printf("Exams:\n");

    show_exams_for_question(id);
    
    return 0;
}

int main(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    return command_exec(commands, argc, argv);
}
