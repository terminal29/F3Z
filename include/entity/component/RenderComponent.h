#pragma once
#include <entity/component/Component.h>
#include <c3d++/C3DRenderer.h>


class RenderComponent :public Component{
public:
	RenderComponent();
	RenderComponent(C3DRenderTarget target);
	RenderComponent(C3DRenderTarget target, bool shadeless);

	virtual void receive(Entity& e, MessageType type) override;
	
	virtual std::string getType() override;

	virtual ~RenderComponent();

	static const std::string typeName;

private:
	virtual void render(Entity& e);
	C3DRenderTarget target_;
	bool shadeless_ = false;
};

