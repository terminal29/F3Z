#include <Entity/Entity.h>
#include <Entity/component/CharacterController.h>
#include <Entity/component/RigidBodyComponent.h>
#include <Entity/component/WorldComponent.h>
#include <utilities/Math.h>

CharacterController::CharacterController()
{
}

void CharacterController::receive(MessageType type)
{
    if (type == MessageType::MSG_UPDATE) {
        update();
    }
}

CharacterController::~CharacterController()
{
}

void CharacterController::setCameraController(std::weak_ptr<CameraController> cameraController)
{
    cameraController_ = cameraController;
}

std::weak_ptr<CameraController> CharacterController::getCameraController() const
{
    return cameraController_;
}

void CharacterController::update()
{
    if (!entity_)
        return;
    if (auto world = entity_->getWorld().lock()) {
        auto direction = world->getDirection();
        auto targetAngle = world->getDirectionAngle(direction);

        const float moveSpeed = 0.01f;
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        circlePosition analogStick;
        hidCircleRead(&analogStick);

        auto characterTransform = entity_->getTransform();
        auto characterPosition = characterTransform.getPos();

        if ((analogStick.dy > 25 || analogStick.dy < -25)) {
            characterPosition.y += (float)analogStick.dy * moveSpeed;
        }
        if ((analogStick.dx > 25 || analogStick.dx < -25)) {
            characterPosition.x += std::cos(Math::deg2rad(targetAngle)) * (float)analogStick.dx * moveSpeed;
            characterPosition.z += std::sin(Math::deg2rad(targetAngle)) * (float)analogStick.dx * moveSpeed;
        }

        if (kDown & KEY_L) {
            world->switchDirection(true);
        } else if (kDown & KEY_R) {
            world->switchDirection(false);
        }
        // characterTransform.setPos(characterPosition);
        // entity_->setTransform(characterTransform);
        auto rigidBodyComponent = entity_->getComponent<RigidBodyComponent>();
        if (auto rigidBody = rigidBodyComponent.lock()) {
            rigidBody->setVelocity({ characterPosition.x, characterPosition.y, characterPosition.z });
        }
    }
}