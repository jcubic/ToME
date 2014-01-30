-- The spellcasting combat subsystem
--
-- Load it with: load_subsystem("combat_spellcasting")
--
-- This is the "default" spellcasting combat system as it exists for ToME
--
-- Parameters:
-- * base_skill_name: The name of the skill which defines this combat method

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat")

-- This requires the easy-cast subsystem
load_subsystem("easy_cast")

safe_new_flag("SPELLCASTER_WEAPON")

-- Make the namespace
combat.spellcasting = {}

combat.spellcasting.INVEN_MAINHAND = get_subsystem_param("combat_spellcasting", "mainhand_slot") or 0

-- Resolve skill name afterwards
hook(hook.INFO_DATA_LOADED,
	function()
		combat.spellcasting.SKILL_MAGIC = find_skill(get_subsystem_param("combat_spellcasting", "base_skill_name"))
	end
)

combat.spellcasting.COMBAT_MAGIC = add_combat_system
{
	name	= "Spellcasting"
	desc	= "Uses spells to inflic pain and suffering, among other nifty things."
	skill	= get_subsystem_param("combat_spellcasting", "base_skill_name")
	energy	= function() return nil end
	available = function()
		if player.inventory_limits(combat.spellcasting.INVEN_MAINHAND) == 0 then return false end
		return true
	end,
	info = function()
	end

	attack = function(y, x, max, dir)
		combat.spellcasting.attack(dir)
	end,

	hooks       =
	{
		[hook.WIELD_POST] = function(obj, item, slot)
			if item_slot_to_inven(slot) == combat.default_weapon.INVEN_MAINHAND and obj and has_flag(obj, FLAG_WEAPON) and get_combat_availability(combat.spellcasting.SKILL_MAGIC) and player.combat_style ~= combat.spellcasting.SKILL_MAGIC then
				if player.combat_style ~= combat.default_weapon.SKILL_MASTERY then
					player.combat_style = combat.default_weapon.SKILL_MASTERY
					message("Autoswitch to weapon combat mode.")
				end
			end
		end
	}
}

function combat.spellcasting.attack(force_dir)
	if player.combat_style ~= combat.spellcasting.SKILL_MAGIC then
		return
	end

	repeat_keys.cur_idx = 0
--	repeat_keys.cur_cnt = 0
	local spl = find_spell("Manathrust")
	repeat_keys.push(spl)
	repeat_keys.push(5)
	easy_cast.cast()
end

if not get_subsystem_param("combat_spellcasting", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('f') then
			if player.wild_mode or player.control > 0 then return end
			combat.spellcasting.attack()
			return true
		end
	end)
end
