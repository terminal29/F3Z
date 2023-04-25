#include <Entity/Entity.h>
#include <Time.h>
#include <entity/component/RigidBodyComponent.h>

double RigidBodyComponent::lastTick = 0.0;

RigidBodyComponent::RigidBodyComponent()
{
    // std::cout << "CTOR" << std::endl;
}

void RigidBodyComponent::onAttach(baba::Entito* e)
{
    // std::cout << "onAttach" << std::endl;
    // createBody();
}

void RigidBodyComponent::createBody()
{
    // std::cout << "createBody -> " << (void*)body << std::endl;
    // assert(entity_ != nullptr && "Entity must be set before creating body");
    // volatile std::string name = entity_->getName();
    // std::cout << *const_cast<std::string*>(&name) << std::endl;
    // if (body) {
    //     removeBody();
    // }
    // body = scene.CreateBody(bodyDef_);

    // for (const auto& boxDef : boxDefs_) {
    //     body->AddBox(boxDef);
    // }
    // const auto& position = entity_->getTransform().getPos();
    // q3Transform globalSpace; // Contains position and orientation, see q3Transform.h for details
    // q3Identity(globalSpace); // Specify the origin, and identity orientation
    // globalSpace.position = q3Vec3(position.x, position.y, position.z);
    // // TODO Rotation

    // body->SetTransform(globalSpace.position);
    // body->SetToAwake();
}

void RigidBodyComponent::removeBody()
{
}

void RigidBodyComponent::onDetach()
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::receive(MessageType type)
{
    // switch (type) {
    // case MessageType::MSG_PHYSICS_TICK: {
    //     // std::cout << "Physics tick: " << lastTick << std::endl;
    //     if (lastTick < 0.01) {
    //         // First physics tick
    //         tickGlobal();
    //     } else if (lastTick + tickRate < Time::t()) {
    //         size_t ticks = (Time::t() - lastTick) / tickRate;
    //         for (size_t i = 0; i < ticks; i++) {
    //             tickGlobal();
    //         }
    //     }
    //     lastTick = Time::t();
    //     tickLocal();
    //     break;
    // }
    // default:
    //     break;
    // }
}

void RigidBodyComponent::tickGlobal()
{
    // scene.Step();
}

void RigidBodyComponent::tickLocal()
{

    // // Move this entity to the position of the rigid body
    // if (entity_ && body) {
    //     std::cout << "tickLocal " << (void*)entity_ << " : " << (void*)body << std::endl;
    //     std::cout << "bodyType " << body->GetMass() << " : " << body->GetGravityScale() << std::endl;
    //     const auto [position, rotation] = body->GetTransform();

    //     float sy = sqrt(rotation.Column0().x * rotation.Column0().x + rotation.Column1().x * rotation.Column1().x);

    //     bool singular = sy < 1e-6; // If

    //     float x, y, z;
    //     if (!singular) {
    //         x = atan2(rotation.Column2().y, rotation.Column2().z);
    //         y = atan2(-rotation.Column2().x, sy);
    //         z = atan2(rotation.Column1().x, rotation.Column0().x);
    //     } else {
    //         x = atan2(-rotation.Column1().z, rotation.Column1().y);
    //         y = atan2(-rotation.Column2().x, sy);
    //         z = 0;
    //     }

    //     auto transform = entity_->getTransform();
    //     transform.setPos({ position.x, position.y, position.z });
    //     transform.setYPR({ z, y, x });
    //     entity_->setTransform(transform);
    // }
}

void RigidBodyComponent::setVelocity(vec<float, 3> velocity)
{
    // if (body) {

    //     std::cout << "Veclocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
    //     body->SetLinearVelocity(q3Vec3(velocity.x, velocity.y, velocity.z));
    // }
}
