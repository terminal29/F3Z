#pragma once
#include <entity/component/Component.h>

class WorldComponent : public Component<WorldComponent> {
public:
    WorldComponent();

    virtual void receive(MessageType type) final override;

    virtual ~WorldComponent();

    inline virtual std::shared_ptr<ComponentBase> clone() const noexcept override final
    {
        return std::make_shared<WorldComponent>(*this);
    }
};