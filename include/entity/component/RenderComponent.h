#pragma once
#include <c3d++/C3DRenderer.h>
#include <entity/component/Component.h>

class RenderComponent : public Component<RenderComponent> {
public:
    RenderComponent(C3DRenderTarget target, MessageType renderMessage);

    RenderComponent(C3DRenderTarget target, MessageType renderMessage, bool shadeless);

    virtual void receive(MessageType type) final override;

    virtual ~RenderComponent();

private:
    void render();
    C3DRenderTarget target_;
    MessageType renderMessage_;
    bool shadeless_ = false;
};
