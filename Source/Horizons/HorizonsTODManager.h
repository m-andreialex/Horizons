// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorizonsTODManager.generated.h"

class ADirectionalLight;

UCLASS()
class HORIZONS_API AHorizonsTODManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHorizonsTODManager();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) ADirectionalLight* Sun;
	bool InRange(float Value, float Min, float Max) {
		if (Value >= Min && Value <= Max) {
			return true;
		}
		return false;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere) float SunSpeed = 20.0f;
	UPROPERTY(EditAnywhere) float NightLength = 0.0f;
};
