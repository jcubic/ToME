constant("objects", {})

-- Give detailed info for non-ID'd objects; generic_or_specific() will
-- ensure that only information on generic objects of that
-- kind will be displayed
object_desc_configuration.details_require_id = false

---- Objects classification
define_objects_categorization
{
	{ "MSTAFF",
		{
			"MAGE_STAFF",
			"CLOUD_STAFF",
			"STONE_STAFF",
			"FLOWING_STAFF",
			"FIRE_STAFF",
			"ARCANE_STAFF",
		},
	},

	{ "POLEARM",
		{
			"PIKE",
			"GLAIVE",
			"FAUCHARD",
			"LANCE",
			"GUISARME",
			"HARAD_SPEAR",
			"LUCERN_HAMMER",
			"HALBERD",
			"TRIDENT",
		},
	},

	{ "AXE",
		{
			"FLINT_AXE",
			"BISCAYAN_AXE",
			"WOODSMAN_AXE",
			"EXECUTION_AXE",
			"RHUNIC_AXE",
			"LABRYS",
			"DANE_AXE",
			"HATCHET",
			"BARUK_KHAZAD"
		},
	},

	{ "HAFTED",
		{
			"CLUB",
			"HAND_HAMMER",
			"FLAIL",
			"THREE_BALL_FLAIL",
			"MACE",
			"WAR_HAMMER",
			"SPIKED_FLAIL",
			"SPIKED_MACE",
			"GREAT_HAMMER"
		},
	},

	{ "KNIFE",
		{
			"KNIFE",
			"MISERICORDE",
			"MAIN_GAUCHE",
			"DAGGER",
			"ATHAME",
			"SAX",
			"DIRK",
			"BASILARD",
			"STILETTO",
			"NECROMANTIC_TEETH"
		}
	},

	{ "SWORD",
		{
			"RAPIER",
			"SCHIAVONA",
			"SABRE",
			"CUTLASS",
			"BASTARD_SWORD",
			"SCIMITAR",
			"FLAMMARD",
			"ESPADON",
			"CLAYMORE"
			"DARK_SWORD"
		},
	},

	{ "BOW",
		{
			"SHORT_BOW",
			"HUNTING_BOW",
			"LONG_BOW",
			"WAR_BOW",
		},
	},

	{ "ARROW",
		{
			"BODKIN_ARROW"
			"SWALLOWTAIL_ARROW",
			"HUNTING_ARROW",
		},
	},

	{ "SLING",
		{
			"SHORT_SLING",
			"HUNTING_SLING",
			"LONG_SLING",
			"STAFF_SLING",
		},
	},

	{ "SHOT",
		{
			"SLING_STONE",
			"BULLET",
			"BARBED_BULLET"
		},
	},

	{ "DEMON_ITEM",
		{
			"DEMON_WEAPON", -- Claw, whip, whatever
			"DEMON_SHIELD",
			"DEMON_HORN",
		},
	},

	{ "LIGHT_ARMOUR",
		{
			"FILTHY_RAG",
			"ROBE",
			"SOFT_LEATHER_JACKET",
			"SOFT_STUDDED_JACKET",
			"HARD_LEATHER_JACKET",
			"HARD_STUDDED_JACKET",
			"LEATHER_SCALE_MAIL",
			"RING_MAIL",
			"DRAGON_SCALE_MAIL",
		},
	},

	{ "HEAVY_ARMOUR",
		{
			"SPLINT_MAIL",
			"METAL_SCALE_MAIL",
			"CHAIN_MAIL",
			"AUGMENTED_CHAIN_MAIL",
			"BRIGANDINE_MAIL",
			"PARTIAL_PLATE_MAIL",
			"FULL_PLATE_MAIL",
			"DRAGON_PLATE_MAIL",
		},
	},

	{ "LIGHT_HELM",
		{
			"LEATHER_CAP",
			"HARD_CAP",
			"CROWN",
			"DRAGON_LEATHER_CAP",
		},
	},
	{ "HEAVY_HELM",
		{
			"PARTIAL_HELM",
			"FULL_HELM",
			"VISORED_HELM",
			"DRAGON_HELM",
		},
	},
	{ "CLOAK",
		{
			"COTTON_CLOAK",
			"LEATHER_CLOAK",
			"WOOLEN_CLOAK",
		},
	},
	{ "GLOVES",
		{
			"LEATHER_GLOVES",
			"HEAVY_LEATHER_GLOVES",
			"GAUNTLETS",
		},
	},
	{ "SHIELD",
		{
			"WOODEN_SHIELD",
			"BUCKLER"
			"HOPLITE",
			"SCUTUM",
			"PAVIS"
		},
	},
	{ "BOOTS",
		{
			"SANDALS",
			"LEATHER_BOOTS",
			"REINFORCED_BOOTS"
		},
	},
	{ "SCROLL",
		{
			"SCROLL_NOTHING",
			"SCROLL_SATISFY_HUNGER",
			"SCROLL_CONFUSE_MONSTER",
			"SCROLL_DARKNESS",
			"SCROLL_DETECT_TRAPS",
			"SCROLL_LIGHT",
			"SCROLL_PHASE_DOOR",
			"SCROLL_DETECT_OBJECTS",
			"SCROLL_DETECT_INVISIBLE",
			"SCROLL_LOCATE_DOORS",
			"SCROLL_VALARIN_FAVOR",
			"SCROLL_DESTROY_DOORS",
			"SCROLL_IDENTIFICATION",
			"SCROLL_SUMMON_MONSTER",
			"SCROLL_REMOVE_CURSE",
			"SCROLL_AGGRAVATE_MONSTER",
			"SCROLL_MAGIC_MAPPING",
			"SCROLL_WORD_OF_RECALL",
			"SCROLL_TELEPORTATION",
			"SCROLL_BLAST_WEAPON",
			"SCROLL_SUMMON_NEVER_MOVING_PET",
			"SCROLL_CREATE_TRAPS",
			"SCROLL_RECHARGING",
			"SCROLL_DESTRUCTION",
			"SCROLL_GENOCIDE",
			"SCROLL_BLAST_ARMOUR",
			"SCROLL_SUMMON_SYMBIOTE",
			"SCROLL_VALARIN_BLESSING",
			"SCROLL_MASS_RESURECTION",
			"SCROLL_TELEPORT_LEVEL",
			"SCROLL_GREATER_REMOVE_CURSE",
			"SCROLL_RESET_RECALL",
			"SCROLL_GREATER_IDENTIFICATION",
			"SCROLL_PROTECTION_FROM_EVIL",
			"SCROLL_SUMMON_UNDEAD",
			"SCROLL_AMNESIA",
			"SCROLL_VALARIN_STRENGTH",
			"SCROLL_MASS_GENOCIDE",
			"SCROLL_SUMMON_DRAGONS",
			"SCROLL_GREATER_RECHARGING",
			"SCROLL_RUNE_OF_PROTECTION",
			"SCROLL_DEINCARNATION",
			"SCROLL_DIVINATION",
		},
	},
	{ "POTION",
		{
			"POTION_SALT_WATER",
			"POTION_APPLE_JUICE",
			"POTION_WATER",
			"POTION_LIGHT_HEALING",
			"POTION_INFRA_VISION",
			"POTION_SLIME_MOLD_JUICE",
			"POTION_SPEED",
			"POTION_RESIST_HEAT",
			"POTION_RESIST_COLD",
			"POTION_LIGHT_CURING",
			"POTION_UGLINESS",
			"POTION_BOLDNESS",
			"POTION_LIGHT_POISON",
			"POTION_WEAKNESS",
			"POTION_BLINDNESS",
			"POTION_CURE_POISON",
			"POTION_INVISIBILITY",
			"POTION_SEE_INVISIBLE",
			"POTION_AUGMENTATION",
			"POTION_RESTORE_CHARISMA",
			"POTION_RESTORE_STRENGTH",
			"POTION_BOOZE",
			"POTION_NAIVETY",
			"POTION_STUPIDITY",
			"POTION_SICKLINESS",
			"POTION_CLUMSINESS",
			"POTION_HEALING",
			"POTION_CURING",
			"POTION_RESTORE_INTELLIGENCE",
			"POTION_RESTORE_CONSTITUTION",
			"POTION_RESTORE_MEMORIES",
			"POTION_ENLIGHTENMENT",
			"POTION_CORRUPTION",
			"POTION_POISON",
			"POTION_STUNNING",
			"POTION_DETONATIONS",
			"POTION_EXPERIENCE",
			"POTION_SELF_KNOWLEDGE",
			"POTION_RESTORE_WISDOM",
			"POTION_RESTORE_DEXTERITY",
			"POTION_RUINATION",
			"POTION_RESISTANCE",
			"POTION_LOSE_MEMORIES",
			"POTION_PARALYZATION",
			"POTION_BERSERK_STRENGTH",
			"POTION_RESTORE_MANA",
			"POTION_FLYING",
			"POTION_GREAT_SPEED",
			"POTION_SLOWNESS",
			"POTION_GREAT_HEALING",
			"POTION_GREAT_POISON",
			"POTION_DISEASE",
			"POTION_FIRE",
			"POTION_COLD",
			"POTION_LIFE",
			"POTION_DEATH",
			"POTION_BLOOD_OF_LIFE",
			"POTION_INVULNERABILITY",
			"POTION_GREAT_SLOWNESS",
			"POTION_LEARNING",
		},
	},
	{ "BOTTLE",
		{
			"BOTTLE_EMPTY",
		},
	},
	{ "LITE",
		{
			"LITE_TORCH",
			"LITE_LANTERN",
			"LITE_FEANORIAN",
		},
	},
	{ "FUEL",
		{
			"FUEL_OIL",
		},
	},
	{ "SYMBIOTE",
		{
			"SYMBIOTE",
		}
	},
	{ "TOTEM",
		{
			"TOTEM_PARTIAL",
			"TOTEM_TRUE",
		}
	},
	-----------------------------------------------------------
	-- Below this point has not been rewritten
	-----------------------------------------------------------
	{ "TOOL",
		{
			"SHOVEL",
			"PICK",
			"THIEVES_TOOLS",
		},
	},
	{ "AMULET",
		{
			"TEST_AMULET",
			"POWERFUL_AMULET",
		},
	},
	--[[{ "AMULET",
		{
			"AMULET_RESIST_ACID",
			"AMULET_RESIST_ELEC",
			"AMULET_SLOW_DIGEST",
			"AMULET_PORTABLE_HOLE",
			"AMULET_EYE_OF_SMAUG",
		},
	}, --]]
	{ "RING",
		{
			"RING_SEARCHING",
			"RING_SLOW_DIGEST",
			"RING_RESIST_FEAR",
			"RING_RESIST_FIRE",
			"RING_RESIST_COLD",
			"RING_RESIST_LITE_AND_DARK",
			"RING_RESIST_SOUND",
			"RING_RESIST_CONFUSION",
			"RING_RESIST_SHARDS",
			"RING_RESIST_NEXUS",
			"RING_RESIST_CHAOS",
			"RING_RESIST_BLINDNESS",
			"RING_RESIST_DISENCHANT",
			"RING_SLAYING",
			"RING_SPEED",
			"RING_FLYING",
			"RING_LEVITATION",
			"RING_PROTECTION",
			"RING_SPELL_MASTERY",
			"RING_ONE",
			"RING_NARYA",
		},
	},
	{ "CHEST",
		{

		},
	},
	{ "GOLD",
		{

		},
	},
	{ "FOOD",
		{
			"FOOD_RATION",
			"FOOD_BISCUIT",
			"FOOD_VENISON",
			"FOOD_ALE",
			"FOOD_WINE"
		},
	},
	{ "MUSHROOM",
		{
			"MUSHROOM_TEST",
		}
	},
	{ "SPIKE",
		{

		},
	},
	{ "PARCHMENT",
		{

		},
	},
	{ "JUNK",
		{
			"BROKEN_HILT",
			"BROKEN_BLADE",
			"JUNK_SLIMY_THING",
			"JUNK_STICKY_WOOD",
			"JUNK",
		},
	},
	{ "MOUNT",
		{
			"MOUNT",
		},
	},
	{ "TRAPKIT",
		{

		},
	},
	{ "GEMSTONE",
		{
			"GEM_AGATE",
			"GEM_RUBIS",
			"GEM_DIAMOND",
		},
	},
	{ "WAND",
		{
			"WAND_LESSER",
			"WAND",
			"WAND_GREATER",
		},
	},
	{ "ORB",
		{
			"ORB_LESSER",
			"ORB",
			"ORB_GREATER",
		},
	},

	{ "BOOK",
		{
			"BOOK_RANDOM",
			"BOOK_CANTRIPS",
			"BOOK_BASIC_ELEMENTS",
			"BOOK_MANA",
			"BOOK_FIRE",
			"BOOK_AIR",
			"BOOK_EARTH",
			"BOOK_WATER",
			"BOOK_TRANSLOCATION",
			"BOOK_NATURE",
			"BOOK_DIVINATION",
			"BOOK_TIME",
			"BOOK_META",
			"BOOK_MIND",
			"BOOK_UDUN",
			"BOOK_ERU",
			"BOOK_MANWE",
			"BOOK_TULKAS",
			"BOOK_MELKOR",
			"BOOK_YAVANNA",
		},
	},

	{ "RUNE1",
		{
			"RUNE_FIRE",
			"RUNE_COLD",
			"RUNE_LIGHTNING",
			"RUNE_ACID",
			"RUNE_ELEMENT",
			"RUNE_CHAOS",
			"RUNE_MIND",
			"RUNE_KNOWLEDGE",
			"RUNE_HOLDING",
			"RUNE_GRAVITY",
			"RUNE_UNDEATH",
			"RUNE_LIFE",
			"RUNE_PROTECTION",
		}
	},

	{ "RUNE2",
		{
			"RUNE_ARROW",
			"RUNE_RAY",
			"RUNE_SPHERE",
			"RUNE_WAVE",
			"RUNE_SWARM",
			"RUNE_VIEW",
			"RUNE_SELF",
			"RUNE_POWER_SURGE",
			"RUNE_ARMAGEDDON",
			"RUNE_FUTURE",
			"RUNESTONE",
		}
	},

	{ "BACKPACK",
		{
			"BACKPACK_DEFAULT",
			"BACKPACK_LARGE",
			"BACKPACK_HUGE",
		},
	},

	{ "QUIVER",
		{
			"QUIVER_SMALL",
		},
	},
}

