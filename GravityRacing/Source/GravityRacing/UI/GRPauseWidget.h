#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GRPauseWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class GRAVITYRACING_API UGRPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnReplayClicked();

	UFUNCTION()
	void OnExitClicked();
};