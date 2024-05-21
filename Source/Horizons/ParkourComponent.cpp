// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourComponent.h"
#include "GameFramework/Character.h"

UParkourComponent::UParkourComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UParkourComponent::EndWR(float ResetTime) {
	if (bIsOnWall) {
		bIsOnWall = false;
		bWRRight = false;
		bWRLeft = false;
		bCanWR = true;
	}
}

void UParkourComponent::WRJump() {
	EndWR(0.35f);
	Character->LaunchCharacter(FVector(WRJumpOffForce * WRNormal.X, WRJumpOffForce * WRNormal.Y, WRJumpHeight), false, true);
}

void UParkourComponent::BeginPlay() {
	Super::BeginPlay();
}

void UParkourComponent::GetWREndPoints(FVector& Right, FVector& Left) {
	Right = Character->GetActorLocation() + Character->GetActorRightVector() * 75.0f + Character->GetActorForwardVector() * -35.0f;
	Left = Character->GetActorLocation() + Character->GetActorRightVector() * -75.0f + Character->GetActorForwardVector() * -35.0f;
}

bool UParkourComponent::IsValidWallVector(FVector InVec, FVector& OutVec) {
	if (InVec.Z > -0.52f && InVec.Z < 0.52) {
		return true;
	} else {
		return false;
	}
	OutVec = InVec;
}

FVector UParkourComponent::OwnerToWallVector() {
	return FVector(Character->GetActorLocation() - WRNormal).GetSafeNormal() * WRNormal;
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CameraTick();
}

void UParkourComponent::CameraTick() {
	if (bWRLeft) {
		CameraTilt(15.0f);
	} else if (bWRRight) {
		CameraTilt(-15.0f);
	} else {
		CameraTilt(0.0f);
	}
}

void UParkourComponent::CameraTilt(float X) {
	FRotator ControllerRot = Character->GetController()->GetControlRotation();
	Character->GetController()->SetControlRotation(FMath::RInterpTo(ControllerRot, FRotator(X, ControllerRot.Pitch, ControllerRot.Yaw), GetWorld()->GetDeltaSeconds(), 10.0f));
}

void UParkourComponent::SetOwner(ACharacter* Owner) {
	Character = Owner;
}

void UParkourComponent::SetJumpHeight(float Height) {
	WRJumpHeight = Height;
}

