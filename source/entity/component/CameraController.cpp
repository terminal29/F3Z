#include <Loader.h>
#include <entity/Entity.h>
#include <entity/component/CameraController.h>
#include <entity/component/WorldComponent.h>
#include <utilities/Math.h>

CameraController::CameraController()
{
}

void CameraController::receive(MessageType type)
{
    if (type == MessageType::MSG_UPDATE) {
        update();
    }
}

float angleLerp(float start, float end, float amount)
{
    float difference = std::abs(end - start);
    if (difference > 180) {
        // We need to add on to one of the values.
        if (end > start) {
            // We'll add it on to start...
            start += 360;
        } else {
            // Add it on to end.
            end += 360;
        }
    }

    // Interpolate it.
    float value = (start + ((end - start) * amount));

    // Wrap it..
    float rangeZero = 360;

    if (value >= 0 && value <= 360)
        return value;

    return (std::fmod(value, rangeZero));
}

void CameraController::update()
{
    if (entity_ == nullptr)
        return;

    if (auto worldComponent = entity_->getWorld().lock()) {
        if (auto followEntity = followEntity_.lock()) {
            auto direction = worldComponent->getDirection();
            std::cout << "World Dir: " << WorldDirectionNames[direction] << std::endl;
            float targetAngle = WorldComponent::getDirectionAngle(direction);

            followPoint_ = followEntity->getTransform().getPos();

            if (!hasSetCurrentFollowPoint) {
                hasSetCurrentFollowPoint = true;
                currentFollowPoint_ = followPoint_;
            }

            const float moveLerpSpeed = 10.0f;
            const float rotateLerpSpeed = 10.0f;

            currentAngle_ = angleLerp(currentAngle_, targetAngle, Time::dt() * rotateLerpSpeed);

            // Calculate camera position
            currentFollowPoint_ = vec<float, 3> { std::lerp(currentFollowPoint_.x, followPoint_.x, Time::dt() * moveLerpSpeed), std::lerp(currentFollowPoint_.y, followPoint_.y, Time::dt() * moveLerpSpeed), std::lerp(currentFollowPoint_.z, followPoint_.z, Time::dt() * moveLerpSpeed) };
            vec<float, 3> cameraPosition = currentFollowPoint_;
            cameraPosition.x += followDistance_ * -std::sin(Math::deg2rad(currentAngle_));
            cameraPosition.z += followDistance_ * std::cos(Math::deg2rad(currentAngle_));

            vec<float, 3> cameraRotation { Math::deg2rad(360 - currentAngle_), 0.0f, 0.0f };

            // Set camera rotation
            auto cameraTransform = entity_->getTransform();
            cameraTransform.setYPR(cameraRotation);
            cameraTransform.setPos(cameraPosition);
            entity_->setTransform(cameraTransform);
            C3DRenderer::setCameraTransform(cameraTransform);
        }
    }
}

CameraController::~CameraController()
{
}

void CameraController::setFollowEntity(std::weak_ptr<Entity> entity)
{
    followEntity_ = entity;
}