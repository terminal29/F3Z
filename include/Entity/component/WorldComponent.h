#pragma once
#include <entity/component/Component.h>

enum class WorldDirection {
    NORTH,
    SOUTH,
    EAST,
    WEST,
};

extern std::unordered_map<WorldDirection, std::string> WorldDirectionNames;

class WorldComponent : public Component<WorldComponent> {
public:
    WorldComponent();

    virtual void receive(MessageType type) final override;

    virtual ~WorldComponent();

    WorldDirection getDirection() const noexcept;

    WorldDirection switchDirection(bool left) noexcept;

    static float getDirectionAngle(WorldDirection direction) noexcept;

private:
    void update();

    WorldDirection worldDirection_ = WorldDirection::NORTH;
};