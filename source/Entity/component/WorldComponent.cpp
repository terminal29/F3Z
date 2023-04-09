#include <Entity/component/WorldComponent.h>

std::unordered_map<WorldDirection, std::string> WorldDirectionNames = {
    { WorldDirection::NORTH, "NORTH" },
    { WorldDirection::SOUTH, "SOUTH" },
    { WorldDirection::EAST, "EAST" },
    { WorldDirection::WEST, "WEST" },
};

WorldComponent::WorldComponent()
{
}

void WorldComponent::receive(MessageType type)
{
    if (type == MessageType::MSG_UPDATE) {
        update();
    }
}

WorldComponent::~WorldComponent()
{
}

WorldDirection WorldComponent::getDirection() const noexcept
{
    return worldDirection_;
}

float WorldComponent::getDirectionAngle(WorldDirection direction) noexcept
{
    switch (direction) {
    case WorldDirection::NORTH:
        return 0.0f;
    case WorldDirection::EAST:
        return 90.0f;
    case WorldDirection::SOUTH:
        return 180.0f;
    case WorldDirection::WEST:
        return 270.0f;
    }
    return 0.0f;
}

WorldDirection WorldComponent::switchDirection(bool left) noexcept
{
    if (left) {
        switch (worldDirection_) {
        case WorldDirection::NORTH:
            worldDirection_ = WorldDirection::WEST;
            break;
        case WorldDirection::SOUTH:
            worldDirection_ = WorldDirection::EAST;
            break;
        case WorldDirection::EAST:
            worldDirection_ = WorldDirection::NORTH;
            break;
        case WorldDirection::WEST:
            worldDirection_ = WorldDirection::SOUTH;
            break;
        default:
            worldDirection_ = WorldDirection::NORTH;
        }
    } else {
        switch (worldDirection_) {
        case WorldDirection::NORTH:
            worldDirection_ = WorldDirection::EAST;
            break;
        case WorldDirection::SOUTH:
            worldDirection_ = WorldDirection::WEST;
            break;
        case WorldDirection::EAST:
            worldDirection_ = WorldDirection::SOUTH;
            break;
        case WorldDirection::WEST:
            worldDirection_ = WorldDirection::NORTH;
            break;
        default:
            worldDirection_ = WorldDirection::NORTH;
        }
    }
    return worldDirection_;
}

void WorldComponent::update()
{
}