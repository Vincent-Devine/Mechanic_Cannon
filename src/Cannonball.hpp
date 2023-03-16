#pragma once

#include <list>
#include "types.hpp"

struct TrajectoryData
{
    float time = 0.f;
    float maxHeight = 0.f;
    float landingDistance = 0.f;
};

struct Init
{
    float2 position = { 0.f, 0.f };
    float speed = 0.f;
    float angle = 0.f;
    float2 v0 = { 0.f, 0.f };
};

class Cannonball
{
	// Attributes
public:
    unsigned int id = 0;
    Init init;
    TrajectoryData live;
    TrajectoryData expected;
    float2 position = { 0.f, 0.f };
    std::list<float2> prevPos;
    float speed = 0.f;
    bool hasHit = false;

    float weight = 25.f;
    bool airDrag = false;
    float radius;
    double dragConst;

    // Methods
public:
    void Init(const float cannonAngle, const float cannonLength, const float2 cannonPosition, const float cannonInitSpeed, const float cannonDeceleration, const unsigned int id, const float radius, const float weight, bool airDrag, const double _Gravity, const double _airViscosity);
    void Update(const float deltaTime, const double _Gravity, const double _airViscosity);
};