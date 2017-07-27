#include "entity/component/RenderComponent.h"

RenderComponent::RenderComponent()
{
}


RenderComponent::~RenderComponent()
{
}

const std::string RenderComponent::typeName("RenderComponent");

void RenderComponent::receive(Entity& e, Component::MessageType type) {
	if (!enabled_)
		return;

	if (type == MessageType::MSG_RENDER) {
		render(e, RenderTarget::RT_TOP);
	}
}

void RenderComponent::render(Entity& e, RenderTarget target) {
	RenderManager::instance().renderModel(e.getModel(), e.getTransform(), target);
}

std::string RenderComponent::getType() {
	return typeName;
}