-- Add object theme scripts
tome_dofile("object_theme.lua")

----------------------------------------------------------------------------

-- The primary damage types used in combat

combat_dam_types = {dam.SLASH, dam.CRUSH, dam.PIERCE}

----------------------------------------------------------------------------

--
-- Automizer customization.
--

-- Figure "quality" string for identified objects.

function objects.by_value(obj, very_bad, bad, good, very_good)
	local value = object_value_real(obj)

	if value <= very_bad then
		return "very bad"
	elseif value > very_bad and value <= bad then
		return "bad"
	elseif value > bad and value < good then
		return "average"
	elseif value >= good and value < very_good then
		return "good"
	elseif value >= very_good then
		return "very good"
	else
		return "Impossible!"
	end
end -- objects.by_value()

function objects.by_base_value(obj, good_pct, very_good_pct, very_bad_pct)
	local value = object_value_real(obj)
	local base  = k_info(obj.k_idx).cost

	-- Don't divide by zero, or deal with objects have a negative
	-- base value.
	if base <= 0 then
		return objects.by_value(obj, -100, -1, 100, 1000)
	end

	local pct = (value * 100 / base) - 100

	if pct < good_pct and pct >= 0 then
		return "average"
	elseif pct >= good_pct and pct < very_good_pct then
		return "good"
	elseif pct > very_good_pct then
		return "very good"
	elseif pct < 0 and pct > very_bad_pct then
		return "bad"
	elseif pct <= very_bad_pct then
		return "very bad"
	else
		error("Impossible!")
	end
