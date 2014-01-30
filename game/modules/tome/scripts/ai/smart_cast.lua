-- Casting AI: The choose spells from known tactial situation

ai.tactics =
{
	ATTACK = 1
	HEAL = 2
	ENHANCEMENT = 3
	ANNOY = 4
	SUMMON = 5
	ESCAPE = 6
	MANA = 7
	TACTIC = 8
	NO_LOS = 9
}

-- Determine if there is a space near the player in which
-- a summoned creature can appear
function ai.summon_possible(y1, x1)
	local num_clear = 0

	-- Start at the player's location, and check 2 grids in each dir
	for y = y1 - 2, y1 + 2 do
		for x = x1 - 2, x1 + 2 do
			-- Ignore illegal locations
			-- Only check a circular area
			-- Require empty floor grid in line of sight
			if in_bounds(y, x) and distance(y1, x1, y, x) <= 2 and cave_empty_bold(y, x) and los(y1, x1, y, x) then
				num_clear = num_clear + 1
			end
		end
	end

	return num_clear
end


ai.new
{
	name	= "SMART_CAST"
	state	=
	{
		MIN_RANGE = -1
		BEST_RANGE = -1
	}
	exec	= function(m_idx, monst, state)
		local y, x = ai.target(monst)
		local dist = ai.distance(monst)
		local cur_mana, max_mana = get_flag(monst, FLAG_MANA_CURR), get_flag(monst, FLAG_MANA_MAX)
		local want_heal, want_annoy, want_escape, want_tactic, want_summon, want_mana = 0, 0, 0, 0, 0, 0

		-- Tactical report on current hp, do we need to heal ?
		if monst.hp < monst.maxhp / 8 then want_heal = want_heal + 3
		elseif monst.hp < monst.maxhp / 4 then want_heal = want_heal + 2
		elseif monst.hp < monst.maxhp / 2 then want_heal = want_heal + 1
		elseif monst.hp < (monst.maxhp * 2) / 3 then want_heal = want_heal + rng(0, 1)
		end

		-- Tactical report on escaping, if we are in bad shape, we probably want to
		-- escape if we cant heal
		if want_heal > 0 then want_escape = want_heal - 1 end

		-- Tactical report position, we really do NOT want to stay too close
		if dist < flag_get(state, FLAG_MIN_RANGE) then
			want_tactic = want_tactic + 2
			-- Ok, we need healing and we are too close ?
			-- Maybe it's time to escape ..
			if want_heal > 0 then want_escape = want_escape + 1 end
		-- Tactical report position, we dont want to stay close
		elseif dist < flag_get(state, FLAG_BEST_RANGE) then want_tactic = want_tactic + 1
		end

		-- Tactical report on mana, we need more mana!
		if cur_mana < max_mana / 4 then want_mana = want_mana + 2
		elseif cur_mana < max_mana / 2 then want_mana = want_mana + 1 end

		-- Tactical report on summon, ohhhhh look lots of empty space, it's just ASKING to be filled!
		local spaces = ai.summon_possible(y, x)
		if spaces > 10 then want_summon = 3
		elseif spaces > 3 then want_summon = 2
		elseif spaces > 0 then want_summon = 1 end

		if wizard then
			print(format("Tactical situation: want_heal(%d), " ..
						 "want_annoy(%d), want_escape(%d), " ..
							 "want_tactic(%d), want_summon(%d), " ..
							 "want_mana(%d)",
						 want_heal, want_annoy, want_escape, want_tactic,
						 want_summon, want_mana))
		end

		-- Greatly prefer escape if monster is afraid
		local escort = state[FLAG_PET_ESCORT_DIST]
		local is_pet = escort

		if monst.monfear > 0 then
			want_escape = want_escape + 3
		elseif is_pet then
			-- Pets only escape when afraid
			want_escape = 0

			-- Escorting, non-afraid pets outside of the escort radius
			-- should only use heal and mana tactics, unless their target
			-- monster is right next to them.  If they don't want any of
			-- those, then cast no spell, and the main pet AI will make
			-- them move closer to the player.
			if ai.adjacent(monst, y, x) then
				-- Do like normal if target is right next to us.
			elseif escort > 0 and escort < monst.cdis then
				if want_heal <= 0 and want_mana <= 0 then
					if wizard then
						print("=> Pet outside of escort radius, returning " ..
							  "to player.")
					end
					monst.ai_action = ai.action.NO_CAST
					return
				end

				if wizard then
					print("=> Pet outside of escort radius, only healing " ..
						  "and/or replenishing mana.")
				end

				want_annoy, want_tactic, want_summon = 0, 0, 0
			end
		end -- if is_pet then

		-- Check spells
		local best_spl = -1
		local best_rating = -1
		local nb_spells, nb_no_mana = 0, 0
		for i = 1, monst.spells.size do
			if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
				local spl, rating = monst.spells.node[i].key
				local tactics = __tmp_spells[spl].tactical

				if (not tactics) then
					message("Monster spell '" .. __tmp_spells[spl].name ..
							"' has no tactics")
					tactics = {}
				end

				nb_spells = nb_spells + 1

				ai.cast.use_spell(m_idx, monst, spl, y, x)

				-- Is it usable ?
				local ret = ai.cast.can(not tactics[ai.tactics.NO_LOS])
				if monst.target == -1 and not spell(spl).no_target then
					-- Monster has no target, yet spell requires one,
					-- so skip this spell
				elseif tactics and not ret then
					rating = (tactics[ai.tactics.ATTACK] or 0)
					if want_heal > 0 then rating = rating + (tactics[ai.tactics.HEAL] or 0) * want_heal
					-- Prevent healing when full life
					elseif tactics[ai.tactics.HEAL] then rating = -100000 end
					if want_mana > 0 then rating = rating + (tactics[ai.tactics.MANA] or 0) * want_mana
					-- Prevent manaing when full mana
					elseif tactics[ai.tactics.MANA] then rating = -100000 end
					if want_escape > 0 then rating = rating + (tactics[ai.tactics.ESCAPE] or 0) * want_escape end
					if want_tactic > 0 then rating = rating + (tactics[ai.tactics.TACTIC] or 0) * want_tactic end
					if want_annoy > 0 then rating = rating + (tactics[ai.tactics.ANNOY] or 0) * want_annoy end
					if want_summon > 0 then rating = rating + (tactics[ai.tactics.SUMMON] or 0) * want_summon end
					if rating > 0 then rating = rating + rng(0, 10) end
					if wizard then
						print("Spell "..spell(spl).name.." rating: "..
							  rating.." mana: "..ai.cast._cur.mana)
					end
					if rating > best_rating then best_rating = rating best_spl = spl end
				elseif ai.cast._cur.mana > flag_get(monst.flags, FLAG_MANA_CURR) then
					nb_no_mana = nb_no_mana + 1
				end
			end
		end

		if best_spl > - 1 then
			if wizard then
				print(" => Choosen spell: "..spell(best_spl).name)
			end
			ai.cast.use_spell(m_idx, monst, best_spl, y, x)
			monst.ai_action = ai.cast.cast()
		elseif nb_no_mana == nb_spells then
			if wizard then
				print(" => NO choosen spell due to low mana!")
			end
			monst.ai_action = ai.action.NO_CAST
		else
			if wizard then
				print(" => NO choosen spell!")
			end
			monst.ai_action = ai.action.REST
		end
	end
}
