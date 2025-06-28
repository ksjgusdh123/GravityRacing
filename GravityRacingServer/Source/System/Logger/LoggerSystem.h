#pragma once
#include "ISystem.h"

class FLoggerSystem : public ISystem
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;
};
