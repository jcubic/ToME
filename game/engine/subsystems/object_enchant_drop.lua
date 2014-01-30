-- The default object enchanter and drops maker subsystem
--
-- Load it with: load_subsystem("object_enchant_drop")
--
-- This does the basic handling of objects enchantement and object dropping from monsters.
--
-- The subsystem defines two flags : BASE_QUANTITY and SCALED_QUANTITY,
-- which define the average size of the stack an item is generated in at
-- its native level and how much it will increase 100 levels later.
--
-- Parameters:
-- * treasure_theme_flag: The theme that cotains only gold, if nil then ONLY_ITEM/ONLY_GOLD flags
--                        will not be respected
-- * corpse_chance: A function that returns the chance for a monster to drop corpse
-- * chances: A table of 3 functions that returns the chances for objects to become ego/double ego/artifacts
-- * drop_creation_check: A function that returns true if the monster is allowed to have objects to drop

constant("object_enchant_drop", {})

safe_new_flag("BASE_QUANTITY")
safe_new_flag("SCALED_QUANTITY")

object_enchant_drop.treasure_theme_flag = get_subsystem_param("object_enchant_drop", "treasure_theme_flag")
object_enchant_drop.corpse_chance = get_subsystem_param("object_enchant_drop", "corpse_chance") or function() return 0 end
object_enchant_drop.drop_creation_check = get_subsystem_param("object_enchant_drop", "drop_creation_check") or nil
object_enchant_drop.chances = get_subsystem_param("object_enchant_drop", "chances") or
{
	artifact = function() return 0 end
	ego = function() return 0 end
	double_ego = function() return 0 end
}
object_enchant_drop.extra_ego_slots = get_subsystem_param("object_enchant_drop", "extra_ego_slots") or {}
object_enchant_drop.on_make_all = get_subsystem_param("object_enchant_drop", "on_make_all") or function() end

----------------------------------------------------------------------------
----------------------------- Object enchants ------------------------------
----------------------------------------------------------------------------

--- @fn
-- @brief Modules should override this to define how items are enchanted
function objects_on_make_all_pre(obj, power)
	for group, data in object_enchant_drop.extra_ego_slots do
		if data.chance(object_level, obj, power) * 100 > rng.number(10000) then
			local ego = rand_obj.get_single_ego(object_level, obj.k_idx, nil, group)
			if ego then obj.ego_id[data.slot] = ego end
		end
	end
end

--- @fn
-- @brief Modules should override this to define how items are enchanted
function objects_on_make_all(obj, power)
	-- Quantities
	if has_flag(obj, FLAG_BASE_QUANTITY) or
		has_flag(obj, FLAG_SCALED_QUANTITY) then

		local kind = k_info(obj.k_idx)
		local level_diff = (object_level - kind.level)

		local base = obj.flags[FLAG_BASE_QUANTITY] or 1
		local scaled = obj.flags[FLAG_SCALED_QUANTITY] or 0

		local mean = base + (level_diff * scaled) / 100

		-- Do not put the value directly into obj.number, since the latter
		-- is a unsigned 8 bit number.
		local num = rng.normal(mean, mean / 2)
		if num < 1 then
			num = 1
		elseif num > 99 then
			num = 99
		end
		obj.number = num
	end

	-- And now do whatever the module needs
	object_enchant_drop.on_make_all(obj, power)
end

--- @fn
-- @brief Modules should override this to define how the engine determines object "power" for latter enchanting
function objects_get_power_level(obj, lev, okay, good, great)
	-- Apply luck
	lev = lev + luck( -7, 7)

	-- Maximum "level" for various things
	if lev > 127 then lev = 127 end

	-- Base chance of being "good"
	local f1 = lev + 10 + luck( -15, 15)

	-- Maximal chance of being "good"
	if f1 > 75 then f1 = 75 end

	-- Base chance of being "great"
	local f2 = f1 / 2

	-- Maximal chance of being "great"
	if f2 > 20 then f2 = 20 end

	-- Assume normal
	local power = 0

	-- Roll for "good"
	if good or rng.percent(f1) then
		-- Assume "good"
		power = 1

		-- Roll for "great"
		if great or rng.percent(f2) then power = 2 end

	-- Roll for "cursed"
	elseif rng.percent(f1) then
		-- Assume "cursed"
		power = -1

		-- Roll for "broken"
		if rng.percent(f2) then power = -2 end
	end
	return power
end

--
-- Ego/arts chance
--

-- Modules may override this function to change the chance of an
-- artifact being randomly generated
rand_obj.get_artifact_chance = object_enchant_drop.chances.artifact

-- Modules may override this function to change the chance of a random item
-- getting a single ego
rand_obj.get_single_ego_chance = object_enchant_drop.chances.ego

-- This is the chance of an item gets a second ego if it already has one
-- ego; if an item has a 0% chance of getting a single ego, then it
-- will never get two egos, regardless of the double chance.
rand_obj.get_double_ego_chance = object_enchant_drop.chances.double_ego

--------------------------------------------------------------------------
---------------------------- Monster drops -------------------------------
--------------------------------------------------------------------------

