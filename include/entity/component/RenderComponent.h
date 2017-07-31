#pragma once
#include <citro3d.h>
#include "Component.h"
#include "../Entity.h"
#include "RenderManager.h"
#include "Types.h"
#include "Globals.h"


class RenderComponent :public Component{
public:
	RenderComponent();

	// Called whenever a message is broadcasted
	virtual void receive(Entity& e, MessageType type) override;
	
	// hacky RTTI
	virtual std::string getType() override;

	//Nothing stored so nothing to remove
	virtual ~RenderComponent();


	static const std::string typeName;

private:
	virtual void render(Entity& e, RenderTarget target);
};

