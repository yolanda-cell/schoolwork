#ifndef CSAPP_HOMEWORK_SOL_WEEK3_H
#define CSAPP_HOMEWORK_SOL_WEEK3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int leftmost_one(unsigned x);
int lower_one_mask(int n);
unsigned rotate_left(unsigned x, int n);
typedef unsigned float_bits;
float_bits float_twice(float_bits f);
float_bits float_half(float_bits f);

void test_leftmost_one();
void test_lower_one_mask();
void test_rotate_left();
void test_float_twice();
void test_float_half();
void test_all_csapp_week3();

/* 2.66(***)
* Generate mask indicating leftmost 1 in x. Assume w=32.
* For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
* If x = 0, then return 0.
*/
int leftmost_one(unsigned x) {
    // 传播最高位1到右边所有位
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    // 右移一位再异或，只剩下最高位1
    return x ^ (x >> 1);
}

/* 2.68(**)
* Mask with least signficant n bits set to 1
* Examples: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
* Assume 1 <= n <= w
*/
int lower_one_mask(int n) {

    int w = sizeof(int) << 3; // 32
    unsigned int mask = ~0U;  // 全1
    mask >>= (w - n);         // 右移 (32 - n) 位，保留低 n 位
    return (int)mask;
}

/* 2.69(***)
* Do rotating left shift. Assume 0 <= n < w
* Examples when x = 0x12345678 and w = 32:
* n=4 -> 0x23456781, n=20 -> 0x67812345
*/
unsigned rotate_left(unsigned x, int n) {

    int w = sizeof(unsigned) << 3;
    unsigned left = x << n;
    unsigned right = x >> (w - n);
    // Mask for n = 0
    unsigned mask = !n - 1;  // n ? mask=0xFFFFFFFF : mask=0
    right = right & mask;

    return left | right;
}

/* 2.94(***)
 * Compute 2*f. If f is NaN, then return f.
 */
float_bits float_twice(float_bits f) {

    unsigned sign = f >> 31;
    unsigned exp  = (f >> 23) & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    // NaN or infinity
    if (exp == 0xFF) {
        return f;
    }
    if (exp == 0) {
        // Denormalized: multiply fraction by 2
        frac <<= 1;
        // Check if it becomes normalized
        if (frac & 0x800000) {
            exp = 1;
            frac &= 0x7FFFFF;
        }
        return (sign << 31) | (exp << 23) | frac;
    }
    // Normalized: increment exponent
    exp += 1;
    if (exp == 0xFF) {
        // Overflow to infinity
        frac = 0;
    }
    return (sign << 31) | (exp << 23) | frac;
}

/* 2.95(***)
 * Compute 0.5*f. If f is NaN, then return f.
 */
float_bits float_half(float_bits f) {

    unsigned sign = f >> 31;
    unsigned exp  = (f >> 23) & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    // NaN or infinity
    if (exp == 0xFF) {
        return f;
    }
    if (exp == 0 || exp == 1) {
        // Denormalized or about to be denormalized
        // Add the implicit leading 1 to the fraction if exp == 1
        if (exp == 1) {
            frac |= 0x800000;
        }
        // round to even
        unsigned add = ((frac & 3) == 3);
        frac = (frac >> 1) + add;
        exp = 0;
        return (sign << 31) | (exp << 23) | frac;
    }

    // Normalized, just decrement exponent
    exp -= 1;
    return (sign << 31) | (exp << 23) | frac;
}

//// 2.66
void test_leftmost_one() {
    struct {unsigned x; int expect;} tests[] = {
        {0xFF00, 0x8000},
        {0x6600, 0x4000},
        {0x80000000, 0x80000000},
        {0x1, 0x1}, {0x0, 0x0},
        {0xF, 0x8}, {0x12345678, 0x10000000}
    };
    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        int r = leftmost_one(tests[i].x);
        if (r != tests[i].expect)
            {printf("[FAILED] leftmost_one(%#x)==%#x, expect %#x\n", tests[i].x, r, tests[i].expect); return;}
    }
    printf("[PASSED] leftmost_one\n");
}

