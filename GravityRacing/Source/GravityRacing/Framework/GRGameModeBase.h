// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GRGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class GRAVITYRACING_API AGRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnScoreChanged OnScoreChanged;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintPure)
	int32 GetScore() const { return Score; }

private:
	int32 Score = 0;
};
