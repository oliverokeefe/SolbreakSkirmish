// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoCPPGame/Game/ProtoCPPGameController.h"

void AProtoCPPGameController::BoxSelect(const FVector& StartPoint, const FVector& EndPoint)
{
	// Implement box selection logic here
	UE_LOG(LogTemp, Log, TEXT("BoxSelect called with StartPoint: %s, EndPoint: %s"), *StartPoint.ToString(), *EndPoint.ToString());
	
	// Example logic: Find all actors within a frustum defined


}

void AProtoCPPGameController::FindActorsInSelectionBox(const FVector& StartPoint, const FVector& EndPoint, TArray<AActor*>& OutActors) const
{
	// Implement logic to find actors within the selection box defined by StartPoint and EndPoint
	UE_LOG(LogTemp, Log, TEXT("FindActorsInSelectionBox called with StartPoint: %s, EndPoint: %s"), *StartPoint.ToString(), *EndPoint.ToString());
	
	// Example logic: Use a box overlap query to find actors within the defined box
	FMatrix FrustumMatrix = FPerspectiveMatrix(
		90.0f, // FOV
		1.0f, // Aspect Ratio
		0.1f, // Near Plane
		FVector::Dist(StartPoint, EndPoint) // Far Plane
	);

	// above won't work, I think the FPerpectiveMatrix is for a camera frustum, not a selection frustum

}