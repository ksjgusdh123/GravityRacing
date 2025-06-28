#include "Network/GRNetworkSubsystem.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"

void UGRNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Warning, TEXT("UGRNetworkSubsystem Initialized!"));
}

void UGRNetworkSubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Warning, TEXT("UGRNetworkSubsystem Deinitialized!"));
}

bool UGRNetworkSubsystem::ConnectToServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("ClientSocket"));
	Socket->SetNonBlocking(false);

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Connecting To Server..."));

	bool bConnected = Socket->Connect(*InternetAddr);
	if (bConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connection Success"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Connection Failed"));
	}

	return bConnected;
}

void UGRNetworkSubsystem::DisconnectFromServer()
{
	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void UGRNetworkSubsystem::ProcessPacket()
{
	TArray<uint8> PacketData;
	while (RecvQueue.Dequeue(PacketData))
	{
		RemainingData.Append(PacketData);
		//todo
	}
}

void UGRNetworkSubsystem::DoSend(uint8* Buf, int32 Size)
{
	int32 BytesSent = 0;
	Socket->Send(Buf, Size, BytesSent);
}

void UGRNetworkSubsystem::DoRecv()
{
	uint32 DataSize;
	if (Socket->HasPendingData(DataSize))
	{
		TArray<uint8> RecvData;
		RecvData.SetNumUninitialized(FMath::Min(DataSize, 65507u));

		int32 BytesRead = 0;
		Socket->Recv(RecvData.GetData(), RecvData.Num(), BytesRead);

		if (BytesRead > 0)
		{
			RecvQueue.Enqueue(RecvData);
		}
	}
}
