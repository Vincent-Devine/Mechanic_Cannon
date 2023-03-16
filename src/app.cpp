
#include "app.hpp"

#include "imgui_utils.hpp"

App::App()
: cannonGame(renderer)
{
}

App::~App()
{
}

void App::Init()
{
    cannonGame.Init();
}

void App::UpdateAndDraw(float deltaTime)
{
    cannonGame.Update(deltaTime);
    cannonGame.Draw(deltaTime);
}
