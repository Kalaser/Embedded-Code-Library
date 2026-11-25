#include "ecl_ring_buffer.h"

#include <stdio.h>

int main(void)
{
    uint8_t storage[8];
    const uint8_t message[] = {'E', 'C', 'L'};
    uint8_t received[8];
    ecl_ring_buffer_t rb;
    size_t written;
    size_t read_count;

    if (ecl_ring_buffer_init(&rb, storage, sizeof(storage)) != ECL_STATUS_OK) {
        return 1;
    }

    written = ecl_ring_buffer_write(&rb, message, sizeof(message));
    read_count = ecl_ring_buffer_read(&rb, received, sizeof(received));

    printf("written=%zu read=%zu payload=%.*s\n",
           written, read_count, (int)read_count, (const char *)received);
    return (written == sizeof(message) && read_count == sizeof(message)) ? 0 : 1;
}
