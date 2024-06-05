// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

FTimerHandle SuppressTimeHandle;
FTimerHandle UpdateTimeHandle;

UParkourComponent::UParkourComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UParkourComponent::SuppressWR(float Delay) {
	bIsWRSuppressed = true;
	GetWorld()->GetTimerManager().SetTimer(SuppressTimeHandle, this, &UParkourComponent::ResetWRSuppression, Delay, false);
}

void UParkourComponent::ResetWRSuppression() {
	bIsWRSuppressed = false;
	GetWorld()->GetTimerManager().ClearTimer(SuppressTimeHandle);
}

void UParkourComponent::EndWR(float ResetTime) {
	if (bIsOnWall) {
		bIsOnWall = false;
		bWRRight = false;
		bWRLeft = false;
		Character->GetCharacterMovement()->GravityScale = DefaultGrav;
		SuppressWR(ResetTime);
	}
}

void UParkourComponent::WRJump() {
	if (bIsOnWall) {
		EndWR(0.35f);
		Character->LaunchCharacter(FVector(WRJumpOffForce * WRNormal.X, WRJumpOffForce * WRNormal.Y, WRJumpHeight), false, true);
	}
}

void UParkourComponent::WRUpdate() {
	if (bCanWR || bIsWRSuppressed) {
		FVector LeftEndPoint;
		FVector RightEndPoint;
		GetWREndPoints(RightEndPoint, LeftEndPoint);
		if (WRMove(Character->GetActorLocation(), RightEndPoint, -1.0f)) {
			bIsOnWall = true;
			bWRRight = bIsOnWall;
			bWRLeft = false;
			Character->GetCharacterMovement()->GravityScale = FMath::FInterpTo(Character->GetCharacterMovement()->GravityScale, WRTargetGrav, GetWorld()->GetDeltaSeconds(), 10.0f);
		} else if (bWRRight) {
			EndWR(1.0f);
		} else if (WRMove(Character->GetActorLocation(), LeftEndPoint, 1.0f)) {
			bIsOnWall = true;
			bWRRight = false;
			bWRLeft = bIsOnWall;
			Character->GetCharacterMovement()->GravityScale = FMath::FInterpTo(Character->GetCharacterMovement()->GravityScale, WRTargetGrav, GetWorld()->GetDeltaSeconds(), 10.0f);
		} else {
			EndWR(1.0f);
		};
	}
}

bool UParkourComponent::WRMove(FVector Start, FVector End, float Direction) {
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Character);
	TraceParams.bTraceComplex = false;
	TraceParams.MobilityType = EQueryMobilityType::Any;

	FHitResult Result;
	bool bDidItHit = GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, TraceParams);

	if (Result.IsValidBlockingHit()) {
		if (Character->GetCharacterMovement()->IsFalling() && IsValidWallVector(Result.Normal)) {
			Character->LaunchCharacter(OwnerToWallVector(), false, false);
			Character->LaunchCharacter(WRNormal.Cross(FVector(0.0f, 0.0f, 1.0f)) * (WRSpeed * Direction), true, !bWRGravity);
			return true;
		}
	}

	return false;
}

void UParkourComponent::SetDefaultGrav(float Gravity) {
	DefaultGrav = Gravity;
}

void UParkourComponent::BeginPlay() {
	Super::BeginPlay();
	Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (IsValid(Character)) {
		DefaultGrav = Character->GetCharacterMovement()->GravityScale;
		GetWorld()->GetTimerManager().SetTimer(UpdateTimeHandle, this, &UParkourComponent::WRUpdate, 0.02f, true);
		UE_LOG(LogTemp, Warning, TEXT("Character is valid, begin Update function."));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Character is not valid, halt Parkour component initialization."));
	};
}

void UParkourComponent::GetWREndPoints(FVector& Right, FVector& Left) {
	Right = Character->GetActorLocation() + Character->GetActorRightVector() * 75.0f + Character->GetActorForwardVector() * -35.0f;
	Left = Character->GetActorLocation() + Character->GetActorRightVector() * -75.0f + Character->GetActorForwardVector() * -35.0f;
}

bool UParkourComponent::IsValidWallVector(FVector InVec) {
	if (InVec.Z > -0.52f && InVec.Z < 0.52) {
		WRNormal = InVec;
		return true;
	} else {
		return false;
	}
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
	Character->GetController()->SetControlRotation(FMath::RInterpTo(ControllerRot, FRotator(ControllerRot.Pitch, ControllerRot.Yaw, X), GetWorld()->GetDeltaSeconds(), 10.0f));
}

void UParkourComponent::SetOwner(ACharacter* Owner) {
	Character = Owner;
}

void UParkourComponent::SetJumpHeight(float Height) {
	WRJumpHeight = Height;
}

void UParkourComponent::OnLanding() {
	EndWR(0.0f);
	bIsWRSuppressed = false;
}

void UParkourComponent::OnJumping() {
	WRJump();
	UE_LOG(LogTemp, Warning, TEXT("OnJumping from Parkour executed."));
}

