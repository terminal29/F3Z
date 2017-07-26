#pragma once
class Component
{
public:
	virtual ~Component() {}
	virtual void receive(int message) = 0;
};

