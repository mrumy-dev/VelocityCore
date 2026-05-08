#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VCEnemyDummy.generated.h"

class UVCHealthComponent;

// Simple damageable dummy enemy for shooting and scoring tests. No AI yet.
UCLASS()
class VELOCITYCORE_API AVCEnemyDummy : public ACharacter
{
	GENERATED_BODY()

public:
	AVCEnemyDummy();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UVCHealthComponent> HealthComponent;

	UFUNCTION()
	void HandleDeath();
};
