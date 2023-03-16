#include "Renderer.hpp"

#include "calc.hpp"

void Renderer::PreUpdate()
{
    // Get frequently used variables
    dl = ImGui::GetBackgroundDrawList();
    io = &ImGui::GetIO();

    // Compute world space
    // ===================
    // Origin:
    //  - x at the center of the window
    //  - y at the 3/4 bottom of the window
    // Scale:
    //  50 meters from left of the window to the right
    //  Uniform scale in y except it's pointing to up
    worldOrigin.x = io->DisplaySize.x / 2.f;
    worldOrigin.y = io->DisplaySize.y - io->DisplaySize.y / 4.f;
    worldScale.x = io->DisplaySize.x / 50.f;
    worldScale.y = -worldScale.x; // Same scale as X bu invert it to invert y coordinates
}

float2 Renderer::ToPixels(float2 coordinatesInMeters)
{
    return (coordinatesInMeters * worldScale) + worldOrigin;
}

float2 Renderer::ToWorld(float2 coordinatesInPixels)
{
    return (coordinatesInPixels - worldOrigin) / worldScale;
}

void Renderer::DrawGround()
{
    float2 left = this->ToPixels({ -100.f, 0.f });
    float2 right = this->ToPixels({ +100.f, 0.f });

    dl->AddLine(left, right, IM_COL32_WHITE);
}

void Renderer::DrawCannon(const float2& position, const float length, const float angle, std::vector<float2> trajectoryPoints)
{
    float2 pos = this->ToPixels(position);
    dl->AddCircleFilled(pos, 10.f, IM_COL32_WHITE);
    float2 finalPos = {
        position.x + length * cosf(angle * (float)TAU / 360.f),
        position.y + length * sinf(angle * (float)TAU / 360.f)
    };
    finalPos = ToPixels(finalPos);

    dl->AddLine(pos, finalPos, IM_COL32_WHITE, 7.5f);
    dl->AddLine(pos + float2{ 40.f, 10.f }, pos + float2{ -40.f, 10.f }, IM_COL32_WHITE);
    dl->AddLine(pos + float2{ 40.f, 10.f }, float2{ pos.x + 40.f, 0.f }, IM_COL32_WHITE);
    dl->AddLine(pos + float2{ -40.f, 10.f }, float2{ pos.x - 40.f, 0.f }, IM_COL32_WHITE);

    for (int i = 0; i < trajectoryPoints.size() - 1; ++i)
    {
        float2 cur = ToPixels(trajectoryPoints[i]);
        float2 next = ToPixels(trajectoryPoints[i + 1]);
        dl->AddLine(cur, next, IM_COL32_WHITE);
    }
}

void Renderer::DrawProjectileMotion(const float2& position, const std::list<float2> prevPos)
{
    float2 pos = ToPixels(position);
    dl->AddCircleFilled(pos, 5.f, IM_COL32_BLACK);

    if (prevPos.size() < 2)
        return;

    auto head = prevPos.begin();
    std::advance(head, prevPos.size() - 1);

    for (auto it = prevPos.begin(); it != head; ++it)
    {
        float2 lastPos = ToPixels(*it);
        ++it;
        float2 curPos = ToPixels(*it);
        --it;
        dl->AddLine({ lastPos.x, lastPos.y }, { curPos.x, curPos.y }, IM_COL32(255, 0, 0, 255));
    }
}

void Renderer::DrawTarget(const float2& position, const float& radius)
{
    float2 pos = ToPixels(position);
    dl->AddCircleFilled(pos, radius * 20.f, IM_COL32(255, 0, 0, 255));

}