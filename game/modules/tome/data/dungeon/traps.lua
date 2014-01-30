--------------------------------------------------------------
-- Trap spells
--------------------------------------------------------------

declare_global_constants {
	"trap_creation",
}

-- Creates a square of traps centered on the player
function trap_creation(radius, faction)
	radius  = radius  or 1
	faction = faction or FACTION_DUNGEON

	for y = player.py - radius, player.py + radius do
		for x = player.px - radius, player.px + radius do
			if in_bounds(y, x) then
				traps.place_random_location_trap(y, x, faction)
			end -- if in_bounds(y, x) then
		end -- x
	end -- y
end -- trap_creation()

traps.add {
	name     = "Test Trap",
	attr         = color.WHITE,
	never_random = true,
	minlevel     = 0,
	level        = { -1 , 1 },
	proba        = 100,
	power        = 1,
	message      = "Test trap activated."

	trap = function(trap, who, y, x, obj)
			   print("Test trap activated.")
		       return true
	       end
} -- Test Trap

traps.TRAP_RUNEBINDER = traps.add
{
	name         = "Runebinder",
	char         = ';'
	attr         = color.GOLD,
	never_random = true,
	minlevel     = 0,
	level        = { 1 , 0 },
	proba        = 100,
	power        = 1,
	message      = "Runebinder activated."

	trap = function(trap, who, y, x, obj)
		set_auto_cast(WHO_PLAYER, trap[FLAG_LEVEL], y, x)
		execute_school_spell(trap[FLAG_SPELL_IDX], "trap", FACTION_PLAYER)
		unset_auto_cast()
		return true
	end
} -- Test Trap

function traps.b()
	local trap = traps.make_trap(traps.trap_list[traps.TRAP_RUNEBINDER], FACTION_PLAYER)
	trap[FLAG_SPELL_IDX] = BLINK
	trap[FLAG_LEVEL] = 20
	traps.place_location_trap(trap, player.py, player.px, true)
end

--------------------------------------------------------------
-- Stat (dark blue) traps
--------------------------------------------------------------

traps.stats = {
	{A_CHR, "Beauty"},
	{A_STR, "Weakness"},
	{A_INT, "Intelligence"},
	{A_WIS, "Wisdom"},
	{A_DEX, "Fumbling Fingers"},
	{A_CON, "Wasting"}
}

function traps.stat_trap_spell(who, stat, severity)
	local dec_type = {STAT_DEC_TEMPORARY, STAT_DEC_NORMAL,
		STAT_DEC_PERMANENT}

	-- Only the player should trigger this
	if who ~= 0 then
		if wizard then
			message(color.VIOLET,
				  "Non-player triggering a stat trap?")
		end

		return false
	end

	do_dec_stat(traps.stats[stat][1], dec_type[severity])

	-- Triggering the trap identifies it
	return true
end

for i = 1, getn(traps.stats) do
	traps.add {
		name         = "Lesser " .. traps.stats[i][2] .. " Trap",
		minlevel     = 2,
		level        = {2, 0},
		proba        = 100,
		power        = 1,
		attr         = color.BLUE,
		player_only  = true,
		needs_target = true,

		trap = function(trap, who, y, x, obj)
				return traps.stat_trap_spell(who, %i, 1)
			end
	}

	traps.add {
		name         = traps.stats[i][2] .. " Trap",
		minlevel     = 20,
		level        = {20, 0},
		proba        = 100,
		power        = 1,
		attr         = color.BLUE,
		player_only  = true,
		needs_target = true,

		trap = function(trap, who, y, x, obj)
				return traps.stat_trap_spell(who, %i, 2)
			end
	}

	traps.add {
		name         = "Greater " .. traps.stats[i][2] .. " Trap",
		minlevel     = 40,
		level        = {40, 0},
		proba        = 100,
		power        = 1,
		attr         = color.BLUE
		player_only  = true,
		needs_target = true,

		trap = function(trap, who, y, x, obj)
				return traps.stat_trap_spell(who, %i, 3)
			end
	}
end

--------------------------------------------------------------
-- Elemental bolt and ball (cyan and green) traps
--------------------------------------------------------------

