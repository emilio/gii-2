/**
 * This is a WIP, for a SSOOI work I did to help a friend with processes and
 * signals.
 *
 * This was done in a few hours, and it's incomplete, but the statement can be
 * found here: http://avellano.usal.es/~ssooi/pract116.htm
 *
 * I put it here just in order to not lose it. There's one big code smell
 * (`PROGRAM_NAME`), but can be easily solved.
 *
 * There are a few TODOs to finish the work, but I doubt I'll do it.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

pid_t children[11] = { 0 };
const char* PROGRAM_NAME = "./bolos";

int build_tree();
int build_tree_a();
int just_wait(pid_t, pid_t);
int build_branch(pid_t, pid_t, pid_t, char);

int main(int argc, char* argv[]) {
    // Block all signals
    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // TODO: Register an empty handler for SIGTERM, since it's the only signal
    // we're handling, and we want it just to wait synchronously as required by
    // the statement

    if (strcmp(argv[0], PROGRAM_NAME) == 0) {
        printf("parent: %d\n", getpid());
        return build_tree();
    }

    assert(argc >= 4);
    pid_t left_child = atoi(argv[1]);
    pid_t right_child = atoi(argv[2]);
    pid_t bottom_child = atoi(argv[3]);

    printf("child %c: %d\n", argv[0][0], getpid());

    switch (argv[0][0]) {
    case 'A':
        return just_wait(left_child, right_child);
    case 'B':
        return build_branch(0, right_child, bottom_child, 'D');
    case 'C':
        return build_branch(left_child, 0, bottom_child, 'F');
    case 'D':
        return build_branch(0, right_child, 0, 'G');
    case 'E':
        return just_wait(left_child, right_child);
    case 'F':
        return build_branch(left_child, 0, 0, 'J');
    case 'G':
    case 'H':
    case 'I':
    case 'J':
        return just_wait(0, 0);
    default:
        assert(0 && "Unexpected child id");
    }

    return 0;
}

// Executed by "P"
int build_tree() {
    pid_t child = fork();

    switch (child) {
    case -1:
        perror("fork");
        exit(1);
    case 0: // A
        children[0] = getpid();
        return build_tree_a();
    default:
        // TODO: ignore SIGCHLD to cleanup resources?
        return 0;
    }
}

int build_tree_a() {
    // The first three As are just placeholders
    char ordered_childs[] = { 'A', 'A', 'A', 'H', 'I', 'E', 'B', 'C' };
    char process_name[2] = { 0 };
    char left_process_id[20];
    char right_process_id[20];
    char bottom_process_id[20];

    for (size_t i = 3; i < sizeof(ordered_childs); ++i) {
        pid_t child = fork();
        char letter = ordered_childs[i];
        switch (child) {
        case -1:
            perror("fork");
            exit(1);
        case 0: // Child
            process_name[0] = letter;
            snprintf(right_process_id, sizeof(right_process_id), "%d", children[ordered_childs[i - 1] - 'A']);
            snprintf(left_process_id, sizeof(left_process_id), "%d", children[ordered_childs[i - 2] - 'A']);
            snprintf(bottom_process_id, sizeof(bottom_process_id), "%d", children[ordered_childs[i - 3] - 'A']);
            if (execl(PROGRAM_NAME, process_name, left_process_id, right_process_id, bottom_process_id, NULL) == -1) {
                perror("execl");
                exit(1);
            }
        default:
            children[letter - 'A'] = child;
        }
    }

    snprintf(left_process_id, sizeof(left_process_id), "%d", children['B' - 'A']);
    snprintf(right_process_id, sizeof(right_process_id), "%d", children['C' - 'A']);

    // TODO: Pass 'H', 'E' and 'I' too, since he needs it to check if they're alive
    if (execl(PROGRAM_NAME, "A", left_process_id, right_process_id, "0", NULL) == -1) {
        perror("execl");
        return 1;
    }

    assert(0 && "What the f*ck!");
    return 1;
}


int just_wait(pid_t left, pid_t right) {
    sigset_t set;

    // Or both or none
    assert((left && right) || (!left && !right));

    // Just catch SIGTERM
    sigfillset(&set);
    sigdelset(&set, SIGTERM);

    while (1) {
        sigsuspend(&set);
        if (errno != EINTR)
            continue; // Try again

        // TODO
    }

    return 0;
}

int build_branch(pid_t left, pid_t right, pid_t bottom, char child_name) {
    char proc_name[2] = {0};
    char left_process_id[20] = {'0', 0};
    char right_process_id[20] = {'0', 0};

    assert(left == 0 || right == 0);

    pid_t child = fork();

    switch (child) {
    case -1:
        perror("fork");
        exit(1);
    case 0: // new child
        proc_name[0] = child_name;

        // if there's no left child, we're creating it, so bottom is its right
        // one
        if (!left)
            snprintf(right_process_id, sizeof(right_process_id), "%d", bottom);
        else
            snprintf(left_process_id, sizeof(left_process_id), "%d", bottom);

        if (execl(PROGRAM_NAME, proc_name, left_process_id, right_process_id, "0", NULL) == -1) {
            perror("execl");
            exit(1);
        }
    }

    if (!left)
        left = child;
    else
        right = child;

    return just_wait(left, right);
}
