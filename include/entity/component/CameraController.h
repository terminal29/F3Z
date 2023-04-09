#pragma once
#include <World.h>
#include <c3d++/C3DRenderer.h>
#include <entity/component/Component.h>
#include <library/linalg.h>
#include <utilities\Time.h>

class CameraController : public Component<CameraController> {
public:
    CameraController();

    void setFollowEntity(std::weak_ptr<Entity> entity);

    virtual void receive(MessageType type) final override;

    virtual ~CameraController();

private:
    virtual void update();

    std::weak_ptr<Entity> followEntity_;
    float currentAngle_ = 0.0f;
    float followDistance_ = 30.0f;
    vec<float, 3> followPoint_ { 0, 0, 0 };
    vec<float, 3> currentFollowPoint_;
    bool hasSetCurrentFollowPoint = false;
};