--
-- Describe items based on tvals
--

object_desc_tvals=
{
		-- Missiles/ Bows/ Weapons
	[TV_SHOT] = { weapon=true }
	[TV_ARROW] = { weapon=true }

	[TV_MOUNT] = function(obj, desc)
		local m_ptr = obj.flags[FLAG_RIDEABLE]

		if m_ptr then
			desc.base = monster_desc(m_ptr, 136)
		else
			desc.base = "error [mount object has no monster]"
		end
	end

	[TV_SYMBIOTE] = function(obj, desc, mode)
		local m_ptr = obj.flags[FLAG_MONSTER_OBJ]

		if m_ptr then
			local name, pronoun, possessive, special =
				symbiosis.get_symbiote_name(obj)

			if special then
				desc.base = monster_desc(m_ptr, 136) .. " named " .. name
				desc.note = gsub(desc.note, "#named [^%$%^]*", "")
			else
				desc.base = name
			end

			if mode > 0 then
				desc.base = desc.base ..
					" (" .. m_ptr.hp .. "/" .. m_ptr.maxhp .. " HP)"
			end
		else
			desc.base = "error [smybiote object has no monster]"
		end
	end

	[TV_TOTEM] = function(obj, desc)
		local m_ptr = obj.flags[FLAG_MONSTER_OBJ]

		if m_ptr then
			desc.mod = monster_desc(m_ptr, 512 | 256 | 128)
			if m_ptr.flags[FLAG_UNIQUE] then
				desc.mod = book_capitals(desc.mod)
			else
				desc.mod = "a " .. desc.mod
			end
		else
			desc.base = "error [totem object has no monster]"
		end
	end

	-- HACK: copied directly from BOW; should be collapsed somehow
	[TV_SLING] = function(obj, name, mode)
				   if mode == 0 then
					   return
				   end

				   obj = generic_or_specific(obj)

				   local mult = get_flag(obj, FLAG_MULTIPLIER)

				   if obj.flags[FLAG_XTRA_MIGHT] then
					   mult = mult + get_flag(obj, FLAG_XTRA_MIGHT)
				   end

				   name.suffix1 = "(x" .. mult .. ")"
			   end

	-- HACK: copied directly to SLING; should be collapsed somehow
	[TV_BOW] = function(obj, name, mode)
				   if mode == 0 then
					   return
				   end

				   obj = generic_or_specific(obj)

				   local mult = get_flag(obj, FLAG_MULTIPLIER)

				   if obj.flags[FLAG_XTRA_MIGHT] then
					   mult = mult + get_flag(obj, FLAG_XTRA_MIGHT)
				   end

				   name.suffix1 = "(x" .. mult .. ")"
			   end

	[TV_HAFTED] = { weapon=true }
	[TV_POLEARM] = { weapon=true }
	[TV_MSTAFF] = { weapon=true }
	[TV_SWORD] = { weapon=true }
	[TV_KNIFE] = { weapon=true }
	[TV_AXE] = { weapon=true }

		-- Armour
	[TV_BOOTS] = { armor=true }
	[TV_GLOVES] = { armor=true }
	[TV_LIGHT_HELM] = { armor=true }
	[TV_HEAVY_HELM] = { armor=true }
	[TV_SHIELD] = { armor=true }
	[TV_LIGHT_ARMOUR] = { armor=true }
	[TV_HEAVY_ARMOUR] = { armor=true }

	[TV_AMULET] = {}
	--[[
		-- Amulets (including a few "Specials")
	[TV_AMULET] = function(obj, desc)
		if is_artifact(obj) then return end
		-- Setup the modifier
		desc.mod = desc.base

		local flavour, color = flavour.get(TV_AMULET, obj.sval)
		obj.color = color

		if (game_options.plain_descriptions and desc.aware) or (obj.ident & IDENT_STOREB ~= 0) then
			desc.base = "& amulet~ of #"
		else
			desc.base = "& "..flavour.." amulet~" .. iif(desc.aware, " #", "")
		end
	end
	--]]

		-- Rings (including a few "Specials")
	[TV_RING] = function(obj, desc)
		if is_artifact(obj) then return end
		-- Setup the modifier
		desc.mod = desc.base

		local flavour, color = flavour.get(TV_RING, obj.sval)
		obj.color = color

		if (game_options.plain_descriptions and desc.aware) or (obj.ident & IDENT_STOREB ~= 0) then
			desc.base = "& ring~ of #"
		else
			desc.base = "& "..flavour.." ring~" .. iif(desc.aware, " #", "")
		end
	end

	[TV_ORB] = function(obj, desc)
		local spl = sticks.orb.obj_to_orb(obj)
		local flavour, color = flavour.get(TV_ORB, spl.index)
		obj.color = color

		desc.hide_charging = true

		if is_aware(obj) then
			desc.mod = strlower(spell(spl.index).name)

			local gem       = obj.flags[FLAG_MANA_SOURCE_OBJ]
			local charges   = 0
			if gem then
				local cheapness = 1

				if obj.flags[FLAG_CHEAPNESS] then
					cheapness = cheapness * obj.flags[FLAG_CHEAPNESS]
				end
				if gem.flags[FLAG_CHEAPNESS] then
					cheapness = cheapness * gem.flags[FLAG_CHEAPNESS]
				end

				sticks.guss_stick_mode(spl.use, spl.max)
				local mana = get_mana(spl.index) / cheapness
				sticks.guss_stick_mode()
				if mana < 1 then
					mana = 1
				end
				charges = (obj_mana.get_curr(gem) or 0) / mana
			end

			if not gem then
				desc.mod = desc.mod .. " (no gem)"
			elseif charges == 0 then
				desc.mod = desc.mod .. " ("..object_desc(gem, 0, 0)..",empty)"
			else
				desc.mod = desc.mod .. " ("..object_desc(gem, 0, 0)..","..charges..")"
			end
		else
			desc.base = "& "..flavour.." orb~"
		end
	end

	[TV_WAND] = function(obj, desc)
		local spell = sticks.wand.obj_to_thaum(obj)
		local flavour, color = flavour.get(TV_WAND,
										   obj.flags[FLAG_FLAVOUR_ID])

		desc.hide_charging = true

		if is_aware(obj) then
			desc.mod = strlower(thaum_spell_name(spell))

			local gem       = obj.flags[FLAG_MANA_SOURCE_OBJ]
			local charges   = 0

			if gem then
				local cheapness = 1

				if obj.flags[FLAG_CHEAPNESS] then
					cheapness = cheapness * obj.flags[FLAG_CHEAPNESS]
				end
				if gem.flags[FLAG_CHEAPNESS] then
					cheapness = cheapness * gem.flags[FLAG_CHEAPNESS]
				end

				local mana = spell.mana / cheapness

				if mana < 1 then
					mana = 1
				end

				charges = (obj_mana.get_curr(gem) or 0) / mana
			end

			if not gem then
				desc.mod = desc.mod .. " (no gem)"
			elseif charges == 0 then
				desc.mod = desc.mod .. " ("..object_desc(gem, 0, 0)..",empty)"
			else
				desc.mod = desc.mod .. " ("..object_desc(gem, 0, 0)..","..charges..")"
			end
		else
			desc.base = "& "..flavour.." wand~"
		end
	end

	[TV_SCROLL] = function(obj, desc)
		-- Setup the modifier
		desc.mod = desc.base

		local flavour, color = flavour.get(TV_SCROLL, obj.sval)
		obj.color = color

		if (game_options.plain_descriptions and desc.aware) or (obj.ident & IDENT_STOREB ~= 0) then
			desc.base = "& scroll~ of #"
		else
			desc.base = '& scroll~ titled "' .. flavour .. '"' .. iif(desc.aware, " #", "")
		end
	end

	[TV_POTION] = function(obj, desc)
		-- Setup the modifier
		desc.mod = desc.base

		local flavour, color = flavour.get(TV_POTION, obj.sval)
		obj.color = color

		if (game_options.plain_descriptions and desc.aware) or
			(obj.ident & IDENT_STOREB ~= 0) then
			desc.base = "& potion~ of #"
		else
			desc.base = "& "..flavour.." potion~" .. iif(desc.aware, " #", "")
		end
	end
--[[
	[TV_FOOD] = function(obj, desc)
		{
			-- Ordinary food is "boring"
			if (o_ptr->sval >= SV_FOOD_MIN_FOOD) break;

			-- Color the object
			call_lua("flavour.get", "(s,d)", "sd", "foods", indexx, &modstr, &flavour_color);
			if (desc.aware) append_name = true;
			if (((game_options.plain_descriptions) && (desc.aware)) || o_ptr->ident & IDENT_STOREB)
				basenm = "& Mushroom~";
			else
				basenm = desc.aware ? "& # Mushroom~" : "& # Mushroom~";
			break;
		}
]]

	[TV_MUSHROOM] = function(obj, desc)
		-- Setup the modifier
		desc.mod = desc.base

		local flavour, color = flavour.get(TV_MUSHROOM, obj.sval)
		obj.color = color

		if (game_options.plain_descriptions and desc.aware) or
			(obj.ident & IDENT_STOREB ~= 0) then
			desc.base = "& mushroom~ of #"
		else
			desc.base = "& "..flavour.." mushroom~" ..
				iif(desc.aware, " #", "")
		end
	end

	-- Cloak of Mimicry
	[TV_CLOAK] = function(obj, desc)
			desc.armor = true
--[[			if (o_ptr->sval == SV_MIMIC_CLOAK)
			{
				call_lua("get_mimic_info", "(d,s)", "s", get_flag(o_ptr, FLAG_MIMIC), "obj_name", &modstr);
			}
			break;
]]	end


	[TV_PARCHMENT] = function(obj, desc)
		desc.mod = desc.base
		desc.base = "& parchment~ - #"
	end


	[TV_CORPSE] = function(obj, desc)
		desc.mod = desc.base

		local monst = obj.flags[FLAG_MONSTER_OBJ]
		if not monst then
			desc.mod  = desc.base
			desc.base = "& BUGGY CORPSE #~"
			return
		end

		local name = monster_desc(monst, 512 | 256 | 128)
		if has_flag(monst, FLAG_UNIQUE) then
			name = book_capitals(name) .. "'s"
			desc.no_article = true
		end

		local rotenness = ""
		if obj.flags[FLAG_DECAY] and obj.flags[FLAG_MONSTER_DECAY] and
			obj.flags[FLAG_ORIG_DECAY]
		then
			local freshness = 200 * obj.flags[FLAG_MONSTER_DECAY] /
				obj.flags[FLAG_ORIG_DECAY]

			if freshness > 200 then
				rotenness = "cured "
			elseif freshness < 17 then
				rotenness = "completely rotten "
			elseif freshness < 33 then
				rotenness = "exremely rotten "
			elseif freshness < 50 then
				rotenness = "very rotten "
			elseif freshness < 67 then
				rotenness = "rotten "
			elseif freshness < 83 then
				rotenness = "moderatly rotten "
			elseif freshness < 100 then
				rotenness = "slightly rotten "
			end
		elseif not has_flag(monst, FLAG_UNIQUE) then
			rotenness = "unrotting "
		end

		desc.base = "& ".. rotenness .. name .. " #~"
	end

	[TV_BOOK] = function(obj, desc)
		if obj.sval == SV_BOOK_RANDOM and tag(obj) == TAG_OBJECT then
			desc.mod = strlower(spell(spell_x(obj, 0)).name)
		end
	end

	[TV_LITE] = function(obj, desc)
		if obj.flags[FLAG_FUEL] then
			desc.mod = "(with ".. obj.flags[FLAG_FUEL] .." turns of light)"
		end
	end

	[TV_CHEST] = function(obj, desc, mode)
		desc.mod = desc.base

		mode = mode or 0
		if mode == 0 then
			return
		end

		if tag(obj) ~= TAG_OBJECT then
			return
		end

		local num_traps = get_num_object_traps(obj, true)

		if not has_flag(obj, FLAG_CLOSED) and mode > 0 then
			desc.suffix1 = "(empty)"
		elseif has_flag(obj, FLAG_TRAP_AWARE) and num_traps == 0 then
			desc.suffix1 = "(disarmed)"
		elseif has_flag(obj, FLAG_TRAP_AWARE) and num_traps > 0 then
			local trap_list = obj.flags[FLAG_TRAP]

			if get_num_traps(trap_list, true) == 1 then
				local max_key = flag_max_key(trap_list)
				local trap
				local idx

				for i = 1, max_key do
					if flag_is_known(trap_list, i) then
						trap = trap_list[i]
						break
					end
				end
				idx  = trap[FLAG_TRAP_IDX]

				desc.suffix1 = "(" .. traps.ident_name(idx, trap) .. ")"
			else
				desc.suffix1 = "(multiple traps)"
			end
		end
	end -- TV_CHEST

	[TV_TRAPKIT] =
		function(obj, desc, mode)
			mode = mode or 0

			if mode == 0 then
				return
			end

			local suffix = ""

			desc.mod = desc.base
			obj      = generic_or_specific(obj)

			local hiddenness = obj.flags[FLAG_HIDDENNESS] or 0
			local difficulty = obj.flags[FLAG_DIFFICULTY] or 0

			if hiddenness ~= 0 or difficulty ~= 0 then
				local tmp = format(" [%s, %s]",
								   signed_number_tostring(hiddenness),
								   signed_number_tostring(difficulty))
				suffix = suffix .. tmp
			end

			if obj.flags[FLAG_MULTIPLIER] then
				local mult = obj.flags[FLAG_MULTIPLIER]

				if obj.flags[FLAG_XTRA_MIGHT] then
					mult = mult + obj.flags[FLAG_XTRA_MIGHT]
				end

				suffix = suffix .. " (x" .. mult .. ")"
			end

			desc.suffix1 = clean_whitespace(suffix)
		end -- TV_TRAPKIT

	[TV_RUNE1] =
		function(obj, desc)
			desc.mod = desc.base
			desc.base = "& rune~ [#]"
		end

	[TV_RUNE2] =
		function(obj, desc, mod)
			if has_flag(obj, FLAG_RUNE_STONE) then
				if has_flag(obj, FLAG_RUNE_DAM) then
					local damname =
						book_capitals(get_dam_type_info(get_flag(obj,
																 FLAG_RUNE_DAM),
														"desc"))
					local shape = ""
					for k, v in runes.shapes do
						if get_flag(obj, k) > 0 then
							shape=shape..","..book_capitals(strlower(v.name))
						end
					end
					shape = strsub(shape, 2) -- chop off initial ","
					local mana = get_flag(obj, FLAG_RUNE_STONE)
					desc.base = "& runestone~ #"
					desc.mod = "("..mana..")["..damname.."]<"..shape..">"
				else
					desc.base = "& runestone~"
				end
			else
				desc.mod = desc.base
				desc.base = "& rune~ [#]"
			end
		end -- TV_RUNE2

	[TV_TOOL] =
		function(obj, desc, mode)
			mode = mode or 0

			if mode == 0 then
				return
			end

			local suffix = ""

			desc.mod = desc.base
			obj      = generic_or_specific(obj)

			if known_flag(obj, FLAG_DIG_POWER) then
				local tmp = {total = 0, num = 0}
				foreach_flags(obj.flags[FLAG_DIG_POWER],
					function(flags, key)
						if flag_is_known(flags, key) then
							%tmp.total = %tmp.total + flags[key]
							%tmp.num   = %tmp.num + 1
						end
					end)
				if tmp.num > 0 then
					local avg = tmp.total / tmp.num
					suffix = suffix .. " (+" .. avg .. ")"
				end
			end -- if known_flag(obj, FLAG_DIG_POWER) then

			desc.suffix1 = clean_whitespace(suffix)
		end -- TV_TOOL

}

