#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "GRUISystem.generated.h"

class UGRLobbyWidget;
class UGRGameWidget;

UCLASS()
class GRAVITYRACING_API UGRUISystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGRUISystem();
	void SetUIMode(bool bShowCursor, UUserWidget* FocusWidget);
	void SetGameMode();

	void OnLobby();
	void OnGame();

private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	TSubclassOf<UUserWidget> GameWidgetClass;

	UPROPERTY()
	UGRLobbyWidget* LobbyWidget;
	UPROPERTY()
	UGRGameWidget* GameWidget;

	void LoadWidgetClasses();
};
