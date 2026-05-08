#include "Game/VCScoreComponent.h"

UVCScoreComponent::UVCScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Score = 0;
	ComboMultiplier = 1.0f;
	BaseKillScore = 100;
	ComboStep = 0.25f;
	MaxComboMultiplier = 5.0f;
}

void UVCScoreComponent::AddScore(int32 BaseScore)
{
	if (BaseScore <= 0)
	{
		return;
	}

	Score += FMath::RoundToInt(static_cast<float>(BaseScore) * ComboMultiplier);
	ComboMultiplier = FMath::Clamp(ComboMultiplier + ComboStep, 1.0f, MaxComboMultiplier);

	BroadcastScoreChanged();
}

void UVCScoreComponent::AddStyleKillScore(EVCStyleBonus StyleBonus)
{
	AddScore(BaseKillScore + GetStyleBonusValue(StyleBonus));
}

void UVCScoreComponent::ResetCombo()
{
	ComboMultiplier = 1.0f;
	BroadcastScoreChanged();
}

int32 UVCScoreComponent::GetStyleBonusValue(EVCStyleBonus StyleBonus) const
{
	switch (StyleBonus)
	{
	case EVCStyleBonus::AirKill:
		return 50;
	case EVCStyleBonus::DiveKill:
		return 75;
	case EVCStyleBonus::WallRunKill:
		return 100;
	case EVCStyleBonus::NormalKill:
	default:
		return 0;
	}
}

void UVCScoreComponent::BroadcastScoreChanged()
{
	OnScoreChanged.Broadcast(Score, ComboMultiplier);
}
