#include "Combat/VCWeaponComponent.h"
#include "Combat/VCHealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Game/VCScoreComponent.h"
#include "Movement/VCParkourComponent.h"

UVCWeaponComponent::UVCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	FireRate = 6.0f;
	Damage = 25.0f;
	Range = 10000.0f;
	TraceChannel = ECC_Visibility;
	bDrawDebugTrace = true;
	DebugTraceDuration = 1.0f;
	DebugImpactPointRadius = 8.0f;
	DebugHitColor = FLinearColor::Green;
	DebugMissColor = FLinearColor::Red;
	bAwardScoreOnKill = true;
	LastFireTime = -1000.0f;
}

bool UVCWeaponComponent::TryFire()
{
	if (!CanFire())
	{
		OnWeaponCooldownBlocked.Broadcast(GetFireCooldownRemaining(), GetFireCooldownDuration());
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	LastFireTime = World->GetTimeSeconds();

	FHitResult HitResult;
	FVector TraceStart = FVector::ZeroVector;
	FVector TraceEnd = FVector::ZeroVector;
	bool bKilledTarget = false;
	const bool bHit = PerformLineTraceShotInternal(HitResult, TraceStart, TraceEnd, bKilledTarget);

	DrawDebugShot(TraceStart, TraceEnd, HitResult, bHit);
	OnWeaponFired.Broadcast(bHit, HitResult);

	if (bHit)
	{
		OnWeaponHit.Broadcast(HitResult, bKilledTarget);
	}
	else
	{
		OnWeaponMissed.Broadcast(TraceEnd);
	}

	return bHit;
}

bool UVCWeaponComponent::PerformLineTraceShot(FHitResult& OutHit)
{
	FVector TraceStart = FVector::ZeroVector;
	FVector TraceEnd = FVector::ZeroVector;
	bool bKilledTarget = false;
	return PerformLineTraceShotInternal(OutHit, TraceStart, TraceEnd, bKilledTarget);
}

bool UVCWeaponComponent::CanFire() const
{
	return GetFireCooldownRemaining() <= 0.0f;
}

float UVCWeaponComponent::GetFireCooldownDuration() const
{
	if (FireRate <= 0.0f)
	{
		return 0.0f;
	}

	return 1.0f / FireRate;
}

float UVCWeaponComponent::GetFireCooldownRemaining() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return GetFireCooldownDuration();
	}

	const float TimeSinceLastFire = World->GetTimeSeconds() - LastFireTime;
	return FMath::Max(0.0f, GetFireCooldownDuration() - TimeSinceLastFire);
}

bool UVCWeaponComponent::PerformLineTraceShotInternal(FHitResult& OutHit, FVector& OutTraceStart, FVector& OutTraceEnd, bool& bOutKilledTarget)
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World)
	{
		return false;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	OutTraceStart = EyeLocation;
	OutTraceEnd = EyeLocation + EyeRotation.Vector() * Range;
	bOutKilledTarget = false;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VCWeaponTrace), false, Owner);
	const bool bHit = World->LineTraceSingleByChannel(OutHit, OutTraceStart, OutTraceEnd, TraceChannel, QueryParams);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (UVCHealthComponent* HealthComponent = HitActor->FindComponentByClass<UVCHealthComponent>())
			{
				const bool bWasAlive = HealthComponent->IsAlive();
				HealthComponent->ApplyDamage(Damage);
				bOutKilledTarget = bWasAlive && !HealthComponent->IsAlive();

				if (bOutKilledTarget && bAwardScoreOnKill)
				{
					AwardKillScore();
				}
			}
		}
	}

	return bHit;
}

void UVCWeaponComponent::DrawDebugShot(const FVector& TraceStart, const FVector& TraceEnd, const FHitResult& HitResult, bool bHit) const
{
	if (!bDrawDebugTrace)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector DebugEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
	const FColor DebugColor = (bHit ? DebugHitColor : DebugMissColor).ToFColor(true);

	DrawDebugLine(World, TraceStart, DebugEnd, DebugColor, false, DebugTraceDuration, 0, 1.5f);

	if (bHit)
	{
		DrawDebugSphere(World, HitResult.ImpactPoint, DebugImpactPointRadius, 12, DebugColor, false, DebugTraceDuration);
	}
}

void UVCWeaponComponent::AwardKillScore()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	if (UVCScoreComponent* ScoreComponent = Owner->FindComponentByClass<UVCScoreComponent>())
	{
		const EVCStyleBonus StyleBonus = DetermineStyleBonus();
		ScoreComponent->AddStyleKillScore(StyleBonus);
		OnWeaponKillScored.Broadcast(StyleBonus);
	}
}

EVCStyleBonus UVCWeaponComponent::DetermineStyleBonus() const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return EVCStyleBonus::NormalKill;
	}

	if (const UVCParkourComponent* ParkourComponent = Owner->FindComponentByClass<UVCParkourComponent>())
	{
		if (ParkourComponent->bIsWallRunning)
		{
			return EVCStyleBonus::WallRunKill;
		}

		if (ParkourComponent->bIsDiving)
		{
			return EVCStyleBonus::DiveKill;
		}
	}

	if (const ACharacter* CharacterOwner = Cast<ACharacter>(Owner))
	{
		if (const UCharacterMovementComponent* MovementComponent = CharacterOwner->GetCharacterMovement())
		{
			if (MovementComponent->IsFalling())
			{
				return EVCStyleBonus::AirKill;
			}
		}
	}

	return EVCStyleBonus::NormalKill;
}
