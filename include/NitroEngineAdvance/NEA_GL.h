#ifndef NEA_GL_H
#define NEA_GL_H
#include "nds/arm9/math.h"
#include "nds/arm9/video.h"

static inline void glShearX(float m, float n) {
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = floattof32(m);
    MATRIX_MULT3x3 = floattof32(n);

    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = 0;

    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = inttof32(1);
}

static inline void glShearY(float m, float n) {
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = 0;

    MATRIX_MULT3x3 = floattof32(m);
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = floattof32(n);

    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = inttof32(1);
}

static inline void glShearZ(float m, float n) {
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = 0;

    MATRIX_MULT3x3 = 0;
    MATRIX_MULT3x3 = inttof32(1);
    MATRIX_MULT3x3 = 0;

    MATRIX_MULT3x3 = floattof32(m);
    MATRIX_MULT3x3 = inttof32(n);
    MATRIX_MULT3x3 = floattof32(1);
}

#endif