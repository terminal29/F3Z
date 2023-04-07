#pragma once
#include <entity/component/Component.h>
#include <entity/component/CameraDirection.h>
#include <c3d++/C3DRenderer.h>
#include <World.h>
#include <utilities\Time.h>
#include <library/linalg.h>

class CameraController : public Component<CameraController>
{
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
	vec<float, 3> followPoint_{0, 0, 0};
	vec<float, 3> currentFollowPoint_;
	bool hasSetCurrentFollowPoint = false;
};