--
-- @fn
-- @brief Describe an item in defails(Greater ID)
--
function object_desc_details(obj, trim_down)
	-- Setup
	desc_obj.cur_obj = obj

	-- The fact that its an Eternity artifact should be listed first.
	desc_obj.bool_flag(FLAG_ULTIMATE, "#vIt is one of the ultimate artifacts.")

	-- Sentient stuff is in /scripts/sentient.lua
	desc_obj.desc_sentient(obj, trim_down)

	-- Auras are in /scripts/combat.lua
	desc_obj.desc_auras(obj, trim_down)

	-- Weapons description broken off into subroutine
	desc_obj.desc_weapon(obj, trim_down)

	-- Wands
	if known_flag(obj, FLAG_THAUM_SPELL) then
		local spell = sticks.wand.obj_to_thaum(obj)
		desc_obj.text_out("It contains a thaumaturgy spell: ")
		thaum_longinfo(nil, spell)
		desc_obj.text_out("  Your fail rate with it is "..
						  sticks.wand.thaum_spell_chance(obj, spell)..
							  "%.\n")
		desc_obj.text_out("\n")
	end

	-- Staves
	if known_flag(obj, FLAG_ORB_SPELL) then
		local spl = sticks.orb.obj_to_orb(obj)
		sticks.guss_stick_mode(spl.use, spl.max)
		set_current_spell_info{index = spl.index, faction = FACTION_PLAYER}

		desc_obj.text_out("It contains a school spell: "..spell(spl.index).name)
		print_device_desc(spl.index)
		desc_obj.text_out("\nYour fail rate with it is #B"..spell_chance(spl.index).."%#w.")
		desc_obj.text_out("\nSpell level: #B"..get_level(spl.index, 50, 0));
		desc_obj.text_out("\nMinimum Magic Device level to increase spell level: #B"..spell(spl.index).skill_level);
		desc_obj.text_out("\nSpell info: #y"..__spell_info[spl.index]());

		desc_obj.text_out("\n")

		unset_current_spell_info()
		sticks.guss_stick_mode()
	end

	-- Equipment slots
	if known_flag(obj, FLAG_EQUIPMENT_SIZE) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_EQUIPMENT_SIZE],
			function(flags, f)
				%tbl[{FLAG_EQUIPMENT_SIZE,f}] = inventory_slot_names[f + 1].desc.." size"
			end)
		desc_obj.value_group(tbl, "It modifies your ", nil, nil, {color = " slot(s)"})
	end

	-- Trapkits
	desc_obj.value_flag(FLAG_AMMO_PER_TRIGGER,
						"It fires $V extra shots per activation.")

	-- Speeds
	desc_obj.value_flag(FLAG_SPEED, "It modifies your general speed by $V.")
	if known_flag(obj, FLAG_SPEEDS) then
		foreach_flags(obj.flags[FLAG_SPEEDS],
			function(flags, f)
				desc_obj.value_flag({FLAG_SPEEDS,f},
									"It modifies your "..get_speed_desc(f)..
										" speed by $V.")
			end
		)
	end

	-- Percent modifiers
	desc_obj.value_group({
		[FLAG_MANA] = "mana"
		[FLAG_LIFE] = "life"
		}, "It modifies ", nil, nil, {color = "%"})

	-- Life and mana drains
	if known_flag(obj, FLAG_DRAIN_MANA) then
		local amnt = obj.flags[FLAG_DRAIN_MANA]
		local str =
			format("It drains mana at a rate of #R%d.%03d SP/turn#w. ",
				   amnt / 1000, imod(amnt, 1000))

		desc_obj.text_out(str)
	end
	if known_flag(obj, FLAG_DRAIN_LIFE) then
		local amnt = obj.flags[FLAG_DRAIN_LIFE]
		local str =
			format("It drains life at a rate of #R%d.%03d HP/turn#w. ",
				   amnt / 1000, imod(amnt, 1000))

		desc_obj.text_out(str)
	end

	-- Non-percent (absoulte) modifiers
	desc_obj.value_group({
		[{FLAG_STATS, A_STR}] = "strength",
		[{FLAG_STATS, A_INT}] = "intelligence"
		[{FLAG_STATS, A_WIS}] = "wisdom"
		[{FLAG_STATS, A_CON}] = "constitution"
		[{FLAG_STATS, A_DEX}] = "dexterity"
		[{FLAG_STATS, A_CHR}] = "charisma"
		[FLAG_LUCK] = "luck"
		[FLAG_STEALTH] = "stealth"
		[FLAG_SEARCH_POWER] = "searching"
		[FLAG_SEARCH_POWER_FEAT] = "secret door searching"
		[FLAG_SEARCH_POWER_TRAP] = "trap searching"
		[FLAG_SEARCH_FREQ]  = "perception"
		[FLAG_INFRA] = "infravision"
		[FLAG_BLOWS] = "melee attack number(per turn)"
		[FLAG_CRIT] = "ability to score critical hits"
		[FLAG_SPELL] = "spell power"
		}, "It modifies ")

	-- Resistances
	if known_flag(obj, FLAG_RESIST) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_RESIST],
			function(flags, f)
				if flags[f] ~= 0 then
					%tbl[{FLAG_RESIST,f}] = get_dam_color_desc(f)
				end
			end)
		desc_obj.value_group(tbl, "It provides resistance ", "to ",
							 nil, {color = "%"})
	end

	-- Absorbs
	if known_flag(obj, FLAG_ABSORB) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_ABSORB],
			function(flags, f)
				%tbl[{FLAG_ABSORB,f}] = get_dam_color_desc(f)
			end)
		desc_obj.value_group(tbl, "It absorbs damage ", "from ")
	end

	-- Protect from
	if known_flag(obj, FLAG_PROTECT_FROM) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_PROTECT_FROM],
			function(flags, f)
				%tbl[{FLAG_PROTECT_FROM,f}] = get_dam_color_desc(f)
			end)
		desc_obj.bool_group(tbl, "It protects your inventory ", "from ")
	end

	-- Various resists-like stuff
	desc_obj.bool_flag(FLAG_FREE_ACT, "It gives your immunity to paralysis.")
	desc_obj.bool_flag(FLAG_HOLD_LIFE, "It prevents life draining attacks.")
	desc_obj.bool_flag(FLAG_WATER_BREATH, "It allows you to breathe underwater.")
	desc_obj.bool_flag(FLAG_MAGIC_BREATH, "It allows you to breathe without air.")

	-- Pass through stuff
	desc_obj.value_flag(FLAG_FLY, "It allows you to fly(power $V).")
	if known_flag(obj, FLAG_PASS_WALL) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_PASS_WALL],
			function(flags, f)
				%tbl[{FLAG_PASS_WALL,f}] = memory.descs[FLAG_PASS_WALL][f]
			end)
		desc_obj.value_group(tbl, "It allows you to ", "")
	end

	-- Object's own digging power
	if known_flag(obj, FLAG_DIG_POWER) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_DIG_POWER],
			function(flags, f)
				%tbl[{FLAG_DIG_POWER,f}] = tunnel.dig_power_desc[f]
			end)
		desc_obj.value_group(tbl, "It can be used ", "to ")
	end

	-- Digging power which will help (or hinder) any digger the
	-- player uses.
	if known_flag(obj, FLAG_ADD_DIG_POWER) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_ADD_DIG_POWER],
			function(flags, f)
				%tbl[{FLAG_ADD_DIG_POWER,f}] = subst_desc[f]
			end)
		desc_obj.value_group(tbl, "It modifes your ability to destroy ", "")
	end

	-- ESPs
	if known_flag(obj, FLAG_ESP) then
		if flag_is_known(obj.flags[FLAG_ESP], FLAG_ESP) then
			desc_obj.text_out("It gives you telepathic powers (radius #G+" ..
							  obj.flags[FLAG_ESP][FLAG_ESP] .. "#w). ")
		end

		local tbl = {}
		foreach_flags(obj.flags[FLAG_ESP],
			function(flags, f)
				if f ~= FLAG_ESP then
					%tbl[{FLAG_ESP,f}] = esp.desc_flag(f)
				end
			end)
		desc_obj.value_group(tbl, "It lets you sense the presence ", "of ",
							 nil, nil, {nocolor = "radius "})
	end

	-- Skill bonuses
	if known_flag(obj, FLAG_SKILL_BONUS) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_SKILL_BONUS],
			function(flags, f)
				%tbl[{FLAG_SKILL_BONUS,f}] =
					s_info[f + 1].name .. " skill"
			end
		)
		desc_obj.value_group_frac(tbl, "It modifes ", 3, "your ")
	end

	-- Sustains
	desc_obj.bool_group({
		[{FLAG_SUST_STATS, A_STR}] = "strength",
		[{FLAG_SUST_STATS, A_INT}] = "intelligence"
		[{FLAG_SUST_STATS, A_WIS}] = "wisdom"
		[{FLAG_SUST_STATS, A_CON}] = "constitution"
		[{FLAG_SUST_STATS, A_DEX}] = "dexterity"
		[{FLAG_SUST_STATS, A_CHR}] = "charisma"
		}, "It sustains your ")

	-- Curses
	if known_flag(obj, FLAG_PERMA_CURSE) then
		desc_obj.text_out("It is permanently cursed.  ")
	elseif known_flag(obj, FLAG_HEAVY_CURSE) then
		desc_obj.text_out("It is heavily cursed.  ")
	elseif (obj.ident & IDENT_CURSED ~= 0) or
		known_flag(obj, FLAG_CURSED) then
		desc_obj.text_out("It is cursed.  ")
	end

	if known_flag(obj, FLAG_AUTO_CURSE) then
		desc_obj.text_out("It can re-curse itself.  ")
	end

	if known_flag(obj, FLAG_CURSE_NO_DROP) then
		desc_obj.text_out("It cannot be dropped or destroyed while cursed.  ")
	end

	-- Misc
	if known_flag(obj, FLAG_FUEL_LITE) then
		desc_obj.value_flag(FLAG_LITE,
							"It provides light (radius $V) when fueled.")
	else
		desc_obj.value_flag(FLAG_LITE,
							"It provides light (radius $V) forever.")
	end

	if known_flag(obj, FLAG_SPELL_CONTAIN) then
		if not has_flag(obj, FLAG_SPELL_IDX) then
			desc_obj.text_out("It can be used to store a spell.  ")
		else
			desc_obj.text_out("It has a spell stored inside [#G"..spell(flag_max_key(obj.flags[FLAG_SPELL_IDX])).name.."#w].  ")
		end
	end

	desc_obj.value_flag(FLAG_SPELL_FAILURE, "It modifies your spell failure chances by $V.", true, true)

	if known_flag(obj, FLAG_METAB_PCT) then
		local pct = get_flag(obj, FLAG_METAB_PCT)
		if  pct == 0 then
			desc_obj.text_out("It halts your metabolism.  ")
		elseif  pct < 0 then
			desc_obj.text_out("It reverses your metabolism.  ")
		elseif pct < 100 then
			desc_obj.text_out("It slows your metabolism.  ")
		elseif pct > 100 then
			desc_obj.text_out("It accelerates your metabolism.  ")
		end
	end

	if known_flag(obj, FLAG_NUTRI_MOD) then
		if get_flag(obj, FLAG_NUTRI_MOD) > 0 then
			desc_obj.text_out("It supplies nutrition.  ")
		else
			desc_obj.text_out("It consumes nutrition.  ")
		end
	end

	desc_obj.value_flag(FLAG_SEE_INVIS, "It allows you to see invisible monsters(power $V).")

	if known_flag(obj, FLAG_NO_TELE) then
		desc_obj.text_out("It prevents teleportation.  ")
	end

	if known_flag(obj, FLAG_TELEPORT) then
		desc_obj.text_out("It induces random teleportation.  ")
	end

	desc_obj.value_flag(FLAG_REFLECT,
						"It reflects bolts and arrows $V% of the time.")

	-- Describing activations broken off into subroutine
	desc_obj.desc_act(obj, trim_down)

	-- Object can't be harmed by...
	if is_known(obj) and is_artifact(obj) then
		desc_obj.text_out("It is indestructable. ")
	elseif known_flag(obj, FLAG_IGNORE) then
		local tbl = {}
		foreach_flags(obj.flags[FLAG_IGNORE],
			function(flags, f)
				%tbl[{FLAG_IGNORE,f}] = get_dam_color_desc(f)
			end
		)
		desc_obj.bool_group(tbl, "It cannot be harmed by ", false, " or ")
	end

	-- XXX Stuff that needs to be implemented in Lua XXX ---
