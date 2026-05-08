#include "Movement/VCParkourComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UVCParkourComponent::UVCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SprintSpeedMultiplier = 1.5f;
	DodgeStrength = 1200.0f;
	DodgeCooldown = 0.5f;
	DiveStrength = 1500.0f;
	DiveCooldown = 0.75f;
	WallRunDuration = 1.0f;
	WallRunGravityScale = 0.25f;
	WallRunTraceDistance = 120.0f;

	bIsSprinting = false;
	bIsDodging = false;
	bIsDiving = false;
	bIsWallRunning = false;

	OriginalMaxWalkSpeed = 600.0f;
	OriginalGravityScale = 1.0f;
	LastDodgeTime = -1000.0f;
	LastDiveTime = -1000.0f;
	CurrentWallRunNormal = FVector::ZeroVector;
}

void UVCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		OriginalMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
		OriginalGravityScale = MovementComponent->GravityScale;
	}
}

void UVCParkourComponent::StartSprint()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent || bIsSprinting)
	{
		return;
	}

	bIsSprinting = true;
	MovementComponent->MaxWalkSpeed = OriginalMaxWalkSpeed * SprintSpeedMultiplier;
}

void UVCParkourComponent::StopSprint()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent || !bIsSprinting)
	{
		return;
	}

	bIsSprinting = false;
	MovementComponent->MaxWalkSpeed = OriginalMaxWalkSpeed;
}

bool UVCParkourComponent::Dodge(FVector Direction)
{
	ACharacter* Character = GetOwnerCharacter();
	UWorld* World = GetWorld();
	if (!Character || !World || !CanDodge())
	{
		return false;
	}

	FVector DodgeDirection = Direction.GetSafeNormal2D();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = Character->GetActorForwardVector();
	}

	LastDodgeTime = World->GetTimeSeconds();
	bIsDodging = true;

	Character->LaunchCharacter(DodgeDirection * DodgeStrength, true, false);
	World->GetTimerManager().SetTimer(DodgeResetTimerHandle, this, &UVCParkourComponent::FinishDodge, 0.2f, false);

	return true;
}

bool UVCParkourComponent::Dive(FVector Direction)
{
	ACharacter* Character = GetOwnerCharacter();
	UWorld* World = GetWorld();
	if (!Character || !World || !CanDive())
	{
		return false;
	}

	FVector DiveDirection = Direction.GetSafeNormal2D();
	if (DiveDirection.IsNearlyZero())
	{
		DiveDirection = Character->GetActorForwardVector();
	}

	FVector DiveVelocity = DiveDirection * DiveStrength;
	DiveVelocity.Z = -DiveStrength * 0.25f;

	LastDiveTime = World->GetTimeSeconds();
	bIsDiving = true;

	Character->LaunchCharacter(DiveVelocity, true, true);
	World->GetTimerManager().SetTimer(DiveResetTimerHandle, this, &UVCParkourComponent::FinishDive, 0.35f, false);

	return true;
}

bool UVCParkourComponent::FindWallRunSurface(FHitResult& OutHit) const
{
	const ACharacter* Character = GetOwnerCharacter();
	UWorld* World = GetWorld();
	if (!Character || !World)
	{
		return false;
	}

	const FVector Start = Character->GetActorLocation();
	const FVector Right = Character->GetActorRightVector();

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VCWallRunTrace), false, Character);
	const FVector TraceEnds[] =
	{
		Start + Right * WallRunTraceDistance,
		Start - Right * WallRunTraceDistance
	};

	for (const FVector& End : TraceEnds)
	{
		FHitResult HitResult;
		if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->ActorHasTag(FName(TEXT("WallRun"))))
			{
				OutHit = HitResult;
				return true;
			}
		}
	}

	return false;
}

bool UVCParkourComponent::StartWallRun(const FVector& WallNormal)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	UWorld* World = GetWorld();
	if (!MovementComponent || !World || bIsWallRunning)
	{
		return false;
	}

	bIsWallRunning = true;
	CurrentWallRunNormal = WallNormal.GetSafeNormal();
	MovementComponent->GravityScale = WallRunGravityScale;

	World->GetTimerManager().SetTimer(WallRunTimerHandle, this, &UVCParkourComponent::EndWallRun, WallRunDuration, false);

	return true;
}

void UVCParkourComponent::EndWallRun()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->GravityScale = OriginalGravityScale;
	}

	bIsWallRunning = false;
	CurrentWallRunNormal = FVector::ZeroVector;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(WallRunTimerHandle);
	}
}

bool UVCParkourComponent::WallJump()
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character || !bIsWallRunning)
	{
		return false;
	}

	const FVector JumpVelocity = CurrentWallRunNormal * DodgeStrength + FVector::UpVector * DodgeStrength * 0.75f;
	EndWallRun();
	Character->LaunchCharacter(JumpVelocity, true, true);

	return true;
}

bool UVCParkourComponent::CanDodge() const
{
	const UWorld* World = GetWorld();
	return World && World->GetTimeSeconds() - LastDodgeTime >= DodgeCooldown;
}

bool UVCParkourComponent::CanDive() const
{
	const UWorld* World = GetWorld();
	return World && World->GetTimeSeconds() - LastDiveTime >= DiveCooldown;
}

ACharacter* UVCParkourComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

UCharacterMovementComponent* UVCParkourComponent::GetCharacterMovement() const
{
	if (const ACharacter* Character = GetOwnerCharacter())
	{
		return Character->GetCharacterMovement();
	}

	return nullptr;
}

void UVCParkourComponent::FinishDodge()
{
	bIsDodging = false;
}

void UVCParkourComponent::FinishDive()
{
	bIsDiving = false;
}
