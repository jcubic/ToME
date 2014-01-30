-- Handles sticks
--
-- Wands: thaumaturgy spells
-- Orbs: school spells

-- Load the generic object using subsystem
load_subsystem("use_object")

constant("sticks", {})

sticks.wand = {}
sticks.orb = {}

new_flag("GEMSTONE")
new_flag("GEM_SOCKET")
new_flag("THAUM_SPELL")
new_flag("ORB_SPELL")

sticks.orb.orb_save_info =
{
	["use"] = 1,
	["max"] = 2,
	["index"] = 3,
}

---------------------------------------------------------------------------
-- General purpose stick functions
---------------------------------------------------------------------------

--- @fn
-- @brief Remove gem from a stick or other object
function sticks.remove_gem(obj)
	if not obj.flags[FLAG_MANA_SOURCE_OBJ] then
		return nil
	end

	-- There's a mana source to remove, but its not accessible to
	-- the player
	if not obj.flags[FLAG_GEM_SOCKET] then
		return nil
	end

	local copy = new_object()
	object_copy(copy, obj.flags[FLAG_MANA_SOURCE_OBJ])
	obj.flags[FLAG_MANA_SOURCE_OBJ].numer = 0
	obj.flags[FLAG_MANA_SOURCE_OBJ]       = nil

	return copy
end

--- @fn
-- @brief Attach a gem from a stick or other object.  The caller
-- is responsible for decreasing the object stack the gem came from.
function sticks.attach_gem(obj, gem, silent)
	if not obj.flags[FLAG_GEM_SOCKET] then
		return nil
	end

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		return nil
	end

	local copy = new_object()
	object_copy(copy, gem)
	copy.number = 1

	-- Now put the copy in the stick
	obj.flags[FLAG_MANA_SOURCE_OBJ] = copy
	flag_learn(obj.flags, FLAG_MANA_SOURCE_OBJ, true)

	return copy
end

--- @fn
-- @brief Attach gem to an empty stick or other object (from pack or floor)
function sticks.attach_select_gem(obj)
	local desc = object_desc(obj)

	if not (obj.flags[FLAG_GEM_SOCKET] and
			flag_is_known(obj.flags, FLAG_GEM_SOCKET)) then
		message(strcap(desc) .. " has no socket to place a gem in.")
		return nil
	end

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		message(strcap(desc) .. " already has a gem attached.")
		return nil
	end

	local ret, item = get_item("Attach which gem into the " .. desc .. "?",
							   "You have no gem to attach to the " .. desc,
							   USE_INVEN | USE_FLOOR,
							   function(o)
								   return has_flag(o, FLAG_GEMSTONE)
							   end)
	if not ret or not item then return nil end

	-- Get the item (in the pack)
	local gem = get_object(item)

	gem = sticks.attach_gem(obj, gem)

	if not gem then
		return nil
	end

	item_increase(item, -gem.number)
	item_describe(item)
	item_optimize(item)

	return gem
end

function sticks.attach_item_tester(o)
	if not o.flags[FLAG_GEM_SOCKET] then
		return false
	end
	if not flag_is_known(o.flags, FLAG_GEM_SOCKET) then
		return false
	end

	return not o.flags[FLAG_MANA_SOURCE_OBJ]
end

--- @fn
-- @brief Select item to attach a gem to
function sticks.attach_select_holder()
	local ret, item = get_item("Which item do you wish to attach a gem to?",
							   "You have no to which you can attach a gem.",
							   USE_INVEN | USE_FLOOR | USE_EQUIP, sticks.attach_item_tester)

	if not ret or not item then return nil end

	local obj = get_object(item)

	return sticks.attach_select_gem(obj)
end

function sticks.remove_item_tester(o)
	if not o.flags[FLAG_GEM_SOCKET] then
		return false
	end
	if not flag_is_known(o.flags, FLAG_GEM_SOCKET) then
		return false
	end

	if o.flags[FLAG_MANA_SOURCE_OBJ] then
		return true
	else
		return false
	end