--[[

		vn = 0
		if (has_flag(obj, FLAG_CHAOTIC))
			text_out("It produces chaotic effects.  ")
		end

		if (has_flag(obj, FLAG_IMPACT))
			text_out("It can cause earthquakes.  ")
		end

		if (has_flag(obj, FLAG_VORPAL))
			text_out("It is very sharp and can cut your foes.  ")
		end

		if (has_flag(obj, FLAG_WOUNDING))
			text_out("It is very sharp and can make your foes bleed.  ")
		end

		if (has_flag(obj, FLAG_INVIS))
			text_out("It makes you invisible.  ")
		end

		if (has_flag(obj, FLAG_WRAITH))
			text_out("It renders you incorporeal.  ")
		end

		if (has_flag(obj, FLAG_REGEN))
			text_out("It speeds your regenerative powers.  ")
		end
		if (has_flag(obj, FLAG_SH_ELEC))
			text_out("It produces an electric sheath.  ")
		end
		if (has_flag(obj, FLAG_NO_MAGIC))
			text_out("It produces an anti-magic shell.  ")
		end

		vn = 0
		if (has_flag(obj, FLAG_DRAIN_MANA))
			vc[vn] = TERM_BLUE
			vp[vn++] = "mana"
		end
		if (has_flag(obj, FLAG_DRAIN_LIFE))
			vc[vn] = TERM_RED
			vp[vn++] = "life"
		end
		if (has_flag(obj, FLAG_DRAIN_EXP))
			vc[vn] = TERM_L_DARK
			vp[vn++] = "experience"
		end
		-- Describe
		if (vn)
			int i

			-- Intro
			text_out("It ")

			-- List
			for (i = 0 i < vn i++)
				-- Connectives
				if (i == 0) text_out("drains ")
				elseif (i < (vn - 1)) text_out(", ")
				else text_out(" and ")

				-- Dump the stat
				text_out_c(vc[i], vp[i])
			end
			text_out(".  ")
		end

		if (has_flag(obj, FLAG_BLESSED))
			text_out("It has been blessed by the gods.  ")
		end
		if (has_flag(obj, FLAG_AUTO_ID))
			text_out("It identifies all items for you.  ")
		end

		if (has_flag(obj, FLAG_IMMOVABLE))
			text_out("It turns movement into semi-controlled pahse door.  ")
		end
		if (has_flag(obj, FLAG_CANNOT_MOVE))
			text_out("It forbids any movements.  ")
		end

		if (has_flag(obj, FLAG_AGGRAVATE))
			text_out("It aggravates nearby creatures.  ")
		end
		if (has_flag(obj, FLAG_NEVER_BLOW))
			text_out("It can't attack.  ")
		end
		if (has_flag(obj, FLAG_BLACK_BREATH))
			text_out("It fills you with the Black Breath.  ")
		end
		if (has_flag(obj, FLAG_FOUL_CURSE))
			text_out("It carries an ancient foul curse.  ")
		end

		if (has_flag(obj, FLAG_MORG_CURSE))
			text_out("It carries an ancient morgothian curse.  ")
		end
		if (has_flag(obj, FLAG_CLONE))
			text_out("It can clone monsters.  ")
		end
		if (has_flag(obj, FLAG_CURSE_NO_DROP))
			text_out("It cannot be dropped while cursed.  ")
		end
		if (has_flag(obj, FLAG_AUTO_CURSE))
			text_out("It can re-curse itself.  ")
		end

		if (has_flag(obj, FLAG_CAPACITY))
			text_out("It can hold more mana.  ")
		end
		if (has_flag(obj, FLAG_CHEAPNESS))
			text_out("It can cast spells for a lesser mana cost.  ")
		end
		if (has_flag(obj, FLAG_FAST_CAST))
			text_out("It can cast spells faster.  ")
		end
		if (has_flag(obj, FLAG_CHARGING))
			text_out("It regenerates its mana faster.  ")
		end

		if (has_flag(obj, FLAG_RES_MORGUL))
			text_out("It can resist being shattered by morgul beings.  ")
		end
		if (has_flag(obj, FLAG_DAM))
			s16b dam = get_flag(obj, FLAG_DAM)
			cptr name

			call_lua("get_dam_type_info", "(d,s)", "s", dam, "desc", &name)
			text_out(format("It does %s damage when used as a weapon.  ", name))
		end


		-- Mega-Hack -- describe activation
		if (has_flag(obj, FLAG_ACTIVATE))
			text_out("It can be activated for ")
			if (is_ego_p(obj, EGO_MSTAFF_SPELL))
				text_out(item_activation(obj, 0))
				text_out(" and ")
				text_out(item_activation(obj, 1))
			end
			else

				cptr r = item_activation(obj, 0)
				text_out(r)
			end

			-- Mega-hack -- get rid of useless line for randarts
			if (obj.tval != TV_RANDART)
				text_out(" if it is being worn. ")
			else
				text_out(".  ")
		end
		-- Granted power
		if (object_power(obj) != -1)
			text_out("It grants you the power of ")
			text_out(powers_type[object_power(obj)].name)
			text_out(" if it is being worn.  ")
		end
	end

	process_hooks(HOOK_OBJECT_DESC, "(O,d,d)", obj, (fff != NULL) ? true : FALSE, trim_down)

	if (!trim_down and !fff)
		describe_device(obj)

		if (object_known_p(obj))
			-- Damage display for weapons
			if (wield_slot(obj) == INVEN_MAINHAND)
				display_weapon_damage(obj)

combat.archery.breakage_chance
			-- Breakage/Damage display for ammo
			if (wield_slot(obj) == INVEN_AMMO)
				if (artifact_p(obj))
					text_out("\nIt can never be broken.")
				-- Exclude exploding arrows
				elseif (!has_flag(obj, FLAG_EXPLODE))
					text_out("\nIt has ")
					text_out_c(TERM_L_RED, format("%d", breakage_chance(obj)))
					text_out("% chance to break upon hit.")
				end
				display_ammo_damage(obj)
			end
		end

		if (!object_known_p(obj))
			text_out("\nYou might need to identify the item to know some more about it...")
		elseif (!(obj.ident & (IDENT_MENTAL)))
			text_out("\nYou might need to *identify* the item to know more about it...")
	end

	-- Copying how others seem to do it. -- neil
	if (obj.tval == TV_RING || obj.tval == TV_AMULET ||
	                !trim_down || (ego_item_p(obj)) || (artifact_p(obj)))
		-- Where did we found it ?
		if (obj.found == OBJ_FOUND_MONSTER)
			char m_name[80]

			monster_race_desc(m_name, obj.found_aux1, obj.found_aux2)
			text_out(format("\nYou found it in the remains of %s %s.",
			                m_name, object_out_desc_where_found(obj.found_aux4, obj.found_aux3)))
		elseif (obj.found == OBJ_FOUND_FLOOR)
			text_out(format("\nYou found it lying on the ground %s.",
			                object_out_desc_where_found(obj.found_aux2, obj.found_aux1)))
		elseif (obj.found == OBJ_FOUND_VAULT)
			text_out(format("\nYou found it lying in a vault %s.",
			                object_out_desc_where_found(obj.found_aux2, obj.found_aux1)))
		elseif (obj.found == OBJ_FOUND_SPECIAL)
			text_out("\nYou found it lying on the floor of a special level.")
		elseif (obj.found == OBJ_FOUND_RUBBLE)
			text_out("\nYou found it while digging a rubble.")
		elseif (obj.found == OBJ_FOUND_REWARD)
			text_out("\nIt was given to you as a reward.")
		elseif (obj.found == OBJ_FOUND_STORE)
			text_out(format("\nYou bought it from the %s.",
			                st_info[obj.found_aux1].name ))
		elseif (obj.found == OBJ_FOUND_STOLEN)
			text_out(format("\nYou stole it from the %s.",
			                st_info[obj.found_aux1].name ))
		elseif (obj.found == OBJ_FOUND_SELFMADE)
			text_out("\nYou made it yourself.")
		end
	end
]]
end