-- Each record:
--
-- DAM_TYPE,
-- Then two of these sub-records, one for bolts and one for balls:
-- ** minlevel, level1, level2, proba, dd, ds
traps.elemental_traps =
{
{dam.ELEC,
	{2, 2, 0, 80, 2, 8},
	{8, 8, 0, 60, 3, 10}
},

{dam.POIS,
	{2, 2, 0, 80, 2, 8},
	{8, 8, 0, 60, 3, 10}
},

{dam.ACID,
	{2, 2, 0, 80, 2, 8},
	{8, 8, 0, 60, 3, 10}
},

{dam.COLD,
	{2, 2, 0, 80, 2, 8},
	{8, 8, 0, 60, 3, 10}
},

{dam.FIRE,
	{2, 2, 0, 80, 2, 8},
	{8, 8, 0, 60, 3, 10}
},

{dam.LITE,
	{8,   8, 0, 80, 5, 10},
	{15, 15, 0, 60, 8, 12}
},

{dam.DARK,
	{8, 8, 0, 80, 5, 10},
	{15, 15, 0, 60, 8, 12}
},

{dam.WATER,
	{8, 8, 0, 80, 5, 10},
	{15, 15, 0, 60, 8, 12}
},

{dam.ICE,
	{8, 8, 0, 80, 5, 10},
	{15, 15, 0, 60, 8, 12}
},

{dam.CONFUSION,
	{8, 8, 0, 80, 6, 10},
	{15, 15, 0, 60, 8, 12}
},

{dam.SHARDS,
	{15, 15, 0, 80, 6, 10},
	{20, 20, 0, 60, 12, 18}
},

{dam.SOUND,
	{15, 15, 0, 80, 10, 10},
	{20, 20, 0, 60, 12, 18}
},

{dam.NEXUS,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.CHAOS,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.SUN_FIRE,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.VALINOREAN_FIRE,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.DARK_FIRE,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.INERTIA,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.GRAVITY,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.FORCE,
	{15, 15, 0, 80, 10, 12},
	{20, 20, 0, 60, 12, 18}
},

{dam.DISENCHANT,
	{25, 25, 0, 80, 15, 16},
	{30, 30, 0, 60, 16, 20}
},

{dam.TIME,
	{25, 25, 0, 80, 15, 16},
	{30, 30, 0, 60, 16, 20}
},

}

function traps.element_trap_spell(trap, y, x, elem_idx, bolt_or_ball)
	local type = traps.elemental_traps[elem_idx][1]
	local rec  = traps.elemental_traps[elem_idx][bolt_or_ball + 1]
	local dd   = rec[5]
	local ds   = rec[6]

	local rad
	if bolt_or_ball == 1 then
		rad = 0
	else
		rad = 2
	end

	-- these traps gets nastier as levels progress
	local trap_idx = flag_get(trap, FLAG_TRAP_IDX)
	local minlevel = traps.trap_list[trap_idx].minlevel

	if (max_dlv[current_dungeon_idx + 1] > (2 * minlevel)) then
		local delta = max_dlv[current_dungeon_idx + 1] / 15
		dd = dd + delta
		ds = ds + delta
	end

	local dmg  = rng.roll(dd, ds)

	return project(WHO_TRAP, rad, y, x, dmg, type, PROJECT_KILL)
end -- traps.element_trap_spell()

for i = 1, getn(traps.elemental_traps) do
	local type      = traps.elemental_traps[i][1]
	local type_name = get_dam_type_info(type, "desc")
	local rec

	-- Capitalize damage name
	type_name = strcap(type_name)

	rec  = traps.elemental_traps[i][2]
	traps.add {
		name         = type_name .. " Bolt Trap",
		minlevel     = rec[1],
		level        = {rec[2], rec[3]},
		proba        = rec[4],
		power        = 1,
		attr         = color.GREEN,
		locations    = traps.LOCATION_FLOOR
		needs_target = true,

		trap = function(trap, who, y, x, obj)
				return traps.element_trap_spell(trap, y, x,
								%i, 1)
			end
	}

	rec = traps.elemental_traps[i][3]
	traps.add {
		name     = type_name .. " Ball Trap",
		minlevel = rec[1],
		level    = {rec[2], rec[3]},
		proba    = rec[4],
		power    = 1,
		attr     = color.LIGHT_BLUE
		trap = function(trap, who, y, x, obj)
				return traps.element_trap_spell(trap, y, x,
								%i, 2)
			end
	}
end

------------

--
-- Spell traps which act like ball or bolt traps
--

