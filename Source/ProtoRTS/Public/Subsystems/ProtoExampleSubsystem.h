// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProtoExampleSubsystem.generated.h"

/**
 * Example subsystem class. Subsystems are a new feature in Unreal Engine that allow you
 * to create globally accessible objects that can be used to manage game state, handle
 * events, and more. This is just an example of how to create a subsystem, and you can
 * customize it to fit your needs.
 */
UCLASS()
class PROTORTS_API UProtoExampleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


private:
	FString SecretMsg;


public:
	UProtoExampleSubsystem();

	UFUNCTION(BlueprintCallable, Category = "ExampleSubsystem")
	FString GetSecretMsg();

	UFUNCTION(BlueprintCallable, Category = "ExampleSubsystem")
	void SetSecretMsg(const FString& NewMsg);
};
