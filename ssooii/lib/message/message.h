#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <macros/inline.h>

typedef int message_queue_t;
typedef long message_type_t;

INLINE
message_queue_t message_queue_create(key_t key) {
    return msgget(key, 0600 | IPC_CREAT);
}

INLINE
int message_send(message_queue_t queue, void* msg, size_t size) {
    return msgsnd(queue, msg, size, 0);
}

INLINE
size_t message_receive(message_queue_t queue, void* buffer, size_t size,
                       message_type_t type) {
    return msgrcv(queue, buffer, size, type, 0);
}

#endif
