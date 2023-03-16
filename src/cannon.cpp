#include <stdio.h>
#include <string>
#include <math.h>
#include <iostream>

#include "calc.hpp"
#include "cannon.hpp"

Cannon::Cannon(const float2& p_position, const float p_angle, const  int p_id)
    : cannonballs(std::list<Cannonball*>())
    , idCannonball(0)
    , position(p_position)
    , initPosition(position)
    , angle(p_angle)
    , initialSpeed(15.f)
    , length(1.f)
    , deceleration(0.f)
    , subDiv(500)
    , trajectoryPoints(std::vector<float2>())
    , cannonWeight(5.f)
    , cannonballWeight(0.2f)
    , cannonVelocity(0.f)
    , id (p_id)
    , points(0)
{
}

Cannon::~Cannon()
{
    // Destroy all cannonball when destroying the cannon
    for (Cannonball* cannonball : cannonballs)
        delete cannonball;
}

void Cannon::Update(const float deltaTime, const double _Gravity, const double _airViscosity)
{
    // Add recoil to the cannon when firing
    if (angle < 90.f)
        position.x -= cannonVelocity * deltaTime;
    else
        position.x += cannonVelocity * deltaTime;

    cannonVelocity -= 0.001f;
    if (cannonVelocity < 0.f)
        cannonVelocity = 0.f;

    // Reset position if the cannon went to far
    if (angle < 90.f && position.x <= -17.f)
    {
        cannonVelocity = 0.f;
        position.x = initPosition.x;
    }
    else if (angle > 90.f && position.x >= 17.f)
    {
        cannonVelocity = 0.f;
        position.x = initPosition.x;
    }

    // Trajectory previsualization, we divide the expected distance by the amount of subdivision we want in order to have a smooth trajectory
    float initAngle = angle * (float)TAU / 360.f;
    float2 offset = { cosf(initAngle) * length, sinf(initAngle) * length };
    float2 initPosition = position + offset;

    // Initial speed at cannon's end = sqrtf(initialSpeed² + 2 * deceleration * cannon length)
    float initSpeed = sqrtf(initialSpeed * initialSpeed + 2.f * deceleration * length);

    float initv0x = initSpeed * cosf(initAngle);
    float initv0y = initSpeed * sinf(initAngle);
    float expectedDistance = initv0x * initv0y + initv0x * sqrtf(initv0y * initv0y + 2.f * (float)_Gravity * position.y) / (float)_Gravity;
    float expectedDivSize = expectedDistance / (subDiv * 1.f);

    // Clear previous frame trajectory calculation
    trajectoryPoints.clear();

    for (int i = 0; i < subDiv; ++i)
    {
        // Without air drag
        // currentX = subdivision size * incrementing integer
        float x = i * expectedDivSize;
        if (!airDrag)
        {
            trajectoryPoints.push_back
            ({
                // x = currentX + x0                 
                x + initPosition.x,

                /*
                         -g * currentX²
                    y = ---------------- + currentX * tan(alpha) + y0
                            2 * V0x²
                */
                (-1.f * (float)_Gravity * x * x) / (2.f * initSpeed * initSpeed * cosf(initAngle) * cosf(initAngle)) + x * tanf(initAngle) + initPosition.y
            });
        }
        // With air drag, same calculation as in "Cannoball.cpp"
        else
        {
            double dragConst = dragConst = 3. * TAU * _airViscosity * cannonballRadius;
            double weightOverDrag = cannonballWeight / dragConst;
            double dragOverWeight = dragConst / cannonballWeight;
            double exponentialExpression = (1. - exp(-1. * dragOverWeight * i * deltaTime));

            trajectoryPoints.push_back
            ({
                (float)(weightOverDrag * initv0x * exponentialExpression + initPosition.x),
                (float)(weightOverDrag * (initv0y + _Gravity * weightOverDrag) * exponentialExpression - weightOverDrag * _Gravity * i * deltaTime + initPosition.y)
            });

        }

        // If predicted trajectory point is below ground level we stop the calculation
        if (trajectoryPoints[trajectoryPoints.size() - 1].y < 0.f)
            break;
    }

    for (Cannonball* cannonball : cannonballs)
    {
        cannonball->Update(deltaTime, _Gravity, _airViscosity);
    }
}
