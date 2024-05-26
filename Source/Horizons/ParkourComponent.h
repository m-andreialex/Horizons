// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORIZONS_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UParkourComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void CameraTick();
	UFUNCTION(BlueprintCallable) void CameraTilt (float X);
	UFUNCTION(BlueprintCallable) void SetOwner(ACharacter* Owner);
	UFUNCTION(BlueprintCallable) void SetJumpHeight(float Height);

	UFUNCTION(BlueprintCallable) void OnLanding();
	UFUNCTION(BlueprintCallable) void OnJumping();

	UFUNCTION(BlueprintCallable, Category = "Abilities|Wallrun") void SuppressWR(float Delay);
	UFUNCTION() void ResetWRSuppression();

	UFUNCTION(BlueprintCallable) void EndWR(float ResetTime);
	UFUNCTION(BlueprintCallable) void WRJump();
	UFUNCTION(BlueprintCallable) void WRUpdate();
	UFUNCTION(BlueprintCallable) bool WRMove(FVector Start, FVector End, float Direction);

	UFUNCTION(BlueprintCallable) void SetDefaultGrav(float Gravity);

	UFUNCTION(BlueprintPure) bool IsWallrunning() { return bIsOnWall; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite) ACharacter* Character;

public:
	UFUNCTION(BlueprintPure) void GetWREndPoints(FVector& Right, FVector& Left);
	UFUNCTION(BlueprintPure) bool IsValidWallVector(FVector InVec, FVector& OutVec);
	UFUNCTION(BlueprintPure) FVector OwnerToWallVector();

private:
	UPROPERTY(EditAnywhere) bool bWRGravity = true;
	UPROPERTY() bool bIsOnWall = false;
	UPROPERTY() bool bWRLeft = false;
	UPROPERTY() bool bWRRight = false;
	UPROPERTY() bool bCanWR;
	UPROPERTY() bool bIsWRSuppressed;
	UPROPERTY() FVector WRNormal;
	UPROPERTY() float WRDirection;
	UPROPERTY(EditAnywhere) float WRSpeed = 850.0f;
	UPROPERTY(EditAnywhere) float WRTargetGrav = 0.25f;
	UPROPERTY() float DefaultGrav;
	UPROPERTY() float WRJumpHeight;
	UPROPERTY(EditAnywhere) float WRJumpOffForce = 300.0f;
};
