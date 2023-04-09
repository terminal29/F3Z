#include <Entity/Entity.h>
#include <Entity/component/CharacterController.h>
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
        std::cout << "ChCr: " << targetAngle << std::endl;

        const float moveSpeed = 0.1f;
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        circlePosition analogStick;
        hidCircleRead(&analogStick);

        auto characterTransform = entity_->getTransform();
        auto characterPosition = characterTransform.getPos();

        if ((analogStick.dy > 25 || analogStick.dy < -25)) {
            std::cout << "Y" << std::endl;
            characterPosition.y += (float)analogStick.dy * Time::dt() * moveSpeed;
        }
        if ((analogStick.dx > 25 || analogStick.dx < -25)) {
            std::cout << "X" << std::endl;
            characterPosition.x += std::cos(Math::deg2rad(targetAngle)) * (float)analogStick.dx * Time::dt() * moveSpeed;
            characterPosition.z += std::sin(Math::deg2rad(targetAngle)) * (float)analogStick.dx * Time::dt() * moveSpeed;
        }

        if (kDown & KEY_L) {
            std::cout << "L " << targetAngle << std::endl;
            world->switchDirection(true);
        } else if (kDown & KEY_R) {
            std::cout << "R " << targetAngle << std::endl;
            world->switchDirection(false);
        }
        characterTransform.setPos(characterPosition);
        entity_->setTransform(characterTransform);
    }
}