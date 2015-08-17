#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <macros/fatal_error.h>
#include <macros/ensure_success.h>

#define MAX_MESSAGE_LENGTH 255

int write_each(int seconds, const char* message, int fd) {
    char buff[MAX_MESSAGE_LENGTH];
    size_t count = 0;

    /** Set stdin non-blocking */
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    while (1) {
        /** Try to read from stdin */
        if (read(STDIN_FILENO, buff, MAX_MESSAGE_LENGTH) == 0)
            break;

        snprintf(buff, MAX_MESSAGE_LENGTH, message, (int)seconds * count);
        buff[MAX_MESSAGE_LENGTH - 1] = '\0';
        ENSURE_SUCCESS(write(fd, buff, strlen(buff)),
                       fcntl(STDIN_FILENO, F_SETFL,
                             fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);
                       close(fd););
        sleep(seconds);
        count++;
    }

    printf("Parent exiting\n");

    /** Put one null byte on the pipe: child will exit */
    *buff = 0;
    write(fd, buff, 1);
    close(fd);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);

    return 0;
}

int listen_to_and_write(int reader, int writer) {
    fd_set set;
    int bytes;
    char buff[MAX_MESSAGE_LENGTH];

    FD_ZERO(&set);
    FD_SET(reader, &set);

    while (1) {
        ENSURE_SUCCESS(select(reader + 1, &set, NULL, NULL, NULL),
                       close(reader);
                       close(writer););

        ENSURE_SUCCESS(bytes = read(reader, buff, MAX_MESSAGE_LENGTH),
                       close(reader);
                       close(writer););

        if (bytes == MAX_MESSAGE_LENGTH)
            buff[MAX_MESSAGE_LENGTH] = '\0';
        else
            buff[bytes] = '\0';

        printf("Received %d bytes (%s)\n", bytes, buff);

        /** We have been told to stop executing */
        if (bytes == 1 && !*buff)
            break;

        ENSURE_SUCCESS(write(writer, buff, bytes));
    }

    printf("Child exiting\n");

    close(reader);
    close(writer);
    return 0;
}

int main(int argc, char** argv) {
    int pipes[2], fd;
    pid_t child_id;

    if (argc < 2)
        FATAL_ERROR("No file specified.");

    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
    ENSURE_SUCCESS(fd);

    ENSURE_SUCCESS(pipe(pipes), close(fd););

    ENSURE_SUCCESS(child_id = fork(), close(pipes[0]); close(pipes[1]);
                   close(fd););

    if (child_id == 0)
        return listen_to_and_write(pipes[0], fd);

    return write_each(2, "[[%d seconds]]\n", pipes[1]);
}
