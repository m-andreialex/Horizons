// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonsCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "HealthComponent.h"
#include "ParkourComponent.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AHorizonsCharacter::AHorizonsCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 70.0f));
	Camera->bUsePawnControlRotation = true;

	MainSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainSpring"));
	MainSpring->SetupAttachment(Camera);
	MainSpring->TargetArmLength = 0.0f;
	MainSpring->bEnableCameraRotationLag = true;
	MainSpring->CameraRotationLagSpeed = 12.0f;

	InnerLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("InnerLight"));
	InnerLight->SetupAttachment(MainSpring);
	InnerLight->AttenuationRadius = 2500.0f;
	InnerLight->InnerConeAngle = 16.0f;
	InnerLight->OuterConeAngle = 24.0f;
	InnerLight->SetVisibility(false);

	OuterLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("OuterLight"));
	OuterLight->SetupAttachment(MainSpring);
	OuterLight->Intensity = 2500.0f;
	OuterLight->AttenuationRadius = 2500.0f;
	OuterLight->InnerConeAngle = 32.0f;
	OuterLight->OuterConeAngle = 48.0f;
	OuterLight->SetVisibility(false);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	ParkourComp = CreateDefaultSubobject<UParkourComponent>(TEXT("ParkourComp"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void AHorizonsCharacter::AddXP(int Amount) {
	XP = XP + Amount;
	if (XP >= ReqXP) {
		AddLevel(1);

		XP = XP - ReqXP;
		SetReqXP(ReqXP * 1.5f);
	}
}

void AHorizonsCharacter::AddLevel(int Amount) {
	Level = Level + Amount;
}

// Called when the game starts or when spawned
void AHorizonsCharacter::BeginPlay() {
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DMC, 0);
		}
	}
	ParkourComp->SetOwner(this);

	Level = 1;
	ReqXP = 128;
}

void AHorizonsCharacter::Look(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X * Sensitivity * DeltaSeconds);
		AddControllerPitchInput(LookAxisVector.Y * Sensitivity * DeltaSeconds);
	}
}

void AHorizonsCharacter::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHorizonsCharacter::Run() {
	if (!IsCrouching()) {
		if (!GetCharacterMovement()->IsFalling()) {
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		} else {
			StopRunning();
		}
	} else {
		StopRunning();
	}
}

void AHorizonsCharacter::StopRunning() {
	if (IsRunning()) {
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AHorizonsCharacter::BeginJump() {
	UWorld* World = GetWorld();
	if (IsValid(World)) {
		FVector CamLoc = Camera->GetComponentLocation();
		FVector End = CamLoc + FVector(0.0f, 0.0f, 96.0f);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.bTraceComplex = true;
		TraceParams.MobilityType = EQueryMobilityType::Any;

		FHitResult JumpHitRes;
		bool bDidItHit = World->LineTraceSingleByChannel(JumpHitRes, CamLoc, End, ECC_Visibility, TraceParams);

		if (bDidItHit) {
			GetCharacterMovement()->JumpZVelocity = GetVelocity().Size();
		} else {
			GetCharacterMovement()->JumpZVelocity = 400.0f;
		}
	}

	if (ParkourComp->IsWallrunning()) {
		ParkourComp->OnJumping();
	} else {
		Jump();
	}
}

void AHorizonsCharacter::UseFlashlight() {
	if (bIsUsingFlashlight) {
		SetFlashlightVis(false);
	} else {
		SetFlashlightVis(true);
	}
}

void AHorizonsCharacter::SetFlashlightVis(bool IsVisible) {
	InnerLight->SetVisibility(IsVisible);
	OuterLight->SetVisibility(IsVisible);
	bIsUsingFlashlight = IsVisible;
}

void AHorizonsCharacter::UpdateParallaxUI() {
	PitchRate = UKismetMathLibrary::FClamp((Camera->GetComponentRotation().Pitch - CameraRot.Pitch) + PitchRate, MinMaxPitchRate * -1.0f, MinMaxPitchRate);
	YawRate = UKismetMathLibrary::FClamp((Camera->GetComponentRotation().Yaw - CameraRot.Yaw) + YawRate, MinMaxYawRate * -1.0f, MinMaxYawRate);
	CameraRot = Camera->GetComponentRotation();
	PitchParallaxOffset = UKismetMathLibrary::FInterpTo(PitchParallaxOffset, PitchRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawParallaxOffset = UKismetMathLibrary::FInterpTo(YawParallaxOffset, YawRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);

	if (ParallaxCollection != nullptr) {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ParallaxCollection, "Offset", FLinearColor(YawParallaxOffset, PitchParallaxOffset, 0.0f));
	}

	PitchRate = UKismetMathLibrary::FInterpTo(PitchRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawRate = UKismetMathLibrary::FInterpTo(YawRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
}

// Called every frame
void AHorizonsCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;

	UpdateParallaxUI();
}

// Called to bind functionality to input
void AHorizonsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorizonsCharacter::Look);
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorizonsCharacter::Move);

		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AHorizonsCharacter::BeginJump);
		EIC->BindAction(JumpAction, ETriggerEvent::None, this, &AHorizonsCharacter::StopJumping);

		EIC->BindAction(RunAction, ETriggerEvent::Started, this, &AHorizonsCharacter::Run);
		EIC->BindAction(RunAction, ETriggerEvent::None, this, &AHorizonsCharacter::StopRunning);

		EIC->BindAction(FlashlightAction, ETriggerEvent::Triggered, this, &AHorizonsCharacter::UseFlashlight);
	}
}

