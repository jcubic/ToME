---------------------------------------------------------------------------
-- Possession (leaving and entering a body)
---------------------------------------------------------------------------

constant("possess", {})

hook(hook.BODY_CHANGE_PRE,
function(old_body, new_body)
	-- Force MONSTER_CHP and MANA_CURR to exist before going into
	-- the body
	if not new_body.flags[FLAG_MANA_CURR] then
		new_body.flags[FLAG_MANA_CURR] = player.csp()
	end

	local hp
	if not new_body.flags[FLAG_MONSTER_CHP] then
		hp = new_body.maxhp
		hp = hp - (rng(hp) / 3)

		new_body.flags[FLAG_MONSTER_CHP] = hp
	end

	if old_body.r_idx == RACE_PLAYER then
		old_body.maxhp = player.mhp()
	end

	-- Save current HP and mana into old body before leaving it
	hp = player.chp() * player.mhp() / max(old_body.maxhp, 1)
	old_body.flags[FLAG_MONSTER_CHP] = hp

	local mana
	if old_body.flags[FLAG_MANA_MAX] then
		mana = old_body.flags[FLAG_MANA_MAX] * player.csp() /
			max(player.msp(), 1)
		old_body.flags[FLAG_MANA_CURR] = min(mana,
											 old_body.flags[FLAG_MANA_MAX])
	else
		old_body.flags[FLAG_MANA_CURR] = player.csp()
	end

	-- Save how hungry we are in the old body before we leave it
	old_body.flags[FLAG_FOOD_VALUE] = timed_effect.get(timed_effect.FOOD)

	-- Previously un-inhabited bodies default to being just short of
	-- full
	if not new_body.flags[FLAG_FOOD_VALUE] then
		new_body.flags[FLAG_FOOD_VALUE] = food.FOOD_FULL - 1
	end
end) -- hook(hook.BODY_CHANGE_PRE)

function possess.recalc_descriptors(new_body)
	-- Ignore body based descriptors if we're not moving into the
	-- player body
	local ignore

	if new_body.r_idx == RACE_PLAYER then
		ignore = {}
	else
		local ignore = {
			race    = true
			subrace = true
			sex     = true

			race_background = true
		}
	end
	birth.compute_final_descriptor(false, ignore)
end

hook(hook.GAME_START,
function()
	possess.recalc_descriptors(player.body_monster)
end)

hook(hook.BODY_CHANGE,
function(old_body, new_body)
	possess.recalc_descriptors(new_body)
end)

hook(hook.BODY_CHANGE_POST,
function(old_body, new_body)
	-- Set current HP and SP from the body's flags
	local hp = player.mhp() * new_body.flags[FLAG_MONSTER_CHP] /
		new_body.maxhp
	player.chp(hp)

	if new_body.flags[FLAG_MANA_MAX] then
		local mana = player.msp() * new_body.flags[FLAG_MANA_CURR] /
			max(new_body.flags[FLAG_MANA_MAX], 1)
		player.csp(mana)
	else
		player.csp(new_body.flags[FLAG_MANA_CURR])
	end

	-- Set current hunger from body
	if new_body.flags[FLAG_FOOD_VALUE] then
		timed_effect.set(timed_effect.FOOD, new_body.flags[FLAG_FOOD_VALUE])
	else
		timed_effect.set(timed_effect.FOOD, food.FOOD_FULL - 1)
	end
end) -- hook(hook.BODY_CHANGE_POST)

hook(hook.CALC_BONUS_BEGIN,
function()
	-- Cancel out body-based class bonuses; race and sex based bonuses
	-- are already gone from the player descriptor by now.
	if player.body_monster.r_idx ~= RACE_PLAYER then
		player.see_infra = player.see_infra - player.descriptor.infra

		for i = 0, stats.MAX do
			-- Keep mind-based class bonuses
			if i ~= A_INT and i ~= A_WIS then
				player.stat_add[i] = player.stat_add[i] -
					player.descriptor.adj[i]
			end
		end
	end -- if new_body.r_idx ~= RACE_PLAYER then
end) -- hook(hook.CALC_BONUS_BEGIN)

