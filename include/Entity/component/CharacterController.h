#pragma once

#include <entity/component/CameraController.h>
#include <entity/component/Component.h>

class CharacterController : public Component<CharacterController> {
public:
    CharacterController();
    virtual ~CharacterController();

    void setCameraController(std::weak_ptr<CameraController> cameraController);
    std::weak_ptr<CameraController> getCameraController() const;

    virtual void receive(MessageType type) final override;

private:
    std::weak_ptr<CameraController> cameraController_;
    void update();
};