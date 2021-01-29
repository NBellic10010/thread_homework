#include "new_controller_func.h"
#include <math.h>

void new_controller_func(double& u, unsigned int k) {
    u = 2.0 * sin((double) k / 45.0);
}