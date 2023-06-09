#pragma once

#include "types.hpp"

static const double TAU = 6.28318530717958;

static inline float2 operator+(float2 a, float2 b) { return { a.x + b.x, a.y + b.y }; }
static inline float2 operator-(float2 a, float2 b) { return { a.x - b.x, a.y - b.y }; }
static inline float2 operator*(float2 a, float2 b) { return { a.x * b.x, a.y * b.y }; }
static inline float2 operator*(float2 a, float b) { return { a.x * b, a.y * b }; }
static inline float2 operator/(float2 a, float2 b) { return { a.x / b.x, a.y / b.y }; }
static inline float2& operator+=(float2& a, float2 b) { a = a + b; return a; }
static inline float2& operator*=(float2& a, float2 b) { a = a * b; return a; }
static inline float2& operator/=(float2& a, float2 b) { a = a / b; return a; }

static inline float sign(float x) { return (x < 0.f) ? -1.f : 1.f; }