end

--- @fn
-- @brief Select item to attach a gem to
function sticks.remove_select_holder()
	local ret, item = get_item("Which item do you wish to remove a gem from?",
							   "You have no item from which you can " ..
								   "remove a gem.",
							   USE_INVEN | USE_FLOOR | USE_EQUIP, sticks.remove_item_tester)

	if not ret or not item then return nil end

	local obj = get_object(item)

	local gem = sticks.remove_gem(obj)

	if not gem then
		return nil
	end

	inven_carry(gem, true)

	return true
end

--- @fn
-- @brief Enters/leaves stick mod o the GUSS
function sticks.guss_stick_mode(use, max)
	if use and max then
		get_level_use_stick = use
		get_level_max_stick = max
		get_level_stick_skill = SKILL_DEVICE
	else
		get_level_use_stick = -1
		get_level_max_stick = -1
		get_level_stick_skill = -1
	end
end

---------------------------------------------------------------------------
-- Wands
---------------------------------------------------------------------------

devices.add_subsystem {
	name = "thaum",

	can_use =
		function(obj, spell_flags, who)
			return true
		end,

	get_spell_name =
		function(obj, spell_flags, who)
			return ""
		end,

	get_spell_desc =
		function(obj, spell_flags, who)
			return ""
		end,

	get_skill_level =
		function(obj, spell_flags, who)
			return get_skill_scale(SKILL_DEVICE, 50)
		end,

	get_mana_cost =
		function(obj, spell_flags, skill, who)
			local spell = sticks.wand.spell_flags_to_thaum(spell_flags)

			return spell.mana
		end,

	on_insuff_mana =
		function(obj, spell_flags, skill, mana_cost, who)
			local gem = obj_mana.to_source(obj)

			message("The wand has no charges left.")
			gem.ident = gem.ident | IDENT_EMPTY
		end,

	get_fail_pct =
		function(obj, spell_flags, skill, who)
			local spell = sticks.wand.spell_flags_to_thaum(spell_flags)
			return sticks.wand.thaum_spell_chance(obj, spell, skill)
		end,

	on_fail =
		function(obj, spell_flags, fail_pct, skill, who)
			message("You failed to use the wand properly.")
		end,

	use_device =
		function(obj, spell_flags, skill, mana_cost, who)
			local spell     = sticks.wand.spell_flags_to_thaum(spell_flags)
			local cancelled = not thaum_spell_cast(spell)

			-- If user cancelled targetting, then undo mana use
			if cancelled then
				obj_mana.change(obj, mana_cost)
				energy_use = 0
				return false
			end

			-- Window stuff
			player.window[FLAG_PW_INVEN] = true
			player.window[FLAG_PW_EQUIP] = true
			player.window[FLAG_PW_PLAYER] = true

			-- An identification was made
			set_aware(obj)

			-- Combine / Reorder the pack (later)
			player.notice = player.notice | PN_COMBINE | PN_REORDER

			return true
		end, -- use_device

	get_time =
		function(obj, spell_flags, skill, who)
			return get_player_energy(SPEED_USE)
		end,
} -- thaum subsystem for decives framework


sticks.wand.thaum_save_info =
{
	["level"] = 1,
	["mana"] = 2,
	["dice"] = 3,
	["sides"] = 4,
	["radius"] = 5,
	["dam_type"] = 6,
	["proj_flags"] = 7,
	["shape"] = 8,
	["dur"] = 9,
}

hook(hook.INFO_DATA_LOADED,
function()
	if thaum_attack_types_num > __thaum_shapes_num then
		sticks.wand.id_multiplier = thaum_attack_types_num
		sticks.wand.id_multiplied = "dam_type"
		sticks.wand.id_added      = "shape"
	else
		sticks.wand.id_multiplier = __thaum_shapes_num
		sticks.wand.id_multiplied = "shape"
		sticks.wand.id_added      = "dam_type"
	end
end)

