#ifndef DYN_INT_H
#define DYN_INT_Y

#include <stdint.h>
#include <cmath>

#include "fixed_types.h"

template<UInt32 N> struct Int {
    SInt64 data: N; // Neat way of restricting an integer bit width at compile time while maintaining operations over the precise bit space
    SInt64 maxValue;
    SInt64 minValue;

    Int():
        maxValue(std::pow(2, (N - 1)) - 1),
        minValue(-maxValue - 1){}
    
    Int(SInt64 initValue): Int() {
        this->data = initValue;
    }

    Int<N> operator+(Int<N> other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data + other.data), maxValue), minValue);
        return newInt;
    }
    Int<N> operator+(SInt32 other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data + other), maxValue), minValue);
        return newInt;
    }
    template<UInt32 M>
    Int<N> operator+=(Int<M> other) {
        data = std::max(std::min((SInt64)(data + other.data), maxValue), minValue);
        return *this;
    }
    Int<N> operator+=(SInt64 other) {
        data = std::max(std::min((SInt64)(data + other), maxValue), minValue);
        return *this;
    }
    Int<N> operator-(Int<N> other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data - other.data), maxValue), minValue);
        return newInt;
    }
    Int<N> operator-=(Int<N> other) {
        data = std::max(std::min((SInt64)(data - other.data), maxValue), minValue);
        return *this;
    }
    template<UInt32 M>
    Int<N> operator*(Int<M> other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data * other.data), maxValue), minValue);
        return newInt;
    }
    Int<N> operator*(SInt64 other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data * other), maxValue), minValue);
        return newInt;
    }
    Int<N> operator*=(Int<N> other) {
        data = std::max(std::min((SInt64)(data * other.data), maxValue), minValue);
        return *this;
    }
    Int<N> operator/(Int<N> other) {
        Int<N> newInt;
        newInt.data = std::max(std::min((SInt64)(data / other.data), maxValue), minValue);
        return newInt;
    }
    Int<N> operator/=(Int<N> other) {
        data = std::max(std::min((SInt64)(data / other.data), maxValue), minValue);
        return *this;
    }
};

#endif // DYN_INT_H