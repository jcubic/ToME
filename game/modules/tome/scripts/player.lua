--
-- Player informations
--

-- Front-loaded skill points
hook(hook.INIT_DONE, function(newgame)
	if newgame then
		player.skill_points = 12
	end
end)

-- Add searching
load_subsystem("search", {})
search.add_default_type("traps")
search.add_default_type("terrain")

-- Define how to stop resting
load_subsystem("resting",
{
	stop_check = function()
		-- Can't rest if impaired in ways that make resting
		-- dangerous
		if player.has_intrinsic(FLAG_FEAR) or
			player.has_intrinsic(FLAG_BLIND) or
			player.has_intrinsic(FLAG_CONFUSED)
		then
			return true
		end

		-- Continue resting if HP is less then max and resting will
		-- regenerate it
		if player.chp() < player.mhp() and player.hp_regen_rate() > 0 then
			return false
		end

		-- Continue resting if mana is less then max and resting will
		-- regenerate it
		if player.csp() < player.msp() and player.mana_regen_rate() > 0 then
			return false
		end

		-- Continue resting if we're about to be recalled
		if player.word_recall > 0 then
			return false
		end

		-- Otherwise, no need to rest
		return true
	end
})

function player.get_sex()
	return player.get_descriptor("sex")
end

function player.get_race()
	return player.get_descriptor("subrace")
end

function player.get_race_and_background()
	local race = player.get_descriptor("subrace")
	local bg = player.get_descriptor("race_background")
	local place = bg.flags[FLAG_PLACE]
	if not place or place == birth.place.AFTER then
		return race.title.." "..bg.title
	elseif place == birth.place.BEFORE then
		return bg.title.." "..race.title
	else
		return race.title
	end
end

function player.get_class()
	return player.get_descriptor("subclass")
end

--- @fn
-- @brief Gains exp from a monster death
-- Redfined from the default one
function player.gain_exp_from_monster(monst)
	-- Maximum player level
	local div = max(player.max_plv, 2)

	if is_friend(monst) <= 0 then
		local race = race_info(monst)

		-- Give some experience for the kill
		local new_exp = 2 * (race.mexp * monst.level) / div

		-- Handle fractional experience
		local new_exp_frac = (imod(2 * (race.mexp * monst.level), div) * 65536 / div) + player.exp_frac

		-- Keep track of experience
		if new_exp_frac >= 65536 then
			new_exp = new_exp + 1
			player.exp_frac = new_exp_frac - 65536
		else
			player.exp_frac = new_exp_frac
		end

		return new_exp
	end

	return 0
end

-- Just return an array of lines.  The engine will either put it up on
-- the terminal or save it to a file
hook.new_hook_type("SELF_KNOWLEDGE")