end -- objects.by_base_value()

function objects.quality_1_100(object)
	return objects.by_base_value(object, 1, 100, -50)
end

auto.add_quality_by_tvals {
	[INVEN_MAINHAND] = objects.quality_1_100,
	[INVEN_OFFHAND]  = objects.quality_1_100,
	[INVEN_RING]     = objects.quality_1_100,
	[INVEN_NECK]     = objects.quality_1_100,
	[INVEN_LITE]     = objects.quality_1_100,
	[INVEN_BODY]     = objects.quality_1_100,
	[INVEN_OUTER]    = objects.quality_1_100,
	[INVEN_HANDS]    = objects.quality_1_100,
	[INVEN_FEET]     = objects.quality_1_100,
	[INVEN_TOOL]     = objects.quality_1_100,
	[INVEN_QUIVER]   = objects.quality_1_100,
	[INVEN_AMMO]     = objects.quality_1_100,
	[INVEN_BACKPACK] = objects.quality_1_100,
}

auto.add_quality_by_slots = {
	[TV_FOOD]   = objects.quality_std_1_100,
	[TV_SCROLL] = objects.quality_std_1_100,
	[TV_BOOK]   = objects.quality_std_1_100,
	[TV_POTION] = objects.quality_std_1_100,
	[TV_WAND]   = objects.quality_std_1_100,
	[TV_ORB]    = objects.quality_std_1_100,
}

----------------------------------------------------------------------------

--
-- Flavours
--

function flavour.easy_know(k_idx)
	local item = k_info(k_idx)

	-- Analyze the item
	local ok = function() return true end
	local no_norm_art = function(item) return not item.flags[FLAG_NORM_ART] end
	local is_easy_know = function(item) return item.flags[FLAG_EASY_KNOW] end
	local switch =
	{
		[TV_FUEL] = ok,
		[TV_BOTTLE] = ok,
		[TV_CORPSE] = ok,
		[TV_SPIKE] = ok,

		[TV_FOOD] = no_norm_art,
		[TV_POTION] = no_norm_art,
		[TV_SCROLL] = no_norm_art,

		[TV_RING] = is_easy_know,
		[TV_AMULET] = is_easy_know,
		[TV_LITE] = is_easy_know,
	}
	if switch[item.tval] then return switch[item.tval](item)
	else return nil
	end
end

-------------------------------------------------------------------------

constant("chests", {})
function chests.cleanup(obj)
	object_level = dun_level

	-- ToME chests can't be closed again
	flag_remove(obj.flags, FLAG_OPEN)

	-- Just to be paranoid, make sure that it never drops anything
	-- again
	set_flag(obj, FLAG_AMOUNT, 0)
	flag_remove(obj.flags, FLAG_OPEN_POST)
end

-- Idetnifying a chest makes you aware of it's trapped status
hook(hook.IDENTIFY_POST,
function(obj, slot, type)
	if obj.tval == TV_CHEST and has_flag(obj, FLAG_TRAP) then
		set_flag(obj, FLAG_TRAP_AWARE, 1)
	end
end)

-------------------------------------------------------------
-- Randomly select a single spell for a book
-------------------------------------------------------------
constant("spell_books", {})