--- @fn
-- @brief Turn a thaum spell into a multiflavour id
function sticks.wand.thuam_to_flav_id(thaum)
	local id = (thaum[sticks.wand.id_multiplied] *
				sticks.wand.id_multiplier) +
		thaum[sticks.wand.id_added]
	return id
end

--- @fn
-- @brief Turn a multiflavour id into a thaum spell
function sticks.wand.flav_id_to_thaum(id)
	local spell = {}

	spell[sticks.wand.id_multiplied] = id / sticks.wand.id_multiplier
	spell[sticks.wand.id_added]      = imod(id, sticks.wand.id_multiplier)

	return spell
end

--- @fn
-- @brief Turn a multiflavour id into a wand name
function sticks.wand.flav_id_to_wand_name(id)
	local spell = sticks.wand.flav_id_to_thaum(id)

	return "wand of " .. strlower(thaum_spell_name(spell))
end

add_multiflav_tval(TV_WAND, {id_to_name = sticks.wand.flav_id_to_wand_name})

--- @fn
-- @brief Store a thaum spell in an object
function sticks.wand.thaum_to_obj(spell, obj)
	local ospl = flag_new()

	for k, e in spell do
		ospl[sticks.wand.thaum_save_info[k]] = e
	end

	obj.flags[FLAG_DEVICE_USE][0][FLAG_THAUM_SPELL] = ospl
end

--- @fn
-- @brief Get a thaum spell the device framework's spell-flags flagset
function sticks.wand.spell_flags_to_thaum(spell_flags)
	local spell = {}
	local ospl = spell_flags[FLAG_THAUM_SPELL]

	for k, e in sticks.wand.thaum_save_info do
		if ospl[e] then spell[k] = ospl[e] end
	end

	return spell
end

--- @fn
-- @brief Get a thaum spell from an object
function sticks.wand.obj_to_thaum(obj)
	return sticks.wand.spell_flags_to_thaum(obj.flags[FLAG_DEVICE_USE][0])
end

--- @fn
-- @brief Thaumaturgy spell chance from a wand
function sticks.wand.thaum_spell_chance(object, spell, skill_level)
	local chance = thaum_spell_chance(spell, skill_level)

	-- Is it simple to use ?
	if has_flag(object, FLAG_EASY_USE) then
		chance = chance / 3
	end
	return chance
end

sticks.wand_item_tester =
	function(o)
		return (o.tval == TV_WAND)
	end

--- @fn
-- @brief Aim a wand (from the pack or floor).
function sticks.wand.aim(item)
	if item and not sticks.wand_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	if not item then
		-- Get an item
		local ret
		ret, item = get_item("Aim which wand?",
							   "You have no wand to aim.",
							   USE_INVEN | USE_FLOOR,
							   sticks.wand_item_tester)
		if not ret or not item then return nil end
	end

	-- Get the item (in the pack)
	local object = get_object(item)

    if not object.flags[FLAG_MANA_SOURCE_OBJ] then
        -- Wand has no gem, try to attach one
        local gem = sticks.attach_select_gem(object)

        if not gem then
            return nil
        end

        -- Take a turn in attaching the gem
        energy_use = get_player_energy(SPEED_USE)

        return
    end

	devices.use(object, 0, WHO_PLAYER)
end

function sticks.wand.make(obj, power, ...)
	-- Has the wand already been set up?
	if obj.flags[FLAG_THAUM_SPELL] then
		return
	end

	local lvl = get_kind(obj).level / 2 + m_bonus(get_kind(obj).level / 2, dun_level)
	if lvl < 1 then lvl = 1 end
	if lvl > 50 then lvl = 50 end
	local allows = {}
	for i = 1, getn(arg) do allows[arg[i]] = true end

	obj.flags[FLAG_DEVICE_USE] = flag_new()
	obj.flags[FLAG_DEVICE_USE][0] = flag_new()
	obj.flags[FLAG_DEVICE_USE][0][FLAG_DEVICE_TYPE] = "thaum"

	local spell = new_thaum_spell(lvl, allows)
	sticks.wand.thaum_to_obj(spell, obj)
	sticks.wand.setup_flavour(obj)
