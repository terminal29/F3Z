#include <entity/component/RenderComponent.h>
#include <entity/Entity.h>

RenderComponent::RenderComponent():RenderComponent(C3DRenderTarget::TOP)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target):target_(target)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target, bool shadeless) : target_(target), shadeless_(shadeless)
{
}


RenderComponent::~RenderComponent()
{
}

const std::string RenderComponent::typeName("RenderComponent");

void RenderComponent::receive(Entity& e, MessageType type) {
	if (!enabled_)
		return;

	if (type == MessageType::MSG_RENDER) {
		render(e);
	}
}

void RenderComponent::render(Entity& e) {
	C3DRenderer::setTarget(target_);
	if (shadeless_)
		C3DRenderer::drawNextShadeless();
	C3DRenderer::draw(e.getModel(), e.getTransform());
}

std::string RenderComponent::getType() {
	return typeName;
}
