// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/ConquestSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UConquestSubsystem::TravelToConquestLevel()
{
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TEXT("/Game/Levels/Conquest/Conquest"));
	}
}

void UConquestSubsystem::LaunchConquestMatch(FConquestSettings MatchSettings)
{
	SaveMatchConfigurationSettings(MatchSettings);
	ResetRoundsWon();
	TravelToConquestLevel();
}

void UConquestSubsystem::LaunchConquestNextRound()
{
	//Will eventually handle launching the appropriate phase for the next round (Capture or Siege), but for now just travels to the conquest level for the next round
	TravelToConquestLevel();
}

void UConquestSubsystem::LaunchConquestRematch()
{
	ResetRoundsWon();
	TravelToConquestLevel();
}

void UConquestSubsystem::SaveMatchConfigurationSettings(FConquestSettings MatchSettings)
{
	ScoreToWin = MatchSettings.ScoreToWin;
	CapturePointsToWin = MatchSettings.CapturePointsToWin;
	bRegicideWinConEnabled = MatchSettings.bRegicideWinConEnabled;
	// Apply other config fields when added.
}

int32 UConquestSubsystem::GetRoundsWon(int32 TeamId) const
{
	switch (TeamId)
	{
	case 1:
		return TeamOneRoundsWon;
	case 2:
		return TeamTwoRoundsWon;
	default:
		return 0;
	}
}

void UConquestSubsystem::SetRoundsWon(int32 TeamId, int32 NewScore)
{
	switch (TeamId)
	{
	case 1:
		TeamOneRoundsWon = NewScore;
		break;
	case 2:
		TeamTwoRoundsWon = NewScore;
		break;
	default:
		// Invalid team id - ignore
		break;
	}
}

int32 UConquestSubsystem::IncrementRoundsWon(int32 TeamId)
{
	int32 updatedScore = 0;

	switch (TeamId)
	{
	case 1:
		TeamOneRoundsWon += 1;
		updatedScore = TeamOneRoundsWon;
		break;
	case 2:
		TeamTwoRoundsWon += 1;
		updatedScore = TeamTwoRoundsWon;
		break;
	default:
		// Invalid team id - ignore
		break;
	}

	return updatedScore;
}

void UConquestSubsystem::ResetRoundsWon()
{
	TeamOneRoundsWon = 0;
	TeamTwoRoundsWon = 0;
}