end

function sticks.wand.setup_flavour(obj)
	local spell   = sticks.wand.obj_to_thaum(obj)
	local flav_id = sticks.wand.thuam_to_flav_id(spell)
	obj.flags[FLAG_FLAVOUR_ID] = flav_id

	-- Wands of the same object_kind (k_idx) have different colours
	local flavour, color = flavour.get(TV_WAND, flav_id)
	obj.d_attr = color
	obj.x_attr = color
end

function sticks.wand.make_lesser(obj, power)
	return sticks.wand.make(obj,
				power,
				THAUM_SHAPE_BEAM,
				THAUM_SHAPE_BLAST,
				THAUM_SHAPE_BOLT
	)
end
function sticks.wand.make_normal(obj, power)
	return sticks.wand.make(obj,
				power,
				THAUM_SHAPE_BALL,
				THAUM_SHAPE_BEAM,
				THAUM_SHAPE_BLAST,
				THAUM_SHAPE_BOLT,
				THAUM_SHAPE_WAVE,
				THAUM_SHAPE_SWORD
	)
end

function sticks.wand.make_greater(obj, power)
	return sticks.wand.make(obj,
				power,
				THAUM_SHAPE_BALL,
				THAUM_SHAPE_BEAM,
				THAUM_SHAPE_BLAST,
				THAUM_SHAPE_BOLT,
				THAUM_SHAPE_SWARM,
				THAUM_SHAPE_WAVE,
				THAUM_SHAPE_CLOUD,
				THAUM_SHAPE_STORM,
				THAUM_SHAPE_SWORD
	)
end

--
-- Code for wishing for wands, rod tips, and staves
--
function sticks.wand.parse_wish_str(wish_str, item)
	-- NOTE: This is *NOT* an actual implementation of the wand parsing
	-- code, but just a placeholder, so the rest of the wishing code
	-- doesn't get errors

	local name = item.name

	name = gsub(name, "& ", "")
	name = gsub(name, "~", "")
	name = gsub(name, " #", "")

	name     = strlower(name)
	wish_str = strlower(wish_str)

	if not strfind(wish_str, name) then
		return wish.UNGRANTED
	end

	local prefix, suffix, spell = thaum_parse_name(wish_str)

	if not prefix then
		return wish.UNGRANTED
	end

	local flavor = strlower(thaum_spell_name(spell))
	local base   = gsub(item.name, "#", flavor)

	return wish.GRANTED, base, {}
end -- sticks.wand.parse_wish_str()

function sticks.wand.grant_wish(wish_str, item)
	local prefix, suffix, spell = thaum_parse_name(wish_str)

	local obj = create_object(item.tval, item.sval)

	spell = new_thaum_spell(50, {[spell.shape] = true}, spell.dam_type)
	sticks.wand.thaum_to_obj(spell, obj)
	sticks.wand.setup_flavour(obj)

	apply_magic(obj, 99, FALSE, true, FALSE)
	return wish.GRANTED, obj
end

-----------------------------------------------------------------------------
-- Orbs (formerly staves in TomE 2)
-----------------------------------------------------------------------------

--- @fn
-- @brief Turn an orb spell into a multiflavour id
function sticks.orb.orb_to_flav_id(orb)
	return orb.index
end

--- @fn
-- @brief Turn a multiflavour id into a orb spell
function sticks.orb.flav_id_to_orb(id)
	return {index = id}
end

--- @fn
-- @brief Turn a multiflavour id into a orb name
function sticks.orb.flav_id_to_orb_name(id)
	return "orb of " .. strlower(spell(id).name)
end

