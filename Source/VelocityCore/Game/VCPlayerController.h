#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VCPlayerController.generated.h"

// Player controller placeholder for prototype input/camera/UI ownership.
UCLASS()
class VELOCITYCORE_API AVCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVCPlayerController();

protected:
	virtual void BeginPlay() override;
};