function player.self_knowledge()
	local lines    = {}

	tinsert(lines, "Your attributes are:")

	-- Report luck
	local luck_str
	local luck_val = abs(luck(-100, 100))

	if luck_val == 0 then
		luck_str = "You have normal luck."
	elseif luck_val > 90 then
		luck_str = "You are incredibly "
	elseif luck_val > 60 then
		luck_str = "You are extremely "
	elseif luck_val > 30 then
		luck_str = "You are very "
	else
		luck_str = "You are "
	end

	if luck_val ~= 0 then
		if luck(-100, 100) > 0 then
			luck_str = luck_str .. "lucky."
		else
			luck_str = luck_str .. "unlucky."
		end
	end

	if wizard then
		luck_str = luck_str .. " (" .. luck(-33, 33) .. ")"
	end

	tinsert(lines, luck_str)

	-- Report resistances
	local resists = player.resists_list()

	for i = 1, getn(resists) do
		local dam_idx    = resists[i]
		local dam_name   = get_dam_type_info(dam_idx, "desc")
		local dam_resist = player.resist(dam_idx)
		local resist_str

		if dam_resist == 0 then
			resist_str = nil
		elseif dam_resist < -99 then
			resist_str = "horribly vulnerable to "
		elseif dam_resist < -88 then
			resist_str = "incredibly vulnerable to "
		elseif dam_resist < -77 then
			resist_str = "extremely vulnerable to "
		elseif dam_resist < -66 then
			resist_str = "very vulnerable to "
		elseif dam_resist < -55 then
			resist_str = "rather vulnerable to "
		elseif dam_resist < -44 then
			resist_str = "vulnerable to "
		elseif dam_resist < -33 then
			resist_str = "moderately vulnderable to "
		elseif dam_resist < -22 then
			resist_str = "somewhat vulnerable to "
		elseif dam_resist < -11 then
			resist_str = "a little bit vulnerable to "
		elseif dam_resist < 0 then
			resist_str = "a tiny bit vulnerable to "
		elseif dam_resist > 99 then
			resist_str = "completely immune to "
		elseif dam_resist >= 90 then
			resist_str = "almost completely immune to "
		elseif dam_resist >= 80 then
			resist_str = "incredibly resistant to "
		elseif dam_resist >= 70 then
			resist_str = "extremely resistant to "
		elseif dam_resist >= 60 then
			resist_str = "very resistant to "
		elseif dam_resist >= 50 then
			resist_str = "rather resistant to "
		elseif dam_resist >= 40 then
			resist_str = "resistant to "
		elseif dam_resist >= 30 then
			resist_str = "moderatly resistant to "
		elseif dam_resist >= 20 then
			resist_str = "somewhat resistant to "
		elseif dam_resist >= 10 then
			resist_str = "a little resistant to "
		elseif dam_resist > 0 then
			resist_str = "a tiny bit resistant to "
		else
			error("Resistance value '" .. dam_resist ..
			      "' not covered!!")
		end

		if resist_str then
			resist_str = "You are " .. resist_str ..
				dam_name .. "."
			if wizard then
				resist_str = resist_str .. " (" ..
					dam_resist .. "%)"
			end
			tinsert(lines, resist_str)
		end
	end -- end reporting resistances

	-- Report sustained stats
	for i = 1, stats.MAX do
		if player.intrinsic_flags[FLAG_SUST_STATS][i] then
			local stat = strlower(player.get_stat_info(i, "name"))

			tinsert(lines, "Your " .. stat .. " is sustained.")
		end
	end

	-- Report ESPs
	if player.has_intrinsic(FLAG_ESP) then
		local esp_flags = player.intrinsic_flags[FLAG_ESP]

		if esp_flags[FLAG_ESP] then
			local str = "You have telepathic powers."
			if wizard then
				str = str .. " (radius " .. esp_flags[FLAG_ESP] .. ")"
			end
			tinsert(lines, str)
		end

		for i = 1, flag_max_key(esp_flags) do
			if esp_flags[i] and i ~= FLAG_ESP then
				local str = "You can sense " .. esp.desc_flag(i) .. "."
				if wizard then
					str = str .. " (radius " .. esp_flags[i] .. ")"
				end
				tinsert(lines, str)
			end
		end
	end

	-- Report on anti-magic field
	if player.antimagic_dis > 0 and player.antimagic > 0 then
		local str = "You are surrounded by an anti-magic field."

		if wizard then
			str = str .. " (radius " .. player.antimagic_dis .. ","
			tinsert(lines, str)

			str = strrep(" ", 44) .. "chance " .. player.antimagic .. "%)"
		end

		tinsert(lines, str)
	end

	-- Report metabolism altering things
	if player.intrinsic_flags[FLAG_METAB_PCT] then
		local pct = player.intrinsic(FLAG_METAB_PCT)
		local str

		if pct == 0 then
			str = "Your metabolism is halted."
		elseif pct < 0 then
			str = "Your metabolism is reversed."
		elseif pct < 100 then
			str = "Your metabolism is slowed."
		elseif pct > 100 then
			str = "Your metabolism is accelerated."
		end

		if str then
			tinsert(lines, str)
		end
	end

	if player.has_intrinsic(FLAG_NUTRI_MOD) then
		local mod = player.intrinsic(FLAG_NUTRI_MOD)
		local str

		if mod > 0 then
			str = "You are supplied with nutrition."
		elseif mod < 0 then
			str = "You are drained of nutrition."
		end

		if str then
			tinsert(lines, str)
		end
	end

	if player.intrinsic(FLAG_FOOD_VALUE) < 1 then
		local val = player.intrinsic(FLAG_FOOD_VALUE)
		local str

		if val == 0 then
			str = "You do not grow hungry over time."
		else
			str = "You become more full over time."
		end

		tinsert(lines, str)
	end

	-- Report on blow responses (passive attacks)
	if player.has_intrinsic(FLAG_BLOW_RESPONSE) then
		local responses = player.intrinsic_flags[FLAG_BLOW_RESPONSE]

		for i = 1, flag_max_key(responses) do
			if responses[i] then
				local response = get_blow_response(i)

				if response.self_know then
					if tag(response.self_know) == TAG_STRING then
						tinsert(lines, response.self_know)
					elseif tag(response.self_know) == TAG_FUNCTION then
						response.self_know(lines, response)
					end
				end
			end
		end -- for i = 1, flag_max_key(resposnes) do
	end -- if player.has_intrinsic(FLAG_BLOW_RESPONSE) then

	-- Passwall stuff
	if player.has_intrinsic(FLAG_PASS_WALL) then
		local pass_wall = player.intrinsic_flags[FLAG_PASS_WALL]

		local desc = {
			[FLAG_PASS_INCORP] =
				"You can pass through solid obstables.",
			[FLAG_PASS_STONE] =
				"You can pass through stone.",
			[FLAG_PASS_DOOR] =
				"You can pass through doors.",
			[FLAG_PASS_LIQUID] =
				"You can pass through non-watertight obstables.",
			[FLAG_PASS_GASEOUS] =
				"You can pass through non-airtight obstables.",
			[FLAG_PASS_WEB] =
				"You can pass through webs.",
			[FLAG_PASS_ICE] =
				"You can pass through ice.",
			[FLAG_PASS_TREES] =
				"You can pass through trees."
		}

		for i, v in desc do
			if pass_wall[i] then
				local str = v

				if wizard then
					str = str .. " (power " .. pass_wall[i] .. ")"
				end

				tinsert(lines, str)
			end
		end
	end -- Passwall stuff

	-- Race and body suff
	if player.has_intrinsic(FLAG_UNDEAD) then
		tinsert(lines, "You are undead.")
	elseif player.has_intrinsic(FLAG_DEMON) then
		tinsert(lines, "You are demonic.")
	elseif player.has_intrinsic(FLAG_NONLIVING) then
		tinsert(lines, "Your body is nonliving.")
	end

	if player.has_intrinsic(FLAG_CORRUPTED) then
		tinsert(lines, "Your body is corrupted.")
	end

	if player.has_intrinsic(FLAG_COLD_BLOOD) then
		tinsert(lines, "You are cold blooded.")
	end

	if player.has_intrinsic(FLAG_MULTIPLY) then
		tinsert(lines, "You are cold blooded.")
	end

	-- Misc stuff
	if player.has_intrinsic(FLAG_SEE_INVIS) then
		tinsert(lines, "You can see invisible creatures.")
	end

	if player.has_intrinsic(FLAG_BLIND) then
		tinsert(lines, "You cannot see.")
	end

	if player.has_intrinsic(FLAG_CONFUSED) then
		tinsert(lines, "You are confused.")
	end

	if player.has_intrinsic(FLAG_HALLUCINATE) then
		tinsert(lines, "You are hallucinating.")
	end

	if player.has_intrinsic(FLAG_REFLECT) then
		local str = "You reflect arrows and bolts."

		if wizard then
			str = str .. "(" .. player.intrinsic(FLAG_REFLECT) ..
				"% of the time)"
		end
		tinsert(lines, str)
	end

	if player.intrinsic(FLAG_DRAIN_MANA) ~= 0 then
		local str = "You are being drained of mana."

		if wizard then
			local amnt = player.intrinsic(FLAG_DRAIN_MANA)
			str = format("%s (%d.%03d SP/turn)", str, amnt / 1000,
						 imod(amnt, 1000))
		end
		tinsert(lines, str)
	end

	if player.intrinsic(FLAG_DRAIN_LIFE) ~= 0 then
		local str = "You are being drained of life."

		if wizard then
			local amnt = player.intrinsic(FLAG_DRAIN_LIFE)
			str = format("%s (%d.%03d HP/turn)", str, amnt / 1000,
						 imod(amnt, 1000))
		end
		tinsert(lines, str)
	end

	if player.body_monster.flags[FLAG_NO_TAKE_ITEM] then
		tinsert(lines, "You cannot pick up objects.")
	end

	if not player.body_monster.flags[FLAG_OPEN_DOOR] then
		tinsert(lines, "You cannot open or close things, or disarm traps.")
	end

	-- Let worn items, the level, the player's body, and the player's
	-- descriptor add stuff.
	item_hooks.process_all_standard(FLAG_SELF_KNOWLEDGE, lines)

	-- And let still other stuff have its say.
	hook.process(hook.SELF_KNOWLEDGE, lines)

	-- Done!  Now indent all of that info a litte, except for the
	-- "Your attributes" line
	for i = 2, getn(lines) do
		lines[i] = "  " .. lines[i]
	end

	return lines
