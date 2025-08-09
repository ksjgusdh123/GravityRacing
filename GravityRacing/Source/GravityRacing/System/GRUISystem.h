#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "GRUISystem.generated.h"

class UGRLobbyWidget;
class UGRGameWidget;
class UGRPauseWidget;
class UGRResultWidget;

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
	void OnPause();
	void OnResult();

private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	TSubclassOf<UUserWidget> GameWidgetClass;
	TSubclassOf<UUserWidget> PauseWidgetClass;
	TSubclassOf<UUserWidget> ResultWidgetClass;

	UPROPERTY()
	UGRLobbyWidget* LobbyWidget;
	UPROPERTY()
	UGRGameWidget* GameWidget;
	UPROPERTY()
	UGRPauseWidget* PauseWidget;
	UPROPERTY()
	UGRResultWidget* ResultWidget;

	void LoadWidgetClasses();
};
