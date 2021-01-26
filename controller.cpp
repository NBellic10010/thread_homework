#include "controller.h" 

void controller_function(double& u, unsigned int k) {
    u = 2.0 * sin((double)k / 20.0);
    char out[100];
    sprintf(out, "u = %f, from 2.0 * sin((double)%d / 20.0)\n", u, k);
    printf("%s", out);
}