hook(hook.GAME_START,
function()
	spell_books.allowed_schools = {
		[SCHOOL_AIR] = true
		[SCHOOL_CONVEYANCE] = true
		[SCHOOL_DIVINATION] = true
		[SCHOOL_EARTH] = true
		[SCHOOL_ERU] = true
		[SCHOOL_FIRE] = true
		[SCHOOL_MANA] = true
		[SCHOOL_MANWE] = true
		[SCHOOL_MELKOR] = true
		[SCHOOL_META] = true
		[SCHOOL_MIND] = true
		[SCHOOL_NATURE] = true
		[SCHOOL_TEMPORAL] = true
		[SCHOOL_TULKAS] = true
		[SCHOOL_UDUN] = true
		[SCHOOL_ULMO] = true
		[SCHOOL_WATER] = true
		[SCHOOL_YAVANNA] = true
	}
end)

spell_books.alloc_table_max_lev = -1

function spell_books.get_alloc_table(max_level)
	if max_level == spell_books.alloc_table_max_lev then
		return spell_books.alloc_table
	end

	spell_books.alloc_table         = {}
	spell_books.alloc_table_max_lev = max_level

	local allowed_spells  = {}
	local max_level_found = -1

	for i = 1, __tmp_spells_num do
		if not __spell_school[i] then
			break
		end
		if spell_books.allowed_schools[__spell_school[i][1]] then
			if spell(i).skill_level <= max_level then
				local entry = {idx = i, level = spell(i).skill_level}
				tinsert(allowed_spells, entry)
				if spell(i).skill_level > max_level_found then
					max_level_found = spell(i).skill_level
				end
			end
		end
	end

	local sort_func = function(a, b)
			return a.level < b.level
		end

	sort(allowed_spells, sort_func)

	local alloc_sum = 0
	for i = 1, getn(allowed_spells) do
		local spell  = allowed_spells[i]
		local chance = max_level_found - spell.level + 1

		alloc_sum = alloc_sum + chance

		local entry = {
			idx   = spell.idx,
			alloc = alloc_sum
		}

		tinsert(spell_books.alloc_table, entry)
	end

	return spell_books.alloc_table
end -- spell_books.get_alloc_table

function spell_books.select_spell(max_level)
	local alloc_table = spell_books.get_alloc_table(max_level)

	local chance = rng(alloc_table[getn(alloc_table)].alloc)

	for i = 1, getn(alloc_table) do
		if chance < alloc_table[i].alloc then
			return alloc_table[i].idx
		end
	end

	wiz_print("Couldn't select random spell?")

	return alloc_table[1].idx
end

function spell_books.make_random_book(obj, max_level)
	local idx   = spell_books.select_spell(max(1, max_level))

	obj.flags[FLAG_SPELL_IDX] = getter.array{ [idx] = 1 }
end


-----------------

-- Sometimes bad things happen to player items
objects.blast_weapon = function()
	-- XXX TODO
	message("objects.blast_weapon unimplemented!")
	return true
end

objects.blast_armour = function()
	-- XXX TODO
	message("objects.blast_armour unimplemented!")
	return true
end

-- And sometimes good things happen to the player
objects.detect = function(radius)
	-- XXX TODO
	message("objects.detect unimplemented!")
	return true
end

objects.recharge = function(level)
	-- XXX TODO
	message("objects.recharge unimplemented!")
	return true
end

---------------------------------------------------------
------------------- Easy mode inventory -----------------
---------------------------------------------------------
local smart_inven_action = function(item,  obj)
	local ret_val = { val=true }
	local tries =
	{
		{ key='e' name="Examine" tester=inventory.examine_item_tester, action=inventory.examine }
		{ key='F' name="Refill lite" tester=lite_sources.item_tester, action=lite_sources.refill }
		{ key='q' name="Quaff" tester=potions.item_tester, action=potions.use }
		{ key='r' name="Read" tester=scrolls.item_tester, action=scrolls.use }
		{ key='a' name="Aim" tester=sticks.wand_item_tester, action=sticks.wand.aim }
		{ key='u' name="Use" tester=sticks.orb_item_tester, action=sticks.orb.use }
		{ key='c' name="Copy spell" tester=spell_copy_to_tester, action=spell_copy_to }
		{ key='A' name="Activate" tester=activations.item_tester, action=activations.use }
		{ key='w' name="Wield" tester=inventory.wield_item_tester, action=inventory.wield }
		{ key='t' name="Takeoff" tester=inventory.takeoff_item_tester, action=inventory.takeoff }
		{ key='d' name="Drop" tester=inventory.drop_item_tester, action=inventory.drop }
		{ key='k' name="Destroy" tester=inventory.destroy_item_tester, action=inventory.destroy }
	}
	local real = {}
	local real_keys = {}
	for act in tries do
		if act.tester(obj, item) then
			tinsert(real, act)
			real_keys[act.key] = act
		end
	end

	local sel = 1
	local ret, wid, hgt = term.get_size()
	local old_term = term.save_to()
	while true do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)

		display_list((hgt - getn(real)) / 2, (wid - 20) / 2, getn(real) + 1, 20, "Actions", real, 1, sel, color.LIGHT_GREEN, function(e)
			return e.key.."> "..e.name
		end)

		local raw = term.inkey()
		local key = strchar(raw)

		if raw == ESCAPE then
			term.load_from(old_term, true)
			break
		elseif key == '2' then
			sel = sel + 1
			if sel > getn(real) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(real) end
		elseif key == '\r' then
			term.load_from(old_term, true)
			real[sel].action(item)
                        break
		elseif real_keys[key] then
			term.load_from(old_term, true)
			real_keys[key].action(item)
                        break
		end

	end

	return ret_val.val
end

-- Load the inventory subsystem with  the special helper
load_subsystem("inventory",{
	show_action = smart_inven_action
})
-- We load it after inventory because it depends on it
-- And we dont want it to auloload inventory since we need specific parameters
hook.new_hook_type("CHARACTER_GENERATED")
load_subsystem("birth_auto_wield", { hook = hook.CHARACTER_GENERATED })