end

-- Describe a savefile
function player.make_savefile_descriptor()
	local depths
	local d_ptr = dungeon(current_dungeon_idx)
	local level_name = get_dungeon_name()

	if (player.wild_mode) then
		depths = "Wilderness Map"
	elseif (level_name ~= "") and (has_flag(level_flags, FLAG_SAVE_LEVEL) or (has_flag(level_flags, FLAG_SPECIAL) and generate_special_feeling)) then
		depths = level_name
	elseif (has_flag(level_flags, FLAG_SPECIAL) and generate_special_feeling) then
		depths = "Special"
	elseif (dun_level == 0) then
		if wf_info[1 + wild_map(player.wilderness_y, player.wilderness_x).feat].name ~= "" then
			depths = wf_info[1 + wild_map(player.wilderness_y, player.wilderness_x).feat].name
		else
                	depths = "Town/Wild"
		end
        elseif d_ptr.mindepth == d_ptr.maxdepth then
		depths = d_ptr.name
        else
		depths = "Level "..(dun_level - d_ptr.mindepth + 1).." of "..d_ptr.name
	end

	return
	{
		"#GName:     #w"..player_name(),
		"#GGender:   #w"..player.get_sex().title,
		"#GRace:     #w"..player.get_race_and_background(),
                "#GClass:    #w"..player.get_class().title,
		"#GLevel:    #w"..player.lev,
		"#GLocation: #w"..depths,
	}
end

-- Whoops died, lets change music!
hook(hook.PLAYER_DEATH, function() sound.play_music("death", -1) end)

--- @fn
-- @brief Registers the score of a dead character. (can and should be overwriten by modules)
function player.register_score()
	local score = player.exp
	local desc = player_name().." (Level "..player.lev..", Race/Class "..player.get_sex().title.." "..player.get_race_and_background().." "..player.get_class().title..")"
	return score, desc
end

-- Monster spell with caster == WHO_PLAYER is actually cast by the
-- player (perhaps possessing a body), rather as opposed to being
-- cast by a symbiote or mount.
function player.player_body_spell()
	if spell_caster == WHO_PLAYER and
		(not __monst_helper.helper or
		 __monst_helper.helper == player.body_monster)
	then
		return true
	end
	return false
end
