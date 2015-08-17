#include <stdlib.h>
#include <stdio.h>
#include <macros/ensure_success.h>
#include <message/message.h>
#include <string.h>
#define MAX_MESSAGE_SIZE 255

typedef struct message {
    message_type_t type;
    char content[MAX_MESSAGE_SIZE];
} message_t;

void show_usage(int argc, char** argv) {
    printf("Usage: %s [-e|-r] <type> <msg>\n", argv[0]);
    printf("Options:\n");
    printf("\t-e\tSend <msg> using type <type>\n");
    printf("\t-r\tReceive message of type <type>\n");
    printf("Notes:\n");
    printf("\tMax size of <msg>: %d\n", MAX_MESSAGE_SIZE);
    exit(1);
}

void send_message(message_queue_t queue, message_type_t type, char* content) {
    message_t message;

    message.type = type;
    strncpy(message.content, content, MAX_MESSAGE_SIZE);
    message.content[MAX_MESSAGE_SIZE - 1] = '\0';

    printf("Sending message of type %ld; Content: \"%s\"\n", message.type,
           message.content);
    ENSURE_SUCCESS(
        message_send(queue, &message, sizeof(message_t) - sizeof(long)));
    printf("... Success!\n");
}

void receive_message(message_queue_t queue, message_type_t type) {
    message_t message;
    printf("Waiting for messages with type: %ld\n", type);
    ENSURE_EQUALS(message_receive(queue, &message,
                                  sizeof(message_t) - sizeof(long), type),
                  sizeof(message_t) - sizeof(long));
    printf("Message received!\n");
    printf("%s\n", message.content);
}

int main(int argc, char** argv) {
    message_type_t type;
    message_queue_t queue;

    if (argc < 3)
        show_usage(argc, argv);

    queue = message_queue_create(ftok(argv[0], 'M'));

    type = strtol(argv[2], NULL, 10);

    /** TODO: refactor */
    if (strcmp(argv[1], "-e") == 0) {
        if (argc != 4)
            show_usage(argc, argv);

        send_message(queue, type, argv[3]);
    } else if (strcmp(argv[1], "-r") == 0) {
        if (argc != 3)
            show_usage(argc, argv);

        receive_message(queue, type);
    } else {
        show_usage(argc, argv);
    }

    return 0;
}
