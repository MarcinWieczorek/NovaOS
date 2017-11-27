#include <math.h>

double pow(double a, double b) {
    double c = a;

    while(--b > 0) {
        a *= c;
    }

    return a;
}
