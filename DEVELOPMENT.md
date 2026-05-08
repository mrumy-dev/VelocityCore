# Development Notes

## Opening the Project

1. Open Unreal Engine 5.
2. Browse to this repository and open `VelocityCore.uproject`.
3. If prompted, rebuild missing modules.
4. In Rider, open `VelocityCore.uproject` or the generated IDE project files.
5. Build the `VelocityCoreEditor` target in `Development Editor` configuration.

If Unreal cannot compile immediately, create a fresh UE5 C++ Third Person project named `VelocityCore`, close the editor, then copy this repository's `Source/`, `Config/`, docs, and `.gitignore` into that project.

On macOS, Unreal requires full Xcode for C++ editor builds. If `xcodebuild -version` says only Command Line Tools are selected, install/open Xcode and run:

```sh
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
sudo xcodebuild -license accept
```

## Rider Workflow

- Keep generated folders out of Git: `Binaries/`, `Intermediate/`, `Saved/`, and `DerivedDataCache/`.
- Prefer editing gameplay classes in `Source/VelocityCore`.
- Let Unreal/Rider generate project files instead of committing IDE output.
- Use `AVCPlayerCharacter` as the C++ base and create Blueprint children for asset references and tuning.

## Enhanced Input Setup

The C++ hooks are already present on `AVCPlayerCharacter`. Create these assets manually in Unreal Editor under `Content/Input`:

| Asset | Value Type | Suggested bindings |
| --- | --- | --- |
| `IA_Move` | Axis2D | W/S on Y, A/D on X |
| `IA_Look` | Axis2D | Mouse X/Y, gamepad right stick |
| `IA_Jump` | Boolean | Space, gamepad face bottom |
| `IA_Sprint` | Boolean | Left Shift, gamepad left stick press |
| `IA_Dodge` | Boolean | Left Ctrl, gamepad face right |
| `IA_Dive` | Boolean | C, gamepad face left |
| `IA_Shoot` | Boolean | Left Mouse Button, gamepad right trigger |
| `IA_Aim` | Boolean | Right Mouse Button, gamepad left trigger |
| `IA_Reload` | Boolean | R, gamepad face top |
| `IMC_Player` | Input Mapping Context | Contains all mappings above |

Then create `BP_VCPlayerCharacter` from `AVCPlayerCharacter` and assign:

- `PlayerMappingContext = IMC_Player`
- `MoveAction = IA_Move`
- `LookAction = IA_Look`
- `JumpAction = IA_Jump`
- `SprintAction = IA_Sprint`
- `DodgeAction = IA_Dodge`
- `DiveAction = IA_Dive`
- `ShootAction = IA_Shoot`
- `AimAction = IA_Aim`
- `ReloadAction = IA_Reload`

## Movement Playground Map

Create `Content/Maps/M_MovementPlayground` with:

- One large floor plane or cube.
- Two long wall-run test walls with the actor tag `WallRun`.
- Several obstacle cubes for jump/dodge/dive spacing.
- One player start.
- One `BP_VCEnemyDummy`.

Keep the first map ugly and readable. Use simple grid-aligned blockout shapes and high-contrast prototype materials.

## Phase 2 Shooting Hooks

`UVCWeaponComponent` now owns the basic hitscan loop:

- `TryFire()` enforces `FireRate` and broadcasts `OnWeaponCooldownBlocked` when the player fires too early.
- `PerformLineTraceShot()` traces from the owning actor's eye viewpoint and damages any hit actor with `UVCHealthComponent`.
- `bDrawDebugTrace`, `DebugTraceDuration`, `DebugImpactPointRadius`, `DebugHitColor`, and `DebugMissColor` control debug visualization.
- `OnWeaponFired`, `OnWeaponHit`, `OnWeaponMissed`, `OnWeaponCooldownBlocked`, and `OnWeaponKillScored` are Blueprint hooks for effects and UI feedback.
- If a shot kills a target and the owner has `UVCScoreComponent`, the weapon awards score using `NormalKill`, `AirKill`, `DiveKill`, or `WallRunKill` based on the owner movement state.

## C++ Architecture Rules

- Put player-facing movement logic in `UVCParkourComponent`.
- Put health/damage state in `UVCHealthComponent`.
- Put weapon firing in `UVCWeaponComponent`.
- Put score/combo state in `UVCScoreComponent`.
- Keep dummy enemies simple until the core movement and shooting loop feels good.
- Expose tuning values with `UPROPERTY` and use Blueprint child classes for iteration.
- Avoid adding plugin dependencies beyond standard UE5 Enhanced Input unless a future task explicitly needs one.
