#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VCHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVCOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FVCOnHealthChanged, float, CurrentHealth, float, MaxHealth, float, Delta);

// Blueprint-friendly health component shared by the player and simple test enemies.
UCLASS(ClassGroup=(VelocityCore), meta=(BlueprintSpawnableComponent))
class VELOCITYCORE_API UVCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVCHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health", meta=(ClampMin="1.0"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FVCOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FVCOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category="Health")
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;
};