--- @fn desc_obj.desc_weapon(obj, trim_down)
-- Give greater details for weapon flags
--
function desc_obj.desc_weapon(obj, trim_down)
	desc_obj.bool_flag(FLAG_COULD2H, "It can be wielded two-handed.")
	desc_obj.bool_flag(FLAG_MUST2H, "It must be wielded two-handed.")

	desc_obj.value_flag(FLAG_ANTIMAGIC, "It generates an antimagic field(strength $V).")

	desc_obj.value_flag(FLAG_XTRA_SHOTS, "It allows you to fire $V extra shots per round.")

	desc_obj.bool_flag(FLAG_XTRA_MIGHT, "It fires missiles with extra might.")

	desc_obj.bool_flag(FLAG_VAMPIRIC, "It can drain life from your foes.")

	-- Breakage/Damage display for ammo
	if obj.flags[FLAG_WIELD_SLOT] == INVEN_AMMO then
		if is_artifact(obj) then
			desc_obj.text_out("It can never be broken.")
		elseif not has_flag(obj, FLAG_EXPLODE) then
			desc_obj.text_out("It has #R " ..
					 combat.archery.breakage_chance(obj) ..
						 "%#w chance to break upon a hit.")
		end
	end

	if known_flag(obj, FLAG_BRAND) then
		local brands = obj.flags[FLAG_BRAND]
		local strs = {}

		for i = 1, flag_max_key(brands) do
			if brands[i] then
				tinsert(strs, get_dam_color_desc(i) .. "(x" ..
						brands[i] .. ")")
			end
		end
		desc_obj.text_out("It does extra damage from ")

		for i = 1, getn(strs) do
			if i > 1 then
				if i == getn(strs) then
					desc_obj.text_out(" and ")
				else
					desc_obj.text_out(", ")
				end
			end
			desc_obj.text_out(strs[i])
			if i == getn(strs) then
				desc_obj.text_out(".")
			end
		end
	end -- if known_flag(obj, FLAG_BRAND) then

	if known_flag(obj, FLAG_SLAY) then
		local slays = obj.flags[FLAG_SLAY]
		local strs  = {}
		for i = 1, flag_max_key(slays) do
			if slays[i] then
				tinsert(strs, "It " .. combat.slays[i].desc .. " (x" ..
						slays[i] .. ").")
			end
		end

		for i = 1, getn(strs) do
			if i > getn(strs) then
				desc_obj.text_out("  ")
			end
			desc_obj.text_out(strs[i])
		end
	end -- if known_flag(obj, FLAG_SLAY)

	if known_flag(obj, FLAG_SPEED_FURY) then
		local slays = obj.flags[FLAG_SPEED_FURY]
		local strs  = {}
		for i = 1, flag_max_key(slays) do
			if slays[i] then
				tinsert(strs, "It strikes at " .. combat.slays[i].target .. " with increased speed (x" ..slays[i].."/"..flag_get2(slays, i).." energy used).")
			end
		end

		for i = 1, getn(strs) do
			if i > getn(strs) then
				desc_obj.text_out("  ")
			end
			desc_obj.text_out(strs[i])
		end
	end -- if known_flag(obj, FLAG_SPEED_FURY)
