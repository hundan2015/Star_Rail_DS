#ifndef NEA_MATH_H
#define NEA_MATH_H

template <typename T>
struct Vecter3 {
    T x;
    T y;
    T z;
    T& u;
    T& v;
    T& w;
    Vecter3<T>() : u(x), v(y), w(z) {}
};

template <typename T>
struct Vecter2 {
    T x;
    T y;
};

#endif