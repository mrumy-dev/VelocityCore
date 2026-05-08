# Future Codex Session Notes

You are working on Velocity Core, a UE5 C++ third-person parkour arena shooter prototype. Keep the scope small and prioritize a clean, beginner-readable foundation.

## Architecture

- Project/module name: `VelocityCore`
- C++ prefix: `VC`
- Main module path: `Source/VelocityCore`
- Player base: `AVCPlayerCharacter`
- Movement component: `UVCParkourComponent`
- Health component: `UVCHealthComponent`
- Weapon component: `UVCWeaponComponent`
- Score component: `UVCScoreComponent`
- Dummy enemy: `AVCEnemyDummy`

## Constraints

- Do not add multiplayer.
- Do not add story systems.
- Do not add inventory.
- Do not add complex enemy AI yet.
- Do not add marketplace assets.
- Do not add new plugins unless the task cannot be completed without one.
- Keep C++ readable and Unreal-conventional.
- Use `UPROPERTY` for tuning values that designers should edit in Blueprints.
- Keep systems modular and component-based.

## Working Style

- Inspect the repo before changing files.
- Keep generated Unreal folders out of Git.
- Do not commit, push, or create pull requests unless explicitly asked.
- If Unreal Editor or UnrealBuildTool is unavailable, document exactly what could not be verified.
- Prefer focused prototype steps: movement feel, shooting feel, dummy targets, score feedback, then wall-run.
