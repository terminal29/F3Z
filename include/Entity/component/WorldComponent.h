#pragma once
#include <entity/component/Component.h>

class WorldComponent : public Component<WorldComponent>
{
public:
    WorldComponent();

    virtual void receive(MessageType type) final override;

    virtual ~WorldComponent();

private:
    virtual void update();
};