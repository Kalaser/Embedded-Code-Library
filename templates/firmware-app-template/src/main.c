#include "app.h"

int main(void)
{
    if (app_init() != 0) {
        return 1;
    }

    /* Replace with the target platform's scheduler/event loop. */
    (void)app_run_once();

    app_deinit();
    return 0;
}
