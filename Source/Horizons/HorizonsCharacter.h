// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "HorizonsCharacter.generated.h"

class UCameraComponent;
class USpotLightComponent;
class USpringArmComponent;

class UPhysicsHandleComponent;
class UHealthComponent;
class UParkourComponent;

class UInputAction;
class UInputMappingContext;

struct FInputActionValue;

UCLASS()
class HORIZONS_API AHorizonsCharacter : public ACharacter, public IGenericTeamAgentInterface {
	GENERATED_BODY()

public:
	AHorizonsCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure) bool IsCrouching() { return bIsCrouching; }
	UFUNCTION(BlueprintPure) bool IsRunning() {
		if (!GetCharacterMovement()->IsFalling() && FMath::IsNearlyEqual(GetVelocity().Size(), SprintSpeed, 0.1f)) {
			return true;
		} else {
			return false;
		}
	};

	UFUNCTION(BlueprintPure) int GetXP() { return XP; }
	UFUNCTION(BlueprintPure) int GetReqXP() { return ReqXP; }
	UFUNCTION(BlueprintPure) int GetPlayerLevel() { return Level; }
	UFUNCTION(BlueprintPure) float GetFearLevel() { return FearLevel; }
	UFUNCTION(BlueprintPure) int GetCurrency() { return Currency; }

	UFUNCTION(BlueprintCallable) void SetIsCrouching(bool Crouch) { bIsCrouching = Crouch; };
	UFUNCTION(BlueprintCallable) void AddXP(int Amount);
	UFUNCTION() void CheckXP();
	UFUNCTION() void SetReqXP(int NewReqXP) { ReqXP = NewReqXP; }
	UFUNCTION(BlueprintCallable) void AddLevel(int Amount);

protected:
	virtual void BeginPlay() override;
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	void Run();
	UFUNCTION(BlueprintCallable) void StopRunning();
	void BeginJump();

	void UseFlashlight();
	void SetFlashlightVis(bool IsVisible);

private:
	UFUNCTION() void UpdateParallaxUI();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parallax", meta = (AllowPrivateAccess = "true")) UMaterialParameterCollection* ParallaxCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpringArmComponent* MainSpring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpotLightComponent* OuterLight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpotLightComponent* InnerLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UHealthComponent* Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UParkourComponent* ParkourComp;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputMappingContext* DMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true")) UInputAction* FlashlightAction;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true")) float Sensitivity = 45.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true")) float WalkSpeed = 360.0f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true")) float SprintSpeed = 610.0f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true")) float CrouchSpeed = 140.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true")) float GrabDistance = 256.0f;

	UPROPERTY() bool bIsCrouching;
	UPROPERTY() bool bIsUsingFlashlight;

	UPROPERTY() float DeltaSeconds;

	UPROPERTY() float YawRate;
	UPROPERTY() float PitchRate;
	UPROPERTY() float YawParallaxOffset;
	UPROPERTY() float PitchParallaxOffset;
	UPROPERTY() float ParallaxSpeed = 12.0f;
	UPROPERTY() FRotator CameraRot;
	UPROPERTY() float MinMaxYawRate = 2.0f;
	UPROPERTY() float MinMaxPitchRate = 12.0f;

	UPROPERTY() int JumpCount;

	UPROPERTY() float FearLevel = 0.0f;
	UPROPERTY() int Level = 0;
	UPROPERTY() int XP = 0;
	UPROPERTY() int ReqXP = 0;
	UPROPERTY() int Currency = 0;
};
