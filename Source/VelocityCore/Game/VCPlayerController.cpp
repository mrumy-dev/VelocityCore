#include "Game/VCPlayerController.h"

AVCPlayerController::AVCPlayerController()
{
	bShowMouseCursor = false;
}

void AVCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}