-- Addons can add to this if they have mkeys which should be hidden
-- while disembodied.
possess.mkey_hide_by_name = {
	["Alchemy"]             = true,
	["Ammo creation"]       = true,
	["Bear-form"]           = true,
	["Boulder-throwing"]    = true,
	["Far reaching attack"] = true,
	["Geomancy"]            = true,
	["Magic"]               = true,
	["Magic-device"]        = true,
	["Mimicry"]             = true,
	["Runecraft"]           = true,
	["Touch of death"]      = true,
	["Trapping"]            = true,
	["Stealing"]            = true,
	["Summoning"]           = true,
	["Symbiosis"]           = true,
}

possess.mkey_hide_by_val = {
	[riding.MKEY] = true,
}

hook(hook.GAME_START,
function()
	possess.mkey_hide_by_val[easy_cast.MKEY_CAST] = true
end)

-- There are a lot of things you can't do while disembodied.
hook(hook.MKEY_HIDE,
function(power, type)
	if player.body_monster.r_idx ~= RACE_DISEMBODIED then
		return
	end

	if possess.mkey_hide_by_val[power] then
		return true
	end

	local name = __mkey_fct.__skills[power] or __mkey_fct.__abilities[power]

	return possess.mkey_hide_by_name[name or ""]
end) -- hook(hook.MKEY_HIDE)

---------------------------------------------------------------------------
---------------------------------------------------------------------------

-- Clear all physical timed effects except for hunger
function possess.clear_effects()
	timed_effect.foreach_effect("physical",
		function(index, effect)
			if index ~= timed_effect.FOOD then
				timed_effect.set(index, 0)
			end
		end)
end

function possess.integrate_body()
	local ret, item, obj

	if player.body_monster.r_idx ~= RACE_DISEMBODIED then
		message("You area already in a body.")
		return false
	end

	ret, item = get_item("Incarnate into which body?",
						 "You have no body into which to incarnate.",
						 USE_INVEN | USE_FLOOR,
		function (obj)
			return (obj.tval == TV_CORPSE) and (obj.sval == SV_CORPSE_CORPSE)
		end)

	if not ret then
		return
	end

	local obj = get_object(item)

	if not obj.flags[FLAG_MONSTER_OBJ] then
		message(color.LIGHT_RED,
				"ERROR: Corpse has no FLAG_MONSTER_OBJ!")
		return false
	end

	if (obj.flags[FLAG_MONSTER_OBJ].flags[FLAG_MANY_SOULS]) then
		message("You can not incarnate in a such a mix of bodies.")
		return false
	end

	if not change_player_body(obj.flags[FLAG_MONSTER_OBJ]) then
		return false
	end

	possess.clear_effects()

	-- Store possed corpse
	player.flags[FLAG_MONSTER_OBJ] = object_dup(obj)

	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)

	message("Your spirit is incarnated in your new body.")

	return true
end -- player.integrate_body()

function possess.leave_body(want_drop, force_drop, force_leave)
	if player.body_monster.r_idx == RACE_DISEMBODIED then
		message("You are already disembodied.")
		return false
	end

	local cursed = {num = 0, perma = 0}

	for_inventory(player, INVEN_PACK, INVEN_TOTAL,
		function(obj)
			if is_cursed(obj) then
				%cursed.num = %cursed.num + 1
			end

			-- Can't use Scroll of Deincarnation to remove The One Ring
			if has_flag(obj, FLAG_PERMA_CURSE) then
				%cursed.perma = %cursed.perma + 1
			end
		end)

	if cursed.perma > 0 or (cursed.num > 0 and not force_leave) then
		message("A cursed object is preventing you from leaving your body.")
		return false
	end

	local old_body = monster_type_new()
	local no_body  = monster_type_new()

	monster_prep(no_body, RACE_DISEMBODIED, 0)

	if not change_player_body(no_body, old_body) then
		delete_monster_type(old_body, true)
		delete_monster_type(no_body, true)
		return false
	end

	delete_monster_type(no_body, true)

	-- No bodily effects while disembodied
	possess.clear_effects()

	-- No mana while disembodied
	player.csp(0)

	local chance = 25 + get_skill_scale(SKILL_POSSESSION, 25) +
		get_skill_scale(SKILL_PRESERVATION, 50)

	if force_drop then
		chance = 100
	elseif not want_drop then
		chance = 0
	end

	if rng.percent(chance) then
		local corpse = player.flags[FLAG_MONSTER_OBJ]

		if corpse then
			corpse = object_dup(corpse)
			player.flags[FLAG_MONSTER_OBJ] = nil

			-- Preserve any changes made to the monster's body while
			-- player was inhabiting it
			monster_type_copy(corpse.flags[FLAG_MONSTER_OBJ], old_body)
		else
			-- Leaving player body, need to create corpse
			corpse = new_object()
			object_prep(corpse, lookup_kind(TV_CORPSE, SV_CORPSE_CORPSE))
			corpse.flags[FLAG_MONSTER_OBJ] = monster_type_dup(old_body)

			hook.process(hook.CORPSE_CREATE_POST, corpse, old_body)
		end

		drop_near(corpse, -1, player.py, player.px)
	elseif want_drop then
		message("You do not manage to keep the corpse from rotting away.")
	end

	delete_monster_type(old_body, true)

	return true
