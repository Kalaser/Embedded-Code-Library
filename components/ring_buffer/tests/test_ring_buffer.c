#include "ecl_ring_buffer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_init_and_queries(void)
{
    ecl_ring_buffer_t rb;
    uint8_t storage[4];
    assert(ecl_ring_buffer_init(&rb, storage, sizeof(storage)) == ECL_STATUS_OK);
    assert(ecl_ring_buffer_capacity(&rb) == 4U);
    assert(ecl_ring_buffer_size(&rb) == 0U);
    assert(ecl_ring_buffer_space(&rb) == 4U);
    assert(ecl_ring_buffer_is_empty(&rb));
    assert(!ecl_ring_buffer_is_full(&rb));
    assert(ecl_ring_buffer_init(NULL, storage, sizeof(storage)) == ECL_STATUS_INVALID_ARGUMENT);
    assert(ecl_ring_buffer_init(&rb, NULL, sizeof(storage)) == ECL_STATUS_INVALID_ARGUMENT);
    assert(ecl_ring_buffer_init(&rb, storage, 0U) == ECL_STATUS_INVALID_ARGUMENT);
}

static void test_partial_write_and_read(void)
{
    ecl_ring_buffer_t rb;
    uint8_t storage[4];
    const uint8_t input[] = {1U, 2U, 3U, 4U, 5U};
    uint8_t output[8] = {0};

    assert(ecl_ring_buffer_init(&rb, storage, sizeof(storage)) == ECL_STATUS_OK);
    assert(ecl_ring_buffer_write(&rb, input, sizeof(input)) == 4U);
    assert(ecl_ring_buffer_is_full(&rb));
    assert(ecl_ring_buffer_write(&rb, input, 1U) == 0U);
    assert(ecl_ring_buffer_write(&rb, NULL, 1U) == 0U);
    assert(ecl_ring_buffer_read(&rb, output, 3U) == 3U);
    assert(output[0] == 1U && output[1] == 2U && output[2] == 3U);
    assert(ecl_ring_buffer_read(&rb, output + 3, sizeof(output) - 3U) == 1U);
    assert(output[3] == 4U);
    assert(ecl_ring_buffer_is_empty(&rb));
    assert(ecl_ring_buffer_read(&rb, output, 1U) == 0U);
}

static void test_wraparound(void)
{
    ecl_ring_buffer_t rb;
    uint8_t storage[5];
    const uint8_t first[] = {10U, 11U, 12U, 13U};
    const uint8_t second[] = {20U, 21U, 22U, 23U};
    uint8_t output[5] = {0};

    assert(ecl_ring_buffer_init(&rb, storage, sizeof(storage)) == ECL_STATUS_OK);
    assert(ecl_ring_buffer_write(&rb, first, sizeof(first)) == sizeof(first));
    assert(ecl_ring_buffer_read(&rb, output, 3U) == 3U);
    assert(ecl_ring_buffer_write(&rb, second, sizeof(second)) == 4U);
    assert(ecl_ring_buffer_read(&rb, output, sizeof(output)) == 5U);
    assert(output[0] == 13U);
    assert(output[1] == 20U && output[2] == 21U && output[3] == 22U && output[4] == 23U);
}

static void test_peek_reset_and_zero_length(void)
{
    ecl_ring_buffer_t rb;
    uint8_t storage[3];
    const uint8_t input[] = {7U, 8U};
    uint8_t value = 0U;

    assert(ecl_ring_buffer_init(&rb, storage, sizeof(storage)) == ECL_STATUS_OK);
    assert(ecl_ring_buffer_peek(&rb, &value) == ECL_STATUS_INVALID_STATE);
    assert(ecl_ring_buffer_write(&rb, input, sizeof(input)) == sizeof(input));
    assert(ecl_ring_buffer_peek(&rb, &value) == ECL_STATUS_OK);
    assert(value == 7U && ecl_ring_buffer_size(&rb) == 2U);
    assert(ecl_ring_buffer_write(&rb, NULL, 0U) == 0U);
    assert(ecl_ring_buffer_read(&rb, NULL, 0U) == 0U);
    assert(ecl_ring_buffer_reset(&rb) == ECL_STATUS_OK);
    assert(ecl_ring_buffer_is_empty(&rb));
    assert(ecl_ring_buffer_peek(&rb, NULL) == ECL_STATUS_INVALID_ARGUMENT);
}

int main(void)
{
    test_init_and_queries();
    test_partial_write_and_read();
    test_wraparound();
    test_peek_reset_and_zero_length();
    puts("ring_buffer tests: PASS");
    return 0;
}