-----------------------------------------------------------------------------
-- Activatable objects
-----------------------------------------------------------------------------
load_subsystem("activations",
{
	get_skill_level =
		function(obj, spell_flags, who)
			return player.skill_dev
		end

	get_fail_pct =
		function(obj, spell_flags, chance, who)
			local lev = get_kind(obj).level

			-- Use artifact level instead
			if is_artifact(obj) then
				lev = a_info[1+obj.artifact_id].level
			end

			-- Confusion hurts skill
			if player.has_intrinsic(FLAG_CONFUSED) then
				chance = chance / 2
			end

			-- High level objects are harder
			chance = chance - ((lev > 50) and 50 or lev)

			if chance < 1 then
				chance = 1
			end

			-- Is it simple to use ?
			if has_flag(obj, FLAG_EASY_USE) then
				chance = chance * 10
			end

			local fail = (activations.USE_DEVICE - 1) * 100 / chance

			-- Give everyone a (slight) chance
			if (chance < activations.USE_DEVICE) then
				fail = (activations.USE_DEVICE - 1) * 100 /
					activations.USE_DEVICE

				local success = 100 - fail

				success = success / (activations.USE_DEVICE - chance + 2)

				fail = 100 - success
			end

			return fail
		end -- get_fail_pct()

	get_time =
		function(obj, spell_flags, skill, who)
			return get_player_energy(SPEED_USE)
		end
})

activations.USE_DEVICE = 3

-- Auto id stuff
-- Default auto id level is object level / 2
-- Default auto *id* level is 50
load_subsystem("auto_id", {
	default_id_level = function(obj)
		local level = k_info(obj.k_idx).level / 2
		-- Make level be at least 1, if the id_level is 0 then the
		-- object will never be auto-id'd, no matter how high the
		-- player's skill level.
		if level < 1 then
			level = 1
		end
		return level
	end
	default_gid_level =
		function(obj)
			return max(50, k_info(obj.k_idx).level)
		end
})

-- Stacking
-----------

hook(hook.OBJECT_SIMILAR, function(obj1, obj2, similar)
	if (obj1.tval == TV_FUEL) and (obj1.sval == SV_FUEL_OIL) and
	   (obj2.tval == TV_FUEL) and (obj2.sval == SV_FUEL_OIL) then
		return true, true
	end
end)

load_subsystem("lite_source", { lite_slot=INVEN_LITE })


-------------------------------------------------------------------------------------
----------------------------- Object drop and enchanting ----------------------------
-------------------------------------------------------------------------------------
load_subsystem("object_enchant_drop",{
	corpse_chance = function() return 10 + get_skill_scale(SKILL_PRESERVATION, 75) end
	treasure_theme_flag = FLAG_THEME_TREASURE
	chances = {
		artifact = function(obj_level, item_kind) return 1 + rescale(obj_level, 100, 7) + luck(-2, 2) end
		ego = function(obj_level, item_kind, ego_info) return 3 + rescale(obj_level, 100, 15) + luck(-3, 3) end
		double_ego = function(obj_level, item_kind, ego_info) return 4 + luck(-3, 3) end
	}
	-- The slot ranges from 1 to 5, 1 and 2 are the default prefix and suffix
	-- The higher the number the more to the right the name goes when displaying item name
	extra_ego_slots =
	{
		-- Add a special material sometimes
		material = { slot=5 chance=function(obj_level, obj, power) return obj.flags[FLAG_ALWAYS_MATERIAL] and 100 or 8 + rescale(obj_level, 100, 18) + luck(-5, 5) end }
	}
	on_make_all = function (obj, power)
		local r, m
		if power >= 1 then
			local weapon_enchant = get_flag(obj, FLAG_WEAPON_ENCHANT)
			local armor_enchant = get_flag(obj, FLAG_ARMOR_ENCHANT)

			if weapon_enchant > 0 then
				-- Start with the basic damage
				local max = 0
				local dams = obj.flags[FLAG_DAM]

				-- Compute the max damage
				if dams then
					for i = 1, dams.size do	if (dams.node[i].flags & FLAG_FLAG_USED) ~= 0 then max = max + flag_get2(dams, dams.node[i].key) end end
				end

				if weapon_enchant & object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH > 0 then
					r = max / 2
					m = max - r
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_h = obj.to_h + rng(r) + m_bonus(m, dun_level)
				end
				if weapon_enchant & object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD > 0 then
					r = max / 2
					m = max - r
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_d = obj.to_d + rng(r) + m_bonus(m, dun_level)
				end
				if weapon_enchant & object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH > 0 then
					r = 5
					m = 5
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_h = obj.to_h + rng(r) + m_bonus(m, dun_level)
				end
				if weapon_enchant & object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TD > 0 then
					r = 5
					m = 5
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_d = obj.to_d + rng(r) + m_bonus(m, dun_level)
				end
			end

			if armor_enchant > 0 then
				local v
				local max_ac = obj.ac
				local max_dr = 0
				local resist = obj.flags[FLAG_RESIST]
				local absorb = obj.flags[FLAG_ABSORB]
				if resist then
					max_dr = (flag_get(resist, dam.SLASH) + flag_get(resist, dam.CRUSH) + flag_get(resist, dam.PIERCE)) / 3
				end
				max_dr = max_dr / 3
				local max_da = 0
				if absorb then
					max_da = (flag_get(absorb, dam.SLASH) + flag_get(absorb, dam.CRUSH) + flag_get(absorb, dam.PIERCE)) / 3
				end
				max_da = max_da / 3

				if armor_enchant & object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC > 0 then
					r = max_ac / 2
					m = max_ac - r
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_a = obj.to_a + rng(r) + m_bonus(m, dun_level)
				end
				if resist and armor_enchant & object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR > 0 then
					r = max_dr / 2
					m = max_dr - r
					if power >= 2 then r = r * 2 m = m * 2 end
					v = rng(r) + m_bonus(m, dun_level)
					if flag_get(resist, dam.SLASH) > 0 then flag_inc(resist, dam.SLASH, v) end
					if flag_get(resist, dam.CRUSH) > 0 then flag_inc(resist, dam.CRUSH, v) end
					if flag_get(resist, dam.PIERCE) > 0 then flag_inc(resist, dam.PIERCE, v) end
				end
				if absorb and armor_enchant & object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DA > 0 then
					r = max_da / 2
					m = max_da - r
					if power >= 2 then r = r * 2 m = m * 2 end
					v = rng(r) + m_bonus(m, dun_level)
					if flag_get(absorb, dam.SLASH) > 0 then flag_inc(absorb, dam.SLASH, v) end
					if flag_get(absorb, dam.CRUSH) > 0 then flag_inc(absorb, dam.CRUSH, v) end
					if flag_get(absorb, dam.PIERCE) > 0 then flag_inc(absorb, dam.PIERCE, v) end
				end
				if armor_enchant & object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_AC > 0 then
					r = 10
					m = 10
					if power >= 2 then r = r * 2 m = m * 2 end
					obj.to_a = obj.to_a + rng(r) + m_bonus(m, dun_level)
				end
				if resist and armor_enchant & object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_DR > 0 then
					r = 5
					m = 5
					if power >= 2 then r = r * 2 m = m * 2 end
					v = rng(r) + m_bonus(m, dun_level)
					if flag_get(resist, dam.SLASH) > 0 then flag_inc(resist, dam.SLASH, v) end
					if flag_get(resist, dam.CRUSH) > 0 then flag_inc(resist, dam.CRUSH, v) end
					if flag_get(resist, dam.PIERCE) > 0 then flag_inc(resist, dam.PIERCE, v) end
				end
				if absorb and armor_enchant & object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_DA > 0 then
					r = 10
					m = 10
					if power >= 2 then r = r * 2 m = m * 2 end
					v = rng(r) + m_bonus(m, dun_level)
					if flag_get(absorb, dam.SLASH) > 0 then flag_inc(absorb, dam.SLASH, v) end
					if flag_get(absorb, dam.CRUSH) > 0 then flag_inc(absorb, dam.CRUSH, v) end
					if flag_get(absorb, dam.PIERCE) > 0 then flag_inc(absorb, dam.PIERCE, v) end
				end
			end
		end

		-- Save physical properties so that they can be repaired if damaged
		if has_flag(obj, FLAG_DAM) then
			local save_weapon = flag_dup(obj.flags[FLAG_DAM])
			obj.flags[FLAG_SAVE_WEAPON_PHYSICAL] = save_weapon
		end

		if has_flag(obj, FLAG_RESIST) or has_flag(obj, FLAG_ABSORB) or obj.ac ~= 0 then
			local save_armor = flag_new()
			save_armor[1] = obj.ac
			save_armor[2] = obj.flags[FLAG_RESIST] and flag_dup(obj.flags[FLAG_RESIST]) or nil
			save_armor[3] = obj.flags[FLAG_ABSORB] and flag_dup(obj.flags[FLAG_ABSORB]) or nil
			obj.flags[FLAG_SAVE_ARMOR_PHYSICAL] = save_armor
		end
	end
})

