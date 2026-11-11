// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HostSessionInstanceSubsystem.generated.h"

/**
 * Struct for storing player session data.
 */
USTRUCT(BlueprintType)
struct FPlayerSessionData
{
	GENERATED_BODY()

	/** The player's assigned slot in the session. -1 if not assigned. */
	UPROPERTY(BlueprintReadWrite, Category = "HostSessionInstance|PlayerData")
	int32 PlayerSlot = -1;

};

/**
 * Subsystem for managing the hosts session data. It is intended for persisting
 * session data across level transitions and for managing the host's session
 * state. Should be interfaced through a GameMode to ensure data comes from the
 * host and not a client.
 */
UCLASS()
class PROTORTS_API UHostSessionInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/** Map of player's unique net ids to their session data */
	UPROPERTY(BlueprintReadOnly, Category = "HostSessionInstance")
	TMap<FUniqueNetIdRepl, FPlayerSessionData> PlayerSessionDataMap;



	/** Get the player session data for a given player id */
	UFUNCTION(BlueprintCallable, Category = "HostSessionInstance")
	FPlayerSessionData GetPlayerSessionData(FUniqueNetIdRepl PlayerId) const;
	
	/** Set the player session data for a given player id */
	UFUNCTION(BlueprintCallable, Category = "HostSessionInstance")
	void SetPlayerSessionData(FUniqueNetIdRepl PlayerId, const FPlayerSessionData& PlayerData);

	/** Clear all host session data */
	UFUNCTION(BlueprintCallable, Category = "HostSessionInstance")
	void ClearAllData();


	/**
	 * Assign player to first available player slot up to a provided max if they
	 * do not already have one. Returns the assigned slot or -1 if no slots are available.
	 * 
	 * Player slots are 1-indexed, so the first slot is 1 and the last slot is maxPlayers.
	 * 
	 * @param maxPlayers - The maximum number of players allowed in the session.
	 * @return slot - The assigned player slot, or -1 if no slots are available.
	 */
	UFUNCTION(BlueprintCallable, Category = "HostSessionInstance")
	int32 AssignFirstAvailablePlayerSlot(FUniqueNetIdRepl PlayerId, int32 maxPlayers);

	/** Get the slot of a player, or -1 if they do not have one. */
	UFUNCTION(BlueprintCallable, Category = "HostSessionInstance")
	int32 GetPlayerSlot(FUniqueNetIdRepl PlayerId) const;

};
