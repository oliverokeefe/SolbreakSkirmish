// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoCPPMainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class PROTORTS_API AProtoCPPMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	AProtoCPPMainMenuController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "UIEvents")
	void HandleFindSessionBtnPress();

	UFUNCTION(BlueprintCallable, Category = "UIEvents")
	void HandleHostSessionBtnPress();

	UFUNCTION(BlueprintCallable, Category = "UIEvents")
	void HandleQuitGameBtnPress();
};
