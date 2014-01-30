--
-- Define player movement modes
-- This uses the "movement_mode" subsystem
--

mov_mode.new
{
	name = "Normal"
	desc = {
		"This is the default movement mode, with no specific benefits or penalities."
	}
	update = function()
		player.update = player.update | PU_BONUS
		player.redraw[FLAG_PR_STATE] = true
		player.redraw[FLAG_PR_SPEED] = true
		player.redraw[FLAG_PR_ARMOR] = true
	end
}

mov_mode.new
{
	name = "Sneaking"
	desc = {
		"Walk slowly without making much sound."
	}
	bonus = function()
		-- Affect Skill -- stealth
		player.skill_stl = player.skill_stl + get_skill_scale(SKILL_SNEAK, 20) + get_skill_scale(SKILL_STEALTH, 20)

		player.inc_speed(-15)
	end
	update = function()
		player.update = player.update | PU_BONUS
		player.redraw[FLAG_PR_STATE] = true
		player.redraw[FLAG_PR_SPEED] = true
		player.redraw[FLAG_PR_ARMOR] = true
	end
}

mov_mode.new
{
	name = "Searching"
	desc = {
		"Search for hidden features and traps around you, but walk slowly."
	}
	bonus = function()
		-- Affect Skill -- search ability (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_POWER,
							 (get_skill_scale(SKILL_SNEAK, 20)))

		-- Affect Skill -- search frequency (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_FREQ,
							 get_skill_scale(SKILL_SNEAK, 40) +
								 get_skill_scale(SKILL_DISARMING, 40))

		player.inc_speed(-15)
	end
	update = function()
		player.update = player.update | PU_BONUS
		player.redraw[FLAG_PR_STATE] = true
		player.redraw[FLAG_PR_SPEED] = true
		player.redraw[FLAG_PR_ARMOR] = true
	end
}

mov_mode.new
{
	name = "Running"
	desc = {
		"Run like the wind! This gives you a good movement speed but reduces your"
		"stealth, searching, chance to hit and chance to cast spells somewhat."
	}
	bonus = function()
		local eff = player.stat(A_STR) + player.stat(A_DEX) / 2 - (calc_total_weight() / 100)
		eff = eff / 10
		if eff > 10 then eff = 10 end
		if eff < 3 then eff = 3 end

		-- Affect Skill -- search ability (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_POWER, eff - 23)

		-- Affect Skill -- search frequency (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_FREQ, eff - 63)

		-- Affect Skill -- stealth
		player.skill_stl = player.skill_stl - 23 + eff

		-- Reduce to hit
		player.to_h = player.to_h - 15 + (eff / 2)
		player.dis_to_h = player.dis_to_h - 15 + (eff / 2)

		-- Reduce spellcasting
		player.inc_intrinsic(FLAG_SPELL_FAILURE, 10 - (eff / 3))

		player.inc_speed(eff)
	end
	update = function()
		player.update = player.update | PU_BONUS
		player.redraw[FLAG_PR_STATE] = true
		player.redraw[FLAG_PR_SPEED] = true
		player.redraw[FLAG_PR_ARMOR] = true
	end
}

mov_mode.new
{
	name = "Berserk"
	desc = {
		"Enrage and become a walking weapon of destruction, but reduces your armor"
		"and searching skills"
	}
	allow = function()
		return has_ability(AB_BERSERK_MODE)
	end
	bonus = function()
		local eff = player.stat(A_STR) + player.stat(A_DEX) / 2
		eff = eff / 10
		if eff > 10 then eff = 10 end
		if eff < 3 then eff = 3 end

		-- Affect Skill -- search ability (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_POWER, -23)

		-- Affect Skill -- search frequency (Sneakiness skill)
		player.inc_intrinsic(FLAG_SEARCH_FREQ, -23)

		-- Affect Skill -- stealth
		player.skill_stl = player.skill_stl - 6

		-- Affect armour
		player.to_a = player.to_a - 30 + eff
		player.dis_to_a = player.dis_to_a - 30 + eff

		-- Increase to hit
		player.to_h = player.to_h + 3 + eff
		player.dis_to_h = player.dis_to_h + 3 + eff

		-- Increase to dam
		player.to_d = player.to_d + 2 + eff
		player.dis_to_d = player.dis_to_d + 2 + eff
	end
	update = function()
		player.update = player.update | PU_BONUS
		player.redraw[FLAG_PR_STATE] = true
		player.redraw[FLAG_PR_SPEED] = true
		player.redraw[FLAG_PR_ARMOR] = true
	end
}

hook(hook.DISTURB,
function(major, flush_output)
	if major > 0 then
		if mov_mode.current()== "Searching" then
			mov_mode.__list[player.movement_mode].update()
			player.movement_mode = 1
			mov_mode.__list[1].update()
		end
	end
end)
