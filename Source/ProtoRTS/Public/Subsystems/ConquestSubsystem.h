// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ConquestSubsystem.generated.h"

/** Lobby configuration for the Conquest match. Expand this as more settings are required. */
USTRUCT(BlueprintType)
struct FConquestSettings
{
	GENERATED_BODY()

	/** Score required to win the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 ScoreToWin = 4;

	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 CapturePointsToWin = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	bool bRegicideWinConEnabled = true;

	// Add future lobby configuration fields here (e.g., allowed maps, friendly fire, etc.)
};

/**
 * Subsystem for managing conquest game mode functionality
 */
UCLASS()
class PROTORTS_API UConquestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	/**
	 * Performs a server travel to the Conquest level
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void TravelToConquestLevel();

	/**
	 * Saves the provided match settings to the subsystem, resets scores, and travels to the conquest level to start a new match.
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void LaunchConquestMatch(FConquestSettings MatchSettings);

	/**
	 * Handles logic for launching the appropriate phase for the next round in a match of Conquest
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void LaunchConquestNextRound();

	/**
	 * Resets scores and travels to the conquest level to start a new match with the same settings (rematch).
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void LaunchConquestRematch();



	// -- Lobby configuration that should be saved in the subsystem --

	/** Score required to win the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 ScoreToWin = 4;

	/** Capture points needed to win a round */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 CapturePointsToWin = 100;

	/** Allow regicide win condition*/
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	bool bRegicideWinConEnabled = true;

	/** Save match configuration settings */
	UFUNCTION(BlueprintCallable, Category = "Conquest|Config")
	void SaveMatchConfigurationSettings(FConquestSettings MatchSettings);



	// -- Team score tracking (two teams) --

	/** Current score for team 1 */
	UPROPERTY(BlueprintReadOnly, Category = "Conquest|Score")
	int32 TeamOneRoundsWon = 0;

	/** Current score for team 2 */
	UPROPERTY(BlueprintReadOnly, Category = "Conquest|Score")
	int32 TeamTwoRoundsWon = 0;

	/** Get the score for the specified team (1 or 2) */
	UFUNCTION(BlueprintPure, Category = "Conquest|Score")
	int32 GetRoundsWon(int32 TeamId) const;

	/** Set the score for the specified team (1 or 2) */
	UFUNCTION(BlueprintCallable, Category = "Conquest|Score")
	void SetRoundsWon(int32 TeamId, int32 NewScore);

	/** Add (or subtract) points from the specified team (1 or 2) and returns their new score */
	UFUNCTION(BlueprintCallable, Category = "Conquest|Score")
	int32 IncrementRoundsWon(int32 TeamId);

	/** Reset both teams' scores to zero */
	UFUNCTION(BlueprintCallable, Category = "Conquest|Score")
	void ResetRoundsWon();
};
