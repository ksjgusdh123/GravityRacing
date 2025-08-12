// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GRGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, const FString&, NewName);

UCLASS()
class GRAVITYRACING_API AGRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnPlayerNameChanged OnPlayerNameChanged;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintPure)
	int32 GetScore() const { return Score; }
	UFUNCTION(BlueprintCallable)
	void SetPlayTime(const FString& TimeText) { PlayTime = TimeText; }
	UFUNCTION(BlueprintPure)
	FString GetPlayTime() const { return PlayTime; }

private:
	int32 Score = 0;
	FString PlayTime;
};