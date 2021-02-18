#ifndef MSGBLOCK_H
#define MSGBLOCK_H

#include <stdint.h> // uint8_t
#include "list.h" // struct list_node

#define MESSAGE_MIN 5
#define MESSAGE_MAX 64
#define MESSAGE_HEADER_SIZE  2
#define MESSAGE_TRAILER_SIZE 3
#define MESSAGE_POS_LEN 0
#define MESSAGE_POS_SEQ 1
#define MESSAGE_TRAILER_CRC  3
#define MESSAGE_TRAILER_SYNC 1
#define MESSAGE_PAYLOAD_MAX (MESSAGE_MAX - MESSAGE_MIN)
#define MESSAGE_SEQ_MASK 0x0f
#define MESSAGE_DEST 0x10
#define MESSAGE_SYNC 0x7E

struct queue_message {
    int len;
    uint8_t msg[MESSAGE_MAX];
    union {
        // Filled when on a command queue
        struct {
            uint64_t min_clock, req_clock;
        };
        // Filled when in sent/receive queues
        struct {
            double sent_time, receive_time;
        };
    };
    uint64_t notify_id;
    struct list_node node;
};

uint16_t msgblock_crc16_ccitt(uint8_t *buf, uint8_t len);
int msgblock_check(uint8_t *need_sync, uint8_t *buf, int buf_len);
struct queue_message *message_alloc(void);
struct queue_message *message_fill(uint8_t *data, int len);
struct queue_message *message_alloc_and_encode(uint32_t *data, int len);
void message_free(struct queue_message *qm);
void message_queue_free(struct list_head *root);

#endif // msgblock.h
