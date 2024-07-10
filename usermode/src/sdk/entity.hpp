#pragma once

enum class e_cs_team : uint8_t
{
	team_unassigned,
	team_spectator,
	team_terrorist,
	team_counter_terrorist
};

enum class e_colors : uint32_t
{
	blue,
	green,
	yellow,
	orange,
	purple,
	white
};

enum class e_cs_weapon_type : uint32_t
{
	weapontype_knife,
	weapontype_pistol,
	weapontype_submachinegun,
	weapontype_rifle,
	weapontype_shotgun,
	weapontype_sniper_rifle,
	weapontype_machinegun,
	weapontype_c4,
	weapontype_taser,
	weapontype_grenade,
	weapontype_equipment,
	weapontype_stackableitem,
	weapontype_fists,
	weapontype_breachcharge,
	weapontype_bumpmine,
	weapontype_tablet,
	weapontype_melee,
	weapontype_shield,
	weapontype_zone_repulsor,
	weapontype_unknown
};

class c_entity_identity
{
public:
	SCHEMA_ADD_OFFSET(uintptr_t, m_pClassInfo, 0x08);
	SCHEMA_ADD_OFFSET(uint32_t, m_Idx, 0x10);
	SCHEMA_ADD_FIELD(const char*, m_designerName, "CEntityIdentity->m_designerName");
	SCHEMA_ADD_FIELD(uint32_t, m_flags, "CEntityIdentity->m_flags");

	bool is_valid()
	{
		return m_Idx() != INVALID_EHANDLE_IDX;
	}

	int32_t get_entry_idx()
	{
		if (!is_valid())
			return ENT_ENTRY_MASK;

		return m_Idx() & ENT_ENTRY_MASK;
	}

	int32_t get_serial_number()
	{
		return m_Idx() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}
};

class c_entity_instance
{
public:
	SCHEMA_ADD_FIELD(c_entity_identity*, m_pEntity, "CEntityInstance->m_pEntity");

	const c_base_handle get_ref_e_handle();
	const std::string get_schema_class_name();
};

class c_game_scene_node
{
public:
	SCHEMA_ADD_FIELD(fvector3, m_vecAbsOrigin, "CGameSceneNode->m_vecAbsOrigin");
};

class c_base_entity : public c_entity_instance
{
public:
	SCHEMA_ADD_FIELD(c_game_scene_node*, m_pGameSceneNode, "C_BaseEntity->m_pGameSceneNode");
	SCHEMA_ADD_FIELD(int32_t, m_iHealth, "C_BaseEntity->m_iHealth");
	SCHEMA_ADD_FIELD(e_cs_team, m_iTeamNum, "C_BaseEntity->m_iTeamNum");
	SCHEMA_ADD_FIELD(c_base_entity*, m_hOwnerEntity, "C_BaseEntity->m_hOwnerEntity");

	const fvector3& get_scene_origin();
};

class c_player_weapon_services
{
public:
	SCHEMA_ADD_FIELD(c_base_handle, m_hActiveWeapon, "CPlayer_WeaponServices->m_hActiveWeapon");
	SCHEMA_ADD_FIELD(c_network_utl_vector_base<class c_base_player_weapon>, m_hMyWeapons, "CPlayer_WeaponServices->m_hMyWeapons");
};

class c_player_item_services
{
public:
	SCHEMA_ADD_FIELD(bool, m_bHasDefuser, "CCSPlayer_ItemServices->m_bHasDefuser");
	SCHEMA_ADD_FIELD(bool, m_bHasHelmet, "CCSPlayer_ItemServices->m_bHasHelmet");
	SCHEMA_ADD_FIELD(bool, m_bHasHeavyArmor, "CCSPlayer_ItemServices->m_bHasHeavyArmor");
};

class c_base_player_pawn : public c_base_entity
{
public:
	SCHEMA_ADD_FIELD(c_player_weapon_services*, m_pWeaponServices, "C_BasePlayerPawn->m_pWeaponServices");
	SCHEMA_ADD_FIELD(c_player_item_services*, m_pItemServices, "C_BasePlayerPawn->m_pItemServices");
};

class c_cs_player_pawn_base : public c_base_player_pawn
{
public:
	SCHEMA_ADD_FIELD(fvector3, m_angEyeAngles, "C_CSPlayerPawnBase->m_angEyeAngles");
};

class c_cs_player_pawn : public c_cs_player_pawn_base
{
public:
	static inline c_cs_player_pawn* m_local_pawn = nullptr;

	SCHEMA_ADD_FIELD(int32_t, m_ArmorValue, "C_CSPlayerPawn->m_ArmorValue");

	const std::string get_model_name();
};

class c_base_player_controller : public c_base_entity
{
public:
	SCHEMA_ADD_FIELD(c_base_handle, m_hPawn, "CBasePlayerController->m_hPawn");
	SCHEMA_ADD_FIELD(uint64_t, m_steamID, "CBasePlayerController->m_steamID");
};

class c_in_game_money_services
{
public:
	SCHEMA_ADD_FIELD(int32_t, m_iAccount, "CCSPlayerController_InGameMoneyServices->m_iAccount");
};

class c_cs_player_controller : public c_base_player_controller
{
public:
	SCHEMA_ADD_FIELD(c_in_game_money_services*, m_pInGameMoneyServices, "CCSPlayerController->m_pInGameMoneyServices");
	SCHEMA_ADD_FIELD(e_colors, m_iCompTeammateColor, "CCSPlayerController->m_iCompTeammateColor");
	SCHEMA_ADD_STRING(m_sSanitizedPlayerName, "CCSPlayerController->m_sSanitizedPlayerName");
	SCHEMA_ADD_FIELD(c_base_handle, m_hPlayerPawn, "CCSPlayerController->m_hPlayerPawn");
	SCHEMA_ADD_FIELD(bool, m_bPawnIsAlive, "CCSPlayerController->m_bPawnIsAlive");

	const e_colors get_color();
	const fvector3& get_vec_origin();
};

class c_planted_c4 : public c_base_entity
{
public:
	SCHEMA_ADD_FIELD(bool, m_bBombTicking, "C_PlantedC4->m_bBombTicking");
	SCHEMA_ADD_FIELD(float, m_flC4Blow, "C_PlantedC4->m_flC4Blow");
	SCHEMA_ADD_FIELD(bool, m_bBombDefused, "C_PlantedC4->m_bBombDefused");
	SCHEMA_ADD_FIELD(bool, m_bBeingDefused, "C_PlantedC4->m_bBeingDefused");
	SCHEMA_ADD_FIELD(float, m_flDefuseCountDown, "C_PlantedC4->m_flDefuseCountDown");
};

class c_cs_weapon_base_v_data
{
public:
	SCHEMA_ADD_FIELD(e_cs_weapon_type, m_WeaponType, "CCSWeaponBaseVData->m_WeaponType");
	SCHEMA_ADD_STRING(m_szName, "CCSWeaponBaseVData->m_szName");
};

class c_base_player_weapon : public c_base_entity
{
public:
	SCHEMA_ADD_FIELD_OFFSET(c_cs_weapon_base_v_data*, m_WeaponData, "C_BaseEntity->m_nSubclassID", 0x08);

	c_base_player_weapon* get(const int32_t idx);
};