--
-- The summoning skill.
--

declare_global_constants
{
	"summoning",
}
summoning = {}

new_flag("NO_TOTEM")
new_flag("NO_TOTEM_PARTIAL")
new_flag("TOTEMIZE_PRE")
new_flag("TOTEMIZE_POST")
new_flag("TOTEM_SUMMON_PRE")
new_flag("TOTEM_SUMMON_POST")
new_flag("PARTIAL_SUMMON")

hook.new_hook_type("TOTEMIZE_PRE")
hook.new_hook_type("TOTEMIZE_POST")
hook.new_hook_type("TOTEM_SUMMON_PRE")
hook.new_hook_type("TOTEM_SUMMON_POST")

function summoning.make_totem()
	if player.has_intrinsic(FLAG_BLIND) or no_lite() then
		message("You need to be able to see to create totems.")
		return
	end

	local ret, item = get_item("Make totem from which corpse?",
						 "You have no corpses from which to make a totem.",
						 USE_INVEN | USE_FLOOR,
		function (corpse)
			return (corpse.tval == TV_CORPSE) and
				(corpse.sval == SV_CORPSE_CORPSE)
		end)

	if not ret then
		return
	end

	local corpse = get_object(item)
	local monst  = corpse.flags[FLAG_MONSTER_OBJ]
	if not monst then
		message(color.LIGHT_RED,
				"ERROR: Corpse has no FLAG_MONSTER_OBJ!")
		return
	end

	if or_msg("Something prevents you from making a totem from " ..
			  "this corpse.", monst.flags[FLAG_NO_TOTEM])
	then
		return
	end

	local do_partial  = false
	local can_partial = not (monst.flags[FLAG_UNIQUE] or
							 monst.flags[FLAG_NO_TOTEM_PARTIAL])

	if can_partial then
		local press, key = get_com("Make partial totem? (y/n)")

		if press then
			local key = strlower(strchar(key))

			if key == "y" then
				do_partial = true
			end
		end
	end -- if can_partial then

	local totem
	if do_partial then
		totem = create_object(TV_TOTEM, SV_TOTEM_PARTIAL)
	else
		totem = create_object(TV_TOTEM, SV_TOTEM_TRUE)
	end
	totem.found = OBJ_FOUND_SELFMADE

	-- Unique totems are indestructable
	if monst.flags[FLAG_UNIQUE] then
		totem.flags[FLAG_NORM_ART] = true
	end

	-- Ask hooks if this is okay.
	if monst.flags[FLAG_TOTEMIZE_PRE] then
		local func
		func = get_func_from_flag_or_registry(monst.flags,
											  FLAG_TOTEMIZE_PRE)

		if func(corpse, monst, totem, do_partial) then
			delete_object(totem)
			return
		end
	end
	if hook.process(hook.TOTEMIZE_PRE, corpse, monst, totem, do_partial) then
		delete_object(totem)
		return
	end

	-- Set up monster
	local monst = monster_type_dup(monst)
	monst.hp    = monst.maxhp
	monst.ml    = true
	if monst.flags[FLAG_MANA_MAX] then
		monst.flags[FLAG_MANA_CURR] = monst.flags[FLAG_MANA_MAX]
	end
	if do_partial then
		monst.flags[FLAG_DROP_CORPSE]   = nil
		monst.flags[FLAG_DROP_SKELETON] = nil
		monst.flags[FLAG_MULTIPLY]      = nil
	end

	-- Paranoia, since the way we create monsters from totems doesn't
	-- create drops.
	monst.flags[FLAG_DROP_60]     = nil
	monst.flags[FLAG_DROP_90]     = nil
	monst.flags[FLAG_DROP_THEME]  = nil
	monst.flags[FLAG_DROP_NUMBER] = nil

	-- More paranoia, since the way we create monsters from totems
	-- shouldn't place escorts or friends anyways.
	monst.flags[FLAG_ESCORT]  = nil
	monst.flags[FLAG_FRIEND]  = nil
	monst.flags[FLAG_FRIENDS] = nil

	local spells_to_remove = {}
	for i = 1, monst.spells.size do
		if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local idx = monst.spells.node[i].key
			local spl = spell(idx)

			if spl.no_totem or (do_partial and spl.no_partial) then
				if wizard then
					print("Removing spell " .. spl.name .. " from totem.")
				end
				tinsert(spells_to_remove, idx)
			end
		end
	end -- for i = 1, monst.spells.size do

	for i = 1, getn(spells_to_remove) do
		monst.spells[spells_to_remove[i]] = nil
	end

	totem.flags[FLAG_MONSTER_OBJ] = monst

	-- Learn everything about totem
	set_aware(totem)
	set_known(totem)
	flag_learn_all(totem.flags)
	totem.ident = IDENT_KNOWN | IDENT_MENTAL

	-- Done, inform hooks
	if monst.flags[FLAG_TOTEMIZE_POST] then
		local func
		func = get_func_from_flag_or_registry(monst.flags,
											  FLAG_TOTEMIZE_POSTa)

		func(corpse, monst, totem, do_partial)
	end
	hook.process(hook.TOTEMIZE_POST, corpse, monst, totem, do_partial)

	-- Remove corpse
	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)

	-- Carry totem
	if inven_carry_okay(totem) then
		inven_carry(totem, true)
	else
		message("There is no room for the totem in your pack.")
		drop_near(totem, -1, player.py, player.px)
	end

	energy_use = get_player_energy(SPEED_SUMMON)
