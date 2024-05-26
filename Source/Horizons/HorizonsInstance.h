// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HardwareInfo.h"
#include "Engine/GameInstance.h"

#if PLATFORM_WINDOWS
#include "Runtime/ApplicationCore/Public/Windows/WindowsPlatformApplicationMisc.h"
#endif

#include "HorizonsInstance.generated.h"

/**
 * 
 */
UCLASS()
class HORIZONS_API UHorizonsInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Game")
	static FString GetGameVersion() {
		FString GameVersion;
		GConfig->GetString(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("ProjectVersion"),
			GameVersion,
			GGameIni
		);
		return GameVersion;
	};

	UFUNCTION(BlueprintPure, Category = "System") static FString GetCPUBrand() { return FPlatformMisc::GetCPUBrand(); };
	UFUNCTION(BlueprintPure, Category = "System") static int32 GetCPUCores() { return FPlatformMisc::NumberOfCores(); };
	UFUNCTION(BlueprintPure, Category = "System") static int32 GetCPUThreads() {
		if (PLATFORM_WINDOWS) {
			return FWindowsPlatformMisc::NumberOfCoresIncludingHyperthreads();
		}
	};
	UFUNCTION(BlueprintPure, Category = "System") static FString GetGPUBrand() { return FPlatformMisc::GetPrimaryGPUBrand(); };
	UFUNCTION(BlueprintPure, Category = "System") static FString GetRHI() { return FHardwareInfo::GetHardwareInfo(NAME_RHI); };
	UFUNCTION(BlueprintPure, Category = "System") static int32 GetRAMCapacity() { return FGenericPlatformMemory::GetPhysicalGBRam(); };
	UFUNCTION(BlueprintPure, Category = "System") static FString GetOSVersion() { return FPlatformMisc::GetOSVersion(); };
};
