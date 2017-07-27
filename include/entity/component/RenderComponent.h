#pragma once
#include <citro3d.h>
#include "Component.h"
#include "../Entity.h"
#include "RenderManager.h"

class RenderComponent :public Component{
public:
	RenderComponent();

	// Called whenever a message is broadcasted
	virtual void receive(Entity& e, MessageType type);
	
	// hacky RTTI
	virtual std::string getType() override;

	//Nothing stored so nothing to remove
	virtual ~RenderComponent();

private:
	virtual void render(Entity& e, RenderTarget target);
};

