// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GRLobbyMode.generated.h"

UCLASS()
class GRAVITYRACING_API AGRLobbyMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

};