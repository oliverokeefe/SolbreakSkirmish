// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ConquestSubsystem.generated.h"

/** Lobby configuration for the Conquest match. Expand this as more settings are required. */
USTRUCT(BlueprintType)
struct FConquestSettings : public FTableRowBase
{
	GENERATED_BODY()

	/** Score required to win the match */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conquest|Config")
	int32 ScoreToWin = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conquest|Config")
	bool bAlternateObjectivesEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conquest|Config")
	bool bCaptureZonesEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conquest|Config")
	int32 CapturePointsToWin = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conquest|Config")
	bool bRegicideWinConEnabled = true;
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
	 * 
	 * NOTE: Note _really_ meant to be called directly, call through LaunchConquestMatch() instead, which will save the match settings to the subsystem before traveling to the level.
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void TravelToConquestLevel(TSoftObjectPtr<UWorld> ConquestLevel);

	/**
	 * Saves the provided match settings to the subsystem, resets scores, and travels to the conquest level to start a new match.
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void LaunchConquestMatch(FName TargetMapRowName, TSoftObjectPtr<UWorld> TargetMap, FConquestSettings MatchSettings);

	/**
	 * Handles logic for launching the appropriate phase for the next round in a match of Conquest.
	 * 
	 * Deprecated: Conquest Maps are now meant to be contained in a single level. Rounds and match resets included.
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	UE_DEPRECATED(5.6, "Conquest Maps are now meant to be contained in a single level. Rounds and match resets included.")
	void LaunchConquestNextRound();

	/**
	 * Resets scores and travels to the conquest level to start a new match with the same settings (rematch).
	 * 
	 * Deprecated: Conquest Maps are now meant to be contained in a single level. Rounds and match resets included.
	 */
	UFUNCTION(BlueprintCallable, Category = "Conquest")
	UE_DEPRECATED(5.6, "Conquest Maps are now meant to be contained in a single level. Rounds and match resets included.")
	void LaunchConquestRematch();



	/** Name data table row for the map to be used for the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	FName MapRowName = FName(TEXT("ConquestSolStation"));

	/** The map to be used for the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	TSoftObjectPtr<UWorld> Map;


	// -- Lobby configuration that should be saved in the subsystem --

	/** Score required to win the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 ScoreToWin = 6;

	/** Allow alternate objectives to be enabled */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	bool bAlternateObjectivesEnabled = true;

	/** Whether capture zones are enabled in the match */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	bool bCaptureZonesEnabled = true;

	/** Capture points needed to win a round */
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	int32 CapturePointsToWin = 100;

	/** Allow regicide win condition*/
	UPROPERTY(BlueprintReadWrite, Category = "Conquest|Config")
	bool bRegicideWinConEnabled = true;


	/** Save match configuration settings */
	UFUNCTION(BlueprintCallable, Category = "Conquest|Config")
	void SaveMatchConfigurationSettings(FName TargetMapRowName, TSoftObjectPtr<UWorld> TargetMap, FConquestSettings MatchSettings);



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
