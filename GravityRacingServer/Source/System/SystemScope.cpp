#include "PCH.h"
#include "SystemScope.h"
#include "Logger/LoggerSystem.h"

FSystemScope::FSystemScope()
{
	Manager.RegisterSystem<FLoggerSystem>();
	Manager.InitAll();
}

FSystemScope::~FSystemScope()
{
	Manager.ShutdownAll();
}