end -- summoning.make_totem()

function summoning.summon_partial(totem, monst, y, x)
	monst = monster_type_dup(monst)

	monst.faction             = player.faction
	monst.flags[FLAG_FACTION] = player.faction

	monst.flags[FLAG_PARTIAL_SUMMON] = true

	return monst, false
end

function summoning.summon_true(totem, monst, y, x)
	monst = monster_type_dup(monst)
	local used, friendly_chance

	if monst.flags[FLAG_UNIQUE] then
		-- Because its unique, the totem will always be destroyed
		used = true

		friendly_chance =
			get_skill_scale(SKILL_SUMMON, 3500) / (monst.level + 1)
	else
		local skill = get_skill(SKILL_SUMMON)
		if skill == 0 then
			used = true
		else
			if rng.percent(monst.level * 50 / skill) then
				used = true
			end
		end

		friendly_chance = skill * 65 / (monst.level + 1)
	end

	local faction
	if not (friendly_chance and rng.percent(friendly_chance)) then
		faction = factions.get_hostile_faction(monst)
	else
		faction = player.faction
	end

	monst.faction = faction

	return monst, used
end -- summoning.summon_true()

summoning.partials = {}

function summoning.summon()
	local ret, item = get_item("Summon monster using which totem?",
						 "You have no totems.",
						 USE_INVEN | USE_FLOOR,
		function (totem)
			return (totem.tval == TV_TOTEM)
		end)

	if not ret then
		return
	end

	local totem = get_object(item)
	local monst = totem.flags[FLAG_MONSTER_OBJ]
	if not monst then
		message(color.LIGHT_RED,
				"ERROR: Totem has no FLAG_MONSTER_OBJ!")
		return
	end

	energy_use = get_player_energy(SPEED_SUMMON)

	local d     = 1
	local tries = 0
	local y, x
	local found = false
	while d <= 4 do
		y, x = scatter(player.py, player.px, d, 0)

		if cave_empty_bold(y, x) then
			found = true
			break
		end
		tries = tries + 1
		if imod(tries, 8 * d) then
			d = d + 1
		end
	end

	if not found then
		message("The summoning failed due to lack of room.")
		return
	end

	-- Ask hooks if this is okay.
	if monst.flags[FLAG_TOTEM_SUMMON_PRE] then
		local func
		func = get_func_from_flag_or_registry(monst.flags,
											  FLAG_TOTEM_SUMMON_PRE)

		if func(totem, monst, y, x) then
			return
		end
	end
	if hook.process(hook.TOTEM_SUMMON_PRE, totem, monst, y, x) then
		return
	end

	-- Do the actual summoning
	local summoned, used
	if totem.sval == SV_TOTEM_PARTIAL then
		monst, used = summoning.summon_partial(totem, monst, y, x)
	else
		monst, used = summoning.summon_true(totem, monst, y, x)
	end

	if not monst then
		-- Make a fake monster structure
		monst = {r_idx = 0, flags = {}}
	end

	-- Inform hooks
	local ret, new_used, new_y, new_x
	if monst.flags[FLAG_TOTEM_SUMMON_POST] then
		local func
		func = get_func_from_flag_or_registry(monst.flags,
											  FLAG_TOTEM_SUMMON_POST)

		ret, new_used, new_y, new_x =
			func(totem, monst, summoned, used, y, x)
		if ret then
			used = new_used
			if new_y then
				y = new_y
				x = new_x
			end
		end
	end
	ret, new_used, new_y, new_x =
		hook.process(hook.TOTEM_SUMMON_POST, totem, monst, summoned,
					 used, y, x)
	if ret then
		used = new_used
		if new_y then
			y = new_y
			x = new_x
		end
	end

	-- Make sure it isn't a fake monster
	if monst.r_idx ~= 0 then
		local m_idx = place_monster_type(y, x, monst)
		if m_idx == 0 then
			message("Summoning failed for some reason.")
			delete_monster_type(monst, true)
		else
			if monst.flags[FLAG_PARTIAL_SUMMON] then
				summoning.add_partial(m_idx, monst)
			end
		end
		if pets.is_pet(monst) then
			pets.set_pet_ai(monst)
		end
	end

	if used then
		item_increase(item, -1)
		item_describe(item)
		item_optimize(item)
	end
