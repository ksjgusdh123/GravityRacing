#pragma once

class ISystem
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};
