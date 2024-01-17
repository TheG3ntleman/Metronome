#include "utils.h"

// funcion to calculate absolute value of floating point number
numeric fabsf(numeric num) {
    if (num < 0) {
        return -num;
    } else {
        return num;
    }
}

// function to calculate the power of a number
numeric powerOptimized(numeric base, int exponent) {
    numeric result = 1;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }

        base *= base;
        exponent /= 2;
    }

    return result;
}

numeric norm_l1(numeric x1, numeric x2) {
    return fabsf(x1) + fabsf(x2);
}

numeric norm_l2(numeric x1, numeric x2) {

    // calculating square root of x1^2+x2^2
    numeric x = x1*x1 + x2*x2;
    numeric epsilon = 1e-3;
    numeric guess = x / 2.0;
    while (fabsf(guess*guess - x) > epsilon) {
        guess = guess - (guess*guess - x) / (2 * guess);
    }

    return guess;
}

numeric norm_lp(numeric x1, numeric x2, uint p) {
    numeric x = powerOptimized(fabsf(x1), p) + powerOptimized(fabsf(x2), p);

    numeric epsilon = 1e-6; 
    numeric guess = x / 2.0;

    // Limit the number of iterations to prevent infinite loop
    int maxIterations = 1000;

    while (fabsf(powerOptimized(guess, p) - x) > epsilon && maxIterations > 0) {
        guess = guess - (powerOptimized(guess, p) - x) / (p * powerOptimized(guess, p - 1));
        maxIterations--;
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
