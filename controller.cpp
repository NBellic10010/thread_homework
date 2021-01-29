#include "controller.h" 

void controller_function(double& u, unsigned int k) {
    u = 2.0 * sin((double)k / 20.5);
}