-- Types of weapons/armors enchants
safe_new_flag("WEAPON_ENCHANT")
safe_new_flag("ARMOR_ENCHANT")
safe_new_flag("SAVE_WEAPON_PHYSICAL")
safe_new_flag("SAVE_ARMOR_PHYSICAL")

object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH = 1
object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH = 2
object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD = 4
object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TD = 8

object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC = 1
object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_AC = 2
object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR = 4
object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_DR = 8
object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DA = 16
object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_DA = 32

-------------------------------------------------------------------------
-- Getting empty bottles from bottles of ale/wine
-------------------------------------------------------------------------
function food.eat_bottled(obj, food_value)
	local bottle = create_object(TV_BOTTLE, SV_BOTTLE_EMPTY)
	bottle.found      = obj.found
	bottle.found_aux1 = obj.found_aux1
	bottle.found_aux2 = obj.found_aux2
	bottle.found_aux3 = obj.found_aux3
	bottle.found_aux4 = obj.found_aux4

	local ret1, ret2 = food.eat_good(obj, food_value)

	if not ret1 then
		delete_object(bottle)
		return nil, ret2
	end

	inven_carry(bottle)

	return ret1, ret2
end

function food.destroy_bottled(obj, amnt)
	inventory.destroy_msg =
		"You pour the " .. object_desc(obj) .. " out onto the ground."

	local bottles = create_object(TV_BOTTLE, SV_BOTTLE_EMPTY)
	bottles.number = amnt

	bottles.found      = obj.found
	bottles.found_aux1 = obj.found_aux1
	bottles.found_aux2 = obj.found_aux2
	bottles.found_aux3 = obj.found_aux3
	bottles.found_aux4 = obj.found_aux4

	inven_carry(bottles)
end

-------------------------------------------------------------------------
-- Setup corpse objects
-------------------------------------------------------------------------
hook(hook.CORPSE_CREATE_POST,
function(obj, mon)
	local hp = mon.maxhp

	hp = hp - (rng(hp) / 3)

	obj.flags[FLAG_MONSTER_CHP] = hp
	mon.flags[FLAG_MONSTER_CHP] = hp

	if mon.flags[FLAG_UNIQUE] then
		-- Unique corpses are artifacts
		obj.flags[FLAG_NORM_ART] = true
		set_known(obj)
		set_aware(obj)

		-- Unique corpses don't rot
		obj.flags[FLAG_ORIG_DECAY]    = nil
		obj.flags[FLAG_MONSTER_DECAY] = nil
		obj.flags[FLAG_DECAY]         = nil
	end
end)

-------------------------------------------------------------------------
-- Hacking up corpse and related effects
-------------------------------------------------------------------------
new_flag("CUT_CORPSE_PRE")
new_flag("ON_CUT_CORPSE")

function food.default_cut_corpse_pre(corpse, meats, results)
end

function food.default_cut_corpse_effect(corpse, meats, results)
end

-- Hooks can prevent meat from being extracted by setting number of
-- objects in the meats stack to 0.
function food.cut_corpse_effect(corpse, meats, results)
	results = results or {}

	local monst = corpse.flags[FLAG_MONSTER_OBJ]
	local race  = monst.sr_ptr or r_info(monst.r_idx)
	local ego

	if monst.ego ~= 0 then
		ego = re_info(monst.ego)
	end

	local source_list = {corpse.flags}
	tinsert(source_list, monst.flags)
	tinsert(source_list, race.flags)
	if ego then
		tinsert(source_list, ego.flags)
	end

	-- Invoke various corpse cutting pre-hooks, to see if we
	-- we can cut the corpse, or if cutting the corpse has been
	-- interrupted.
	local func_list = food.get_func_list(source_list, FLAG_CUT_CORPSE_PRE,
										 food.default_cut_corpse_pre)
	for i = 1, getn(func_list) do
		if func_list[i](corpse, meats, results) then
			return
		end
	end

	-- Invoke various corpse cutting hooks, to get the effect for
	-- corpse cutting.  Will return true if the effect causes an
	-- interruption
	local func_list = food.get_func_list(source_list, FLAG_ON_CUT_CORPSE,
										 food.default_cut_corpse_effect)
	for i = 1, getn(func_list) do
		if func_list[i](corpse, meats, results) then
			return
		end
	end
