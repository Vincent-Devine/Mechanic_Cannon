#pragma once

#include <imgui.h>
#include <list>
#include <vector>

#include "types.hpp"
#include "Cannonball.hpp"
#include "Renderer.hpp"

class Cannon
{
    // Attributes
public:
    std::list<Cannonball*> cannonballs;
    unsigned int idCannonball;

     int id;
    float2 position;
    float2 initPosition;
    float angle;
    float initialSpeed;
    float length;
    float deceleration; // Deceleration create by the cannon to the cannonball
    int subDiv; // number of point create to draw the trajectory
    std::vector<float2> trajectoryPoints;
    float cannonWeight;
    float cannonballWeight;
    bool airDrag = false;
    float cannonballRadius = 0.5f;
    float cannonVelocity;
    unsigned int points;

    // Methods
public:
    Cannon(const float2& p_position, const float p_angle, const  int p_id);
    ~Cannon();

    void Update(float deltaTime, const double _Gravity, const double _airViscosity);
};
