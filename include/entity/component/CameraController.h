#pragma once
#include <entity/component/Component.h>
#include <c3d++/C3DRenderer.h>
#include <World.h>
#include <utilities\Time.h>
#include <library/linalg.h>

class CameraController :public Component {
public:
	CameraController(); 
	CameraController(bool freeFlight);

	virtual void receive(Entity& e, MessageType type) override;

	virtual std::string getType() override;

	virtual ~CameraController();

	static const std::string typeName;

private:
	virtual void update(Entity& e);
	bool freeFlight_ = false;
};