end -- player.leave_body()

---------------------------------------------------------------------------
---------------------------------------------------------------------------

-- Disincarnate "spell"
possess.SPELL_INATE_DISINCARNATE = add_spell
{
	["name"]     = "Disincarnate",
	["school"]   = {0},
	["level"]    = 1,
	["mana"]     = 0,
	["mana_max"] = 0,
	["fail"]     = 0,
	["random"]   = -1,
	["spell"]    = function()
		possess.leave_body(true)
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Leave your body.",
	}
	-- Leave body even if anti-magic
	["antimagic"] = true,
}

-- Controlled monster-corpse forced casting (copied from riding.lua)
function possess.use_corpse_powers()
	-- Copy spells
	local flag_spells = flag_dup(player.body_monster.spells)

	-- Add disincarnate spell
	flag_set_full(flag_spells, possess.SPELL_INATE_DISINCARNATE,
				  FLAG_FLAG_BOTH, 0, 100)

	local spells = {}
	local skill  = max(get_skill(SKILL_POSSESSION), 0)

	-- List the monster spells
	for i = 1, flag_spells.size do
		if (flag_spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local spl   = flag_spells.node[i].key
			local level = flag_get(flag_spells, spl)

			local success        = flag_get2(flag_spells, spl)
			local player_success = spell(spl).player_success

			if player_success then
				success = player_success(spl, level, success)
			end

			if level > skill and skill < 100 then
				-- Decrease level and increase failure rate if the
				-- casting level is greater than the possession skill
				local fail = 100 - success
				local pct = 100 * skill / max(level, 1)
				level = level * pct / 100

				pct = 200 - (100 * skill / max(level, 1))
				fail = fail * pct / 100
				success = 100 - fail
			elseif skill > level then
				-- Increase success rate if possession skill is
				-- greater than the casting level
				success = success * skill / max(level, 1)
			end

			if success < 0 then
				success = 0
			elseif success > 100 then
				success = 100
			end

			set_auto_cast(0, level, player.py, player.px)
			set_monst_helper(player.body_monster)
			local mana = get_mana(spl)
			unset_auto_cast()
			unset_monst_helper()

			local free_use = spell(spl).free_use
			if free_use and tag(free_use) ~= TAG_FUNCTION then
				mana = 0
			end

			local use_msg, warn_msg
			local can_use = true
			local clr     = color.LIGHT_GREEN

			if spell(spl).level > skill and skill < 100 then
				can_use = false
				clr     = color.LIGHT_DARK
				use_msg = "Your possession skill is not high enough to " ..
					"use that spell."
				level   = get_skill(SKILL_POSSESSION) - spell(spl).level
			elseif success <= 0 then
				can_use = false
				clr     = color.LIGHT_DARK
				use_msg = "No chance of success."
			elseif mana > player.csp() then
				clr      = color.ORANGE
				warn_msg = "WARNING: Using more mana than you currently " ..
					"have will expell you from your body."
			end

			-- Verify level
			if not spell(spl).no_player then
				tinsert(
						spells,
						{
							index     = spl,
							success   = success,
							level     = level,
							raw_level = flag_get(flag_spells, spl)
							mana      = mana,
							can_use   = can_use,
							use_msg   = use_msg,
							warn_msg  = warn_msg or spell(spl).warn_msg,
							color     = clr,
						}
					)
			end -- if not spell(spl).no_user then
		end
	end
	-- Sort by level and name
	sort(spells,
		 function(s1, s2)
			 return (s1.raw_level < s2.raw_level) or
				 ((s1.level == s2.level) and
				  (spell(s1.index).name < spell(s2.index).name))
		 end)

	-- Ask what spell to cast
	local ret = choose_from_list(
		spells, "Spells", "Cast which spell?",
		function (y, x, thing)
			if not thing then
				term.blank_print(format("%-20s%-16s Level Cost Fail Info",
										"Name", "School"), y, x)
			else
				local str
				local color = thing.color
				set_get_level(thing.level)
				str = format("%-20s%-16s   %3d %4s %3d%s %s",
							 spell(thing.index).name, "Monster",
							 thing.level, thing.mana,
							 100 - thing.success, "%",
							 __spell_info[thing.index]())
				term.blank_print(color, str, y, x)
				unset_get_level()
			end
		end,
		function (y, thing)
			set_get_level(thing.level)
			print_spell_desc(thing.index, y)
			unset_get_level()
		end
	) -- choose_from_list()

	-- Something to do ?
	if ret then
		if ret.can_use and ret.warn_msg then
			message(ret.warn_msg)
			local press, key = get_com("Cast anyways? (y/n)")

			if not press then
				ret.can_use = false
			end

			local key = strlower(strchar(key))

			if key ~= "y" then
				ret.can_use = false
			end
		end

		local expel = false

		energy_use = get_player_energy(SPEED_GLOBAL)

		if not ret.can_use then
			if ret.use_msg then
				message(ret.use_msg)
			end
			energy_use = 0
		elseif not rng.percent(ret.success) then
			message("You fail to control your possessed corpse!")
			increase_mana(-ret.mana)
		else
			set_get_level(ret.level)
			set_monst_helper(player.body_monster)
			local spell_ret = __spell_spell[ret.index]()

			if spell_ret == SPELL_NOTHING then
				energy_use = 0
			else
				local free_use = spell(ret.index).free_use
				if free_use and tag(free_use) == TAG_FUNCTION and
					free_use(ret)
				then
					ret.mana = 0
				end

				local player_expel = spell(ret.index).player_expel
				if player_expel then
					if tag(player_expel) == TAG_FUNCTION then
						expel = player_expel(ret)
					else
						expel = true
					end
				end
				if ret.mana > player.csp() then
					expel = true
				end

				increase_mana(-ret.mana)
			end -- if ret ~= SPELL_NOTHING then
			unset_get_level()
			unset_monst_helper()
		end -- can use and fail check passed

		if expel then
			message("You loose control of your body!")

			if not possess.leave_body(true) then
				message(color.VIOLET<
						"Forced back into your body by a cursed item, " ..
							"you suffer a system shock!")

				player.chp(min(1, player.chp))
			end -- if not possess.leave_body(true) then
		end -- if player.csp() < 0 then
	end -- if ret then

	-- Free memory
	flag_free(flag_spells, true)

	return true
end

---------------------------------------------------------------------------
---------------------------------------------------------------------------

load_subsystem("combat_body_default",
{
	stat_str        = A_STR,
	base_skill_name = "Possession",
	mainhand_slot   = INVEN_MAINHAND,
	offhand_slot    = INVEN_OFFHAND,
	use_weapon_objs = true,
	get_global_extra_blows =
		function(blow_table, body, m_idx)
			local global_blows = {num = player.extra_blows}

			if combat.body_default.use_weapon_objs then
				-- Extra blows from a weapon only apply to that weapon, not
				-- globablly.
				for_inventory(player, INVEN_PACK, INVEN_TOTAL,
							  function(obj)
								  if obj.flags[FLAG_WEAPON] then
									  %global_blows.num = %global_blows.num -
										  get_flag(obj, FLAG_BLOWS)
								  end
							  end)
			end
			return global_blows.num
		end -- get_global_extra_blows()
}) -- load_subsystem("combat_body_default")

-- Make sure that monsters have the body parts needs by combat_body_default
function possess.validate_monster_blows()
	for i = 0, max_r_idx - 1 do
		local race = r_info(i)

		for ap_cnt = 0, flag_max_key(race.blow) do
			local blow_struct = flag_get_blow(race.blow, ap_cnt)

			if not blow_struct then
				break
			end

            local effect_idx = blow_struct.effect
            local method_idx = blow_struct.method

			if (method_idx == 0) then
				-- No more attacks
				break
			end

            local effect = __monster_attack_types[effect_idx]
            local method = __monster_attack_methods[method_idx]

			local slot1, slot2

			if method.weapon then
				slot1 = method.slot
			elseif method.barehand then
				slot1 = method.hand1_slot
				slot2 = method.hand2_slot
			end

			if slot1 and (not race.body_parts[slot1] or
						  race.body_parts[slot1] == 0)
			then
				print(race.name .. " is missing " ..
					  inventory_slot_names[slot1 + 1].desc)
			end
			if slot2 and (not race.body_parts[slot2] or
						  race.body_parts[slot2] == 0)
			then
				print(race.name .. " is missing " ..
					  inventory_slot_names[slot2 + 1].desc)
			end
		end
	end
end

---------------------------------------------------------------------------
-- Object automizer clauses
---------------------------------------------------------------------------

-- Is the player possessing a monster's body?
auto.add_clause("is_possessing",
function(rule)
	return function(object)
			   if player.body_monster.r_idx ~= RACE_PLAYER and
				   player.body_monster.r_idx ~= RACE_DISEMBODIED
			   then
				   return true
			   end
		   end
end)

-- Does the name of the body we're currently occupying contain the
-- given string?
auto.add_clause("body_name",
function(rule)
	return function(object)
			   local monst = player.body_monster
			   local name  = monster_desc(monst, 512 | 256 | 128)
			   return strfind(strlower(name), strlower(%rule[1]), 1, 1)
		   end
end)

-- If we're possessing a monster's body, then compare it to any
-- corpses lying on the ground.
auto.add_clause("body",
function(rule)
	if rule[1] ~= "level" and rule[1] ~= "maxhp" then
		message(color.LIGHT_RED, "ERROR: Automizer 'body' clause " ..
				"unknown comparsion type '" .. rule[1] .. "'")
		return function() end
	end

	if not (rule.args.min or rule.args.max) then
		message(color.LIGHT_RED, "ERROR: Automizer 'body' clause " ..
				"must have one of 'min' or 'max' defined.")
		return function() end
	end

	if rule.args.min then
		rule.args.min = tonumber(rule.args.min)
	else
		rule.args.min = -infinity
	end

	if rule.args.max then
		rule.args.max = tonumber(rule.args.max)
	else
		rule.args.max = infinity
	end

	return function(object)
			   -- Not a corpse
			   if not (object.tval == TV_CORPSE and
					   (object.sval == SV_CORPSE_CORPSE or
						object.sval == SV_CORPSE_SKELETON))
			   then
				   return false
			   end

			   -- Not on ground
			   if not (object.iy > 0 and object.ix > 0) then
				   return false
			   end

			   -- Not possessing
			   if player.body_monster.r_idx == RACE_PLAYER or
				   player.body_monster.r_idx == RACE_DISEMBODIED
			   then
				   return false
			   end

			   local monst = object.flags[FLAG_MONSTER_OBJ]
			   if not monst then
				   return false
			   end

			   return compare_monsters(player.body_monster, monst,
									   %rule[1], %rule.args)
		   end
end)
---------------------------------------------------------------------------
---------------------------------------------------------------------------

-- Everyone always had the incarnate ability, but it is only accessible
-- when the player is disembodied.  This way there can't be any bug
-- that leaves the player disembodied but without the Incarnate ability
-- turned on.
add_mkey
{
	mkey = "Incarnate",
	type = "ability",
	fct	 = function()
		possess.integrate_body()
	end,
	hide = function()
		-- Can only incarnate while disembodied
		return (player.body_monster.r_idx ~= RACE_DISEMBODIED)
	end,
}

hook(hook.GAME_START,
function()
	ab_info(AB_INCARNATE).acquired = true
	ab_info(AB_INCARNATE).hidden  = true
end)

add_mkey
{
	mkey = "Possession",
	type = "skill",
	hide = function()
		-- No possession powers while disemodied.
		return (player.body_monster.r_idx == RACE_DISEMBODIED)
	end
	fct	= possess.use_corpse_powers
}
