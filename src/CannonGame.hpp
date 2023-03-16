#pragma once

#include <list>
#include <vector>

#include "types.hpp"
#include "Cannon.hpp"
#include "Renderer.hpp"
#include "Target.hpp"

class CannonGame
{
    // Attributes
private:
    std::vector<Cannon> cannons;
    Renderer& renderer;
    std::vector<Target> targets;
    double _airViscosity = 1.81e-05;
    double _Gravity = 9.80665;

    // Methods
public:
    CannonGame(Renderer& renderer);
    ~CannonGame();

    void Init();

    void Update(float deltaTime);
    void Draw(const float deltaTime);

private:
    void DrawImGUI(const float deltaTime);
    void CheckCollision();
};