function default_monster_drop(m_idx, m_ptr, r_ptr)
	local ego

	if object_enchant_drop.drop_creation_check and not object_enchant_drop.drop_creation_check(m_idx, m_ptr, r_ptr) then return end

	if m_ptr.ego then
		ego = re_info[m_ptr.ego + 1]
	else
		local dflags = flag_new()
		ego = {flags = dflags}
	end

	if not r_ptr.flags[FLAG_DROP_THEME] and	not ego.flags[FLAG_DROP_THEME] and not dungeon(current_dungeon_idx).flags[FLAG_OBJ_THEME] then
		error("default_monster_drop(): no drop theme for race " ..
			  r_ptr.name.." nor any dungeon theme")
		return
	end

	local num_drop = 0

	if has_flag(r_ptr, FLAG_DROP_60) and rng.number(100) < 60 then
		num_drop = num_drop + 1
	end
	if has_flag(r_ptr, FLAG_DROP_90) and rng.number(100) < 90 then
		num_drop = num_drop + 1
	end
	if has_flag(r_ptr, FLAG_DROP_NUMBER) then
		num_drop = num_drop + rng(get_flag2(r_ptr, FLAG_DROP_NUMBER), get_flag(r_ptr, FLAG_DROP_NUMBER))
	end

	if num_drop == 0 then
		return
	end

	local good  = has_flag(r_ptr, FLAG_DROP_GOOD) or
		has_flag(ego, FLAG_DROP_GOOD)
	local great = has_flag(r_ptr, FLAG_DROP_GREAT) or
		has_flag(ego, FLAG_DROP_GREAT)

	local only_gold = has_flag(r_ptr, FLAG_ONLY_GOLD) or
		has_flag(ego, FLAG_ONLY_GOLD)
	local only_item = has_flag(r_ptr, FLAG_ONLY_ITEM) or
		has_flag(ego, FLAG_ONLY_ITEM)

	if only_gold and only_item then
		message(color.VIOLET, "ONLY_ITEM *and* ONLY_GOLD for race " ..
				r_ptr.name)
		return
	end

	-- Don't mess with the actual flag set
	local drop_flags = flag_new()
	flag_copy(drop_flags, flag_get_flags(r_ptr.flags, FLAG_DROP_THEME) or flag_get_flags(dungeon(current_dungeon_idx).flags, FLAG_OBJ_THEME))

	if ego.flags[FLAG_DROP_THEME] then
		-- Add the ego flags into the race flags
		foreach_flags(ego.flags[FLAG_DROP_THEME],
			function(flags, key)
				local dflags = %drop_flags
				local val = flag_get(flags, key)

				if theme_obj.functions[key] then
					-- Add in registered theme types
					if dflags[key] then
						val = val + flag_get(dflags, key)
					end

					if val < 0 then
						val = 0
					end
				else
					-- Drop flags that aren't registered theme types might
					-- be information for the theme type code, but they
					-- certainly aren't probabilities to be added to another
					-- probability.  Thus we don't alter the value.
				end
				flag_set(dflags, key, val)
			end)
	end

	if object_enchant_drop.treasure_theme_flag then
		if only_item then
			flag_set(drop_flags, object_enchant_drop.treasure_theme_flag, 0)
		elseif only_gold then
			drop_flags = flag_new()
			flag_set(drop_flags, object_enchant_drop.treasure_theme_flag, 100)
		end
	end

	local old_level = object_level

	object_level = (dun_level + r_ptr.level) / 2

	for i = 1, num_drop do
		local egos, artifact_chance = nil, nil
		if great then
			egos = { single_chance=70 double_chance=5 }
			artifact_chance = 5
		elseif good then
			egos = { single_chance=20 double_chance=1 }
			artifact_chance = 1
		end

		local obj = rand_obj.get_themed_obj(object_level, drop_flags, nil, egos, artifact_chance, good or great)

		if obj then
			obj.found      = OBJ_FOUND_MONSTER
			obj.found_aux1 = m_ptr.r_idx
			obj.found_aux2 = m_ptr.ego
			obj.found_aux3 = current_dungeon_idx
			obj.found_aux4 = level_or_feat(current_dungeon_idx, dun_level)

			monster_carry(m_ptr, m_idx, obj)
		elseif wizard then
			message(color.VIOLET, "Theme drop item creation " ..
				"attempt " .. i ..
				" failed for race " .. r_ptr.name)
		end
	end

	object_level = old_level
end

--------------------------------------------------------------------------
--------------------------- Handle preservation skill --------------------
--------------------------------------------------------------------------

new_flag("DO_DROP_CORPSE", true)
new_flag("DROP_CORPSE_FORCE")
hook(hook.DO_DROP_CORPSE,
function(m_ptr)
	local chance = object_enchant_drop.corpse_chance()

	chance = item_hooks.process_all_chain_pack(FLAG_DO_DROP_CORPSE, chance, m_ptr)

	if m_ptr.flags[FLAG_DROP_CORPSE_FORCE] then
		return true
	end

	if chance and rng.percent(chance) then
		return true
	end
end)
