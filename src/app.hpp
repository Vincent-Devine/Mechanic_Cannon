#pragma once

#include "CannonGame.hpp"
#include "Renderer.hpp"

class App
{
public:
    App();
    ~App();

    void Init();
    void UpdateAndDraw(float deltaTime);

private:
    Renderer renderer;
    CannonGame cannonGame;
};
