// SnowyOwl All Rights Reserved


#include "WarriorGameplayTags.h"

namespace WarriorGameplayTags
{
	/** Input Tags **/
	// 이동, 카메라 회전 인풋태그
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");

	// 무장, 무장해제 인풋태그
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, "InputTag.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, "InputTag.UnequipAxe");

	// 약공격, 강공격 인풋태그
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe, "InputTag.LightAttack.Axe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe, "InputTag.HeavyAttack.Axe");

	/** Player Tags **/
	// 무장, 무장해제 플레이어 어빌리티 태그
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "Player.Ability.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "Player.Ability.Unequip.Axe");

	// 약공격, 강공격 플레이어 어빌리티 태그
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, "Player.Ability.Attack.Light.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "Player.Ability.Attack.Heavy.Axe");

	// 플레이어 무기 태그
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");

	// 플레이어 이벤트 태그
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "Player.Event.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "Player.Event.Unequip.Axe");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "Player.Status.JumpToFinisher");

	/** Enemy Tags **/
	// 적 무기 태그
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	/** Shared Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
}
