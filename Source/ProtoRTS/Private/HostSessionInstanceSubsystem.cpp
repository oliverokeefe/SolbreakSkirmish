// Fill out your copyright notice in the Description page of Project Settings.


#include "HostSessionInstanceSubsystem.h"


FPlayerSessionData UHostSessionInstanceSubsystem::GetPlayerSessionData(FUniqueNetIdRepl PlayerId) const
{
	if (const FPlayerSessionData* PlayerData = PlayerSessionDataMap.Find(PlayerId))
	{
		return *PlayerData;
	}
	
	// Return default struct if player not found
	return FPlayerSessionData();
}

void UHostSessionInstanceSubsystem::SetPlayerSessionData(FUniqueNetIdRepl PlayerId, const FPlayerSessionData& PlayerData)
{
	PlayerSessionDataMap.Add(PlayerId, PlayerData);
}

void UHostSessionInstanceSubsystem::ClearAllData()
{
	PlayerSessionDataMap.Empty();
}

int32 UHostSessionInstanceSubsystem::AssignFirstAvailablePlayerSlot(FUniqueNetIdRepl PlayerId, int32 maxPlayers)
{
	// Check if PlayerId is valid
	if (!PlayerId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AssignPlayerSlot] WARN: Called with invalid PlayerId."));
		return -1;
	}

	// Validate maxPlayers: ensure at least 1
	if (maxPlayers < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("[AssignPlayerSlot] Error: Called with maxPlayers < 1 (%d). Using 1 as fallback."), maxPlayers);
		maxPlayers = 1;
	}

	// Check if player already has a slot assigned
	if (const FPlayerSessionData* ExistingData = PlayerSessionDataMap.Find(PlayerId))
	{
		UE_LOG(LogTemp, Log, TEXT("[AssignPlayerSlot] INFO: Player already has a slot assigned: %d"), ExistingData->PlayerSlot);
		return ExistingData->PlayerSlot;
	}
	
	// Find first available slot (starting from 1 to maxPlayers inclusive)
	for (int32 Slot = 1; Slot <= maxPlayers; ++Slot)
	{
		bool bSlotAvailable = true;
		
		// Check if any other player is using this slot
		for (const auto& PlayerDataPair : PlayerSessionDataMap)
		{
			if (PlayerDataPair.Value.PlayerSlot == Slot)
			{
				bSlotAvailable = false;
				break;
			}
		}
		
		// If slot is available, assign it to the player
		if (bSlotAvailable)
		{
			FPlayerSessionData NewPlayerData;
			NewPlayerData.PlayerSlot = Slot;
			PlayerSessionDataMap.Add(PlayerId, NewPlayerData);
			return Slot;
		}
	}
	
	// No available slots
	return -1;
}

int32 UHostSessionInstanceSubsystem::GetPlayerSlot(FUniqueNetIdRepl PlayerId) const
{
	if (const FPlayerSessionData* PlayerData = PlayerSessionDataMap.Find(PlayerId))
	{
		return PlayerData->PlayerSlot;
	}
	
	return -1;
}



