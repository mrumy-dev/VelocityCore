#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "TimerManager.h"
#include "VCParkourComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;

// Owns the prototype's advanced movement state: sprinting, dodging, diving, and wall-run placeholders.
UCLASS(ClassGroup=(VelocityCore), meta=(BlueprintSpawnableComponent))
class VELOCITYCORE_API UVCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVCParkourComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Sprint", meta=(ClampMin="1.0"))
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Dodge", meta=(ClampMin="0.0"))
	float DodgeStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Dodge", meta=(ClampMin="0.0"))
	float DodgeCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Dive", meta=(ClampMin="0.0"))
	float DiveStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Dive", meta=(ClampMin="0.0"))
	float DiveCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Wall Run", meta=(ClampMin="0.0"))
	float WallRunDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Wall Run", meta=(ClampMin="0.0"))
	float WallRunGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parkour|Wall Run", meta=(ClampMin="1.0"))
	float WallRunTraceDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Parkour|State")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Parkour|State")
	bool bIsDodging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Parkour|State")
	bool bIsDiving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Parkour|State")
	bool bIsWallRunning;

	UFUNCTION(BlueprintCallable, Category="Parkour|Sprint")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category="Parkour|Sprint")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category="Parkour|Dodge")
	bool Dodge(FVector Direction);

	UFUNCTION(BlueprintCallable, Category="Parkour|Dive")
	bool Dive(FVector Direction);

	UFUNCTION(BlueprintCallable, Category="Parkour|Wall Run")
	bool FindWallRunSurface(FHitResult& OutHit) const;

	UFUNCTION(BlueprintCallable, Category="Parkour|Wall Run")
	bool StartWallRun(const FVector& WallNormal);

	UFUNCTION(BlueprintCallable, Category="Parkour|Wall Run")
	void EndWallRun();

	UFUNCTION(BlueprintCallable, Category="Parkour|Wall Run")
	bool WallJump();

	UFUNCTION(BlueprintPure, Category="Parkour|Dodge")
	bool CanDodge() const;

	UFUNCTION(BlueprintPure, Category="Parkour|Dive")
	bool CanDive() const;

protected:
	virtual void BeginPlay() override;

private:
	ACharacter* GetOwnerCharacter() const;
	UCharacterMovementComponent* GetCharacterMovement() const;
	void FinishDodge();
	void FinishDive();

	float OriginalMaxWalkSpeed;
	float OriginalGravityScale;
	float LastDodgeTime;
	float LastDiveTime;
	FVector CurrentWallRunNormal;
	FTimerHandle DodgeResetTimerHandle;
	FTimerHandle DiveResetTimerHandle;
	FTimerHandle WallRunTimerHandle;
};
