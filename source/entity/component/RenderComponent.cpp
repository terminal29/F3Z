#include <entity/Entity.h>
#include <entity/component/RenderComponent.h>

RenderComponent::RenderComponent(C3DRenderTarget target,
    MessageType renderMessage)
    : target_(target)
    , renderMessage_(renderMessage)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target,
    MessageType renderMessage, bool shadeless)
    : target_(target)
    , shadeless_(shadeless)
    , renderMessage_(renderMessage)
{
}

RenderComponent::~RenderComponent() { }

void RenderComponent::receive(MessageType type)
{
    std::cout << "RC::receive() : " << enabled_ << " : " << MessageTypeNames[type]
              << " : " << MessageTypeNames[renderMessage_] << std::endl;

    if (!enabled_)
        return;

    if (type == renderMessage_) {
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
