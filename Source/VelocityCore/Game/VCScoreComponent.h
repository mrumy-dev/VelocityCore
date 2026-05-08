#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VCScoreComponent.generated.h"

UENUM(BlueprintType)
enum class EVCStyleBonus : uint8
{
	NormalKill UMETA(DisplayName="Normal Kill"),
	AirKill UMETA(DisplayName="Air Kill"),
	DiveKill UMETA(DisplayName="Dive Kill"),
	WallRunKill UMETA(DisplayName="Wall-Run Kill")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVCOnScoreChanged, int32, Score, float, ComboMultiplier);

// Tracks the prototype score and combo multiplier. More detailed style rules can plug into this later.
UCLASS(ClassGroup=(VelocityCore), meta=(BlueprintSpawnableComponent))
class VELOCITYCORE_API UVCScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVCScoreComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
	float ComboMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score", meta=(ClampMin="1"))
	int32 BaseKillScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score", meta=(ClampMin="0.0"))
	float ComboStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score", meta=(ClampMin="1.0"))
	float MaxComboMultiplier;

	UPROPERTY(BlueprintAssignable, Category="Score")
	FVCOnScoreChanged OnScoreChanged;

	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 BaseScore);

	UFUNCTION(BlueprintCallable, Category="Score")
	void AddStyleKillScore(EVCStyleBonus StyleBonus);

	UFUNCTION(BlueprintCallable, Category="Score")
	void ResetCombo();

private:
	int32 GetStyleBonusValue(EVCStyleBonus StyleBonus) const;
	void BroadcastScoreChanged();
};
