#pragma once
#include <entity/component/Component.h>
#include <c3d++/C3DRenderer.h>

class RenderComponent : public Component<RenderComponent>
{
public:
	RenderComponent();
	RenderComponent(C3DRenderTarget target);
	RenderComponent(C3DRenderTarget target, bool shadeless);

	virtual void receive(MessageType type) override;

	virtual ~RenderComponent();

private:
	virtual void render();
	C3DRenderTarget target_;
	bool shadeless_ = false;
};
