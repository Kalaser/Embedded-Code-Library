# Ring Buffer

- **Version:** 1.0.0
- **Language:** C99
- **Portability class:** P0 (pure C; no OS calls)
- **Status:** validated on the current host build used for this package; no MCU/RTOS validation claimed
- **Memory:** caller supplies fixed storage; no dynamic allocation
- **Thread safety:** not thread-safe; caller must serialize concurrent access
- **ISR safety:** not guaranteed; target-specific review required
- **Capacity:** all `capacity` bytes can be used

## API

`ecl_ring_buffer_init`, `write`, `read`, `peek`, `reset`, `capacity`, `size`, `space`, `is_empty`, `is_full`.

`write` and `read` may transfer fewer bytes than requested. The caller must inspect the returned byte count. `length == 0` transfers zero bytes. For a non-zero length, a null data pointer is rejected and returns zero.

## Example

```c
#include "ecl_ring_buffer.h"

static uint8_t storage[128];
static ecl_ring_buffer_t rx_ring;

void module_init(void)
{
    (void)ecl_ring_buffer_init(&rx_ring, storage, sizeof(storage));
}
```

## Preconditions and limitations

- Except `init`, every API requires a successfully initialized object. Do not pass an uninitialized stack object.
- `read`/`write` buffer ranges must not overlap the internal storage.
- No internal lock or atomic operations.
- Does not distinguish empty/full with a reserved sentinel byte; an explicit count is maintained.
- The storage buffer must remain valid and exclusively owned for the ring's lifetime.
- Do not pass a source/destination pointer that aliases the internal storage unless the application has explicitly proven the copy order safe.
