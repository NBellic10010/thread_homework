#include "controller.h"

extern void controller_function(double& u, unsigned int k);

int main() {
    double u; unsigned int k = 55;
    controller_function(u, k);
    return 0;
}