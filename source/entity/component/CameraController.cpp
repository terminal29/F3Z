#include <entity/component/CameraController.h>
#include <entity/Entity.h>
#include <numbers>
#include <Loader.h>

CameraController::CameraController()
{
}

void CameraController::receive(MessageType type)
{
	if (type == MessageType::MSG_UPDATE)
	{
		update();
	}
}

float getAngleForDirection(CameraDirection direction)
{
	switch (direction)
	{
	case CameraDirection::NORTH:
		return 0.0f;
	case CameraDirection::EAST:
		return 90.0f;
	case CameraDirection::SOUTH:
		return 180.0f;
	case CameraDirection::WEST:
		return 270.0f;
	}
	return 0.0f;
}

float angleLerp(float start, float end, float amount)
{
	float difference = std::abs(end - start);
	if (difference > 180)
	{
		// We need to add on to one of the values.
		if (end > start)
		{
			// We'll add it on to start...
			start += 360;
		}
		else
		{
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

constexpr float deg2rad(float deg)
{
	return deg * (std::numbers::pi / 180);
}

constexpr float rad2deg(float rad)
{
	return rad * (180 / std::numbers::pi);
}

void CameraController::update()
{
	if (entity_ == nullptr)
		return;

	const float moveSpeed = 0.5f;
	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();

	circlePosition analogStick;
	hidCircleRead(&analogStick);

	float targetAngle = getAngleForDirection(direction_);

	if ((analogStick.dy > 25 || analogStick.dy < -25))
	{
		std::cout << "Y" << std::endl;
		followPoint_.y += (float)analogStick.dy * Time::dt() * moveSpeed;
	}
	if ((analogStick.dx > 25 || analogStick.dx < -25))
	{
		std::cout << "X" << std::endl;
		followPoint_.x += std::cos(deg2rad(targetAngle)) * (float)analogStick.dx * Time::dt() * moveSpeed;
		followPoint_.z += std::sin(deg2rad(targetAngle)) * (float)analogStick.dx * Time::dt() * moveSpeed;
	}

	if (!hasSetCurrentFollowPoint)
	{
		hasSetCurrentFollowPoint = true;
		currentFollowPoint_ = followPoint_;
	}

	if (kDown & KEY_L)
	{
		std::cout << "L" << std::endl;
		direction_ = switchDirection(direction_, true);
	}
	else if (kDown & KEY_R)
	{
		std::cout << "R" << std::endl;
		direction_ = switchDirection(direction_, false);
	}
	const float moveLerpSpeed = 10.0f;
	const float rotateLerpSpeed = 10.0f;

	std::cout << "Target angle: " << targetAngle << std::endl;
	currentAngle_ = angleLerp(currentAngle_, targetAngle, Time::dt() * rotateLerpSpeed);

	// Calculate camera position
	currentFollowPoint_ = vec<float, 3>{std::lerp(currentFollowPoint_.x, followPoint_.x, Time::dt() * moveLerpSpeed), std::lerp(currentFollowPoint_.y, followPoint_.y, Time::dt() * moveLerpSpeed), std::lerp(currentFollowPoint_.z, followPoint_.z, Time::dt() * moveLerpSpeed)};
	vec<float, 3> cameraPosition = currentFollowPoint_;
	cameraPosition.x += followDistance_ * -std::sin(deg2rad(currentAngle_));
	cameraPosition.z += followDistance_ * std::cos(deg2rad(currentAngle_));

	vec<float, 3> cameraRotation{deg2rad(360 - currentAngle_), 0.0f, 0.0f};

	std::cout << "Camera position: " << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << std::endl;
	std::cout << "Follow position: " << followPoint_.x << ", " << followPoint_.y << ", " << followPoint_.z << std::endl;
	std::cout << "Camera rotation: " << cameraRotation.x << ", " << cameraRotation.y << ", " << cameraRotation.z << std::endl;

	// Set camera rotation
	auto cameraTransform = entity_->getTransform();
	cameraTransform.setYPR(cameraRotation);
	cameraTransform.setPos(cameraPosition);
	entity_->setTransform(cameraTransform);
	C3DRenderer::setCameraTransform(cameraTransform);
}

CameraController::~CameraController()
{
}

CameraDirection CameraController::switchDirection(CameraDirection direction, bool left)
{
	if (left)
	{
		switch (direction)
		{
		case CameraDirection::NORTH:
			return CameraDirection::WEST;
		case CameraDirection::SOUTH:
			return CameraDirection::EAST;
		case CameraDirection::EAST:
			return CameraDirection::NORTH;
		case CameraDirection::WEST:
			return CameraDirection::SOUTH;
		default:
			return CameraDirection::NORTH;
		}
	}
	else
	{
		switch (direction)
		{
		case CameraDirection::NORTH:
			return CameraDirection::EAST;
		case CameraDirection::SOUTH:
			return CameraDirection::WEST;
		case CameraDirection::EAST:
			return CameraDirection::SOUTH;
		case CameraDirection::WEST:
			return CameraDirection::NORTH;
		default:
			return CameraDirection::NORTH;
		}
	}
}