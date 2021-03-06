#pragma once

#include "buffer.h"
#include "epoll.h"

struct visca_header_t
{
    uint16_t payload_type;
    uint16_t payload_length;
    uint32_t seq_number;
} __attribute__ ((packed));

struct message_t
{
    struct visca_header_t *header;
    const uint8_t *payload;
    size_t payload_length;
};

#define VOIP_MAX_PAYLOAD_LENGTH 16
#define VOIP_HEADER_LENGTH 8
#define VOIP_MAX_MESSAGE_LENGTH (VOIP_HEADER_LENGTH + VOIP_MAX_PAYLOAD_LENGTH)

#define bad_byte_detail(X, R) \
    do { \
        log("%s:%d: unexpected byte 0x%02x", __func__, __LINE__, message->payload[X]); \
        return R; \
    } while (0)
#define bad_byte_null(X) bad_byte_detail(X, NULL)
#define bad_byte(X) bad_byte_detail(X, )

#define check_length_detail(X, R) \
    if (message->payload_length != (X)) { \
        log("%s: bad length %zu, expected %d", __func__, message->payload_length, X); \
        return R; \
    }
#define check_length_null(X) check_length_detail(X, NULL)
#define check_length(X) check_length_detail(X, )

buffer_t* compose_control_reply(uint32_t seq_number);
void sony_visca_handle_message(const buffer_t *message_buf, const struct event_t *event);

