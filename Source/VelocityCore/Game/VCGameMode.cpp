#include "Game/VCGameMode.h"
#include "Characters/VCPlayerCharacter.h"
#include "Game/VCPlayerController.h"

AVCGameMode::AVCGameMode()
{
	DefaultPawnClass = AVCPlayerCharacter::StaticClass();
	PlayerControllerClass = AVCPlayerController::StaticClass();
}
