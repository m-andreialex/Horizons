// Fill out your copyright notice in the Description page of Project Settings.

#include "HorizonsTODManager.h"
#include "Engine/DirectionalLight.h"

AHorizonsTODManager::AHorizonsTODManager() {
	PrimaryActorTick.bCanEverTick = true;

}

void AHorizonsTODManager::BeginPlay() {
	Super::BeginPlay();
	
}

void AHorizonsTODManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (IsValid(Sun)) {
		Sun->AddActorWorldRotation(FRotator((DeltaTime * FMath::FloatSelect(SunSpeed, (SunSpeed * NightLength), InRange(Sun->GetActorRotation().Pitch, -90.0f, 0.0f))), 0.0f, 0.0f));
	} else {
		UE_LOG(LogTemp, Error, TEXT("A Sun actor has not been set. Please set a Sun actor for the TOD manager to function."));
	}
	
}