end -- summoning.summon()

function summoning.display_upkeep()
	local lines   = {}
	local monsts  = {}
	local max_len = -1

	for m_idx, monst in summoning.partials do
		local name = "a " .. monster_desc(monst, 512 | 256 | 128)
		max_len = max(max_len, strlen(name))
		tinsert(monsts, monst)
	end

	local frmt = format("%%-%ds (%%d.%%03d SP/turn)", max_len)

	for i = 1, getn(monsts) do
		local monst   = monsts[i]
		local cost    = summoning.partial_cost(monst.level)
		local sp      = cost / 1000
		local sp_frac = imod(cost, 1000)
		local name    = "a " .. monster_desc(monst, 512 | 256 | 128)

		local line = format(frmt, name, sp, sp_frac)
		tinsert(lines, line)
	end
	sort(lines)

	local sp      = summoning.maint_cost / 1000
	local sp_frac = imod(summoning.maint_cost, 1000)

	tinsert(lines, "------------------------------------------------")
	tinsert(lines, "Total: " .. summoning.total_partials ..
			" partial summon(s)")
	tinsert(lines, "Mana regeneration rate: " ..
			(100 - summoning.regen_factor) .. "% of normal")
	tinsert(lines, format("Mana drain: %d.%03d SP/turn", sp, sp_frac))

	show_arr_as_list(lines, "Partial summons upkeep costs")
end -- summoning.display_upkeep()

function summoning.use_totems()
	local press, key =
		get_com("[E]xtract a totem, [S]ummon a monster, or "
				.. "[D]isplay upkeep costs?")

	if not press then
		return
	end

	local key = strlower(strchar(key))

	if key == "s" then
		if player.has_intrinsic(FLAG_CONFUSED) then
			message("You are too confused summon anything!")
			return false
		end
		summoning.summon()
	elseif key == "e" then
		if player.has_intrinsic(FLAG_CONFUSED) then
			message("You are too confused to make a totem!")
			return false
		end
		summoning.make_totem()
	elseif key == "d" then
		summoning.display_upkeep()
	end
end -- summoning.use_totems()