traps.add {
	name     = "Noxious Cloud Trap",
	attr     = color.LIGHT_BLUE,
	minlevel = 2,
	level    = { -1 , 25 },
	proba    = 35,
	power    = 1,
	message  = "@Target@ @isare@ suddenly in the center of a thick " ..
		"poisonous cloud!",

	trap = function(trap, who, y, x, obj)
		       set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL),
				     y, x)

		       local ret
		       ret = execute_school_spell(NOXIOUSCLOUD, "trap", trap[FLAG_FACTION])

		       unset_auto_cast()

		       return ret
		end
} -- Noxius Cloud Trap

traps.add {
	name     = "Fireflash Trap",
	attr     = color.LIGHT_BLUE,
	minlevel = 10,
	level    = { 5 , 25 },
	proba    = 25,
	power    = 1,
	message  = "@Target@ @isare@ suddenly in the center of a raging " ..
		"inferno!",

	trap = function(trap, who, y, x, obj)
		       set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL),
				     y, x)

		       local ret
		       ret = execute_school_spell(FIREFLASH, "trap", trap[FLAG_FACTION])

		       unset_auto_cast()

		       return ret
	       end
} -- Fireflash Trap

------------------------------------------------------------------------
-- Dungeon affecting (orange) traps
------------------------------------------------------------------------

traps.add {
	name     = "Earthquake Trap",
	attr     = color.ORANGE,
	minlevel = 10,
	level    = { -1 , 20 },
	proba    = 20,
	power    = 1,
	location = traps.LOCATION_FLOOR | traps.LOCATION_DOOR

	message        = "A localized earthquake starts around @target@",
	message_no_see = "The earth shakes beneath your feet.",
	message_no_los = "You feel a rumble pass through the ground.",

	trap = function(trap, who, y, x, obj)
		       set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL),
				     y, x)

		       local ret
		       ret = execute_school_spell(SHAKE, "trap", trap[FLAG_FACTION])

			   return ret
		end
} -- Earthquake Trap

traps.add {
	name     = "Trap of New Traps",
	attr     = color.ORANGE,
	minlevel = 4,
	level    = { 4 , 0 },
	proba    = 100,
	power    = 1,

	message = "A loud click emmenates from @target_possessive@ " ..
		"vincinity.",
	message_no_see = "You hear a loud click.",

	trap = function(trap, who, y, x, obj)
		       local old_level = dun_level
		       dun_level = flag_get(trap, FLAG_LEVEL)


			   -- Newly created traps should be hostile towards whomever
			   -- trips the trap.
		       local faction = who_to_faction(who)
		       if factions.get_friendliness(FACTION_PLAYER, faction) >= 0 then
			       faction = FACTION_DUNGEON
		       else
			       faction = FACTION_PLAYER
		       end

		       fill_traps(y, x, 1, 0, faction)

		       dun_level = old_level

		       return true
		   end,
	post_placement = function(trap, y, x)
				trap[FLAG_LEVEL] = max(dun_level, trap[FLAG_LEVEL])
			end
} -- Trap of new Traps

traps.add {
	name       = "Trap of Filling",
	attr       = color.ORANGE,
	minlevel   = 25,
	level      = { -1 , 1 },
	proba      = 50,
	power      = 1,
	never_know = true,

	message        = "The floor around @target@ vibrates in a strange way.",
	message_no_see = "You feel the floor vibrate.",

	trap = function(trap, who, y, x, obj)
			local old_level = dun_level
			dun_level = flag_get(trap, FLAG_LEVEL)

			   -- Newly created traps should be hostile towards whomever
			   -- trips the trap.
		       local faction = who_to_faction(who)
		       if factions.get_friendliness(FACTION_PLAYER, faction) >= 0 then
			       faction = FACTION_DUNGEON
		       else
			       faction = FACTION_PLAYER
		       end

		       fill_traps(y, x, 8, 3, faction)

		       dun_level = old_level

		       return false
		end
	post_placement = function(trap, y, x)
				trap[FLAG_LEVEL] = max(dun_level, trap[FLAG_LEVEL])
			end
} -- Trap of Filling

traps.add {
	name       = "Trap of Disarming",
	attr       = color.ORANGE,
	minlevel   = 10,
	level      = { -1 , 1 },
	proba      = 25,
	power      = 1,
	never_know = 1,
	no_message = true,
	trap = function(trap, who, y, x, obj)
			   -- Like reading a Scroll of Trap/Door Destruction;
			   -- even disarms itself.
			   local obvious = destroy_doors_touch(who, y, x)
			   obvious = disarm_traps_touch(who, y, x) or obvious
			   obvious = unlock_all_touch(who, y, x) or obvious

			   return obvious
		end
} -- Trap of Disarming

