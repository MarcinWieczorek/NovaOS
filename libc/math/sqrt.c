#include <math.h>

double sqrt(double n) {
    double g = 2.0 / n;

    for(int i = 0; i < 5; i++) {
        g = g * (1.5 - (n / 2.0) * g * g);
    }

    return 1.0 / g;
}

