#include "Cannonball.hpp"

#include "calc.hpp"
/*
    Variable name:
        - g             = 9.8067 m/s² initialy
        - V0x           = cos(alpha) * initial Speed
        - V0y           = sin(alpha) * initial Speed
        - x0            = initial horizontal position
        - y0            = initial vertical position
        - alpha         = initial angle
        - t             = time
        - m             = mass
        - k             = drag constant
        - AIR_VISCOSITY = 1.81e-05 kg/m.s initialy as taken at 15°C
*/

void Cannonball::Init(const float cannonAngle, const float cannonLength, const float2 cannonPosition, const float cannonInitSpeed, const float cannonDeceleration, const unsigned int id, const float radius, const float weight, bool airDrag, const double _Gravity, const double _airViscosity)
{
    this->id = id;
    init.angle = cannonAngle * (float)TAU / 360.f;
    // Our cannonball initial position is the end of the cannon and not the center of its base
    float2 offset = { cosf(init.angle) * cannonLength, sinf(init.angle) * cannonLength };
    init.position = cannonPosition + offset;
    init.speed = sqrtf(cannonInitSpeed * cannonInitSpeed + 2.f * cannonDeceleration * cannonLength);
    init.v0.x = init.speed * cosf(init.angle);
    init.v0.y = init.speed * sinf(init.angle);

    if(!airDrag)
        expected =
        {
            /*
                               V0y + sqrt(V0y² + 2 * g * y0)
                Landing time = -----------------------------
                                             g
            */
            (init.v0.y + sqrtf(init.v0.y * init.v0.y + 2.f * (float)_Gravity * init.position.y)) / (float)_Gravity,

            /*
                                   V0x * V0y + V0x * sqrt(V0y² + 2 * g * y0)
                Landing distance = ----------------------------------------- + x0
                                                      g
            */
            init.v0.y * init.v0.y / (2.f * (float)_Gravity) + init.position.y,

            /*
                                     V0y²
                Landing distance = ------- + y0
                                    2 * g
            */
            (init.v0.x * init.v0.y + init.v0.x * sqrtf(init.v0.y * init.v0.y + 2.f * (float)_Gravity * init.position.y)) / (float)_Gravity
        };

    this->radius = radius;
    this->weight = weight;
    this->airDrag = airDrag;

    /*
        Air drag constant calculation taken from https://www.physagreg.fr/mecanique-12-chute-frottements.php as k = 6 * PI * AIR_VISCOSITY * radius
        with AIR_VISCOSITY taken at 15°C which means 1.81e-05 kg/m.s initialy
    */
    dragConst = 3. * TAU * _airViscosity * radius;
}

void Cannonball::Update(const float deltaTime, const double _Gravity, const double _airViscosity)
{
    if (live.landingDistance != 0.f)
        return;
    
    // Stop projectile calculation if the cannonball hit the ground
    if (position.y < 0.f)
    {
        live.landingDistance = position.x - init.position.x;
        position.y = 0.f;
        prevPos.clear();
        return;
    }

    // Without air drag
    if (!airDrag)
    {
        // x(t) = V0x * t + x0
        position.x = init.speed * cosf(init.angle) * live.time + init.position.x;

        /*
                    -g * t²
            y(t) = --------- + V0y * t  + y0
                       2
        */
        position.y = -(float)_Gravity / 2 * live.time * live.time + init.speed * sinf(init.angle) * live.time + init.position.y;

        // Speed = sqrt( V0x² + (-g * t + V0y)² )
        speed = sqrtf(init.v0.x * init.v0.x + (-1.f * live.time * (float)_Gravity + init.v0.y) * (-1.f * live.time * (float)_Gravity + init.v0.y));
    }
    // With air drag
    else
    {
        double weightOverDrag = weight / dragConst;
        double dragOverWeight = dragConst / weight;
        double exponentialExpression = (1. - exp(-1. * dragOverWeight * live.time));

        /*
                    m                       k
            x(t) = --- * V0x * ( 1 - e^( - --- * t ) ) + x0
                    k                       m
        */
        position.x = (float)(weightOverDrag * init.v0.x * exponentialExpression + init.position.x);

        /*
                    m                 m                   k                 m
            y(t) = --- * ( V0y + g * --- ) * ( 1 - e^( - --- * t ) ) - g * --- * t + y0
                    k                 k                   m                 k
        */
        position.y = (float)(weightOverDrag * (init.v0.y + _Gravity * weightOverDrag) * exponentialExpression - weightOverDrag * _Gravity * live.time + init.position.y);
    }
    
    prevPos.push_back(position);
    if (prevPos.size() > 10)
        prevPos.pop_front();

    // This takes the maximum value between current height of the cannonball and max height currently stored and then stores it into max height
    if (position.y > live.maxHeight)
        live.maxHeight = position.y;

    // Incrementing the time (t) variable
    live.time += deltaTime;
}