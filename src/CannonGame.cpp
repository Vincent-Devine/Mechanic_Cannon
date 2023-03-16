#include "CannonGame.hpp"
#include "calc.hpp"
#include <string>
#include <iostream>

CannonGame::CannonGame(Renderer& renderer)
    : renderer(renderer)
{
}

CannonGame::~CannonGame()
{
}

void CannonGame::Init()
{
    // Add cannon to the games
    cannons.push_back(Cannon({ -15.f, 0.4f }, 45.f, 0));
    cannons.push_back(Cannon({ +15.f, 0.4f }, 135.f, 1));
    // Add target to the games
    targets.push_back(Target({ -20.f, 11.f }, 1.f, 2.f, 3.f));
    targets.push_back(Target({ -18.f, 15.f }, 1.f, 2.f, 2.f));
    targets.push_back(Target({ -22.f, 7.f }, 1.f, 2.f, 4.f));
    targets.push_back(Target({ +20.f, 11.f }, 1.f, 2.f, 3.f));
    targets.push_back(Target({ +18.f, 15.f }, 1.f, 2.f, 2.f));
    targets.push_back(Target({ +22.f, 7.f }, 1.f, 2.f, 4.f));
}

void CannonGame::Update(const float deltaTime)
{
    renderer.PreUpdate();
    
    for (Cannon& cannon : cannons)
    {
        cannon.Update(deltaTime, _Gravity, _airViscosity);
    }

    for (Target& target : targets)
    {
        target.Update(deltaTime);
    }
    // Check the collision with cannonball and target
    CheckCollision();
}

void CannonGame::Draw(const float deltaTime)
{
    DrawImGUI(deltaTime);

    // Draw the ground
    renderer.DrawGround();
    
    // Draw the cannon and the cannonball
    for (Cannon& cannon : cannons)
    {
        renderer.DrawCannon(cannon.position, cannon.length, cannon.angle, cannon.trajectoryPoints);
        for (const Cannonball* cannonball : cannon.cannonballs)
        {
            renderer.DrawProjectileMotion(cannonball->position, cannonball->prevPos);
        }
    }

    // Draw the target
    for (Target& target : targets)
    {
        renderer.DrawTarget(target.position, target.radius);
    }
}

void CannonGame::DrawImGUI(const float deltaTime)
{
    if (ImGui::Begin("Constants : ", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static float gravity = 9.80665f, airViscosity = 1.81f;
        ImGui::SliderFloat("Gravity ", &gravity, 0.001f, 15.f);
        ImGui::SliderFloat("Air Viscosity ", &airViscosity, 0.001f, 50.f);
        _Gravity = gravity;
        _airViscosity = airViscosity * 10.e-5;
    }
    ImGui::End();

    for (unsigned int i = 0; i < cannons.size(); i++)
    {
        if (ImGui::Begin(("Cannon : " + std::to_string(i)).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Score : %d", cannons[i].points);
            ImGui::SliderFloat(("CannonHeight " + std::to_string(i)).c_str(), &cannons[i].position.y, 0.4f, 15.f);
            if (i == 0)
                ImGui::SliderFloat(("CannonAngle " + std::to_string(i)).c_str(), &cannons[i].angle, 0.f, 90.f);
            else
                ImGui::SliderFloat(("CannonAngle " + std::to_string(i)).c_str(), &cannons[i].angle, 90.f, 180.f);
            ImGui::SliderFloat(("CannonLength " + std::to_string(i)).c_str(), &cannons[i].length, 0.5f, 2.f);
            ImGui::SliderFloat(("Initial Speed " + std::to_string(i)).c_str(), &cannons[i].initialSpeed, 5.f, 25.f);
            ImGui::SliderFloat(("Deceleration " + std::to_string(i)).c_str(), &cannons[i].deceleration, -25.f, 0.f);
            ImGui::SliderFloat(("Cannon Weight " + std::to_string(i)).c_str(), &cannons[i].cannonWeight, 0.f, 10.f);
            ImGui::SliderFloat(("Cannonball Weight " + std::to_string(i)).c_str(), &cannons[i].cannonballWeight, 0.001f, 0.2f);
            ImGui::SliderFloat(("Cannonball Radius " + std::to_string(i)).c_str(), &cannons[i].cannonballRadius, 1.f, 10.f);
            ImGui::Checkbox(("Air Drag " + std::to_string(i)).c_str(), &cannons[i].airDrag);
            if (ImGui::Button(("Fire ! " + std::to_string(i)).c_str(), ImVec2(300.f, 20.f)))
            {
                Cannonball* cannonball = new Cannonball;
                cannonball->Init(cannons[i].angle, cannons[i].length, cannons[i].position, cannons[i].initialSpeed, cannons[i].deceleration, cannons[i].idCannonball, cannons[i].cannonballRadius, cannons[i].cannonballWeight, cannons[i].airDrag, _Gravity, _airViscosity);
                cannons[i].cannonballs.push_back(cannonball);
                cannons[i].idCannonball++;
                if (cannons[i].cannonballs.size() > 5)
                {
                    delete cannons[i].cannonballs.front();
                    cannons[i].cannonballs.pop_front();
                }
                if (i == 0)
                    cannons[i].cannonVelocity += (cannons[i].cannonballWeight * cannonball->init.speed) / (cannons[i].cannonballWeight + cannons[i].cannonWeight) * deltaTime * cosf(cannonball->init.angle);
                else
                    cannons[i].cannonVelocity += ((cannons[i].cannonballWeight * cannonball->init.speed) / (cannons[i].cannonballWeight + cannons[i].cannonWeight) * deltaTime * (-cosf(cannonball->init.angle)));
            }
        }
        ImGui::End();

        if (ImGui::Begin(("Stats cannon : " + std::to_string(i)).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            for (const Cannonball* cannonball : cannons[i].cannonballs)
            {
                std::string name = "Cannonball [" + std::to_string(cannonball->id) + ']';
                if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_NoAutoOpenOnLog))
                {
                    if (ImGui::CollapsingHeader(std::string(std::to_string(cannonball->id) + " Live").c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::Text("MaxHeight %.2fm", cannonball->live.maxHeight);
                        ImGui::Text("FlyingTime %.2fs", cannonball->live.time);
                        ImGui::Text("MaxDistance %.2fm", cannonball->live.landingDistance);
                        if(!cannonball->airDrag)
                            ImGui::Text("Speed %.2fm.s-1", cannonball->speed);
                    }
                    if (!cannonball->airDrag && ImGui::CollapsingHeader(std::string(std::to_string(cannonball->id) + " Expected").c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::Text("MaxHeight %.2fm", cannonball->expected.maxHeight);
                        ImGui::Text("FlyingTime %.2fs", cannonball->expected.time);
                        ImGui::Text("MaxDistance %.2fm", cannonball->expected.landingDistance);
                    }
                }
            }
        }
        ImGui::End();
    }
}

void CannonGame::CheckCollision()
{
    /*
        Collision point/circle:
        if distance between the point and the center of the circle is less than the circle radius then the collision occured
    */
    for ( Cannon& cannon : cannons)
    {
        for ( Cannonball* cannonball : cannon.cannonballs)
        {
            if (!cannonball->hasHit)
            {
                for ( Target& target : targets)
                {   
                    float2 distance = cannonball->position - target.position;
                    if (sqrt(distance.x * distance.x + distance.y * distance.y) <= target.radius)
                    {
                        cannon.points += target.point;
                        cannonball->hasHit = true;
                    }
                }
            }
        }
    }
}
