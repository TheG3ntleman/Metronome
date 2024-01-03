#include "utils.h"

numeric norm_l1(numeric x1, numeric x2) {
    return fabsf(x1) + fabsf(x2);
}

numeric norm_l2(numeric x1, numeric x2) {

    // calculating x1^2 and x2^2
    numeric ans1 = x1*x1;
    numeric ans2 = x2*x2;

    // calculating square root of x1^2+x2^2
    numeric x = ans1 + ans2;
    numeric epsilon = 1e-3;
    numeric guess = x / 2.0;
    while (fabsf(powf(guess, 2) - x) > epsilon) {
        guess = guess - (powf(guess, 2) - x) / (2 * powf(guess, 1));
    }

    return guess;
}

numeric norm_lp(numeric x1, numeric x2, uint p) {

    // calculating x1^p
    uint q = p;
    numeric ans1 = 1;
    while (p > 0) {
        int last_bit = (p & 1);
        if (last_bit) {
            ans1 = ans1 * x1;
        }
        x1 = x1 * x1;
        p = p >> 1;
    }

    // calculating x2^p
    p = q;
    numeric ans2 = 1;
    while (p > 0) {
        int last_bit = (p & 1);
        if (last_bit) {
            ans2 = ans2 * x2;
        }
        x2 = x2 * x2;
        p = p >> 1;
    }

    // calculating the pth root of x1^p + x2^p using newtons method
    p = q;
    numeric x = ans1 + ans2;
    numeric epsilon = 1e-3;
    numeric guess = x / 2.0;
    while (fabsf(powf(guess, p) - x) > epsilon) {
        guess = guess - (powf(guess, p) - x) / (p * powf(guess, p - 1));
    }

    return guess;
}

numeric norm_linfty(numeric x1, numeric x2){
    // returning the bigger absolute value
    if (fabsf(x1) > fabsf(x2)){
        return x1;
    }
    else{
        return x2;
    }
}
