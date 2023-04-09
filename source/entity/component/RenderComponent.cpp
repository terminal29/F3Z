#include <entity/Entity.h>
#include <entity/component/RenderComponent.h>

RenderComponent::RenderComponent(C3DRenderTarget target,
    MessageType renderMessage)
    : target_(target)
    , renderMessage_(renderMessage)
    , shadeless_(false)
    , billboard_(false)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target,
    MessageType renderMessage, bool shadeless)
    : target_(target)
    , renderMessage_(renderMessage)
    , shadeless_(shadeless)
    , billboard_(false)
{
}

RenderComponent::RenderComponent(C3DRenderTarget target,
    MessageType renderMessage, bool shadeless, bool billboard)
    : target_(target)
    , renderMessage_(renderMessage)
    , shadeless_(shadeless)
    , billboard_(billboard)
{
}

RenderComponent::~RenderComponent() { }

void RenderComponent::receive(MessageType type)
{

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
    if (billboard_) {
        auto flatTransform = entity_->getTransform();
        flatTransform.setYPR(C3DRenderer::getCameraTransform().getYPR());
        C3DRenderer::draw(entity_->getModel(), flatTransform);
    } else {
        C3DRenderer::draw(entity_->getModel(), entity_->getTransform());
    }
}