traps.remote_chain_number = 0

hook(hook.TRAP_RECURSION_START,
function()
	-- This is the first trap to be directly activated by a
	-- monster or the player, so no chain reaction has started
	-- yet.
	traps.remote_chain_number = 0
end)

-- Activate all traps adjacent to the trap
function traps.trap_activate_touch(trap, who, y, x)
	local obvious = false
	local ret1, ret2

	-- Stop runaway chain reactions
	if traps.remote_chain_number > 30 then
		return false
	end

	traps.remote_chain_number = traps.remote_chain_number + 1

	-- Don't need to wory about re-activing the trap which is doing the
	-- activating, since a trap that is in the middle of being processed
	-- will be skipped.
	for yy = y - 1, y + 1 do
		for xx = x - 1 , x + 1 do
			if in_bounds(yy, xx) then
				-- First activate all object traps at this square
				local obj_list = {}
				for_inventory_inven(cave(yy, xx).inventory,
									function(obj)
										if obj.flags[FLAG_TRAP] then
											tinsert(%obj_list, obj)
										end
									end)
				for i = 1, getn(obj_list) do
					local obj = obj_list[i]

					-- Make sure that activating one object trap hasn't
					-- removed a different one
					if obj.flags[FLAG_TRAP] and not
						obj.flags[FLAG_TRAP][FLAG_GONE] then
						ret1, ret2 =
							traps.activate_object_traps(who, obj_list[i],
														true)
					end

					obvious = obvious or ret2

					-- Has this trap, which has been activating other traps,
					-- been prematurely removed?
					if trap[FLAG_GONE] then
						return obvious
					end
				end -- for i = 1, getn(obj_list) do
				-- Done activating object traps

				-- Now activate the square's traps (if there is any)
				ret1, ret2 = traps.activate_location_traps(who, yy, xx, true)
				obvious = obvious or ret2

				-- Has this trap, which has been activating other traps,
				-- been prematurely removed?
				if trap[FLAG_GONE] then
					return obvious
				end
			end -- if in_bounds(yy, xx) then
		end -- 	for xx = x - 1, x + 1 do
	end -- 	for yy = y - 1, y + 1 do

	return obvious
end -- traps.trap_activate_touch()

traps.add {
	name       = "Trap Activation Trap",
	attr       = color.ORANGE,
	minlevel   = 15,
	level      = { -1 , 1 },
	proba      = 25,
	power      = 1,
	no_message = true,

	trap = function(trap, who, y, x, obj)
			   return traps.trap_activate_touch(trap, who, y, x)
		   end
} -- Trap Activation Trap

------------------------------------------------------------------------
-- Teleportation (white) traps
------------------------------------------------------------------------

traps.add {
	name         = "Phase Door Trap",
	attr         = color.WHITE,
	minlevel     = 2,
	level        = { -1 , 50 },
	proba        = 59,
	power        = 1,
	needs_target = true,

	message     = "@target@ suddently disappear@s@!",
	message_you = "You suddenly find yourself elsewhere!",

	trap = function(trap, who, y, x, obj)
		       -- Keep level less than 30, so that a Void
		       -- Jumpgate doesn't form
		       local level = flag_get(trap, FLAG_LEVEL)
		       if level >= 30 then
			       level = 29
		       end

		       -- Caster needs to be whoever tripped the
		       -- trap.
		       set_auto_cast(who, level, y, x)

			   local faction = who_to_faction(who)
		       local ret     = execute_school_spell(BLINK, "trap", faction)

		       unset_auto_cast()

		       return ret
		end
} -- Phase door trap

traps.add {
	name         = "Teleport Trap",
	attr         = color.WHITE,
	minlevel     = 4,
	level        = { -1 , 50 },
	proba        = 45,
	power        = 1,
	needs_target = true,

	message     = "@target@ suddently disappear@s@!",
	message_you = "You suddenly find yourself elsewhere!",

	trap = function(trap, who, y, x, obj)
		       local faction
		       if who == 0 then
			       faction = FACTION_PLAYER
		       else
			       faction = monster(who).faction
		       end

			   -- Save and then restore player energy, since the
			   -- teleport spell code uses up player energy, but when
			   -- it's cast by a trap it needs no energy
			   local energy = player.energy

		       -- Caster needs to be whoever tripped the
		       -- trap.
		       set_auto_cast(who, flag_get(trap, FLAG_LEVEL), y, x)

			   local faction = who_to_faction(who)
		       local ret     = execute_school_spell(TELEPORT, "trap", faction)

		       unset_auto_cast()

			   player.energy = energy

		       return ret
		end
} -- Teleport Trap

