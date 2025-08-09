#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "GPUISystem.generated.h"

class UGPLobbyWidget;
class UGPGameWidget;

UCLASS()
class GRAVITYRACING_API UGPUISystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGPUISystem();
	void SetUIMode(bool bShowCursor, UUserWidget* FocusWidget);
	void SetGameMode();

	void OnLobby();
	void OnGame();

private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	TSubclassOf<UUserWidget> GameWidgetClass;

	UPROPERTY()
	UGPLobbyWidget* LobbyWidget;
	UPROPERTY()
	UGPGameWidget* GameWidget;

	void LoadWidgetClasses();
};