--------------------------------------------------------------------------
-- Deal with partials
--------------------------------------------------------------------------
function summoning.partial_cost(level)
	level = level * 1000
	local skill = get_skill(SKILL_SUMMON)
	local floor = level * 19 / 990 + 40
	local cost  = (level / skill - 1000) / 4

	return max(cost, floor)
end

function summoning.add_partial(m_idx, monst)
	summoning.partials[m_idx] = monst
	summoning.total_partials  = summoning.total_partials + 1
	summoning.total_levels    = summoning.total_levels   + monst.level

	local cost = summoning.partial_cost(monst.level)
	summoning.maint_cost = summoning.maint_cost + cost
	player.inc_intrinsic(FLAG_DRAIN_MANA, cost)

	summoning.recalc_regen()
end

function summoning.remove_partial(m_idx, monst)
	summoning.partials[m_idx] = nil
	summoning.total_partials  = summoning.total_partials - 1
	summoning.total_levels    = summoning.total_levels   - monst.level

	local cost = summoning.partial_cost(monst.level)
	summoning.maint_cost = summoning.maint_cost - cost
	player.inc_intrinsic(FLAG_DRAIN_MANA, -cost)

	summoning.recalc_regen()
end

function summoning.recalc_regen()
	local regen_divider = 40
	local regen_factor  = 0
	if has_ability(AB_PERFECT_CASTING) then
		regen_divider = 30
	end

	if summoning.total_partials > (1 + player.lev / regen_divider) then
		regen_factor = summoning.total_levels
		if regen_factor > 100 then
			regen_factor = 100
		elseif regen_factor < 10 then
			regen_factor = 10
		end
	end

	summoning.regen_factor = regen_factor

	if summoning.old_regen_factor ~= regen_factor then
		player.update = player.update | PU_BONUS | PU_MANA
	end

	summoning.old_regen_factor = regen_factor
end

function summoning.recalc_all()
	player.inc_intrinsic(FLAG_DRAIN_MANA, -summoning.maint_cost)
	summoning.maint_cost     = 0
	summoning.total_levels   = 0
	summoning.total_partials = 0

	for m_idx, monst in summoning.partials do
		if monst.r_idx == 0 then
			summoning.partials[m_idx] = nil
		else
			summoning.add_partial(m_idx, monst)
		end
	end

	summoning.recalc_regen()
end -- summoning.calc_upkeep()

function summoning.recalc_maint()
	player.inc_intrinsic(FLAG_DRAIN_MANA, -summoning.maint_cost)
	summoning.maint_cost = 0

	for m_idx, monst in summoning.partials do
		if monst.r_idx == 0 then
			summoning.partials[m_idx] = nil
		else
			summoning.maint_cost =
				summoning.maint_cost + summoning.partial_cost(monst.level)
		end
	end
	player.inc_intrinsic(FLAG_DRAIN_MANA, summoning.maint_cost)
end -- summoning.recalc_maint()

hook(hook.PROCESS_WORLD_NON_WILD,
function()
	local sp = player.csp() * 1000

	-- Keep loosing partials until their maintancne cost is less than
	-- or equal to current mana.
	for m_idx, monst in summoning.partials do
		if monst.r_idx == 0 then
			summoning.partials[m_idx] = nil
		elseif summoning.maint_cost > sp then
			message("You lose control of " ..
					monster_desc(monst, 256 | 128) .. ".")
			delete_monster_idx(m_idx)
		else
			break
		end
	end
end) -- hook(hook.PROCESS_WORLD_NON_WILD)

hook(hook.MONSTER_REMOVED,
function(m_idx, monst)
	if summoning.partials[m_idx] then
		summoning.remove_partial(m_idx, monst)
	end
end)

-- Partials go away when level is gone (except for companions, which
-- we add back in later)
hook(hook.GEN_LEVEL_BEGIN,
function()
	summoning.partials = {}
	player.inc_intrinsic(FLAG_DRAIN_MANA, -summoning.maint_cost)
	summoning.maint_cost = 0
end)

