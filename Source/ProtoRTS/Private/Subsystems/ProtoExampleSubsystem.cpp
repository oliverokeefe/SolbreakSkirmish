// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ProtoExampleSubsystem.h"

UProtoExampleSubsystem::UProtoExampleSubsystem()
{
	SecretMsg = TEXT("DEFAULT SECRET");
}

FString UProtoExampleSubsystem::GetSecretMsg()
{
	return SecretMsg;
}

void UProtoExampleSubsystem::SetSecretMsg(const FString& NewMsg)
{
	SecretMsg = NewMsg;
}