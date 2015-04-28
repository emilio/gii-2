// vim:set syntax=c:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "question.h"
#include "answer.h"
#include "macros.h"

#include <commands.h>


// The list of our supported commands
struct command commands[] = {
    { "question",  question, "manages questions",
        "Usage: question [args...]\n"
        "\t-a\tcreates a new question\t-a <statement>\n"
        "\t-d\tdeletes a question\t-d <id>\n"
        "\t-e\tedits a question\t-e <id> <statement>\n"
        "\t-s\tshows a question\t-s <id>\n"
        "\t-l\tlists all questions\t-l\n"
        "Prints the question id on success to stdout." },
    { "answer", answer, "manages answers",
        "Usage: answer [args...]"
        "\t-a\tcreates an answer\t-a <question_id> <statement> [--correct]"
        "\t-l\tlist answers for a question\t-l <question_id>"
        "\t-d\tdeletes a question\t-d <id>" },
    { NULL, NULL, NULL, NULL } // End of the list
};


// TODO: Recover graciously
void handle_error() {
    fprintf(stderr, "[ERROR] %s | SQLSTATE(%s), SQLCODE(%ld)\n", sqlca.sqlerrm.sqlerrmc, SQLSTATE, SQLCODE);
    exit(1);
}





int main(int argc, char** argv) {
    EXEC SQL WHENEVER SQLERROR DO handle_error();
    EXEC SQL CONNECT TO exams;

    int ret = command_exec(commands, argc, argv);

    EXEC SQL COMMIT;

    return ret;
}

