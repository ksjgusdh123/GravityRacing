#pragma once
#include "SystemManager.h"

class FSystemScope
{
public:
	FSystemScope();
	~FSystemScope();

private:
	FSystemManager Manager;
};