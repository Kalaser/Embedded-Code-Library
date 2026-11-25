#ifndef ECL_RING_BUFFER_ECL_RING_BUFFER_H
#define ECL_RING_BUFFER_ECL_RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ecl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Fixed-capacity byte ring buffer.
 *
 * Ownership: storage is supplied and owned by the caller.
 * Threading: not thread-safe; caller must serialize concurrent access.
 * ISR safety: not guaranteed; validate atomicity and call context per target.
 * Capacity: all supplied bytes are usable; no sentinel slot is reserved.
 */
typedef struct ecl_ring_buffer {
    uint8_t *storage;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} ecl_ring_buffer_t;

/* Initialize using caller-owned storage. capacity must be greater than zero. */
ecl_status_t ecl_ring_buffer_init(ecl_ring_buffer_t *rb,
                                  uint8_t *storage,
                                  size_t capacity);

/* Reset the logical contents. Does not clear or free caller-owned storage. */
ecl_status_t ecl_ring_buffer_reset(ecl_ring_buffer_t *rb);

/* Query functions require a successfully initialized object. */
size_t ecl_ring_buffer_capacity(const ecl_ring_buffer_t *rb);
size_t ecl_ring_buffer_size(const ecl_ring_buffer_t *rb);
size_t ecl_ring_buffer_space(const ecl_ring_buffer_t *rb);
bool ecl_ring_buffer_is_empty(const ecl_ring_buffer_t *rb);
bool ecl_ring_buffer_is_full(const ecl_ring_buffer_t *rb);

/*
 * Write/read up to length bytes and return the number actually transferred.
 * A short transfer is normal when the buffer is full/empty.
 * For length > 0, data must not be NULL. Source/destination ranges must not
 * overlap the ring storage. All operations require a successfully initialized
 * object.
 */
size_t ecl_ring_buffer_write(ecl_ring_buffer_t *rb,
                             const uint8_t *data,
                             size_t length);
size_t ecl_ring_buffer_read(ecl_ring_buffer_t *rb,
                            uint8_t *data,
                            size_t length);

/* Read the next byte without consuming it. */
ecl_status_t ecl_ring_buffer_peek(const ecl_ring_buffer_t *rb,
                                  uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif /* ECL_RING_BUFFER_ECL_RING_BUFFER_H */
