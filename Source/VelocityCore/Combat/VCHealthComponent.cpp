#include "Combat/VCHealthComponent.h"

UVCHealthComponent::UVCHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void UVCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}

void UVCHealthComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || !IsAlive())
	{
		return;
	}

	const float PreviousHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, CurrentHealth - PreviousHealth);

	if (PreviousHealth > 0.0f && CurrentHealth <= 0.0f)
	{
		OnDeath.Broadcast();
	}
}

void UVCHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || !IsAlive())
	{
		return;
	}

	const float PreviousHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, CurrentHealth - PreviousHealth);
}

bool UVCHealthComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}
