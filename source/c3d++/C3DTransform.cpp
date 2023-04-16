#include <c3d++/C3DTransform.h>
#include <numbers>

C3DTransform::C3DTransform()
{
}

C3DTransform::~C3DTransform()
{
}

vec<float, 3> C3DTransform::getPos() const
{
    return position_;
}

void C3DTransform::setPos(vec<float, 3> position)
{
    position_ = position;
}

vec<float, 3> C3DTransform::getYPR() const
{
    return ypr_;
}

void C3DTransform::setYPR(vec<float, 3> ypr)
{
    ypr_ = ypr;
}

float C3DTransform::getScale() const
{
    return scale_;
}

void C3DTransform::setScale(float scale)
{
    scale_ = scale;
}

vec<float, 3> C3DTransform::getForward() const
{
    vec<float, 3> mYPR = getYPR();
    vec<float, 3> direction;
    direction.x = cos(std::numbers::pi * mYPR.x / 180.0f) * cos(std::numbers::pi * mYPR.y / 180.f);
    direction.y = sin(std::numbers::pi * mYPR.y / 180.f);
    direction.z = sin(std::numbers::pi * mYPR.x / 180.f) * cos(std::numbers::pi * mYPR.y / 180.f);
    return linalg::normalize(direction);
}

vec<float, 3> C3DTransform::getUp() const
{
    vec<float, 3> mYPR = getYPR();
    vec<float, 3> direction;
    direction.x = cos(std::numbers::pi * mYPR.x / 180.0f) * cos(std::numbers::pi * (mYPR.y + 90.f) / 180.f);
    direction.y = sin(std::numbers::pi * (mYPR.y + 90.f) / 180.f);
    direction.z = sin(std::numbers::pi * mYPR.x / 180.f) * cos(std::numbers::pi * (mYPR.y + 90.f) / 180.f);
    return linalg::normalize(direction);
}

vec<float, 3> C3DTransform::getRight() const
{
    vec<float, 3> mYPR = getYPR();
    vec<float, 3> direction;
    direction.x = cos(std::numbers::pi * (mYPR.x + 90.f) / 180.0f) * cos(std::numbers::pi * mYPR.y / 180.f);
    direction.y = sin(std::numbers::pi * mYPR.y / 180.f);
    direction.z = sin(std::numbers::pi * (mYPR.x + 90.f) / 180.f) * cos(std::numbers::pi * mYPR.y / 180.f);
    return linalg::normalize(direction);
}
