#pragma once

#include "CoreMinimal.h"

#if !UE_BUILD_SHIPPING


#define GRLOG(Format, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
		UE_LOG(LogTemp, Log, TEXT(Format), ##__VA_ARGS__); \
	}

#define GRWARN(Format, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Warning: ") TEXT(Format), ##__VA_ARGS__)); \
		UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__); \
	}

#define GRERR(Format, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error: ") TEXT(Format), ##__VA_ARGS__)); \
		UE_LOG(LogTemp, Error, TEXT(Format), ##__VA_ARGS__); \
	}

#endif
