#include "Characters/VCPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Combat/VCHealthComponent.h"
#include "Combat/VCWeaponComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/VCScoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Movement/VCParkourComponent.h"
#include "VelocityCore.h"

AVCPlayerCharacter::AVCPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 650.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	ParkourComponent = CreateDefaultSubobject<UVCParkourComponent>(TEXT("ParkourComponent"));
	HealthComponent = CreateDefaultSubobject<UVCHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UVCWeaponComponent>(TEXT("WeaponComponent"));
	ScoreComponent = CreateDefaultSubobject<UVCScoreComponent>(TEXT("ScoreComponent"));
}

void AVCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (PlayerMappingContext)
				{
					Subsystem->AddMappingContext(PlayerMappingContext, 0);
				}
			}
		}
	}
}

void AVCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogVelocityCore, Warning, TEXT("AVCPlayerCharacter requires Enhanced Input to bind prototype controls."));
		return;
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVCPlayerCharacter::Move);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVCPlayerCharacter::Look);
	}

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AVCPlayerCharacter::StopSprint);
	}

	if (DodgeAction)
	{
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::HandleDodge);
	}

	if (DiveAction)
	{
		EnhancedInputComponent->BindAction(DiveAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::HandleDive);
	}

	if (ShootAction)
	{
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::HandleShoot);
	}

	if (AimAction)
	{
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AVCPlayerCharacter::StopAim);
	}

	if (ReloadAction)
	{
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AVCPlayerCharacter::HandleReload);
	}
}

void AVCPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AVCPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AVCPlayerCharacter::StartSprint()
{
	if (ParkourComponent)
	{
		ParkourComponent->StartSprint();
	}
}

void AVCPlayerCharacter::StopSprint()
{
	if (ParkourComponent)
	{
		ParkourComponent->StopSprint();
	}
}

void AVCPlayerCharacter::HandleDodge()
{
	if (ParkourComponent)
	{
		ParkourComponent->Dodge(GetLastMovementInputVector());
	}
}

void AVCPlayerCharacter::HandleDive()
{
	if (ParkourComponent)
	{
		ParkourComponent->Dive(GetLastMovementInputVector());
	}
}

void AVCPlayerCharacter::HandleShoot()
{
	if (WeaponComponent)
	{
		WeaponComponent->TryFire();
	}
}

void AVCPlayerCharacter::StartAim()
{
	// Aim tuning belongs in Phase 2 once camera/weapon feel is tested.
}

void AVCPlayerCharacter::StopAim()
{
	// Aim tuning belongs in Phase 2 once camera/weapon feel is tested.
}

void AVCPlayerCharacter::HandleReload()
{
	// Ammo and reload timing are intentionally deferred until the weapon loop needs them.
}
