#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ProtoRTSOnlineSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleteBP, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsCompleteBP, bool, bWasSuccessful, int32, NumResults);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionCompleteBP, bool, bWasSuccessful, const FString&, ConnectInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionCompleteBP, bool, bWasSuccessful);

// Blueprint-exposed struct for session search results
USTRUCT(BlueprintType)
struct FSessionResultInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	FString SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	FString SessionId;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	int32 Index;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	FString HostName;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	int32 Ping = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Online|Session")
	bool bIsLAN = true;
};

UCLASS(BlueprintType)
class UProtoRTSOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Blueprint events
	UPROPERTY(BlueprintAssignable)
	FOnCreateSessionCompleteBP OnCreateSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FOnFindSessionsCompleteBP OnFindSessionsComplete;

	UPROPERTY(BlueprintAssignable)
	FOnJoinSessionCompleteBP OnJoinSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FOnDestroySessionCompleteBP OnDestroySessionComplete;

	// Blueprint-callable functions
	UFUNCTION(BlueprintCallable, Category = "Online|Session")
	void CreateSession(int32 MaxPlayers = 2, bool bIsLAN = true);

	UFUNCTION(BlueprintCallable, Category = "Online|Session")
	void FindSessions(int32 MaxSearchResults = 20, bool bIsLAN = true);

	UFUNCTION(BlueprintCallable, Category = "Online|Session")
	void JoinSession(int32 SearchResultIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Online|Session")
	void DestroySession();

	UFUNCTION(BlueprintCallable, Category = "Online|Session")
	void GetSessionSearchResults(TArray<FSessionResultInfo>& OutResults);

protected:
	// Internal handlers
	void OnCreateSessionCompleteInternal(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsCompleteInternal(bool bWasSuccessful);
	void OnJoinSessionCompleteInternal(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleteInternal(FName SessionName, bool bWasSuccessful);

	// Helpers
	IOnlineSessionPtr GetSessionInterface() const;

	// State
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};
