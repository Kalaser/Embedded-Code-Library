#include "ecl_ring_buffer.h"

static bool ecl_ring_buffer_is_valid(const ecl_ring_buffer_t *rb)
{
    return (rb != NULL) && (rb->storage != NULL) && (rb->capacity > 0U) &&
           (rb->head < rb->capacity) && (rb->tail < rb->capacity) &&
           (rb->count <= rb->capacity);
}

static size_t ecl_ring_buffer_advance(size_t index, size_t amount, size_t capacity)
{
    /* amount is always <= capacity; avoid index + amount overflow. */
    const size_t until_end = capacity - index;
    return (amount >= until_end) ? (amount - until_end) : (index + amount);
}

ecl_status_t ecl_ring_buffer_init(ecl_ring_buffer_t *rb,
                                  uint8_t *storage,
                                  size_t capacity)
{
    if ((rb == NULL) || (storage == NULL) || (capacity == 0U)) {
        return ECL_STATUS_INVALID_ARGUMENT;
    }

    rb->storage = storage;
    rb->capacity = capacity;
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
    return ECL_STATUS_OK;
}

ecl_status_t ecl_ring_buffer_reset(ecl_ring_buffer_t *rb)
{
    if (!ecl_ring_buffer_is_valid(rb)) {
        return ECL_STATUS_INVALID_STATE;
    }

    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
    return ECL_STATUS_OK;
}

size_t ecl_ring_buffer_capacity(const ecl_ring_buffer_t *rb)
{
    return ecl_ring_buffer_is_valid(rb) ? rb->capacity : 0U;
}

size_t ecl_ring_buffer_size(const ecl_ring_buffer_t *rb)
{
    return ecl_ring_buffer_is_valid(rb) ? rb->count : 0U;
}

size_t ecl_ring_buffer_space(const ecl_ring_buffer_t *rb)
{
    return ecl_ring_buffer_is_valid(rb) ? (rb->capacity - rb->count) : 0U;
}

bool ecl_ring_buffer_is_empty(const ecl_ring_buffer_t *rb)
{
    return ecl_ring_buffer_is_valid(rb) && (rb->count == 0U);
}

bool ecl_ring_buffer_is_full(const ecl_ring_buffer_t *rb)
{
    return ecl_ring_buffer_is_valid(rb) && (rb->count == rb->capacity);
}

size_t ecl_ring_buffer_write(ecl_ring_buffer_t *rb,
                             const uint8_t *data,
                             size_t length)
{
    size_t amount;
    size_t first;

    if (!ecl_ring_buffer_is_valid(rb) || ((length > 0U) && (data == NULL))) {
        return 0U;
    }

    amount = length;
    if (amount > (rb->capacity - rb->count)) {
        amount = rb->capacity - rb->count;
    }
    if (amount == 0U) {
        return 0U;
    }

    first = amount;
    if (first > (rb->capacity - rb->tail)) {
        first = rb->capacity - rb->tail;
    }

    for (size_t i = 0U; i < first; ++i) {
        rb->storage[rb->tail + i] = data[i];
    }
    for (size_t i = first; i < amount; ++i) {
        rb->storage[i - first] = data[i];
    }

    rb->tail = ecl_ring_buffer_advance(rb->tail, amount, rb->capacity);
    rb->count += amount;
    return amount;
}

size_t ecl_ring_buffer_read(ecl_ring_buffer_t *rb,
                            uint8_t *data,
                            size_t length)
{
    size_t amount;
    size_t first;

    if (!ecl_ring_buffer_is_valid(rb) || ((length > 0U) && (data == NULL))) {
        return 0U;
    }

    amount = (length < rb->count) ? length : rb->count;
    if (amount == 0U) {
        return 0U;
    }

    first = amount;
    if (first > (rb->capacity - rb->head)) {
        first = rb->capacity - rb->head;
    }

    for (size_t i = 0U; i < first; ++i) {
        data[i] = rb->storage[rb->head + i];
    }
    for (size_t i = first; i < amount; ++i) {
        data[i] = rb->storage[i - first];
    }

    rb->head = ecl_ring_buffer_advance(rb->head, amount, rb->capacity);
    rb->count -= amount;
    return amount;
}

ecl_status_t ecl_ring_buffer_peek(const ecl_ring_buffer_t *rb, uint8_t *value)
{
    if (!ecl_ring_buffer_is_valid(rb) || (value == NULL)) {
        return ECL_STATUS_INVALID_ARGUMENT;
    }
    if (rb->count == 0U) {
        return ECL_STATUS_INVALID_STATE;
    }

    *value = rb->storage[rb->head];
    return ECL_STATUS_OK;
}
