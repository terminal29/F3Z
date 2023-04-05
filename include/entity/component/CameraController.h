#pragma once
#include <entity/component/Component.h>
#include <c3d++/C3DRenderer.h>
#include <World.h>
#include <utilities\Time.h>
#include <library/linalg.h>

class CameraController : public Component<CameraController>
{
public:
	CameraController();
	CameraController(bool freeFlight);

	virtual void receive(Entity &e, MessageType type) final override;

	virtual ~CameraController();

private:
	virtual void update(Entity &e);
	bool freeFlight_ = false;
};