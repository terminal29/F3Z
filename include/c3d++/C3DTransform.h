#pragma once

#include <citro3d.h>
#include <cmath>
#include <library/linalg.h>

using namespace linalg;

class C3DTransform {
public:
    C3DTransform();
    ~C3DTransform();

    vec<float, 3> getPos() const;
    void setPos(vec<float, 3> position);

    float getScale() const;
    void setScale(float scale);

    // Degrees
    void setYPR(vec<float, 3> ypr);
    vec<float, 3> getYPR() const;

    vec<float, 3> getForward() const;

    vec<float, 3> getUp() const;

    vec<float, 3> getRight() const;

private:
    vec<float, 3> position_ = { 0.0f, 0.0f, 0.0f };
    vec<float, 3> ypr_ = { 0, 0, 0 };
    float scale_ = 1;
};
