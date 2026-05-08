# Content Folder Plan

The project currently has no `.uasset` files. Create assets in Unreal Editor using this layout:

```text
Content/
  Maps/
    M_MovementPlayground
  Input/
    IA_Move
    IA_Look
    IA_Jump
    IA_Sprint
    IA_Dodge
    IA_Dive
    IA_Shoot
    IA_Aim
    IA_Reload
    IMC_Player
  Blueprints/
    Characters/
      BP_VCPlayerCharacter
    Enemies/
      BP_VCEnemyDummy
    Game/
      BP_VCGameMode if Blueprint tuning is needed
    UI/
      WBP_ScoreHUD later
  Materials/
    Prototype colors and wall-run readability materials
  Prototyping/
    Temporary blockout meshes and test-only assets
```
