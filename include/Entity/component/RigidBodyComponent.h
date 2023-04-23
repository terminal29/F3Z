#pragma once
#include <entity/component/Component.h>
#include <memory>
#include <utilities/Time.h>

class q3Scene;
class q3Body;

class RigidBodyComponent : public Component<RigidBodyComponent> {
public:
    RigidBodyComponent();
    virtual ~RigidBodyComponent();

    virtual void receive(MessageType type) final override;

    virtual void onAttach(Entity* e) final override;
    virtual void onDetach() final override;

private:
    void tickGlobal();
    void tickLocal();

    static std::unique_ptr<q3Scene> scene;
    static double lastTick;
    static constexpr double tickRate = 1.0 / 60.0;
    q3Body* body;
};
