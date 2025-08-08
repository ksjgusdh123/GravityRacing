#pragma once
#include "CoreMinimal.h"

#if UE_BUILD_SHIPPING
#define GRLOG(...)
#define GRLOG_W(...)
#define GRLOG_E(...)
#else
#define GRLOG(Format, ...) \
    do { if (GEngine) { \
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
      UE_LOG(LogTemp, Log, TEXT(Format), ##__VA_ARGS__); \
    }} while(0)

#define GRLOG_W(Format, ...) \
    do { if (GEngine) { \
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("[Warn]") TEXT(Format), ##__VA_ARGS__)); \
      UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__); \
    }} while(0)

#define GRLOG_E(Format, ...) \
    do { if (GEngine) { \
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[Err]") TEXT(Format), ##__VA_ARGS__)); \
      UE_LOG(LogTemp, Error, TEXT(Format), ##__VA_ARGS__); \
    }} while(0)
#endif