function sticks.orb.setup_flavour(obj)
	local spell   = sticks.orb.obj_to_orb(obj)
	local flav_id = spell.index
	obj.flags[FLAG_FLAVOUR_ID] = flav_id

	-- Orbs of the same object_kind (k_idx) have different colours
	local flavour, color = flavour.get(TV_ORB, flav_id)
	obj.d_attr = color
	obj.x_attr = color
end

add_multiflav_tval(TV_ORB, {id_to_name = sticks.orb.flav_id_to_orb_name})

--- @fn
-- @brief Store a orb spell in an object
function sticks.orb.orb_to_obj(spell, obj)
	local ospl = flag_new()

	for k, e in spell do
		ospl[sticks.orb.orb_save_info[k]] = e
	end

	obj.flags[FLAG_ORB_SPELL] = ospl
end

--- @fn
-- @brief Get a orb spell from an object
function sticks.orb.obj_to_orb(obj)
	local spell = {}
	local ospl = obj.flags[FLAG_ORB_SPELL]

	for k, e in sticks.orb.orb_save_info do
		if ospl[e] then spell[k] = ospl[e] end
	end

	return spell
end

sticks.orb_item_tester = function(o) return has_flag(o, FLAG_ORB_SPELL) end

--- @fn
-- @brief Use a orb (from the pack or floor).
function sticks.orb.use(item)
	if item and not sticks.orb_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	if not item then
		-- Get an item
		local ret
		ret, item = get_item("Use which orb?",
					   "You have no orb to use.",
					   USE_INVEN | USE_FLOOR,
					   sticks.orb_item_tester)
		if not ret or not item then return nil end
	end

	-- Get the item (in the pack)
	local object = get_object(item)
	local spell = sticks.orb.obj_to_orb(object)

	-- Check for power
	local gem = object.flags[FLAG_MANA_SOURCE_OBJ]
	if not gem then
		-- Staff has no gem, try to attach one
		gem = sticks.attach_select_gem(object)
		if not gem then
			return nil
		end

		-- Take a turn in attaching the gem
		energy_use = get_player_energy(SPEED_USE)
		return
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_USE)

	-- Not identified yet
	local ident = false

	-- Enter "stick mode"
	sticks.guss_stick_mode(spell.use, spell.max)

	-- Compute chance
	local chance = spell_chance(spell.index)

	-- Roll for usage
	if rng.percent(chance) then
		message("You failed to use the orb properly.")
		sticks.guss_stick_mode()
		return
	end

	-- The orb is already empty!
	if obj_mana.get_curr(object) == 0 then
		message("The orb has no charges left.")
		gem.ident = gem.ident | IDENT_EMPTY
		sticks.guss_stick_mode()
		return
	end

	local cheapness = 1
	if object.flags[FLAG_CHEAPNESS] then
		cheapness = cheapness * object.flags[FLAG_CHEAPNESS]
	end
	if gem.flags[FLAG_CHEAPNESS] then
		cheapness = cheapness * gem.flags[FLAG_CHEAPNESS]
	end

	-- The orb is already empty!
	if obj_mana.get_curr(object) < (get_mana(spell.index) / cheapness) then
		message("The orb has no charges left.")
		gem.ident = gem.ident | IDENT_EMPTY
		sticks.guss_stick_mode()
		return
	end

	-- Cast the spell
	local use_charge, obvious = activate_stick(spell.index)

	-- some uses are "free"
	if not use_charge then
		sticks.guss_stick_mode()
		return
	end

	-- Window stuff
	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true

	-- Use a single charge
	obj_mana.use(object, get_mana(spell.index))

	-- An identification was made
	if obvious then
		set_aware(object)
	end

	-- Combine / Reorder the pack (later)
	player.notice = player.notice | PN_COMBINE | PN_REORDER
	sticks.guss_stick_mode()
end