//// 2.68
void test_lower_one_mask() {
    struct {int n; int expect;} tests[] = {
        {1, 0x1}, {6, 0x3F}, {17, 0x1FFFF}, {32, ~0U}
    };
    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        int r = lower_one_mask(tests[i].n);
        if (r != tests[i].expect)
            {printf("[FAILED] lower_one_mask(%d)==%#x, expect %#x\n", tests[i].n, r, tests[i].expect); return;}
    }
    printf("[PASSED] lower_one_mask\n");
}

//// 2.69
void test_rotate_left() {
    struct {unsigned x; int n; unsigned expect;} tests[] = {
        {0x12345678, 4, 0x23456781},
        {0x12345678, 20, 0x67812345},
        {0x80000000, 1, 0x1},
        {0xA, 0, 0xA},
        {0xA, 32-1, 0x5}
    };
    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        unsigned r = rotate_left(tests[i].x, tests[i].n);
        if (r != tests[i].expect)
            {printf("[FAILED] rotate_left(%#x,%d)==%#x, expect %#x\n", tests[i].x, tests[i].n, r, tests[i].expect); return;}
    }
    printf("[PASSED] rotate_left\n");
}

//// float to bits helpers
float_bits float_to_bits(float x) {
    float_bits r; memcpy(&r, &x, 4); return r;
}
float bits_to_float(float_bits b) {
    float x; memcpy(&x, &b, 4); return x;
}

//// 2.94
void test_float_twice() {
    struct {float x; float expect;} tests[] = {
        {0.0f, 0.0f},
        {-0.0f, -0.0f},
        {1.5f, 3.0f},
        {-1.5f, -3.0f},
        {powf(2,-149), powf(2,-148)}, // smallest denorm
        {NAN, NAN},
        {INFINITY, INFINITY}
    };
    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        float_bits b = float_to_bits(tests[i].x);
        float_bits expect_b = float_to_bits(tests[i].expect);
        float_bits r = float_twice(b);
        // NaN需要特殊测试
        float rv = bits_to_float(r), ev = tests[i].expect;
        if (isnan(ev)) {if (!isnan(rv)) {printf("[FAILED] float_twice(%g)==%g, expect NAN\n", tests[i].x, rv); return;}}
        else if (rv != ev) {printf("[FAILED] float_twice(%g)==%g, expect %g\n", tests[i].x, rv, ev); return;}
    }
    printf("[PASSED] float_twice\n");
}

//// 2.95
void test_float_half() {
    struct {float x; float expect;} tests[] = {
        {0.0f, 0.0f},
        {-0.0f, -0.0f},
        {3.0f, 1.5f},
        {-3.0f, -1.5f},
        {powf(2,-148), powf(2,-149)},
        {NAN, NAN},
        {INFINITY, INFINITY}
    };
    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        float_bits b = float_to_bits(tests[i].x);
        float_bits expect_b = float_to_bits(tests[i].expect);
        float_bits r = float_half(b);
        float rv = bits_to_float(r), ev = tests[i].expect;
        if (isnan(ev)) {if (!isnan(rv)) {printf("[FAILED] float_half(%g)==%g, expect NAN\n", tests[i].x, rv); return;}}
        else if (rv != ev) {printf("[FAILED] float_half(%g)==%g, expect %g\n", tests[i].x, rv, ev); return;}
    }
    printf("[PASSED] float_half\n");
}

void test_all_csapp_week3() {
    printf("----- Testing CSAPP Homework Week3 -----\n");
    test_leftmost_one();
    test_lower_one_mask();
    test_rotate_left();
    test_float_twice();
    test_float_half();
}

#endif //CSAPP_HOMEWORK_SOL_WEEK3_H