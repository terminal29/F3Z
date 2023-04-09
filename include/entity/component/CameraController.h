#pragma once
#include <World.h>
#include <c3d++/C3DRenderer.h>
#include <entity/component/CameraDirection.h>
#include <entity/component/Component.h>
#include <library/linalg.h>
#include <utilities\Time.h>

class CameraController : public Component<CameraController> {
public:
    CameraController();

    virtual void receive(MessageType type) final override;

    virtual ~CameraController();

    CameraDirection getCameraDirection();

private:
    virtual void update();

    CameraDirection switchDirection(CameraDirection direction, bool left);
    CameraDirection direction_ = CameraDirection::NORTH;
    float currentAngle_ = 0.0f;
    float followDistance_ = 30.0f;
    vec<float, 3> followPoint_ { 0, 0, 0 };
    vec<float, 3> currentFollowPoint_;
    bool hasSetCurrentFollowPoint = false;
};