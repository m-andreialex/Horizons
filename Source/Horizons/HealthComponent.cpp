// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	Health = MaxHealth;
}

float UHealthComponent::CalcFallDamage(float Velocity, int VelocityLimit, float FallDamageMultiplier) {
	if (Velocity > 0.0f) {
		return FMath::Clamp((Velocity - VelocityLimit) * FallDamageMultiplier, 0, 9999);
	}
	return 0;
}

void UHealthComponent::BeginPlay() {
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
	Health = FMath::Clamp(Health - int(Damage), 0, MaxHealth);
	if (IsValid(GetOwner()) && Health <= 0) { GetOwner()->Destroy(); }
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

