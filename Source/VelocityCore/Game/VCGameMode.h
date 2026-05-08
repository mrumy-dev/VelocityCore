#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VCGameMode.generated.h"

// Prototype game mode that wires the default pawn and controller classes.
UCLASS()
class VELOCITYCORE_API AVCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVCGameMode();
};
