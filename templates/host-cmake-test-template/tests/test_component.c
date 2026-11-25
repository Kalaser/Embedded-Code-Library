#include "component.h"
#include <assert.h>
int main(void)
{
    int result = 0;
    assert(component_process(42, &result) == 0);
    assert(result == 42);
    assert(component_process(42, 0) != 0);
    return 0;
}
