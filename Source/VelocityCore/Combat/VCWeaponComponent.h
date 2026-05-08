#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/HitResult.h"
#include "Game/VCScoreComponent.h"
#include "VCWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVCOnWeaponFired, bool, bHit, const FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVCOnWeaponHit, const FHitResult&, HitResult, bool, bKilledTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVCOnWeaponMissed, FVector, TraceEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVCOnWeaponCooldownBlocked, float, RemainingCooldown, float, CooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVCOnWeaponKillScored, EVCStyleBonus, StyleBonus);

// Minimal hitscan weapon entry point with debug visualization and Blueprint feedback hooks.
UCLASS(ClassGroup=(VelocityCore), meta=(BlueprintSpawnableComponent))
class VELOCITYCORE_API UVCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVCWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(ClampMin="0.1"))
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(ClampMin="0.0"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(ClampMin="1.0"))
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug")
	bool bDrawDebugTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(ClampMin="0.0", EditCondition="bDrawDebugTrace"))
	float DebugTraceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(ClampMin="0.0", EditCondition="bDrawDebugTrace"))
	float DebugImpactPointRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(EditCondition="bDrawDebugTrace"))
	FLinearColor DebugHitColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(EditCondition="bDrawDebugTrace"))
	FLinearColor DebugMissColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Scoring")
	bool bAwardScoreOnKill;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FVCOnWeaponFired OnWeaponFired;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FVCOnWeaponHit OnWeaponHit;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FVCOnWeaponMissed OnWeaponMissed;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FVCOnWeaponCooldownBlocked OnWeaponCooldownBlocked;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FVCOnWeaponKillScored OnWeaponKillScored;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool TryFire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool PerformLineTraceShot(FHitResult& OutHit);

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintPure, Category="Weapon")
	float GetFireCooldownDuration() const;

	UFUNCTION(BlueprintPure, Category="Weapon")
	float GetFireCooldownRemaining() const;

private:
	bool PerformLineTraceShotInternal(FHitResult& OutHit, FVector& OutTraceStart, FVector& OutTraceEnd, bool& bOutKilledTarget);
	void DrawDebugShot(const FVector& TraceStart, const FVector& TraceEnd, const FHitResult& HitResult, bool bHit) const;
	void AwardKillScore();
	EVCStyleBonus DetermineStyleBonus() const;

	float LastFireTime;
};
