#pragma once

#include <vector>
#include <list>

#include "types.hpp"

class Renderer
{
    // Attributes
private:
    // Fast access to drawlist and io
    ImDrawList* dl;
    ImGuiIO* io;

    // World coordinates conversion (world is expressed in meters)
    float2 worldOrigin; // Origin in pixels
    float2 worldScale;  // Scale relative to pixels

    // Methods
public:
    void PreUpdate();

    float2 ToPixels(float2 coordinatesInMeters);
    float2 ToWorld(float2 coordinatesInPixels);

    // Draw functions
    void DrawGround();
    void DrawCannon(const float2& position, const float length, const float angle, std::vector<float2> trajectoryPoints);
    void DrawProjectileMotion(const float2& position, const std::list<float2> prevPos);
    void DrawTarget(const float2& position, const float& radius);
};