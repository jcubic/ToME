-- Melee weapon code

constant("COMBAT_WEAPON", add_combat_system
{
	name = "Weapon fighting",
	desc = "Uses various weapons to inflict pain and suffering.",
	skill = "SKILL_NONE",
	energy = function() return get_player_energy(SPEED_WEAPON) end,

	info = function()
		return "???"
	end,

	available = function() return true end,

	attack = function(y, x, max)
		-- Do not fight illusory monsters
		local m_idx = cave(y,x).m_idx
		if m_idx == 0 then return end
		
		-- Get the mob
		local monst = monster(m_idx)
		
		-- Get the damage/type
		local typ = dam.MELEE
		local dam = player.adj_str_td[player.stat(A_STR) + 1]
		
		if player.inventory[INVEN_MAINHAND][1] and player.inventory[INVEN_MAINHAND][1].flags[FLAG_DAM] then
			local dams = player.inventory[INVEN_MAINHAND][1].flags[FLAG_DAM]
			typ = flag_max_key(dams)
			
			-- Randomize the damage
			dam = dam + rng(flag_get(dams, typ), flag_get2(dams, typ))
		end
		
		-- Reduce damage using monster AC
		dam = dam - rng(monst.ac / 3, monst.ac)
		if dam < 0 then dam = 0 end
		-- Ensure it is not completly frustrating
		if dam < 1 and monst.level <= player.lev then dam = 1 end

		-- Handles the combat skill
		dam = dam + get_skill(SKILL_COMBAT)
		
		-- DIE MONSTER DIE !! MUAHAHAHAHAHHA !! .. erm .. sorry
		-- Basically we just call the damage giver function on the monster spot
		-- telling it to do a radius 0 attack of the given type and damage
		-- caused by the player
		project(WHO_PLAYER, 0, y, x, dam, typ, PROJECT_JUMP | PROJECT_HIDE | PROJECT_HIDE_BLAST | PROJECT_STOP | PROJECT_KILL | PROJECT_NO_REFLECT)
	end,

	hooks 	=
	{
	},
})