end -- food.cut_corpse_effect()

function food.hack_corpse_do()
	local ret, item, obj

	ret, item = get_item("Hack up which corpse?",
						 "You don't have a corpse.",
						 USE_INVEN | USE_FLOOR,
		function (obj)
			return (obj.tval == TV_CORPSE) and (obj.sval == SV_CORPSE_CORPSE)
		end)

	if not ret then
		return
	end

	local obj = get_object(item)

	local min_weight   = obj.flags[FLAG_ORIG_WEIGHT] / 3
	local avail_weight = obj.weight - min_weight
	local meat_k_idx   = lookup_kind(TV_CORPSE, SV_CORPSE_MEAT)
	local num_pieces   = avail_weight / k_info(meat_k_idx).weight

	if num_pieces < 1 then
		message("Not enough meat left.")
		return
	end

	-- Hack off at most 10 pieces at a time
	num_pieces = min(num_pieces, 10)

	-- Prepare meats object now, so hooks can alter it
	local meats = new_object()
	object_prep(meats, meat_k_idx)
	meats.number = num_pieces

	-- Copy over "where we found it" info
	meats.found      = obj.found
	meats.found_aux1 = obj.found_aux1
	meats.found_aux2 = obj.found_aux2
	meats.found_aux3 = obj.found_aux3
	meats.found_aux4 = obj.found_aux4

	-- Copy over corpse flags, like the monster object and decay info
	flag_add(meats.flags, obj.flags)

	-- Re-copy over default food value of meat pieces
	meats.flags[FLAG_FOOD_VALUE] = k_info(meat_k_idx).flags[FLAG_FOOD_VALUE]

	-- Invoke cutting effects
	food.cut_corpse_effect(obj, meats)

	if meats.number <= 0 then
		-- Hacking up corpse interrupted or otherwise failed
		delete_object(meats)
		return
	end

	local msg = format("You hacked off %s piece%s of meat.",
					   meats.number, iif(meats.number > 1, "s", ""))
	message(msg)

	obj.weight = obj.weight - (meats.weight * meats.number)

	-- Hooks caused whole corpse to be cut up
	if obj.weight <= 0 then
		message("You have completely hacked away the corpse.")
		item_increase(item, -1)
		item_optimize(item)
	end

	if inven_carry_okay(meats) then
		local slot = inven_carry(meats, false)
		local meats = get_object(item_slot_to_item(slot))

		if meats then
			hook.process(hook.GET_POST, meats, slot)
		end
	else
		message("Unable to carry any more, you place the meat on the ground")

		if drop_near(meats, -1, player.py, player.px) == 0 then
			-- Drop faied
			delete_object(meats)
		end
	end
end -- food.hack_corpse_do()

hook(hook.KEYPRESS,
function (key)
	if key == strbyte('h') then
		food.hack_corpse_do()
		return true
	end
end)

-------------------------------------------------------------------------
-- Eating corpse effects
-------------------------------------------------------------------------
function food.default_eat_corpse_pre(obj)
end

function food.dec_stat(stat)
	local prev = player.stat(stat)
	do_dec_stat(stat, STAT_DEC_NORMAL)
	retur (player.stat(stat) < prev)
end

function food.suck_life(dice, sides)
	if not player.has_intrinsic(FLAG_BLIND) then
		message("A black aura surrounds the corpse!")
	end
	return (monsters.suck_life(dice, sides) > 0)
end

