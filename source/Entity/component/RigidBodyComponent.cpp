#include <Entity/Entity.h>
#include <Time.h>
#include <entity/component/RigidBodyComponent.h>
#define u32 q3_u32
#include <q3.h>
#undef u32

double RigidBodyComponent::lastTick = 0.0;

std::unique_ptr<q3Scene> RigidBodyComponent::scene = nullptr;

RigidBodyComponent::RigidBodyComponent()
{
    // Create scene first time rigid body component is used
    if (!scene) {
        scene = std::make_unique<q3Scene>(RigidBodyComponent::tickRate);
        scene->SetGravity(q3Vec3(0.0f, -9.81f, 0.0f));
    }
}

void RigidBodyComponent::onAttach(Entity* e)
{
    q3BodyDef bodyDef;
    bodyDef.bodyType = eDynamicBody;
    body = scene->CreateBody(bodyDef);

    q3BoxDef boxDef; // See q3Box.h for settings details
    q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
    q3Identity(localSpace); // Specify the origin, and identity orientation

    const auto& position = e->getTransform().getPos();
    localSpace.position = q3Vec3(position.x, position.y, position.z);
    boxDef.Set(localSpace, q3Vec3(1.0, 1.0, 1.0));
    body->AddBox(boxDef);
}

void RigidBodyComponent::onDetach()
{
    scene->RemoveBody(body);
    body = nullptr;
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::receive(MessageType type)
{
    switch (type) {
    case MessageType::MSG_PHYSICS_TICK: {
        // std::cout << "Physics tick: " << lastTick << std::endl;
        if (lastTick < 0.01) {
            // First physics tick
            tickGlobal();
        } else if (lastTick + tickRate < Time::t()) {
            size_t ticks = (Time::t() - lastTick) / tickRate;
            for (size_t i = 0; i < ticks; i++) {
                tickGlobal();
            }
        }
        lastTick = Time::t();
        tickLocal();
    }
    default:
        break;
    }
}

void RigidBodyComponent::tickGlobal()
{
    scene->Step();
}

void RigidBodyComponent::tickLocal()
{
    // Move this entity to the position of the rigid body
    if (entity_) {
        const auto [position, rotation] = body->GetTransform();

        float sy = sqrt(rotation.Column0().x * rotation.Column0().x + rotation.Column1().x * rotation.Column1().x);

        bool singular = sy < 1e-6; // If

        float x, y, z;
        if (!singular) {
            x = atan2(rotation.Column2().y, rotation.Column2().z);
            y = atan2(-rotation.Column2().x, sy);
            z = atan2(rotation.Column1().x, rotation.Column0().x);
        } else {
            x = atan2(-rotation.Column1().z, rotation.Column1().y);
            y = atan2(-rotation.Column2().x, sy);
            z = 0;
        }

        auto transform = entity_->getTransform();
        transform.setPos({ position.x, position.y, position.z });
        transform.setYPR({ z, y, x });
        entity_->setTransform(transform);
    }
}
