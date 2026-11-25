#include "component.h"
int component_process(int input, int *output)
{
    if (output == 0) return -1;
    *output = input;
    return 0;
}