function food.default_eat_corpse_effect(obj, weight_eaten)
	local monst = obj.flags[FLAG_MONSTER_OBJ]
	local info  = {num_harmful = 0, num_beneficial = 0}

	-- Small chance of cutting corpse when eating it; it's easier to
	-- accidently cut smaller corpses than larger corpses
	if obj.sval == SV_CORPSE_CORPSE and rng(obj.weight / 5) == 0 then
		local result = {}
		food.cut_corpse_effect(obj, nil, result)

		if result.interrupted or result.no_eat then
			return
		end

		if result.beneficial then
			info.num_beneficial = info.num_beneficial + 1
		end
		if result.harmful then
			info.num_harmful = info.num_harmful + 1
		end
	end

	local corpse_power
	if obj.flags[FLAG_MONSTER_DECAY] then
		-- Corpses have less power as they decay
		corpse_power = 100 * obj.flags[FLAG_MONSTER_DECAY] /
			obj.flags[FLAG_ORIG_DECAY]
		corpse_power = min(100, corpse_power)
	else
		corpse_power = 100
	end

	-- Scan through blows
	local blows = {}
	if flag_exists(monst.blow, 0) then
		for ap_cnt = 0, flag_max_key(monst.blow) do
			local blow_struct = flag_get_blow(monst.blow, ap_cnt)

            local effect_idx = blow_struct.effect
            local method_idx = blow_struct.method

            local effect = __monster_attack_types[effect_idx]
            local method = __monster_attack_methods[method_idx]

			if method.on_eat and effect.on_eat then
				local blow = {
					effect = effect,
					method = method,
					d_dice = blow_struct.d_dice,
					d_side = blow_struct.d_side,
					power  = corpse_power,
				}
				tinsert(blows, blow)
			end
		end
	end

	randomize_list(blows)
	for i = 1, getn(blows) do
		local blow   = blows[i]
		local result = {}

		if tag(blow.effect.on_eat) == TAG_FUNCTION then
			blow.effect.on_eat(obj, monst, blow, result, info)
		elseif tag(blow.effect.on_eat) == TAG_TABLE then
			result.dam = blow.effect.on_eat

			if result.dam.beneficial then
				result.beneficial = true
			end
			if result.dam.harmful then
				result.harmful = true
			end
		else
			result.dam = {}
		end

		if tag(blow.method.on_eat) == TAG_FUNCTION then
			blow.method.on_eat(obj, monst, blow, result, info)
		end

		if result.dam then
			local dmg = result.dam.dam
			local typ = result.dam.type or result.dam.typ or
				blow.effect.type

			result.dam.death_str = result.dam.death_str or
				" eating " .. object_desc(obj)

			if not dmg then
				dmg = rng.roll(result.dam.d_dice or blow.d_dice,
							   result.dam.d_side or blow.d_side)
				dmg = dmg * corpse_power / 100
			end

			if result.dam.use_project then
				local flags
				flags = (PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
						 PROJECT_CASTER | PROJECT_HIDE_BLAST)

				force_die_from_msg(result.dam.death_str)
				project_p(0, 0, player.py, player.px, dmg, typ, 0, flags, 5)
				clear_die_from_msg()

				if not result.beneficial then
					result.harmful = true
				end
			else
				local resist = dam.get_resist(typ, "player")
				dmg = dam.apply_resistance(dmg, resist, true)

				if dmg < 0 then
					hp_player(dmg)
					result.beneficial = true
				elseif dmg > 0 then
					local absorb = dam.get_absorb(typ, "player")
					dmg = dam.apply_absorb(dmg, absorb)
					take_hit(dmg, result.dam.death_str)
					result.harmful = true
				end
			end

			if dmg > 0 and result.dam.dam_msg then
				result.msg = result.msg or result.dam.dam_msg
			elseif dmg < 0 and result.dam.heal_msg then
				result.msg = result.msg or result.dam.heal_msg
			end
		end -- if result.dam then

		if result.msg then
			local msg = result.msg
			-- Flags: No article, ignore hallucination, assume visible
			msg = gsub(msg, "(@monster@)",
					   monster_desc(monst, 512 | 256 | 128))
			msg = gsub(msg, "(@corpse@)", object_desc(obj))
			message(strcap(msg))
		end

		if result.beneficial then
			info.num_beneficial = info.num_beneficial + 1
		end
		if result.harmful then
			info.num_harmful = info.num_harmful + 1
		end

		-- Only one blow-related effect per bite
		if result.harmful or result.beneficial or result.effect then
			break
		end
	end -- for i = 1, getn(blows) do

	-- Rotten meat?
	if corpse_power < 50 and (obj.sval == SV_CORPSE_CORPSE or
							  obj.sval == SV_CORPSE_MEAT)
	then
		message("Ugh, *rotten* meat!")
		food.eat_msg = "" -- Message already given

		local dmg = weight_eaten * (50 - corpse_power) / 10
		dmg = dmg + rng(dmg)
		dmg = max(1, dmg)

		local typ = dam.POISONING

		local tmp = dmg
		local resist = dam.get_resist(typ, "player")
		tmp = dam.apply_resistance(tmp, resist, true)

		local absorb = dam.get_absorb(typ, "player")
		tmp = dam.apply_absorb(tmp, absorb)

		if tmp > 0 and resist < 66 then
			local flags
			flags = (PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
					 PROJECT_CASTER | PROJECT_HIDE_BLAST)

			force_die_from_msg("poisonous food")
			project_p(0, 0, player.py, player.px, dmg, typ, 0, flags, 5)
			clear_die_from_msg()
			info.num_harmful = info.num_harmful + 1
		end
	end

	local char = strchar(monst.d_char)

	local imm_acid = (monst.flags[FLAG_RESIST][dam.ACID] and
					  monst.flags[FLAG_RESIST][dam.ACID] >= 100)

	if char == 'j' and imm_acid then
		local dmg = rng.roll(8, 8)
		local typ = dam.ACID

		local resist = dam.get_resist(typ, "player")
		dmg = dam.apply_resistance(dmg, resist, true)

		local absorb = dam.get_absorb(typ, "player")
		dmg = dam.apply_absorb(dmg, absorb)

		if dmg > 0 then
			take_hit(dmg, "acidic food")
			info.num_harmful = info.num_harmful + 1
		end
	end

	local imm_pois = (monst.flags[FLAG_RESIST][dam.POIS] and
					  monst.flags[FLAG_RESIST][dam.POIS] >= 100)

    -- Hack -- Jellies, kobolds, spiders, icky things, molds, and mushrooms
    -- are immune to poison because their body already contains
    -- poisonous chemicals.
	if strfind("ijkm,S", char, 1, 1) then
		local dmg = rng(15) + 10
		local typ = dam.POISONING

		local tmp = dmg
		local resist = dam.get_resist(typ, "player")
		tmp = dam.apply_resistance(tmp, resist, true)

		local absorb = dam.get_absorb(typ, "player")
		tmp = dam.apply_absorb(tmp, absorb)

		if tmp > 0 and resist < 66 then
			local flags
			flags = (PROJECT_STOP | PROJECT_KILL | PROJECT_HIDE |
					 PROJECT_CASTER | PROJECT_HIDE_BLAST)

			force_die_from_msg("poisonous food")
			project_p(0, 0, player.py, player.px, dmg, typ, 0, flags, 5)
			clear_die_from_msg()
			info.num_harmful = info.num_harmful + 1
		end
	end

	-- That's it if the player got harmed
	if info.num_harmful > 0 then
		return
	end

	-- Only corpses give benefits when eaten
	if obj.sval ~= SV_CORPSE_CORPSE then
		return
	end

	-- The monster's immunities cause good effects.  Some of the
	-- effects are removal of certain negative times effects,
	-- others are temporary resistance.
	local ignore = {
		[dam.PURE] = true,
		[dam.SLASH] = true,
		[dam.CRUSH] = true,
		[dam.PIERCE] = true,
	}

	local remove_effects = {
		[dam.FEAR]      = timed_effect.AFRAID,
		[dam.STUN]      = timed_effect.STUN,
		[dam.BLIND]     = timed_effect.BLIND,
		[dam.CONFUSE]   = timed_effect.CONFUSED,
		[dam.CONFUSION] = timed_effect.CONFUSED,
	}

	foreach_flags(monst.flags[FLAG_RESIST],
		function(flags, key)
			if flags[key] >= 100 and not %ignore[key] then
				if %remove_effects[key] then
					timed_effect.set(%remove_effects[key], 0)
				else
					local effect_name = dam.res.get_temp_name(key)
					local effect_idx  = timed_effect[effect_name]

					timed_effect.set(effect_idx, rng(10) + 10)
				end
				%info.num_beneficial = %info.num_beneficial + 1
			end
		end)
end -- food.default_eat_corpse_effect()


-----------------------------
-- Custom Store generators --
-----------------------------
-- Not working yet:
--[[
-- For Khazad-dum's mithril smith
function store.make_obj.mithril(store_info, level)
	-- How do we generate a list of only items capable of receiving
	-- the mithril material ego?
	local k_idx = rand_obj.rand_k_idx(level, 1, THEME_ARMOR)
	local obj = new_object()
	object_prep(obj, k_idx)
	return obj
end
]]
