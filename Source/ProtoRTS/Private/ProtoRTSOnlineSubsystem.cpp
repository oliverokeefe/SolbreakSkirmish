#include "ProtoRTSOnlineSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"

void UProtoRTSOnlineSubsystem::CreateSession(int32 MaxPlayers, bool bIsLAN)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions.IsValid())
	{
		OnCreateSessionComplete.Broadcast(false);
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = MaxPlayers;
	Settings.bIsLANMatch = bIsLAN;
	Settings.bShouldAdvertise = true;
	Settings.bUsesPresence = true;
	Settings.bUseLobbiesIfAvailable = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowJoinViaPresence = true;

	OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UProtoRTSOnlineSubsystem::OnCreateSessionCompleteInternal));

	Sessions->CreateSession(0, FName(TEXT("NAME_GameSession")), Settings);
}

void UProtoRTSOnlineSubsystem::FindSessions(int32 MaxSearchResults, bool bIsLAN)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions.IsValid())
	{
		OnFindSessionsComplete.Broadcast(false, 0);
		return;
	}

	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = MaxSearchResults;
	SessionSearch->bIsLanQuery = bIsLAN;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName(TEXT("LOBBYSEARCH")), true, EOnlineComparisonOp::Equals);

	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UProtoRTSOnlineSubsystem::OnFindSessionsCompleteInternal));

	Sessions->FindSessions(0, SessionSearch.ToSharedRef());
}

void UProtoRTSOnlineSubsystem::JoinSession(int32 SearchResultIndex)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions.IsValid() || !SessionSearch.IsValid() || SessionSearch->SearchResults.Num() == 0)
	{
		OnJoinSessionComplete.Broadcast(false, FString());
		return;
	}

	int32 Index = FMath::Clamp(SearchResultIndex, 0, SessionSearch->SearchResults.Num() - 1);
	const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[Index];

	OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UProtoRTSOnlineSubsystem::OnJoinSessionCompleteInternal));

	Sessions->JoinSession(0, FName(TEXT("NAME_GameSession")), SearchResult);
}

void UProtoRTSOnlineSubsystem::DestroySession()
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions.IsValid())
	{
		OnDestroySessionComplete.Broadcast(false);
		return;
	}

	OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UProtoRTSOnlineSubsystem::OnDestroySessionCompleteInternal));

	Sessions->DestroySession(FName(TEXT("NAME_GameSession")));
}

void UProtoRTSOnlineSubsystem::GetSessionSearchResults(TArray<FSessionResultInfo>& OutResults)
{
	OutResults.Empty();

	if (!SessionSearch.IsValid() || SessionSearch->SearchResults.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
	{
		const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[i];
		FSessionResultInfo Result;

		// Set index (for UI selection)
		Result.Index = i;

		// Get session name
		SearchResult.Session.SessionSettings.Get(FName(TEXT("SESSION_NAME")), Result.SessionName);

		// Get session ID
		Result.SessionId = SearchResult.Session.GetSessionIdStr();

		// Get host name
		if (SearchResult.Session.OwningUserName.Len() > 0)
		{
			Result.HostName = SearchResult.Session.OwningUserName;
		}
		else
		{
			Result.HostName = TEXT("Unknown Host");
		}

		// Get player counts
		Result.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
		Result.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

		// Get ping
		Result.Ping = SearchResult.PingInMs;

		// Get LAN flag
		Result.bIsLAN = SearchResult.Session.SessionSettings.bIsLANMatch;

		OutResults.Add(Result);
	}
}

// Internal handlers
void UProtoRTSOnlineSubsystem::OnCreateSessionCompleteInternal(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UProtoRTSOnlineSubsystem::OnFindSessionsCompleteInternal(bool bWasSuccessful)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	}

	int32 NumResults = SessionSearch.IsValid() ? SessionSearch->SearchResults.Num() : 0;
	OnFindSessionsComplete.Broadcast(bWasSuccessful, NumResults);
}

void UProtoRTSOnlineSubsystem::OnJoinSessionCompleteInternal(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	FString ConnectInfo;
	if (Sessions.IsValid())
	{
		Sessions->GetResolvedConnectString(SessionName, ConnectInfo);
	}

	// Log the resolved connect string for debugging before broadcasting/travel
	UE_LOG(LogTemp, Warning, TEXT("[ProtoRTSOnlineSubsystem] Resolved connect string for session '%s': '%s'"), *SessionName.ToString(), *ConnectInfo);

	bool bWasSuccessful = (Result == EOnJoinSessionCompleteResult::Success);
	OnJoinSessionComplete.Broadcast(bWasSuccessful, ConnectInfo);

	if (bWasSuccessful && !ConnectInfo.IsEmpty())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			// Append the lobby map to the connect string to ensure we join at the correct level
			//FString LobbyMap = TEXT("/Game/Levels/Lobby/Default/Lobby");
			//FString TravelURL = FString::Printf(TEXT("%s?map=%s"), *ConnectInfo, *LobbyMap);

			// Also log that we'll attempt to travel
			UE_LOG(LogTemp, Warning, TEXT("[ProtoRTSOnlineSubsystem] Attempting ClientTravel to '%s'"), *ConnectInfo);
			PC->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UProtoRTSOnlineSubsystem::OnDestroySessionCompleteInternal(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionComplete.Broadcast(bWasSuccessful);
}

IOnlineSessionPtr UProtoRTSOnlineSubsystem::GetSessionInterface() const
{
	//IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	//return Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	return Online::GetSessionInterface(GetWorld());

}