-- Partial companions?
hook(hook.LEVEL_END_GEN,
function()
    for_each_monster(function(m_idx, monst)
		if monst.flags[FLAG_PARTIAL_SUMMON] then
			summoning.partials[m_idx] = monst
		end
	end)
	summoning.recalc_all()
end)

hook(hook.INIT_GAME,
function()
	summoning.partials         = {}
	summoning.old_regen_factor = 0
	summoning.maint_cost       = 0
	summoning.regen_factor     = 0
end)

-- Re-create list on game startup, rather than saving the list
hook(hook.GAME_START,
function()
    for_each_monster(function(m_idx, monst)
		if monst.flags[FLAG_PARTIAL_SUMMON] then
			summoning.partials[m_idx] = monst
		end
	end)
	summoning.recalc_all()
end)

hook(hook.PLAYER_LEVEL,
function()
	summoning.recalc_regen()
end)

hook(hook.RECALC_SKILLS,
function()
	summoning.recalc_maint()
end)

---------------------------------------------------------------------------
-- Object automizer clauses
---------------------------------------------------------------------------

-- Is the player possessing a monster's body?
auto.add_clause("totem",
function(rule)
	if rule[1] ~= "level" and rule[1] ~= "maxhp" then
		message(color.LIGHT_RED, "ERROR: Automizer 'totem' clause " ..
				"unknown comparsion type '" .. rule[1] .. "'")
		return function() end
	end

	if not (rule.args.min or rule.args.max) then
		message(color.LIGHT_RED, "ERROR: Automizer 'totem' clause " ..
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

	rule.args.type = rule.args.type or "all"

	if not (rule.args.type == "all" or rule.args.type == "partial" or
			rule.args.type == "true")
	then
		message(color.LIGHT_RED, "ERROR: Automizers 'tomem' clauses's " ..
				"'type' argument must be 'all' or 'partial' or 'true'.")
		return function() end
	end

	rule.args.match = rule.args.match or "all"

	if not (rule.args.match == "all" or rule.args.match == "any") then
		message(color.LIGHT_RED, "ERROR: Automizers 'tomem' clauses's " ..
				"'match' argument must be 'all' or 'any'.")
		return function() end
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

			   local corpse_monst = object.flags[FLAG_MONSTER_OBJ]
			   if not corpse_monst then
				   message(color.LIGHT_RED, "ERROR: Corpse is missing " ..
						   "MONSTER_OBJ!")
				   return false
			   end

			   local totems = {}
			   local typ    = %rule.args.type
			   for_inventory_inven(player.inventory[INVEN_INVEN],
					function(obj)
						if obj.tval == TV_TOTEM then
							if (%typ == "all" or
								(%typ == "partial" and
								 obj.sval == SV_TOTEM_PARTIAL) or
								(%typ == "true" and
								 obj.sval == SV_TOTEM_TRUE))
							then
								tinsert(%totems, obj)
							end
						end
					end)

			   -- No (matching) totems in inventory
			   if getn(totems) == 0 then
				   return false
			   end

			   for i = 1, getn(totems) do
				   local totem = totems[i]
				   local totem_monst = totem.flags[FLAG_MONSTER_OBJ]

				   if not totem_monst then
					   message(color.LIGHT_RED, "ERROR: Totem is missing " ..
							   "MONSTER_OBJ!")
					   return false
				   end

				   local ret = compare_monsters(totem_monst, corpse_monst,
												%rule[1], %rule.args)

				   if ret and %rule.args.match == "any" then
					   return true
				   elseif not ret and %rule.args.match == "all" then
					   return false
				   end
			   end -- for i = 1, getn(totems) do

			   return (%rule.args.match == "all")
		   end
end) -- 'totem' automizer clause

--------------------------------------------------------------------------

add_mkey
{
	mkey = "Summoning",
	type = "skill",
	fct	 = summoning.use_totems
}

