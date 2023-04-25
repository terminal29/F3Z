#pragma once
#include <entity/component/Component.h>
#include <library/linalg.h>
#include <library/reactphysics3d/reactphysics3d.h>
#include <memory>
#include <utilities/Time.h>
#include <vector>

class RigidBodyComponent : public Component<RigidBodyComponent> {
public:
    RigidBodyComponent();
    virtual ~RigidBodyComponent();

    virtual void receive(MessageType type) final override;

    virtual void onAttach(baba::Entito* e) final override;
    virtual void onDetach() final override;

    void setVelocity(linalg::vec<float, 3> velocity);

    // create copy and assign operators
    RigidBodyComponent(const RigidBodyComponent& other) {
        // std::cout << "CCTOR" << std::endl;
        // removeBody();
        // bodyDef_ = other.bodyDef_;
        // boxDefs_ = other.boxDefs_;
        // if (entity_ != nullptr)
        //     createBody();
    };

    RigidBodyComponent& operator=(const RigidBodyComponent& other)
    {
        // std::cout << "CASS" << std::endl;
        // removeBody();
        // bodyDef_ = other.bodyDef_;
        // boxDefs_ = other.boxDefs_;
        // if (entity_ != nullptr)
        //     createBody();
        return *this;
    };

    static constexpr double tickRate = 1.0 / 60.0;

private:
    void tickGlobal();
    void tickLocal();
    void createBody();
    void removeBody();

    static double lastTick;
};
