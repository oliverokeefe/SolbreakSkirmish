// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoCPPGame/MainMenu/ProtoCPPMainMenuController.h"

#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

AProtoCPPMainMenuController::AProtoCPPMainMenuController()
{
	//bShowMouseCursor = true;
}

void AProtoCPPMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("MMControllerStart"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("MMControllerStart"));
	}
}

void AProtoCPPMainMenuController::HandleFindSessionBtnPress()
{
	UE_LOG(LogTemp, Warning, TEXT("Find Session Button Pressed"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Find Session Button Pressed"));
	}
}

void AProtoCPPMainMenuController::HandleHostSessionBtnPress()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Session Button Pressed"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Host Session Button Pressed"));
	}
}

void AProtoCPPMainMenuController::HandleQuitGameBtnPress()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game Button Pressed"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Quit Game Button Pressed"));
	}
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}