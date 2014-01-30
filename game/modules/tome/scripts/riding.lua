--
-- Riding system
--

declare_global_constants
{
	"riding",
}

new_flag("RIDEABLE")
new_flag("RIDE_POWER_COOLDOWN")
riding = {}

-- Dismount "spell"
riding.SPELL_INATE_DISMOUNT = add_spell
{
	["name"] = 	"Dismount",
	["school"] = 	{0},
	["level"] = 	1,
	["mana"] = 	0,
	["mana_max"] = 	0,
	["fail"] = 	0,
	["random"] =    -1,
	["spell"] = 	function()
		local mount = player.inventory[INVEN_MOUNT][1]
		message("Unmount in which direction?")

		local ret, dir = get_rep_dir()
		if not ret then return end
		local y, x = explode_dir(dir)
		y, x = y + player.py, x + player.px
		local m_idx = place_monster_type(y, x, monster_type_dup(mount.flags[FLAG_RIDEABLE]))
		if m_idx > 0 then
			flag_remove(player.inventory[INVEN_MOUNT], 1)
			player.update = player.update | PU_BONUS | PU_MANA
		end
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Dismount",
	}
}


-- Controlled monster forced casting
function riding.use_mount_powers()
	local mount = player.inventory[INVEN_MOUNT][1]
	local m_ptr = mount.flags[FLAG_RIDEABLE]

	-- Copy spells
	local flag_spells = flag_dup(m_ptr.spells)

	-- Add end control spell
	flag_set_full(flag_spells, riding.SPELL_INATE_DISMOUNT, FLAG_FLAG_BOTH, 0, 100)

	local spells = {}

	-- List the monster spells
	for i = 1, flag_spells.size do
		if (flag_spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local spl = flag_spells.node[i].key
			local fail = flag_get2(flag_spells, spl)
			local cooldown = nil
			fail = rescale(fail, 100, 100 + get_skill_scale(SKILL_RIDING, 35))
			fail = 100 - fail
			if fail > 100 then fail = 100 end
			if fail < 0 then fail = 0 end

			if turn - (mount.flags[FLAG_RIDE_POWER_COOLDOWN][spl] or 0) < (10000 / m_ptr.freq_spell) then
				cooldown = ((10000 / m_ptr.freq_spell) - (turn - (mount.flags[FLAG_RIDE_POWER_COOLDOWN][spl] or 0))) / 10
			end

			-- Verify level
			tinsert
			(
				spells,
				{
					index = spl,
					fail = fail,
					level = flag_get(flag_spells, spl)
					cooldown = cooldown,
				}
			)
		end
	end
	-- Sort by level and name
	sort(spells, function(s1, s2) return (s1.level < s2.level) or ((s1.level == s2.level) and (spell(s1.index).name < spell(s2.index).name)) end)

	-- Ask what spell to cast
	local ret = choose_from_list(
		spells, "Spells", "Cast which spell?",
		function (y, x, thing)
			if not thing then
				term.blank_print(format("%-20s%-16s Level Cost Fail Info", "Name", "School"), y, x)
			else
				local color = color.LIGHT_GREEN
				set_get_level(thing.level)
				-- cooldown ?
				if thing.cooldown then
					term.blank_print(color, format("%-20s%-16s   %3d %4s %3d%s %s", spell(thing.index).name, "Monster", thing.level, 0, thing.fail, "%", "Cooldown "..thing.cooldown.." turns"), y, x)
				else
					term.blank_print(color, format("%-20s%-16s   %3d %4s %3d%s %s", spell(thing.index).name, "Monster", thing.level, 0, thing.fail, "%", __spell_info[thing.index]()), y, x)
				end
				unset_get_level()
			end
		end,
		function (y, thing)
			set_get_level(thing.level)
			print_spell_desc(thing.index, y)
			unset_get_level()
		end
	)

	-- Something to do ?
	if ret then
		if ret.cooldown then
			message("You can not use that ability yet.")
		else
			energy_use = get_player_energy(SPEED_GLOBAL)

			set_get_level(ret.level)
			if rng.percent(ret.fail) then
				message("You fail to control your mount!")
			else
				__spell_spell[ret.index]()
			end

			-- engage cooldown
			if ret.index ~= riding.SPELL_INATE_DISMOUNT then
				mount.flags[FLAG_RIDE_POWER_COOLDOWN][ret.index] = turn
			end

			unset_get_level()
		end
	end

	-- Free memory
	flag_free(flag_spells, true)

	return true
end


riding.MKEY = add_mkey
{
	fct = function()
		-- Mount
		if not player.inventory[INVEN_MOUNT][1] then
			local ret, dir = get_rep_dir()
			if not ret then return end
			local y, x = explode_dir(dir)
			y, x = y + player.py, x + player.px

			if cave(y, x).m_idx == 0 then
				message("You must target a rideable beast.")
				return
			else
				local monst = monster(cave(y, x).m_idx)
				if monst.flags[FLAG_RIDEABLE] and monst.flags[FLAG_RIDEABLE] <= get_skill(SKILL_RIDING) and factions.get_friendliness(FACTION_PLAYER, monst.faction) > 0 then
					local mount = create_object(TV_MOUNT, SV_MOUNT)
					mount.flags[FLAG_WIELD_SLOT] = INVEN_MOUNT
					mount.flags[FLAG_RIDEABLE] = monster_type_dup(monst)
					if not mount.flags[FLAG_RIDE_POWER_COOLDOWN] then mount.flags[FLAG_RIDE_POWER_COOLDOWN] = flag_new() end
					message("You mount the "..monster_desc(monst, 0))
					delete_monster_idx(cave(y, x).m_idx)
					set_object(compute_slot(INVEN_MOUNT, 1), mount)
					energy_use = get_player_energy(SPEED_GLOBAL)
					player.update = player.update | PU_BONUS | PU_MANA
				else
					message("You can not ride this beast.")
				end
			end

		-- Unmount
		else
			riding.use_mount_powers()
		end
	end
}

hook(hook.CALC_BONUS_BEGIN, function()
	if player.inventory[INVEN_MOUNT][1] then
		local mount = player.inventory[INVEN_MOUNT][1].flags[FLAG_RIDEABLE]
		local speed = mount.mspeed - 110
		speed = (speed * get_skill_scale(SKILL_RIDING, 135)) / 100
		player.inc_speed(SPEED_WALK, speed)

		if get_flag(mount, FLAG_CAN_FLY) > player.intrinsic(FLAG_CAN_FLY) then player.set_intrinsic(FLAG_CAN_FLY, get_flag(mount, FLAG_CAN_FLY)) end

		local mult = iif(has_ability(AB_COMBAT_RIDING), 1, 0)
		flag_inc(player.skill_bonuses, SKILL_COMBAT, 	-15000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
		flag_inc(player.skill_bonuses, SKILL_MASTERY, 	-15000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
		flag_inc(player.skill_bonuses, SKILL_ARCHERY, 	-15000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
		flag_inc(player.skill_bonuses, SKILL_HAND, 	-15000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
		flag_inc(player.skill_bonuses, SKILL_MAGIC, 	-15000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
		flag_inc(player.skill_bonuses, SKILL_STEALTH, 	-30000 + get_skill_scale(SKILL_RIDING, 15000) * mult)
	end
end)
