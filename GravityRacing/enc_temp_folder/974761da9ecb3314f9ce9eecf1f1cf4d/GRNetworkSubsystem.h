#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRNetworkSubsystem.generated.h"

UCLASS()
class GRAVITYRACING_API UGRNetworkSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection);
	void Deinitialize();
private:
	bool ConnectToServer();
	void DisconnectFromServer();
	void ProcessPacket();

	void DoSend(uint8* Buf, int32 Size);
	void DoRecv();

private:
	class FSocket* Socket;
	FString IpAddress = "127.0.0.1";
	int16 Port = 7777;

	TArray<uint8> RemainingData;
	TQueue<TArray<uint8>, EQueueMode::Mpsc> RecvQueue;
};
