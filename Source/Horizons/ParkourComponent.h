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

	UFUNCTION(BlueprintCallable) void EndWR(float ResetTime);
	UFUNCTION(BlueprintCallable) void WRJump();

	UFUNCTION(BlueprintPure) bool IsWallrunning() { return bIsOnWall; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere) ACharacter* Character;

public:
	UFUNCTION(BlueprintPure) void GetWREndPoints(FVector& Right, FVector& Left);
	UFUNCTION(BlueprintPure) bool IsValidWallVector(FVector InVec, FVector& OutVec);
	UFUNCTION(BlueprintPure) FVector OwnerToWallVector();

private:
	UPROPERTY() bool bWRGravity;
	UPROPERTY() bool bIsOnWall;
	UPROPERTY() bool bWRLeft;
	UPROPERTY() bool bWRRight;
	UPROPERTY() bool bCanWR;
	UPROPERTY() FVector WRNormal;
	UPROPERTY() float WRDirection;
	UPROPERTY() float WRSpeed;
	UPROPERTY() float WRTargetGrav;
	UPROPERTY() float DefaultGrav;
	UPROPERTY() float WRJumpHeight;
	UPROPERTY() float WRJumpOffForce;
};