function sticks.orb.make(obj, power)
	-- Has the orb already been set up?
	if obj.flags[FLAG_ORB_SPELL] then
		return
	end

	local spl = {}
	local lvl = ((get_kind(obj).level + object_level) / 2) + (power * 4)
	if lvl < 1 then lvl = 1 end
	spl.index = get_random_stick(TV_ORB, lvl)
	spl.use = get_stick_base_level(TV_ORB, lvl, spl.index)
	spl.max = get_stick_max_level(TV_ORB, lvl, spl.index)
	sticks.orb.orb_to_obj(spl, obj)
	sticks.orb.setup_flavour(obj)
end

--
-- Code for wishing for wands, rod tips, and staves
--
function sticks.orb.parse_wish_str(wish_str, item)
	-- NOTE: This is *NOT* an actual implementation of the wand parsing
	-- code, but just a placeholder, so the rest of the wishing code
	-- doesn't get errors

	local name = item.name

	name = gsub(name, "& ", "")
	name = gsub(name, "~", "")
	name = gsub(name, " #", "")

	name     = strlower(name)
	wish_str = strlower(wish_str)

	if not strfind(wish_str, name) then
		return wish.UNGRANTED
	end

	local prefix, suffix, spell = thaum_parse_name(wish_str)

	if not prefix then
		return wish.UNGRANTED
	end

	local flavor = strlower(thaum_spell_name(spell))
	local base   = gsub(item.name, "#", flavor)

	return wish.GRANTED, base, {}
end -- sticks.orb.parse_wish_str()

function sticks.orb.grant_wish(wish_str, item)
	local prefix, suffix, spell = thaum_parse_name(wish_str)

	local obj = create_object(item.tval, item.sval)

	spell = new_thaum_spell(50, {[spell.shape] = true}, spell.dam_type)
	sticks.orb.thaum_to_obj(spell, obj)
	sticks.orb.setup_flavour(obj)

	apply_magic(obj, 99, FALSE, true, FALSE)

	return wish.GRANTED, obj
end

--------------------------------------------------
-- Mkey stuff
--------------------------------------------------
sticks.gem_MKEY = add_mkey
{
	mkey = "Magic-device",
	type = "skill",
	fct =
		function()
		local act = choose_from_list(sticks.gem_actions, "Actions",
									 "Perform which action?",
									 sticks.gem_action_shortinfo)

		if act and (act.exec) then act.exec() end
	end
	hide = function()
		local ret = { hide = true }
		for_inventory(player, INVEN_INVEN, INVEN_TOTAL, function(obj)
			if sticks.attach_item_tester(obj) or sticks.remove_item_tester(obj) then %ret.hide = false end
		end)
		for_inventory_inven(cave(player.py, player.px).inventory, function(obj)
			if sticks.attach_item_tester(obj) or sticks.remove_item_tester(obj) then %ret.hide = false end
		end)
		return ret.hide
	end
}

sticks.gem_actions = {
	{
		name = "Attach a gem to an object",
		exec = function()
					sticks.attach_select_holder()
			end
	},
	{
		name = "Remove a gem from an object",
		exec = function()
				sticks.remove_select_holder()
			end
	}
}

function sticks.gem_action_shortinfo (y, x, a)
	if a then term.blank_print(a.name, y, x)
 end end

--------------------------------------------------
-- Hooks
--------------------------------------------------

-- Reduce mana consumption if FLAG_CHEAPNESS is present
hook(hook.MANA_USE_PRE,
function(source, amnt, sink)
	if obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	if source.flags[FLAG_CHEAPNESS] then
		amnt = amnt / source.flags[FLAG_CHEAPNESS]
	end

	if sink and sink.flags[FLAG_CHEAPNESS] then
		amnt = amnt / sink.flags[FLAG_CHEAPNESS]
	end

	if amnt < 1 then
		amnt = 1
	end

	obj_mana.new_amnt = amnt
end)

--
-- Init
--
hook(hook.KEYPRESS, function (key)
	if key == strbyte('a') then sticks.wand.aim() return true end
	if key == strbyte('u') then sticks.orb.use() return true end
end)