--------------------------------------------------------------------------
-- Player affecting (dark red) traps
--------------------------------------------------------------------------

traps.add {
	name        = "Greater Speed Drain Trap",
	attr        = color.YELLOW,
	minlevel    = 80,
	level       = { -1 , 1 },
	proba       = 50,
	power       = 1,
	player_only = true,
	message     = "A malicious aura of slowness rushes through @target@!",

	spell =
	function(trap, who, y, x, obj)
		set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL), y, x)

		local ret
		ret = execute_school_spell(MONST_SPELL_STAR_DRAIN_SPEED, "trap", FACTION_DUNGEON)

		unset_auto_cast()

		return ret
	end
} -- Greater Speed Drain Trap

----------------------------------------------------------------------------
-- Equipment/inventory affecting (yellow) traps
----------------------------------------------------------------------------

traps.add {
	name        = "Trap of Curse Armor",
	attr        = color.YELLOW,
	minlevel    = 20,
	level       = { -1 , 1 },
	proba       = 10,
	power       = 1,
	player_only = true,
	message     = "Your armor doesn't exactly get better by setting " ..
		"of this trap...",
	trap = function(trap, who, y, x, obj)
			set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL),
				      y, x)

			local ret
			ret = execute_school_spell(MONST_SPELL_CURSE_ARMOR, "trap", FACTION_DUNGEON)

			return ret
		end
} -- Trap of Curse Armor

traps.add {
	name        = "Trap of Curse Weapon",
	attr        = color.YELLOW,
	minlevel    = 20,
	level       = { -1 , 1 },
	proba       = 10,
	power       = 1,
	player_only = true,
	message     = "Your weapon will never be the same...",
	trap = function(trap, who, y, x, obj)
			set_auto_cast(WHO_TRAP, flag_get(trap, FLAG_LEVEL),
				      y, x)

			local ret
			ret = execute_school_spell(MONST_SPELL_CURSE_WEAPON, "trap", FACTION_DUNGEON)

			unset_auto_cast()

			return ret
		end
} -- Trap of Curse Weapon

--------------------------------------------------------------------------
-- Summoning (purple/violet) traps
--------------------------------------------------------------------------

function traps.spot_ok_default(y, x)
	local c_ptr      = cave(y, x)
	local feat       = c_ptr.feat
	local feat_flags = f_info[feat + 1].flags

	if feat == FEAT_GLYPH then
		return false
	end

	if not feat_flags[FLAG_FLOOR] or feat_flags[FLAG_WALL] then
		return false
	end

	return true
end

function traps.spot_ok_by_monst(y, x, monst)
	local c_ptr      = cave(y, x)
	local feat       = c_ptr.feat
	local feat_flags = f_info[feat + 1].flags

	if feat == FEAT_GLYPH then
		return false
	end

	return monst_can_pass_square(monst, y, x)
end

function traps.find_summon_spot(yy, xx, checker, check_arg)
	if not checker then
		checker = traps.spot_ok_default
	end

	local spots = {}

	for y = yy - 1, yy + 1 do
		for x = xx - 1, xx + 1 do
			if not in_bounds(y, x) then
				-- Bad
			elseif y == player.py and x == player.px then
				-- Bad
			elseif cave(y, x).m_idx > 0 then
				-- Bad
			else
				-- Might be okay
				if checker(y, x, check_arg) then
					tinsert(spots, {y, x})
				end
			end

		end
	end

	if getn(spots) == 0 then
		return nil, nil
	end

	local spot = spots[rng.number(getn(spots)) + 1]

	return spot[1], spot[2]
end

traps.add {
	name        = "Summon Monster Trap",
	attr        = color.VIOLET,
	minlevel    = 2,
	level       = { -1 , 1 },
	proba       = 10,
	power       = 1,
	trap =
		function(trap, who, y, x, obj)
			local yy, xx = traps.find_summon_spot(y, x)

			if not yy or not xx then
				return false
			end

			local old_level, ret

			old_level = dun_level
			dun_level = trap[FLAG_LEVEL]
			ret       = place_monster(yy, xx, false, false)
			dun_level = old_level

			return ret
		end
} -- Summon Monster Trap

