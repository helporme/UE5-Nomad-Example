# UE5-Nomad-Example

![](Images/Thumbnail.png)

## Prelude

UE5-Nomad-Example is a single-player ARTS example project for Unreal Engine 5 implemented with the GameplayAbilitySystem plugin. You can use the project to see how to implement some common gameplay mechanics in the ARTS genre. Note that this project is not intended to be a production-ready solution for ARTS games, but a reference for learning the Unreal Engine and the GameplayAbilitySystem plugin in particular.

[Gamemakin UE4 Style Guide](https://github.com/Allar/ue5-style-guide) is used for the directory structure and asset names.

All visual assets are included from the Starter Content pack, [Mannequins pack](https://www.unrealengine.com/marketplace/en-US/product/mannequins-asset-pack), [icons8](https://icons8.com/), created by my friend (Meshes & Textures), or me.

## Input
Normal mode:

| Key                | Action      |
|--------------------|-------------|
| Q,W,ER,T,Y,U,I     | Cast spell  |
| Right mouse button | Movement    |
| Press mouse wheel  | Change view |
| Scroll mouse wheel | Zoom        |

Spell targeting mode:

| Key                | Action      |
|--------------------|-------------|
| Left mouse button  | Confirm     |
| Right mouse button | Cancel      |


## Gameplay systems covered
 * [Spell targeting](Source/Nomad/Spells/Targeting/SpellTarget.h) (GameplayAbilityTargetActor)
 * [Spell reticles](Source/Nomad/Spells/Targeting/SpellReticle.h) (GameplayAbilityWorldReticles)
 * Spell visualization (GameplayCue)
 * [Spell input](Source/Nomad/NomadPlayerController.h) (Binding EnhancedInput to the GAS input)
 * [Spell cast stages](Source/Nomad/Spells/NomadSpell.h)
 * Spell cast montages (with blending and cast time adjusting)
 * [Spell logic](Source/Nomad/Spells/Tasks/NomadTask.h) (Ability tasks exposed to blueprints)
 * Passive spells (auto-triggering)
 * Sending information between spells
 * Implementation of movement via abilities
 * [Interactable UI](Source/Nomad/Spells/Widgets)

## Useful projects
 * [GASDocumentation](https://github.com/tranek/GASDocumentation)
 * [LearningUnreal](https://github.com/ibbles/LearningUnreal)
 * [Unreal Community Wiki](https://unrealcommunity.wiki/)
