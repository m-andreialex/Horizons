// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORIZONS_API UHealthComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure) float GetHealth() { return Health / 100.0f; }
	UFUNCTION(BlueprintPure) float GetMaxHealth() { return MaxHealth / 100.0f; }
	UFUNCTION(BlueprintCallable) void SetHealth(float InHealth) { Health = InHealth; }
	UFUNCTION(BlueprintCallable) void SetMaxHealth(float InMaxHealth) { MaxHealth = InMaxHealth; }

	UFUNCTION(BlueprintPure) float CalcFallDamage(float Velocity, int VelocityLimit, float FallDamageMultiplier);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(VisibleAnywhere) int MaxHealth;
	UPROPERTY(VisibleAnywhere) int Health;
};