traps.add {
	name        = "Trap of Calling Out",
	attr        = color.VIOLET,
	minlevel    = 2,
	level       = { -2 , 1 },
	proba       = 10,
	power       = 1,
	trap =
		function(trap, who, y, x, obj)
			local has_floor = false
			local fly       = 16000
			local can_pass  = {}

			-- Find minimum requirements for a monster to be placed
			-- near the trap
			for zy = y - 1, y + 1 do
				for zx = x - 1, x + 1 do
					local c_ptr      = cave(zy, zx)
					local feat       = c_ptr.feat
					local feat_flags = f_info[feat + 1].flags

					if not in_bounds(zy, zx) then
						-- Bad
					elseif zy == player.py and zx == player.px then
						-- Bad
					elseif cave(zy, zx).m_idx > 0 then
						-- Bad
					elseif feat == FEAT_GLYPH then
						-- Bad
					else
						-- Might be okay
						if feat_flags[FLAG_FLOOR] then
							has_floor = true
						end
						if feat_flags[FLAG_CAN_FLY] then
							if feat_flags[FLAG_CAN_FLY] < fly then
								fly = feat_flags[FLAG_CAN_FLY]
							end
						end
						if feat_flags[FLAG_CAN_PASS] then
							local pass_flags = feat_flags[FLAG_CAN_PASS]
							local max_key    = flag_max_key(pass_flags)
							for i = 1, max_key do
								if pass_flags[i] then
									if not can_pass[i] or
										pass_flags[i] < can_pass[i] then
										can_pass[i] = pass_flags[i]
									end
								end
							end
						end
					end -- else
				end
			end -- Find minimum requirements

			if not has_floor and getn(can_pass) == 0 and fly == 16000 then
				-- All spots where monsters could be placed are either
				-- occupied or can't have any monster placed in them
				-- (like permanent walls)
				if wizard then
					message("Nowhere to place monster")
				end
				return false
			end

			local info = {}
			info.max_lev   = -1
			info.max_m_idx = -1

			-- Find highest level monster which meets the minimum
			-- requirements for being placed near the trap
			for_each_monster(
				function(m_idx, monst)
					if monst.level <= %info.max_lev then
						return
					end

					-- Don't snatch up monsters already next to the
					-- trap
					if abs(monst.fx - %x) <= 1 and
						abs(monst.fy - %y) <= 1 then
						return
					end

					if %has_floor then
						-- Okay
					elseif monst.flags[FLAG_CAN_FLY] and
						monst.flags[FLAG_CAN_FLY] >= %fly then
						-- Okay
					elseif monst.flags[FLAG_PASS_WALL] then
						-- Monster MIGHT be able to pass through terain
						local pass_flags = monst.flags[FLAG_PASS_WALL]
						local max_key = flag_max_key(pass_flags)
						local pass = false

						for i = 1, max_key do
							if %can_pass[i] > 0 and
								pass_flags[i] >= %can_pass[i] then
								-- Okay
								pass = true
								break
							end
						end

						if not pass then
							-- Bad
							return
						end
					else
						-- No floor, can't fly over terrain, and monster
						-- can't pass through ANY terrain, so BAD
						return
					end
					%info.max_lev = monst.level
					%info.max_m_idx = m_idx

				end
			) -- Find highest level monster which could be placed

			if info.max_m_idx == -1 then
				if wizard then
					message("No matching monster found")
				end
				return false
			end

			local monst = monster(info.max_m_idx)

			local yy, xx = traps.find_summon_spot(y, x,
												  traps.spot_ok_by_monst,
												  monst)

			if not yy or not xx then
				if wizard then
					local race = r_info[monst.r_idx + 1]

					message("Couldn't place selected monster: " .. race.name)
				end
				return false
			end

			-- Now we move the monster from one spot to the other
			local old_my = monst.fy
			local old_mx = monst.fx

			monst.fy = yy
			monst.fx = xx

			cave(yy, xx).m_idx = info.max_m_idx

			cave(old_my, old_mx).m_idx = 0

			return true
		end
} -- Trap of Calling Out

---------------------------

traps.magic_disarm = function(radius)
	-- XXX TODO
	message("traps.magic_disarm unimplemented!")
	return true
end
