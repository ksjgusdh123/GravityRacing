// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GRGameModeBase.h"

void AGRGameModeBase::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}
