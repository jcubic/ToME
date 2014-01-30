-- Flags used by the ToME module

-- To store a <y,x> coordinate; uses both vals of a flag
new_flag("COORD")

-- For changed features that might change back, like glyphs
new_flag("ORIG_FEAT")

-- For warding and explosive glyphs
new_flag("GLYPH_POWER")

-- What feature will this tree turn into when it dies?  This
-- flag implies that the current feature is a living tree.
new_flag("DEAD_TREE_FEAT")

-- CAN_PASS and PASS_WALL flags
new_flag("PASS_WEB")
new_flag("PASS_MOUNTAIN")
new_flag("PASS_ICE")
new_flag("PASS_TREES")

memory.flag_misc_add{
	{FLAG_PASS_WALL, FLAG_PASS_WEB, "pass through webs"},
	{FLAG_PASS_WALL, FLAG_PASS_MOUNTAIN, "climb over mountains"},
	{FLAG_PASS_WALL, FLAG_PASS_ICE, "pass through ice"},
	{FLAG_PASS_WALL, FLAG_PASS_TREES, "pass through trees"},
}

-- Substance flags
new_flag("SUBST_WEB")
new_flag("SUBST_ICE")
new_flag("SUBST_TREE")
new_flag("SUBST_SAND")
new_flag("SUBST_MAGMA")
new_flag("SUBST_QUARTZ")
new_flag("EASY_DIG")

memory.flag_misc_add{
	{FLAG_KILL_WALL, FLAG_DEAD_TREE_FEAT, "cut down living trees"},
	{FLAG_KILL_WALL, FLAG_SUBST_TREE, "cut down trees"},
	{FLAG_KILL_WALL, FLAG_SUBST_WEB, "cut down webs"},
	{FLAG_KILL_WALL, FLAG_SUBST_ICE, "bore through ice"},
	{FLAG_KILL_WALL, FLAG_SUBST_MAGMA, "bore through magma"},
	{FLAG_KILL_WALL, FLAG_SUBST_QUARTZ, "bore through quartz"},
	{FLAG_KILL_WALL, FLAG_SUBST_SAND, "dig through sand"},
	{FLAG_KILL_WALL, FLAG_EASY_DIG, "dig through soft material"},
}

constant("subst_desc",
{
	[FLAG_SUBST_ROCK]     = "rock",
	[FLAG_SUBST_GRANITE]  = "granite",
	[FLAG_SUBST_RUBBLE]   = "rubble",
	[FLAG_SUBST_WEB]      = "webs",
	[FLAG_SUBST_ICE]      = "ice",
	[FLAG_DEAD_TREE_FEAT] = "living trees",
	[FLAG_SUBST_TREE]     = "trees",
	[FLAG_SUBST_SAND]     = "sand",
	[FLAG_SUBST_MAGMA]    = "magma",
	[FLAG_SUBST_QUARTZ]   = "qaurtz",
})

-- Alchemy
new_flag("ALCHEMY_SRC")
new_flag("ALCHEMY_DEST")

-- Probability travel
new_flag("PROB_TRAVEL_UPDOWN")
new_flag("PROB_TRAVEL_WALLS")
new_flag("PROB_TRAVEL_NO_UPDOWN")
new_flag("PROB_TRAVEL_NO_WALLS")
new_flag("PROB_TRAVEL_NO_END")

-- Fun things
new_flag("ABSORBED_LEVELS")

-- Monster flags
new_flag("TOWN_PEOPLE_CAN_HUMAN")
new_flag("TOWN_PEOPLE_CAN_ELF")
new_flag("TOWN_PEOPLE_CAN_DWARF")
new_flag("TOWN_PEOPLE_CAN_HOBBIT")

new_flag("ALLOW_IN_TOWN_BREE")

new_flag("ALLOW_IN_EVERYWHERE")
new_flag("ALLOW_IN_FOREST")
new_flag("ALLOW_IN_DESERT")
new_flag("ALLOW_IN_PLAIN")
new_flag("ALLOW_IN_CAVE")
new_flag("ALLOW_IN_SWAMP")
new_flag("ALLOW_IN_OCEAN")
new_flag("ALLOW_IN_RIVER")
new_flag("ALLOW_IN_MOUNTAIN")
new_flag("ALLOW_IN_COLD")
new_flag("ALLOW_IN_LAVA")

