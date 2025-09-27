//
// Created by Yolan on 2025/9/23.
//

#ifndef CSAPP_HOMEWORK_SOL_WEEK3_H
#define CSAPP_HOMEWORK_SOL_WEEK3_H

/* 2.59(**)
 * Write a C expression that will yield a word consisting of the least significant byte of x and the remaining bytes of y.
 * For operands x = 0x89ABCDEF and y = 0x76543210, this would give 0x765432EF. */
int sol2_59(int x, int y){
    x= x & 0xff;
    y = y & ~0xff;
    return x | y;
}



#endif //CSAPP_HOMEWORK_SOL_WEEK3_H