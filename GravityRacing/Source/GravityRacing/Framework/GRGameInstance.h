// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/MusicSystem.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GRGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class GRAVITYRACING_API UGRGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnStart() override;
	UPROPERTY()
	UMusicSystem* MusicSystem;
};