new_flag("ALLOW_IN_BARROW_DOWNS")
new_flag("ALLOW_IN_OLD_FOREST")
new_flag("ALLOW_IN_MIRKWOOD")

new_flag("ORC")
new_flag("TROLL")
new_flag("GIANT")
new_flag("DRAGON")
new_flag("DEMON")
new_flag("UNDEAD")
new_flag("EVIL")
new_flag("ANIMAL")
new_flag("HUMAN")
new_flag("CORRUPTED")
new_flag("THUNDERLORD")
new_flag("GOOD")
new_flag("MAMMAL")
new_flag("AERIAL")
new_flag("INSECT")
new_flag("DWARF")
new_flag("HOBBIT")

-- For monsters whose AC, resists and abosrbs aren't inate (thick skin,
-- scales, etc), but due to worn armour which the player won't
-- necessarily wear (and that doesn't necessarily make it into
-- the monster's object drop).
new_flag("NON_INNATE_ARMOUR")

-- Trap kit (monster trap) flags
new_flag("TRAPKIT_OBJ")
new_flag("AMMO_OBJ")
new_flag("AMMO_PER_TRIGGER")
new_flag("SELECT_AMMO")
new_flag("NO_AMMO")
new_flag("HIDDENNESS")
new_flag("DIFFICULTY")
new_flag("REARM_CHANCE")
new_flag("TRAPKIT_TRIP")
new_flag("ON_TRAPKIT")

new_flag("MONST_DISARM_SKILL")
new_flag("TRAP_NOTED")    -- Monster has already discovered a player trap
new_flag("TRAP_DISARMED") -- Monster has already disarmed a player trap
new_flag("TRAP_CANT_TRIP")
new_flag("AMMO_EXHAUSTED")
new_flag("TRAPKIT_UNSET")
new_flag("REARM_PRE")
new_flag("REARM_POST")

-- Player metabolism/hunger/food-usage
new_flag("METAB_PCT") -- Set metabolism to X% of normal
new_flag("NUTRI_MOD") -- Object supplies (or consumes) nutrition

new_flag("WALK_WATER")
new_flag("STORE")

new_flag("DROP_OBJECTS")

new_flag("NEVER_RANDOM_EGO")
new_flag("ALWAYS_MATERIAL")

-- Magical intrinsics
new_flag("MELEE_CONFUSE")

new_flag("SYMBIOTE")

-- Corpse flags
new_flag("MONSTER_CHP")

-- Hooks to provide self knolwedge
new_flag("SELF_KNOWLEDGE", true)

-- For possessors
new_flag("MANY_SOULS")

-------------------------------------------------------
-- ESP flag descriptions
-------------------------------------------------------
constant("esp", {})
settag(esp, TAG_NAMESPACE) -- Tag as a namespace

esp.race_flags = {
	[FLAG_ANIMAL]          = true,
	[FLAG_CORRUPTED]       = true,
	[FLAG_DEMON]           = true,
	[FLAG_DRAGON]          = true,
	[FLAG_ELDRITCH_HORROR] = true,
	[FLAG_GIANT]           = true,
	[FLAG_HUMAN]           = true,
	[FLAG_NAZGUL]          = true,
	[FLAG_ORC]             = true,
	[FLAG_SPIDER]          = true,
	[FLAG_THUNDERLORD]     = true,
	[FLAG_TROLL]           = true,
	[FLAG_UNDEAD]           = true,
}

function esp.desc_flag(flag)
	local name = get_flag_name(flag)

	if name == "ESP" then
		return "everything"
	end

	if starts_with(name, "ESP_") then
		name = strsub(name, 5, -1)
	end

	name = strlower(name)

	if esp.race_flags[flag] then
		return (name .. "s")
	end

	return (name .. " beings")
end
