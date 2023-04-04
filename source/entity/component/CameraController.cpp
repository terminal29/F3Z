#include <entity/component/CameraController.h>
#include <entity/Entity.h>

CameraController::CameraController() : CameraController(false) {}

CameraController::CameraController(bool freeFlight) : freeFlight_(freeFlight)
{
}

void CameraController::receive(Entity &e, MessageType type)
{
	if (type == MessageType::MSG_UPDATE)
	{
		update(e);
	}
}

void CameraController::update(Entity &e)
{
	if (freeFlight_)
	{
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		circlePosition analogStick;
		hidCircleRead(&analogStick);

		auto currentTransform = C3DTransform{e.getTransform()};
		if ((analogStick.dy > 25 || analogStick.dy < -25))
		{
			currentTransform.setYPR(currentTransform.getYPR() + vec<float, 3>(0, analogStick.dy * Time::dt() / 5000.0, 0));
		}
		if ((analogStick.dx > 25 || analogStick.dx < -25))
		{
			currentTransform.setYPR(currentTransform.getYPR() + vec<float, 3>(-analogStick.dx * Time::dt() / 5000.0, 0, 0));
		}

		if (kHeld & KEY_L)
		{
			currentTransform.setPos(currentTransform.getPos() + currentTransform.getForward() * vec<float, 3>{Time::dt()} * vec<float, 3>{0.3});
		}
		else if (kHeld & KEY_R)
		{
			currentTransform.setPos(currentTransform.getPos() - currentTransform.getForward() * vec<float, 3>{Time::dt()} * vec<float, 3>{0.3});
		}
		e.setTransform(currentTransform);
		C3DRenderer::setCameraTransform(currentTransform);
	}
}

std::string CameraController::getType()
{
	return typeName;
};

CameraController::~CameraController()
{
}

const std::string CameraController::typeName = "CameraController";
