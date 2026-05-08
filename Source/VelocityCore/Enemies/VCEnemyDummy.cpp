#include "Enemies/VCEnemyDummy.h"
#include "Combat/VCHealthComponent.h"
#include "Engine/DamageEvents.h"

AVCEnemyDummy::AVCEnemyDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UVCHealthComponent>(TEXT("HealthComponent"));
}

void AVCEnemyDummy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AVCEnemyDummy::HandleDeath);
	}
}

float AVCEnemyDummy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(DamageAmount);
	}

	return DamageAmount;
}

void AVCEnemyDummy::HandleDeath()
{
	Destroy();
}
