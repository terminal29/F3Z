#include <entity/component/RenderComponent.h>
#include <entity/Entity.h>

RenderComponent::RenderComponent() : RenderComponent(C3DRenderTarget::TOP)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target) : target_(target)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target, bool shadeless) : target_(target), shadeless_(shadeless)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::receive(MessageType type)
{
	if (!enabled_)
		return;

	if (type == MessageType::MSG_RENDER)
	{
		render();
	}
}

void RenderComponent::render()
{
	if (entity_ == nullptr)
		return;
	C3DRenderer::setTarget(target_);
	if (shadeless_)
		C3DRenderer::drawNextShadeless();
	C3DRenderer::draw(entity_->getModel(), entity_->getTransform());
}