end

--- @fn desc_obj.desc_act(obj, trim_down)
-- Give greater details for activations
function desc_obj.desc_act(obj, trim_down)
	local num = activations.get_num_activations(obj, true)

	if num == 0 then
		-- No known activations
		return
	elseif num > 1 then
		desc_obj.text_out("It can be activated for multiple things.  ")
		return
	end

	-- Sanity checking
	local source = obj_mana.to_source(obj)

	if not (has_flag(obj,    FLAG_DEVICE_USE) and
			has_flag(source, FLAG_MANA_CURR) and
			has_flag(source, FLAG_MANA_MAX) and
			obj_mana.is_autocharging(obj)) then
		desc_obj.text_out("WARNING: Activatable item doesn't have all the " ..
						  "needed flags.  ")
		return
	end

	-- Just one activation to describe
	local act_list = activations.get_activations(obj, true)

	local spell_idx   = act_list[1].index
	local spell_flags = act_list[1].spell_flags

	local skill = devices.get_skill(obj, spell_idx, WHO_PLAYER)

	if not skill then
		desc_obj.text_out("WARNING: devices.get_skill() failed.  ")
		return
	end

	local fail_pct = devices.get_fail_pct(obj, spell_idx, skill, WHO_PLAYER)

	if not fail_pct then
		desc_obj.text_out("WARNING: devices.get_fail_pct() failed.  ")
		return
	end

	local fail_str = "with a failure rate of " .. fail_pct .. "%.  "

	local act_desc = devices.get_spell_desc(obj, spell_idx, WHO_PLAYER)
	local freq = ""

	if not activations.is_free_use(obj, spell_flags, skill, WHO_PLAYER) and
		obj_mana.is_autocharging(obj, true, obj_mana.auto_charge_types.TIME)
	then
		local rate  = source.flags[FLAG_AUTO_CHARGE_RATE]
		local or_so = false

		if source ~= obj then
			rate = rate + (obj.flags[FLAG_AUTO_CHARGE_RATE] or 0)
		end

		local base = (spell_flags[FLAG_ACT_COST_BASE] or 0)
		local orig_base = base

		base = base / rate
		or_so = (imod(base, rate) ~= 0)

		local dice  = spell_flags[FLAG_ACT_COST_DICE]
		local sides = spell_flags[FLAG_ACT_COST_SIDES]

		local orig_dice, orig_sides = dice, sides

		if dice and sides then
			-- Try to divide rate into part which it will divide
			-- evenly, otherwise divide into sides unless that would
			-- reduces sides to 0
			if (dice / rate) > 0 and imod(dice, rate) == 0 then
				dice = dice / rate
			elseif (sides / rate) > 0 and imod(sides, rate) == 0 then
				sides = sides / rate
			elseif (sides / rate) > 0 then
				sides = sides / rate
				or_so = true
			else
				dice  = dice / rate
				or_so = true
			end

			if dice == 0 or sides == 0 then
				dice  = nil
				sides = nil
				or_so = true
			end
		end

		if base == 0 and not (dice and sides) then
			freq = ""
		elseif base == 1 and not (dice and sides) then
			freq = " once every turn"
		elseif base > 0 and not (dice and sides) then
			freq = " every " .. base .. " turns"
		elseif orig_dice == 1 and (dice and sides) then
			local low  = base + 1
			local high = base + sides

			freq = " every " .. low .. " to " .. high .. " turns"
		elseif base == 0 and (dice and sides) then
			freq = " every " .. dice .. "d" .. sides .. " turns"
		elseif base > 0 and (dice and sides) then
			freq = "every " .. base .. " + " .. dice .. "d" .. sides ..
				" turns"
		elseif base > 1 then
			freq = " every " .. base .. " turns"
		else
			freq = ""
		end

		if or_so then
			if freq == "" then
				freq = " once every turn or so"
			else
				freq = freq .. " or so"
			end
		end
	end  -- Contruct frequency string

	desc_obj.text_out("It can be activated" .. freq .. " " .. act_desc ..
					  " " .. fail_str)
end
