#include <entity/component/RenderComponent.h>
#include <entity/Entity.h>

RenderComponent::RenderComponent():RenderComponent(C3DRenderTarget::TOP)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target):target_(target)
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
		render(e);
	}
}

void RenderComponent::render(Entity& e) {
	C3DRenderer::setTarget(target_);
	C3DModel m = e.getModel();
	C3DTransform t = e.getTransform();
	C3DRenderer::draw(m, t);
}

std::string RenderComponent::getType() {
	return typeName;
}
