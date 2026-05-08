# Velocity Core

Velocity Core is a small Unreal Engine 5 C++ prototype for a third-person parkour arena shooter. The current goal is not a full game. The foundation should make it quick to test movement, shooting, dummy enemies, scoring, and combo feel in a compact arena.

## Current Prototype Goal

Build a clean first playable loop:

move fast -> parkour -> shoot dummy enemies -> gain score/combo -> survive longer -> replay

The first pass intentionally avoids multiplayer, story systems, inventory, complex enemy AI, and marketplace assets.

## Required Software

- Unreal Engine 5
- JetBrains Rider with Unreal Engine support
- Git
- A C++ toolchain supported by your UE5 install

This repository contains source and config scaffolding only. It has not yet been opened, generated, or compiled by Unreal Editor in this environment.

## Project Structure Plan

```text
VelocityCore.uproject
Config/
  DefaultEngine.ini
  DefaultGame.ini
Source/
  VelocityCore/
    Characters/    Player character and pawn-facing code
    Movement/      Parkour, sprint, dodge, dive, wall-run systems
    Combat/        Health, weapons, damage-facing components
    Enemies/       Dummy targets first, simple enemies later
    Game/          Game mode, controller, score, match rules
    UI/            HUD/widgets later
    Upgrades/      Upgrade hooks later, no inventory system
Content/
  Maps/            M_MovementPlayground and prototype arenas
  Input/           Enhanced Input actions and mapping contexts
  Blueprints/      BP children of C++ classes
  Materials/       Simple prototype materials
  Prototyping/     Temporary blockout meshes and test assets
```

## Core C++ Classes

- `AVCPlayerCharacter`: third-person player base with movement, look, jump, sprint, dodge, dive, shoot, aim, and reload input hooks.
- `UVCParkourComponent`: prototype movement state for sprinting, dodging, diving, wall-run detection, wall-run start/end, and wall jump.
- `UVCHealthComponent`: Blueprint-friendly health, healing, damage, health changed, and death event.
- `UVCWeaponComponent`: fire entry point with line trace damage, debug trace drawing, cooldown feedback, hit/miss hooks, and kill scoring.
- `UVCScoreComponent`: score, combo multiplier, and style kill bonus placeholder.
- `AVCEnemyDummy`: damageable test enemy with no AI.
- `AVCGameMode`: prototype default pawn/controller wiring.
- `AVCPlayerController`: placeholder owner for player-specific UI/input responsibilities.

## First Playable Checklist

- [ ] Open `VelocityCore.uproject` in Unreal Engine 5 and let Unreal build the C++ module.
- [ ] Create `Content/Maps/M_MovementPlayground`.
- [ ] Add a floor, two tagged wall-run test walls, and obstacle cubes.
- [ ] Create `BP_VCPlayerCharacter` from `AVCPlayerCharacter`.
- [ ] Create the Enhanced Input assets listed in `DEVELOPMENT.md`.
- [ ] Assign `IMC_Player` and input actions on `BP_VCPlayerCharacter`.
- [ ] Place or set the player character as the default pawn.
- [ ] Create/place `BP_VCEnemyDummy` from `AVCEnemyDummy`.
- [ ] Verify line trace shooting damages and destroys the dummy enemy.
- [ ] Tune and verify dodge.
- [ ] Tune and verify dive.
- [ ] Allow shooting while diving.
- [ ] Implement simple wall-run startup on actors tagged `WallRun`.
- [ ] Add a basic score counter HUD.

## Early Milestones

1. Phase 1: C++ foundation, repo docs, and prototype architecture.
2. Phase 2: basic shooting feel, hit feedback, debug trace visualization, and dummy scoring.
3. Phase 3: dodge/dive tuning and shoot-while-diving style bonus.
4. Phase 4: simple wall-run and wall-jump on tagged walls.
5. Phase 5: lightweight arena loop with spawned dummy targets and a score HUD.
