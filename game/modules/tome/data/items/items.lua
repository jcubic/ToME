-- First we declare some validators as to prevent stupid mistakes when
-- defining objects
constant("item_valid", {})

function item_valid.bow(t)
	local bowname = "Bow " .. t.name .. " "

	-- The WEAPON flag would cause the bow's damage to be displayed,
	-- but bows have a multiplier and bonuses without any damage on
	-- their own
	assert(not t.flags.WEAPON, bowname .. "doesn't need WEAPON")

	-- SHOW_MODS forces the display of "(to_h, to_d)" when the item
	-- is known, even if both are 0
	assert(t.flags.SHOW_COMBAT_MODS,
		   bowname .. "needs SHOW_COMBAT_MODS")

	assert(not t.flags.DAM, bowname .. "doesn't need DAM type")
	assert(t.flags.SKILL, bowname .. "needs SKILL")
	assert(t.flags.AMMO, bowname .. "needs AMMO type")
	assert(t.flags.BASE_RANGE, bowname .. "needs BASE_RANGE")
	assert(t.flags.MULTIPLIER, bowname .. "needs MULTIPLIER")
end
register_item_kind_validator(item_valid.bow, {TV_BOW, TV_SLING})

-----------------------
function item_valid.ammo(t)
	local ammoname = "Ammo " .. t.name .. " "

	assert(t.flags.WEAPON, ammoname .. "needs WEAPON")
	assert(t.flags.DAM, ammoname .. "needs DAM type")
	assert(not t.flags.SKILL, ammoname .. "doesn't need SKILL")
end
register_item_kind_validator(item_valid.ammo, {TV_SHOT, TV_ARROW})

-----------------------

function item_valid.armor(t)
	local armorname = "Armor " .. t.name .. " "

	assert(t.ac, armorname .. "needs ac")
end
register_item_kind_validator(item_valid.armor,
			     {TV_BOOTS, TV_GLOVES, TV_LIGHT_HELM, TV_HEAVY_HELM,
				     TV_SHIELD, TV_CLOAK, TV_LIGHT_ARMOUR,
				     TV_HEAVY_ARMOUR})

-----------------------

function item_valid.weapon(t)
	local weapname = "Weap " .. t.name .. " "

	assert(t.flags.WEAPON, weapname .. "needs WEAPON")
	assert(t.flags.DAM, weapname .. "needs DAM type")
	assert(t.flags.SKILL, weapname .. "needs SKILL")
end
register_item_kind_validator(item_valid.weapon,
			     {TV_HAFTED, TV_POLEARM, TV_KNIFE, TV_SWORD, TV_AXE, TV_MSTAFF})

--------------------------

function item_valid.scroll(t)
	local scrollname = "Scroll " .. t.name .. " "

	assert(t.flags.ON_READ, scrollname .. "needs ON_READ")
end
register_item_kind_validator(item_valid.scroll, {TV_SCROLL, TV_PARCHMENT})

--------------------------

function item_valid.potion(t)
	local potionname = "Potion " .. t.name .. " "

	assert(t.flags.BREAKAGE_CHANCE, potionname .. "needs BREAKAGE_CHANCE")
	assert(t.flags.ON_QUAFF, potionname .. "needs ON_QUAFF")
end
register_item_kind_validator(item_valid.potion, TV_POTION)

--------------------------

function item_valid.book(t)
	local bookname = "Book " .. t.name .. " "

	assert(t.flags.EASY_KNOW, bookname .. "needs EASY_KNOW")
	assert(t.flags.FULL_NAME, bookname .. "needs FULL_NAME")
	assert(t.flags.SPELL_IDX, bookname .. "needs SPELL_IDX")
	assert(t.flags.SPELLBOOK, bookname .. "needs SPELLBOOK")
end
register_item_kind_validator(item_valid.book, TV_BOOK)

-- sval 255 is the artifact book created for the player
item_kind_skip_validator(TV_BOOK, 255)
item_kind_skip_validator(TV_BOOK, SV_BOOK_RANDOM)

--------------------------

function item_valid.gold(t)
	local goldname = "Gold " .. t.name .. " "

	assert(t.flags.GOLD_VALUE, goldname .. "needs GOLD_VALUE")
end
register_item_kind_validator(item_valid.gold, TV_GOLD)

--------------------------

function item_valid.food(t)
	local foodname = "Food " .. t.name .. " "

	assert(t.flags.ON_EAT, foodname .. "needs ON_EAT")
	if type(t.flags.ON_EAT) == "function" then
		assert(t.flags.FOOD_VALUE, foodname .. "needs FOOD_VALUE")
	end
end
register_item_kind_validator(item_valid.food, TV_FOOD)

--------------------------

function item_valid.lite(t)
	local litename = "Lite " .. t.name .. " "

	assert(t.flags.LITE, litename .. "needs LITE")
end
register_item_kind_validator(item_valid.lite, TV_LITE)

--------------------------

function item_valid.trapkit(t)
	local trapname = "Trapkit " .. t.name .. " "

	assert(t.flags.AMMO_PER_TRIGGER, trapname .. "needs AMMO_PER_TRIGGER")
	assert(t.flags.SELECT_AMMO, trapname .. "needs SELECT_AMMO")
	assert(t.flags.TRAPKIT_TRIP, trapname .. "needs TRAPKIT_TRIP")
end
register_item_kind_validator(item_valid.trapkit, TV_TRAPKIT)

-----------------------------------------------------------------------------

-- Chest stuff

function chests.setup(obj, power)
	if not traps.place_random_object_trap(obj, FACTION_DUNGEON) then
		if wizard then
			message(color.VIOLET,
				"ERROR: Unable to place trap on chest!")
		end
	end


	set_flag(obj, FLAG_LEVEL, dun_level)
	set_flag(obj, FLAG_DUNGEON, current_dungeon_idx)

	local theme = flag_new(2)
	flag_copy(theme,
		  flag_get_flags(d_info[current_dungeon_idx + 1].flags,
				 FLAG_OBJ_THEME))
	flag_set_full(obj.flags, FLAG_OBJ_THEME, FLAG_FLAG_FLAGS, 0, 0,
		      theme)
end -- chests.setup()

function chests.open_small(o_ptr, o_idx, y, x)
	object_level = get_flag(o_ptr, FLAG_LEVEL) + 10

	local amount = get_flag(o_ptr, FLAG_AMOUNT)

	if (not amount or amount < 1) and wizard then
		message(color.VIOLET, "No amount to drop for small chest")
	end

	-- 75% chance of small chests containing money
	if rng.percent(75) then
		for i = 1, amount do
			local new_obj = new_object()

			make_gold(new_obj)
			drop_near(new_obj, -1, y, x)
		end
	end

	chests.cleanup(o_ptr)
end -- chests.open_small()

function chests.open_large(o_ptr, o_idx, y, x)
	object_level = get_flag(o_ptr, FLAG_LEVEL) + 10

	local amount = get_flag(o_ptr, FLAG_AMOUNT)
	local theme  = flag_get_flags(o_ptr.flags,
				      FLAG_OBJ_THEME)

	if (not amount or amount < 1) and wizard then
		message(color.VIOLET, "No amount to drop for large chest")
	end

	-- It's small chests that contain money, not large chests
	flag_remove(theme, FLAG_THEME_TREASURE)

	-- Chests shouldn't contain other chests
	flag_remove(theme, FLAG_THEME_CHESTS)

	-- Treasure chests shouldn't contain junk
	flag_remove(theme, FLAG_THEME_JUNK)

	-- When you think of the contents of a treasure chest,
	-- "food" isn't something that immediately comes to mind
	flag_remove(theme, FLAG_THEME_FOOD)

	-- The loop does more checks to make sure that the chest doesn't
	-- spit out junk, money, or more chests, since a non-standard
	-- theme which wasn't removed above might contain them.
	for i = 1, amount do
		local new_obj

		new_obj = rand_obj.make_object(false, false, theme)

		-- Retry make_object if it gives us nothing, or gives us
		-- a chest, money, or junk (since large chests should drop
		-- only interesting non-money items, and shouldn't contain
		-- more chests).
		local tries = 0
		while (not new_obj or new_obj.tval == TV_CHEST or
		       new_obj.tval == TV_JUNK or new_obj.tval == TV_GOLD)
			and tries < 10 do
			tries = tries + 1

			if new_obj then
				delete_object(new_obj)
			end
			new_obj = rand_obj.make_object(false, false, theme)
		end -- while(tries)

		-- Treasure chests should neverspit junk or other chests,
		-- so if it tries to, replace it with money, even though
		-- large chest aren't really supposed to contain money.
		if new_obj and (new_obj.tval == TV_CHEST or
				new_obj.tval == TV_JUNK) then
			if wizard and new_obj.tval == TV_CHEST then
				message(color.VIOLET,
					"Chest attempted to make a chest")
			elseif wizard and new_obj.tval == JUNK then
				message(color.VIOLET,
					"Chest attempted to make junk")
			end

			delete_object(new_obj)

			new_obj = new_object()
			make_gold(new_obj)
		end -- if(bad treasure type)

		if new_obj then
			drop_near(new_obj, -1, y, x)
		elseif wizard then
			message(color.VIOLET,
				"Large chest couldn't generate an object")
		end
	end -- for i = 1, amount do

	chests.cleanup(o_ptr)
end -- chest.open_large()

-----------------------------------------------------------------------------
new_item_kinds
{

    ---------------- MAGE STAVES --------------------
	[TV_MSTAFF] =
		{
		color = color.LIGHT_RED
		name = "magestaves"
		desc = "Staves designed for wielders of magic, by the greats of the art."
		symbol = '\\'
		defaults = {
			flags = {
				WIELD_SLOT=INVEN_MAINHAND
				WEAPON    = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL     = getter.skill("Hafted-mastery")
				COULD2H   = obvious(true)
				HARMED_BY = getter.resists{FIRE=true ACID=true}
				ID_SKILL  = getter.skill("Magic")
			}
		}

		[SV_MAGE_STAFF] = {
			name = "& mage staff~"
			display = '\\' color = color.LIGHT_BLUE
			desc = {
				"It looks like a simple walking stick, plain and nondescript.",
				"In the hands of a spellcaster, it can be a deadly weapon."
			}
			level = 5 weight = 60 cost = 300
			allocation = { {5,2} }
			flags = {
				DAM       = getter.damages{CRUSH={1,3}}
				-- +3 to casting speed gives, when applied to normal casting
				-- speed, a decrease of casting time to 77% of normal,
				-- according to the extra_energy[] array in tables.c
				SPEEDS = obvious(getter.speeds{CAST=3})
				ID_VALUE = 3
			}
		}

		[SV_CLOUD_STAFF] = {
			name = "& cloud staff~"
			display = '\\' color = color.WHITE
			desc = {
				"This metallic staff feels light enough to float in the air. When",
				"held, it gives the wielder the faintest electric shock."
			}
			level = 30 weight = 12 cost = 800
			allocation = { {30,2} }
			flags = {
				COULD2H   = obvious(false)
				DAM       = getter.damages{ELEC={1,3}}
				SKILL_BONUS = obvious(getter.skills{["Air"] = getter.random(1000, 10000)})
				ID_VALUE = 15
			}
		}

		[SV_STONE_STAFF] = {
			name = "& stone staff~"
			display = '\\' color = color.LIGHT_UMBER
			desc = {
				"This marbled stone staff is heavier than its kindred, and",
				"could surely leave a lasting impression on the head of a",
				"wayward student."
			}
			level = 30 weight = 120 cost = 800
			allocation = { {30,2} }
			flags = {
				DAM       = getter.damages{CRUSH={2,8}}
				SKILL_BONUS = obvious(getter.skills{["Earth"] = getter.random(1000, 10000)})
				ID_VALUE = 15
			}
		}

		[SV_FLOWING_STAFF] = {
			name = "& flowing staff~"
			display = '\\' color = color.BLUE
			desc = {
				"It is a staff carved of white wood, with long, flowing patterns.",
				"running along it.  Its head is capped with the crest of a wave",
				"from the great sea."
			}
			level = 30 weight = 60 cost = 800
			allocation = { {30,2} }
			flags = {
				DAM       = getter.damages{COLD={1,3}}
				SKILL_BONUS = obvious(getter.skills{["Water"] = getter.random(1000, 10000)})
				HARMED_BY = getter.resists{FIRE=true}
				ID_VALUE = 15
			}
		}

		[SV_FIRE_STAFF] = {
			name = "& fire staff~"
			display = '\\' color = color.LIGHT_RED
			desc = {
				"This deep red staff has a soft glow surrounding it, and is",
				"warm to the touch, but not uncomfortably so."
			}
			level = 40 weight = 60 cost = 1000
			allocation = { {40,2} }
			flags = {
				LITE      = obvious(1)
				DAM       = getter.damages{FIRE={1,3}}
				SKILL_BONUS = obvious(getter.skills{["Fire"] = getter.random(1000, 12000)})
				HARMED_BY = getter.resists{ACID=true}
				ID_VALUE = 20
			}
		}

		[SV_ARCANE_STAFF] = {
			name = "& arcane staff~"
			display = '\\' color = color.LIGHT_WHITE
			desc = {
				"This ornate wooden staff is engraved with ornate lettering of",
				"an ancient form.  Its importance and might are obvious even to",
				"the untrained eye."
			}
			level = 80 weight = 60 cost = 4000
			allocation = { {80,5} }
			flags = {
				DAM       = getter.damages{PURE={1,3}}
				SKILL_BONUS = obvious(getter.skills{["Mana"] = getter.random(1000, 12000)})
				SPELL     = obvious(getter.random(1, 5))
				SPEEDS    = obvious(getter.speeds{CAST=3})
				HARMED_BY = getter.resists{}
				ID_VALUE = {40,50}
			}
		}
	}

	---------------------- WEAPONS ----------------------
	[TV_POLEARM] =
	{
		color = color.WHITE
		name = "polearms"
		desc =
		{
			"Polearms are long, heavy weapons designed for use in a formation.",
		}
		symbol = '/'
		defaults = {
			display = '/'
			color = color.SLATE
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Polearm-mastery")
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Weaponmastery")
			}
		}

		[SV_PIKE] = {
			name = "& pike~"
			desc =
			{
				"A staff, 16-18 feet long, that has a small piercing head about 10 inches",
				"long. The pike is often used by infantry to fend off cavalry.  It is very",
				"effective against mounted troops."
			}
			level = 5 weight = 160 cost = 250
			allocation = { {5,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,3} PIERCE = {1,7} }
			}
		}

		[SV_GLAIVE] = {
			name = "& glaive~"
			desc =
			{
				"A polearm with a long, slightly curved knife-like blade. It has spurs on",
				"the dull side of the blade. It's primarily a slashing and chopping weapon. Glaives",
				"are often used to protect archers, crossbowmen, and gunners while they reload.",
				"Outside of combat they are a popular processional weapon and therefore many",
				"have ornately carved blades."
			}
			level = 10 weight = 190 cost = 350
			allocation = { {10,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,6} SLASH = {1,10}}
			}
		}

		[SV_FAUCHARD] = {
			name = "& fauchard~"
			desc =
			{
				"A curved blade atop a 6-7 foot pole with a pair of scrolly hooks attached ",
				"to the back of the blade. The Fauchard is primarily used for slashing attacks,",
				"with the hooks being used to bring down mounted foes."
			}
			level = 20 weight = 140 cost = 450
			allocation = { {20,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,3} SLASH = {1,6} PIERCE = {1,2}}
			}
		}

		[SV_LANCE] = {
			name = "& lance~"
			color = color.LIGHT_DARK
			desc =
			{
				"This is the original polearm. It is shaped like a spear but is bigger. It's",
				"meant to fend off enemies, not to be thrown."
			}
			level = 30 weight = 300 cost = 600
			allocation = { {30,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {2,5} PIERCE = {3,16}}
			}
		}

		[SV_GUISARME] = {
			name = "& guisarme~"
			desc =
			{
			 "The guisarme had a long curved bladed edged on the concave side, with a ",
			 "slender spear point opposite it. The guisarme could be used to thrust at",
			 "an oncoming opponent, slash, and even topple a rider."
			}
			level = 40 weight = 165 cost = 700
			allocation = { {40,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,3} SLASH = {2,4} PIERCE = {2,6}}
			}
		}

		[SV_HARAD_SPEAR] = {
			name = "& Harad spear~"
			desc =
			{
				"Broad-headed spears have a wide head, with cutting edges down each side.",
				"These score a hit either by stabbing with the point, or by slicing an edge across",
				"the opponent, and have done so many times over the years in the wars between",
				"Gondor and Harad."
			}
			level = 50 weight = 110 cost = 800
			allocation = { {50,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{SLASH = {1,4} PIERCE = {2,7}}
			}
		}

		[SV_LUCERN_HAMMER] = {
			name = "& Lucern hammer~"
			desc =
			{
				"A war hammer combined with a spearpoint, mounted on a long pole."
			}
			level = 60 weight = 120 cost = 2500
			allocation = { {60,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {3,12} PIERCE = {2,8}}
			}
		}

		[SV_HALBERD] = {
			name = "& halberd~"
			color = color.LIGHT_DARK
			desc =
			{
				"The halberd has a broad, short axe blade on a 5 - 6ft long haft, with a",
				"spearpoint at the top, often a back-spike and occasionally a butt-spike. Used to",
				"combat heavier armour. It's usually used for cutting and stabbing.  It's the most",
				"versatile polearm in Middle-earth. The axe is used in general melee. The top",
				"pike is be used to pierce armour, or is set against a cavalry charge. The back",
				"hook could unseat horses, trip opponents, parry a blow, or be a general",
				"piercing weapon."
			}
			level = 70 weight = 190 cost = 3000
			allocation = { {70,3} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {2,4} SLASH = {3,10} PIERCE = {3,10}}
			}
		}

		[SV_TRIDENT] = {
			name = "& trident~"
			color = color.YELLOW
			desc =
			{
				"The trident is based on the pitchfork.  In fact, when not used as a weapon,",
				"it is often employed as a pitchfork. It is famous for its uses in",
				"gladiatorial arenas."
			}
			level = 80 weight = 70 cost = 3500
			allocation = { {80,2} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,3} SLASH = {1,2} PIERCE = {2,8}}
			}
		}
	}

	[TV_AXE] =
	{
		color = color.WHITE
		name = "axes"
		desc =
		{
			"A hafted weapon with a wide blade on one end.",
		}
		symbol = '/'
		defaults = {
			display = '/'
			color = color.WHITE
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Axe-mastery")
				DAM_COMPUTE = combat.COMPUTE_ROLL
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Weaponmastery")
			}
		}

		[SV_FLINT_AXE] = {
			name = "& flint axe~"
			desc =
			{
				"A flinty axe"
			}
			level = 5 weight = 220 cost = 250
			allocation = { {5,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {1,9}}
			}
		}

		[SV_BISCAYAN_AXE] = {
			name = "& biscayan axe~"
			desc =
			{
				"An axe from France"
			}
			level = 10 weight = 150 cost = 400
			allocation = { {10,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{SLASH = {2,8}}
			}
		}

		[SV_WOODSMAN_AXE] = {
			name = "& woodsman axe~"
			desc =
			{
				"An axe used on trees"
			}
			level = 20 weight = 200 cost = 500
			allocation = { {20,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {4,16}}
			}
		}

		[SV_EXECUTION_AXE] = {
			name = "& execution axe~"
			color = color.YELLOW
			desc =
			{
				"An axe used to remove heads from bodies."
			}
			level = 30 weight = 300 cost = 600
			allocation = { {30,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {5,30}}
			}
		}

		[SV_RHUNIC_AXE] = {
			name = "& Rhunic axe~"
			desc =
			{
				"An axe used by Easterling warriors."
			}
			level = 40 weight = 190 cost = 700
			allocation = { {40,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {5,20}}
			}
		}

		[SV_LABRYS] = {
			name = "& labrys~"
			color = color.YELLOW
			desc =
			{
				"An double-edged battle axe."
			}
			level = 50 weight = 250 cost = 1500
			allocation = { {50,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {7,28}}
			}
		}

		[SV_DANE_AXE] = {
			name = "& Dane axe~"
			desc =
			{
				"A combat axe."
			}
			level = 60 weight = 200 cost = 1800
			allocation = { {60,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {5,20}}
			}
		}

		[SV_HATCHET] = {
			name = "& hatchet~"
			desc =
			{
				"A hand axe."
			}
			level = 70 weight = 120 cost = 2400
			allocation = { {70,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{SLASH = {2,14}}
			}
		}

		[SV_BARUK_KHAZAD] = {
			name = "& baruk Khazad~"
			color = color.RED
			desc =
			{
				"A Dwarven battle axe."
			}
			level = 80 weight = 210 cost = 3000
			allocation = { {80,3} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{SLASH = {8,32}}
			}
		}

	}

	[TV_HAFTED] =
	{
		color = color.WHITE
		name = "hafted weapons"
		desc =
		{
			"A stick with something dangerous looking on one end",
		}
		symbol = '\\'
		defaults = {
			display = '\\'
			color = color.WHITE
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Hafted-mastery")
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Weaponmastery")
			}
		}

		[SV_CLUB] = {
			name = "& club~"
			desc =
			{
				"A big, heavy piece of wood with a slight shape given to it."
			}
			level = 5 weight = 180 cost = 250
			allocation = { {5,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {1,9}}
			}
		}

		[SV_HAND_HAMMER] = {
			name = "& hand hammer~"
			desc =
			{
				"A blacksmith's tool can hurt if it hits one."
			}
			level = 10 weight = 150 cost = 400
			allocation = { {10,1} }
			flags = {
				COULD2H = obvious(true)
				DAM = getter.damages{CRUSH = {2,8}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_FLAIL] = {
			name = "& flail~"
			desc =
			{
				"A ball on a chain on a stick."
			}
			level = 20 weight = 200 cost = 500
			allocation = { {20,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {4,16}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_THREE_BALL_FLAIL] = {
			name = "& three-ball flail~"
			desc =
			{
				"Three balls on a chain on a stick."
			}
			color = color.LIGHT_DARK
			level = 30 weight = 300 cost = 700
			allocation = { {30,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {5,20}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_MACE] = {
			name = "& mace~"
			desc =
			{
				"An advanced version of a club, with"
				"painful ridges and better weight distribution."
			}
			level = 40 weight = 190 cost = 800
			allocation = { {40,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {5,15}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_WAR_HAMMER] = {
			name = "& war hammer~"
			desc =
			{
				"A long stick with a heavy hammer head on one end"
			}
			color = color.LIGHT_DARK
			level = 50 weight = 250 cost = 1200
			allocation = { {50,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {7,28}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_SPIKED_FLAIL] = {
			name = "& spiked flail~"
			desc =
			{
				"Like a flail, but with metal spikes on the ball."
			}
			level = 60 weight = 200 cost = 1800
			allocation = { {60,1} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {4,12} PIERCE = {1,8}}
			}
		}

		[SV_SPIKED_MACE] = {
			name = "& spiked mace~"
			desc =
			{
				"Like a mace, but with metal spikes on the head."
			}
			color = color.LIGHT_DARK
			level = 70 weight = 190 cost = 2500
			allocation = { {70,2} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {5,15} PIERCE = {3,9}}
			}
		}

		[SV_GREAT_HAMMER] = {
			name = "& great hammer~"
			desc =
			{
				"A cross between a war hammer and a hand hammer."
			}
			color = color.YELLOW
			level = 80 weight = 210 cost = 3500
			allocation = { {80,3} }
			flags = {
				MUST2H = obvious(true)
				DAM = getter.damages{CRUSH = {8,32}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}
	}

	[TV_KNIFE] =
	{
		color = color.WHITE
		name = "knives"
		desc =
		{
			"Sharp, short, and pointy.",
		}
		symbol = '|'
		defaults = {
			display = '|' color = color.WHITE
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				DUAL_WIELD = obvious(true)
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Knife-mastery")
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Weaponmastery")
			}
		}

		[SV_KNIFE] = {
			name = "& knife~"
			desc =
			{
				"You call that a knife?  This is a knife."
			}
			level = 5 weight = 5 cost = 250
			allocation = { {5,1} }
			flags = {
				DAM = getter.damages{PIERCE = {1,3}}
			}
		}

		[SV_MISERICORDE] = {
			name = "& misericorde~"
			desc =
			{
				"Used to kill the wounded."
			}
			level = 10 weight = 10 cost = 400
			allocation = { {10,1} }
			flags = {
				DAM = getter.damages{PIERCE = {1,4}}
			}
		}

		[SV_MAIN_GAUCHE] = {
			name = "& main gauche~"
			desc =
			{
				"Swordsman's secondary weapon."
			}
			level = 20 weight = 15 cost = 500
			allocation = { {20,1} }
			flags = {
				DAM = getter.damages{PIERCE = {2,5}}
			}
		}

		[SV_DAGGER] = {
			name = "& dagger~"
			desc =
			{
				"Miniature sword-looking knife."
			}
			level = 30 weight = 25 cost = 600
			allocation = { {30,1} }
			flags = {
				DAM = getter.damages{PIERCE = {2,6}}
			}
		}

		[SV_ATHAME] = {
			name = "& athame~"
			desc =
			{
				"Knife of rituals."
			}
			level = 40 weight = 20 cost = 1200
			color = color.YELLOW
			allocation = { {40,3} }
			flags = {
				DAM = getter.damages{SLASH = {1,3} PIERCE = {3,6}}
			}
		}

		[SV_SAX] = {
			name = "& sax~"
			desc =
			{
				"Knife of northern raiders."
			}
			level = 50 weight = 35 cost = 750
			allocation = { {50,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,2} PIERCE = {2,5}}
			}
		}

		[SV_DIRK] = {
			name = "& dirk~"
			desc =
			{
				"Knife."
			}
			level = 60 weight = 40 cost = 900
			allocation = { {60,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,2} PIERCE = {2,6}}
			}
		}

		[SV_BASILARD] = {
			name = "& basilard~"
			desc =
			{
				"Large Swiss dagger."
			}
			color = color.LIGHT_DARK
			level = 70 weight = 55 cost = 2200
			allocation = { {70,2} }
			flags = {
				DAM = getter.damages{SLASH = {1,4} PIERCE = {3,8}}
			}
		}

		[SV_STILETTO] = {
			name = "& stiletto~"
			desc =
			{
				"Knife."
			}
			color = color.LIGHT_DARK
			level = 80 weight = 25 cost = 3000
			allocation = { {80,2} }
			flags = {
				DAM = getter.damages{SLASH = {2,3} PIERCE = {3,6}}
			}
		}
	}

	[TV_SWORD] =
	{
		color = color.WHITE
		name = "swords"
		desc =
		{
			"Sharp, long, and deadly.",
		}
		symbol = '|'
		defaults = {
			display = '|' color = color.SLATE
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				WEAPON = true
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Sword-mastery")
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Weaponmastery")
			}
		}

		[SV_RAPIER] = {
			name = "& rapier~"
			desc =
			{
				"A sword."
			}
			level = 5 weight = 60 cost = 250
			allocation = { {5,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,2} PIERCE = {1,2}}
			}
		}

		[SV_SCHIAVONA] = {
			name = "& schiavona~"
			desc =
			{
				"A sword."
			}
			level = 10 weight = 80 cost = 400
			allocation = { {10,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,3} PIERCE = {1,2}}
			}
		}

		[SV_SABRE] = {
			name = "& sabre~"
			desc =
			{
				"A sword."
			}
			level = 20 weight = 100 cost = 500
			allocation = { {20,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,7} PIERCE = {1,2}}
			}
		}

		[SV_CUTLASS] = {
			name = "& cutlass~"
			desc =
			{
				"A sword."
			}
			level = 30 weight = 120 cost = 750
			allocation = { {30,1} }
			flags = {
				DAM = getter.damages{SLASH = {3,12}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_BASTARD_SWORD] = {
			name = "& bastard sword~"
			desc =
			{
				"A sword."
			}
			color = color.YELLOW
			level = 40 weight = 150 cost = 1200
			allocation = { {40,2} }
			flags = {
				COULD2H   = obvious(true)
				DAM = getter.damages{SLASH = {4,16}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_SCIMITAR] = {
			name = "& scimitar~"
			desc =
			{
				"A sword."
			}
			level = 50 weight = 170 cost = 1000
			allocation = { {50,1} }
			flags = {
				COULD2H   = obvious(true)
				DAM = getter.damages{SLASH = {3,15}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_FLAMMARD] = {
			name = "& flammard~"
			desc =
			{
				"A sword."
			}
			level = 60 weight = 200 cost = 1200
			allocation = { {60,1} }
			flags = {
				COULD2H   = obvious(true)
				DAM = getter.damages{SLASH = {5,20}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_ESPADON] = {
			name = "& espadon~"
			desc =
			{
				"A sword."
			}
			color = color.YELLOW
			level = 70 weight = 220 cost = 1800
			allocation = { {70,2} }
			flags = {
				MUST2H   = obvious(true)
				DAM = getter.damages{SLASH = {8,24}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}

		[SV_CLAYMORE] = {
			name = "& claymore~"
			desc =
			{
				"A sword."
			}
			color = color.YELLOW
			level = 80 weight = 250 cost = 2400
			allocation = { {80,3} }
			flags = {
				MUST2H   = obvious(true)
				DAM = getter.damages{SLASH = {8,32}}
				DAM_COMPUTE = combat.COMPUTE_ROLL
			}
		}
	}

	---------------- BOWS/ARROWS -----------------
	[TV_BOW] =
	{
		color = color.UMBER
		name = "bows"
		desc = "Used for firing projectiles."
		symbol = '}'
		defaults = {
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				AMMO = TV_ARROW
				SHOW_COMBAT_MODS = true
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH | object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TD
				SKILL = getter.skill("Bow-mastery")
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Archery")
			}
			display = '}'
			color = color.UMBER
		}

		[SV_SHORT_BOW] = {
			name = "& short bow~"
			desc = {
				"A bow"
			}
			level = 5 weight = 30 cost = 250
			allocation = { {5,1} }
			flags = {
				MULTIPLIER = 2
				BASE_RANGE = 5
				COULD2H = obvious(true)
			}
		}

		[SV_HUNTING_BOW] = {
			name = "& hunting bow~"
			desc = {
				"A better bow"
			}
			level = 30 weight = 60 cost = 1000
			allocation = { {30,1} }
			flags = {
				MULTIPLIER = 3
				BASE_RANGE = 6
				COULD2H = obvious(true)
			}
		}

		[SV_LONG_BOW] = {
			name = "& long bow~"
			desc = {
				"An even better bow"
			}
			level = 60 weight = 90 cost = 2000
			allocation = { {60,1} }
			flags = {
				MULTIPLIER = 4
				BASE_RANGE = 7
				COULD2H = obvious(true)
			}
		}

		[SV_WAR_BOW] = {
			name = "& war bow~"
			desc = {
				"A big bow"
			}
			color = color.LIGHT_UMBER
			level = 90 weight = 120 cost = 3000
			allocation = { {90,3} }
			flags = {
				MULTIPLIER = 5
				BASE_RANGE = 9
				MUST2H = obvious(true)
			}
		}
	}

	[TV_ARROW] =
	{
		color = color.UMBER
		name = "arrows"
		desc =
		{
			"Ammunition for anybody who has a bow.",
		}
		symbol = '{'
		defaults = {
			flags = {
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				BREAKAGE_CHANCE = 25
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				WIELD_SLOT = obvious(INVEN_AMMO)
				BASE_QUANTITY = 20 
				SCALED_QUANTITY = 50
				ID_SKILL  = getter.skill("Archery")
			}
			display = '{'
			color = color.UMBER
		}

		[SV_BODKIN_ARROW] = {
			name = "& bodkin arrow~"
			desc = {
				"A pointy arrow"
			}
			level = 1 weight = 2 cost = 1
			allocation = { {1,1} }
			flags = {
				DAM = getter.damages{PIERCE = {1,4}}
			}
		}

		[SV_SWALLOWTAIL_ARROW] = {
			name = "& swallowtail arrow~"
			desc = {
				"A slashing arrow used to take down a horse"
			}
			level = 15 weight = 2 cost = 1
			allocation = { {15,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,4}}
			}
		}

		[SV_HUNTING_ARROW] = {
			name = "& hunting arrow~"
			desc = {
				"A broad arrow designed for an instant kill"
			}
			level = 50 weight = 3 cost = 5
			allocation = { {50,1} }
			flags = {
				DAM = getter.damages{SLASH = {1,3} PIERCE = {1,4}}
			}
		}
	}

	---------------------- SLINGS/SHOTS ----------------------
	[TV_SLING] =
	{
		color = color.WHITE
		name = "slings"
		desc = "Used for firing projectiles."
		symbol = '}'
		defaults = {
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				AMMO = TV_SHOT
				SHOW_COMBAT_MODS = true
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH | object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TD
				SKILL = getter.skill("Sling-mastery")
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Archery")
			}
			display = '}'
			color = color.WHITE
		}

		[SV_SHORT_SLING] = {
			name = "& short sling~"
			desc = {
				"A sling"
			}
			level = 5 weight = 5 cost = 250
			allocation = { {5,1} }
			flags = {
				MULTIPLIER = 2
				BASE_RANGE = 5
			}
		}

		[SV_HUNTING_SLING] = {
			name = "& hunting sling~"
			desc = {
				"A better sling"
			}
			level = 30 weight = 10 cost = 1000
			allocation = { {30,1} }
			flags = {
				MULTIPLIER = 3
				BASE_RANGE = 6
			}
		}

		[SV_LONG_SLING] = {
			name = "& long sling~"
			desc = {
				"An even better sling"
			}
			color = color.LIGHT_WHITE
			level = 60 weight = 15 cost = 2000
			allocation = { {60,1} }
			flags = {
				MULTIPLIER = 4
				BASE_RANGE = 7
				COULD2H = obvious(true)
			}
		}

		[SV_STAFF_SLING] = {
			name = "& staff sling~"
			desc = {
				"A sling on a stick"
			}
			color = color.LIGHT_UMBER
			level = 90 weight = 120 cost = 3000
			allocation = { {90,3} }
			flags = {
				MULTIPLIER = 5
				BASE_RANGE = 9
				MUST2H = obvious(true)
			}
		}
	}

	[TV_SHOT] =
	{
		color = color.WHITE
		name = "shots"
		desc =
		{
			"Ammunition for anybody who as a sling.",
		}
		symbol = '{'
		defaults = {
			flags = {
				WEAPON = obvious(true)
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				BREAKAGE_CHANCE = 10
				HARMED_BY = getter.resists{ACID=true}
				WIELD_SLOT = obvious(INVEN_AMMO)
				BASE_QUANTITY = 5
				SCALED_QUANTITY = 20
				ID_SKILL  = getter.skill("Archery")
			}
			display = '{'
			color = color.WHITE
		}

		[SV_SLING_STONE] = {
			name = "& sling stone~"
			desc = {
				"A moderately-sized rock"
			}
			level = 1 weight = 15 cost = 1
			allocation = { {1,1} }
			flags = {
				DAM = getter.damages{CRUSH = {1,5}}
			}
		}

		[SV_BULLET] = {
			name = "& bullet~"
			desc = {
				"A rock well-shaped for slinging"
			}
			level = 15 weight = 8 cost = 2
			allocation = { {15,1} }
			flags = {
				DAM = getter.damages{CRUSH = {1,5}}
			}
		}

		[SV_BARBED_BULLET] = {
			name = "& barbed bullet~"
			desc = {
				"A constructed sling bullet with sharp barbs added"
			}
			color = color.LIGHT_WHITE
			level = 50 weight = 8 cost = 5
			allocation = { {50,1} }
			flags = {
				DAM = getter.damages{CRUSH = {1,5} PIERCE = {2,3}}
			}
		}
	}

	---------------- DEMONOLOGY -----------------------

	[TV_DEMON_ITEM] =
	{
		color = color.LIGHT_UMBER
		name = "demon items"
		desc =
		{
			"Items used by a student of Demonology to invoke the",
			"power of a balrog."
		}
		defaults = {
			color = color.LIGHT_UMBER
			ID_SKILL  = getter.skill("Demonology")
		}

		-- Make this into a whip or a claw or whatever we want
		[SV_DEMON_WEAPON] = {
			name = "& demon whip~"
			desc = {
				"A dark, barbed demonic whip."
			}
			display = '|'
			level = 15 weight = 100 cost = 2000
			allocation = { {15,3} }
			flags = {
				WIELD_SLOT = INVEN_MAINHAND
				WEAPON = true
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TH | object_enchant_drop.WEAPON_ENCHANT_RELATIVE_TD
				SKILL = getter.skill("Demonology")
				DAM = getter.damages{SLASH = {1,7} PIERCE = {1,7}}
				SPELL_IDX = getter.spells{"Demon Whip","Demon Madness","Demon Field"}
				SPELLBOOK = true
				WIELD_CAST = true
			}

		}

		[SV_DEMON_HORN] = {
			name = "& demon horn~"
			desc =
			{
				"The horn of a demon fitted on a leather strap."
			}
			display = '('
			level = 30 weight = 50 cost = 4000
			allocation = { {30,3} }
			flags = {
				WIELD_SLOT = INVEN_HEAD
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				RESIST = obvious(getter.resists{ CRUSH=5 SLASH=5 PIERCE=5 })
				SPELL_IDX = getter.spells{"Summon Demon","Discharge Minion","Control Demon"}
				SPELLBOOK = true
				WIELD_CAST = true
			}
			ac = 4
		}

		[SV_DEMON_SHIELD] = {
			name = "& demon shield~"
			desc =
			{
				"A moderately-sized shield emblazoned with demonic symbols."
			}
			display = ')'
			level = 45 weight = 80 cost = 6000
			allocation = { {45,3} }
			flags = {
				WIELD_SLOT = INVEN_OFFHAND
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DA
				ABSORB = obvious(getter.absorbs{ CRUSH=9 SLASH=9 PIERCE=9 })
				SPELL_IDX = getter.spells{"Doom Shield","Demon Cloak","Unholy Word"}
				SPELLBOOK = true
				WIELD_CAST = true
			}
			ac = 6
		}

	}


	---------------------- BODY ARMOUR ----------------------
	[TV_LIGHT_ARMOUR] =
	{
		color = color.SLATE
		name = "light armours"
		desc =
		{
			"Though it won't provide as much protection as heavier armours might,",
			"light armour is easier to move in.",
		}
		symbol = '('
		defaults =
		{
			display = '('
			color = color.SLATE
			flags =
			{
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				WIELD_SLOT = INVEN_BODY
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_FILTHY_RAG] = {
			name = "& filthy rag~"
			desc =
			{
				"A rag smelling of oil."
			}
			level = 0 weight = 5 cost = 2
			allocation = { {0,1} }
			ac = 1
			flags =
			{
				FUEL = obvious(2000)
				FUEL_SOURCE = obvious(1)
				HARMED_BY = getter.resists{FIRE=true}
			}

		}

		[SV_ROBE] = {
			name = "& robe~"
			desc =
			{
				"A robe."
			}
			level = 1 weight = 30 cost = 50
			allocation = { {1,1} }
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=2 SLASH=2 PIERCE=2 })
			}
			ac = 2
		}

		[SV_SOFT_LEATHER_JACKET] = {
			name = "& soft leather jacket~"
			desc =
			{
				"A jacket made of soft leather."
			}
			level = 5 weight = 60 cost = 200
			allocation = { {5,1} }
			ac = 5
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=4 SLASH=4 PIERCE=4 })
				SPELL_FAILURE = obvious(1)
			}
		}

		[SV_SOFT_STUDDED_JACKET] = {
			name = "& soft studded leather jacket~"
			desc =
			{
				"A jacket made of soft leather with reinforcing studs."
			}
			level = 10 weight = 65 cost = 300
			allocation = { {10,1} }
			ac = 6
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=5 SLASH=5 PIERCE=5 })
				SPELL_FAILURE = obvious(1)
			}
		}

		[SV_HARD_LEATHER_JACKET] = {
			name = "& hard leather jacket~"
			desc =
			{
				"A jacket made of hard leather."
			}
			level = 15 weight = 80 cost = 600
			allocation = { {15,1} }
			ac = 9
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=7 SLASH=7 PIERCE=7 })
				SPELL_FAILURE = obvious(3)
			}
		}

		[SV_HARD_STUDDED_JACKET] = {
			name = "& hard studded leather jacket~"
			desc =
			{
				"A jacket made of hard leather with reinforcing studs."
			}
			level = 20 weight = 85 cost = 700
			allocation = { {20,1} }
			ac = 10
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=8 SLASH=8 PIERCE=8 })
				SPELL_FAILURE = obvious(3)
			}
		}

		[SV_LEATHER_SCALE_MAIL] = {
			name = "& leather scale mail~"
			desc =
			{
				"Soft leather armour layered with hard leather scales."
			}
			level = 25 weight = 90 cost = 900
			allocation = { {25,1} }
			ac = 12
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=10 SLASH=10 PIERCE=10 })
				SPELL_FAILURE = obvious(5)
			}
		}

		[SV_RING_MAIL] = {
			name = "& ring mail~"
			desc =
			{
				"Soft leather armour layered with metal rings."
			}
			level = 30 weight = 110 cost = 1100
			allocation = { {30,1} }
			ac = 15
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=12 SLASH=12 PIERCE=12 })
				SPELL_FAILURE = obvious(5)
			}
		}

		[SV_DRAGON_SCALE_MAIL] = {
			name = "& dragon scale mail~"
			desc =
			{
				"Soft dragon leather armour layered with small dragon scales."
			}
			color = color.LIGHT_WHITE
			level = 80 weight = 70 cost = 8000
			allocation = { {80,4} }
			ac = 20
			flags = {
				ALWAYS_MATERIAL = true
				RESIST = obvious(getter.resists{ CRUSH=15 SLASH=15 PIERCE=15 })
				SPELL_FAILURE = obvious(3)
			}
		}

	}

	[TV_HEAVY_ARMOUR] =
	{
		color = color.SLATE
		name = "heavy armours"
		desc =
		{
			"This armour offers better protection that the lighter types,",
			"But is also more difficult to move in",
		}
		symbol = '['
		defaults =
		{
			display = '['
			color = color.SLATE
			flags =
			{
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				WIELD_SLOT = INVEN_BODY
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_SPLINT_MAIL] = {
			name = "& splint mail~"
			desc =
			{
				"Hard leather armour covered with metal splints."
			}
			level = 5 weight = 130 cost = 400
			allocation = { {5,1} }
			ac = 12
			to_h = -1
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=8 SLASH=8 PIERCE=8 })
				SPELL_FAILURE = obvious(10)
			}
		}

		[SV_METAL_SCALE_MAIL] = {
			name = "& metal scale mail~"
			desc =
			{
				"Hard leather armour layered with metal scales."
			}
			level = 15 weight = 180 cost = 600
			allocation = { {15,1} }
			ac = 15
			to_h = -2
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=10 SLASH=10 PIERCE=10 })
				SPELL_FAILURE = obvious(14)
			}
		}

		[SV_CHAIN_MAIL] = {
			name = "& chain mail~"
			desc =
			{
				"Armor made of interlocking metal chains."
			}
			level = 25 weight = 220 cost = 1100
			allocation = { {25,1} }
			ac = 20
			to_h = -3
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=14 SLASH=14 PIERCE=14 })
				SPELL_FAILURE = obvious(18)
			}
		}

		[SV_AUGMENTED_CHAIN_MAIL] = {
			name = "& augmented chain mail~"
			desc =
			{
				"Armor made of interlocking metal chains, and augmented ",
				"with metal plates."
			}
			level = 35 weight = 250 cost = 1800
			allocation = { {35,1} }
			ac = 25
			to_h = -4
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=18 SLASH=18 PIERCE=18 })
				SPELL_FAILURE = obvious(22)
			}
		}

		[SV_BRIGANDINE_MAIL] = {
			name = "& brigandine mail~"
			desc =
			{
				"This is a leather armour with many small metal plates fixed to it, covering",
				"it entirely."
			}
			level = 45 weight = 320 cost = 2400
			allocation = { {45,1} }
			ac = 30
			to_h = -5
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=22 SLASH=22 PIERCE=22 })
				SPELL_FAILURE = obvious(26)
			}
		}

		[SV_PARTIAL_PLATE_MAIL] = {
			name = "& partial plate mail~"
			desc =
			{
				"An armour made of steel plates, covering only the body of the wearer."
			}
			level = 55 weight = 400 cost = 3000
			allocation = { {55,1} }
			ac = 35
			to_h = -6
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=26 SLASH=26 PIERCE=26 })
				SPELL_FAILURE = obvious(30)
			}
		}

		[SV_FULL_PLATE_MAIL] = {
			name = "& full plate mail~"
			desc =
			{
				"A suit of armour made of metal plates, covering the body, arms and upper legs",
				"A very effective but very heavy armour."
			}
			level = 65 weight = 500 cost = 4500
			allocation = { {65,1} }
			ac = 40
			to_h = -7
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=30 SLASH=30 PIERCE=30 })
				SPELL_FAILURE = obvious(34)
			}
		}

		[SV_DRAGON_PLATE_MAIL] = {
			name = "& dragon plate mail~"
			desc =
			{
				"A suit of plate mail made with the hide and scales of a dragon,"
				"in place of more traditional materials."
			}
			color = color.LIGHT_WHITE
			level = 80 weight = 250 cost = 9000
			allocation = { {80,4} }
			ac = 50
			to_h = -4
			flags = {
				ALWAYS_MATERIAL = true
				RESIST = obvious(getter.resists{ CRUSH=35 SLASH=35 PIERCE=35 })
				SPELL_FAILURE = obvious(20)
			}
		}
	}

	---- Hats ----

	[TV_LIGHT_HELM] =
	{
		color = color.SLATE
		name = "light helms"
		desc =
		{
			"This armour offers modest protection to the head."
		}
		symbol = ')'
		defaults =
		{
			display = ')'
			color = color.SLATE
			flags =
			{
				WIELD_SLOT = INVEN_HEAD
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_LEATHER_CAP] = {
			name = "& leather cap~"
			desc =
			{
				"A leather cover for the top of the head."
			}
			level = 1 weight = 5 cost = 50
			allocation = { {1,1} }
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=2 SLASH=2 PIERCE=2 })
			}
			ac = 2
		}

		[SV_HARD_CAP] = {
			name = "& metal cap~"
			desc =
			{
				"A hard hat for work in dangerous dungeons."
			}
			level = 20 weight = 30 cost = 500
			allocation = { {20,1} }
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=4 SLASH=4 PIERCE=4 })
			}
			ac = 3
		}

		[SV_CROWN] = {
			name = "& crown~"
			desc =
			{
				"An ornate head covering."
			}
			level = 70 weight = 30 cost = 1000
			allocation = { {70,1} }
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=0 SLASH=0 PIERCE=0 })
			}
			ac = 0
		}

		[SV_DRAGON_LEATHER_CAP] = {
			name = "& dragon leather cap~"
			desc =
			{
				"A strong, light piece of headgear made of a dragon's hide."
			}
			color = color.LIGHT_WHITE
			level = 80 weight = 10 cost = 4000
			allocation = { {80,4} }
			flags = {
				ALWAYS_MATERIAL = true
				RESIST = obvious(getter.resists{ CRUSH=6 SLASH=6 PIERCE=6 })
			}
			ac = 6
		}

	}

	[TV_HEAVY_HELM] =
	{
		color = color.SLATE
		name = "heavy helm"
		desc =
		{
			"This armour offers better protection to the head, at the expense",
			"of clear sight and surety of motion."
		}
		symbol = ']'
		defaults =
		{
			display = ']'
			color = color.SLATE
			flags =
			{
				WIELD_SLOT = INVEN_HEAD
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_PARTIAL_HELM] = {
			name = "& partial helm~"
			desc =
			{
				"A hard helm that covers all but the face."
			}
			level = 40 weight = 40 cost = 800
			allocation = { {40,1} }
			ac = 4
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=6 SLASH=6 PIERCE=6 })
				SKILL_BONUS = obvious(getter.skills{["Sneakiness"] = -2000 })
			}
		}

		[SV_FULL_HELM] = {
			name = "& full helm~"
			desc =
			{
				"A unitary hard helm covering the entire head."
			}
			level = 50 weight = 60 cost = 1000
			allocation = { {50,1} }
			ac = 5
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=8 SLASH=8 PIERCE=8 })
				SKILL_BONUS = obvious(getter.skills{["Sneakiness"] = -6000 })
			}
		}

		[SV_VISORED_HELM] = {
			name = "& visored helm~"
			desc =
			{
				"A hard helm covering the whole head, but with a liftable",
				"visor to allow better sight when necessary."
			}
			level = 60 weight = 70 cost = 2000
			allocation = { {60,1} }
			ac = 6
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=9 SLASH=9 PIERCE=9 })
				SKILL_BONUS = obvious(getter.skills{["Sneakiness"] = -4000 })
			}
		}

		[SV_DRAGON_HELM] = {
			name = "& dragon helm~"
			desc =
			{
				"A hard helm shaped from the scales of a dragon."
			}
			color = color.LIGHT_WHITE
			level = 90 weight = 50 cost = 6000
			allocation = { {90,4} }
			ac = 8
			flags = {
				ALWAYS_MATERIAL = true
				RESIST = obvious(getter.resists{ CRUSH=10 SLASH=10 PIERCE=10 })
				SKILL_BONUS = obvious(getter.skills{["Sneakiness"] = -2000 })
			}
		}

	}

	[TV_CLOAK] =
	{
		color = color.BLUE
		name = 'cloaks'
		desc = {
			"Cloaks are large wraps designed to cover one's body and ",
			"hopefully give some protection from the elements.  They are ",
			"unlikely to serve well as armour, though."
		}
		symbol = '('
		defaults =
		{
			display = '('
			color = color.BLUE
			flags =
			{
				WIELD_SLOT = INVEN_OUTER
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DR
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_COTTON_CLOAK] = {
			name = "& cotton cloak~"
			desc =
			{
				"A relatively light cloak woven from cotton.  It's better",
				"than nohting."
			}
			level = 1 weight = 10 cost = 50
			allocation = { {1,1} }
			ac = 1
		}

		[SV_LEATHER_CLOAK] = {
			name = "& leather cloak~"
			desc =
			{
				"A somewhat denser cloak stitched in leather."
			}
			level = 30 weight = 40 cost = 500
			allocation = { {30,1} }
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=2 SLASH=2 PIERCE=2 })
			}
			ac = 4
		}

		[SV_WOOLEN_CLOAK] = {
			name = "& woolen cloak~"
			desc =
			{
				"A heavy cloak that can make walking difficult."
			}
			level = 60 weight = 80 cost = 2000
			allocation = { {60,1} }
			ac = 10
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=4 SLASH=4 PIERCE=4 })
				SPEEDS = obvious(getter.speeds{WALK=-3})
			}
		}
	}

	[TV_GLOVES] =
	{
		color = color.UMBER
		name = 'gloves'
		desc = {
			"Hand coverings",
		}
		symbol = ']'
		defaults =
		{
			display = ']'
			color = color.UMBER
			flags =
			{
				WIELD_SLOT = INVEN_HANDS
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC
				HARMED_BY = getter.resists{ACID=true FIRE=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_LEATHER_GLOVES] = {
			name = "& set~ of leather gloves"
			desc =
			{
				"Soft hand coverings."
			}
			level = 5 weight = 5 cost = 50
			allocation = { {5,1} }
			ac = 1
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=0 SLASH=0 PIERCE=0 })
				SPELL_FAILURE = obvious(15)
			}
		}

		[SV_HEAVY_LEATHER_GLOVES] = {
			name = "& set~ of heavy leather gloves"
			desc =
			{
				"Thick hand coverings."
			}
			level = 25 weight = 10 cost = 500
			allocation = { {25,1} }
			ac = 5
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=2 SLASH=2 PIERCE=2 })
				SPELL_FAILURE = obvious(25)
			}
		}

		[SV_GAUNTLETS] = {
			name = "& set~ of gauntlets"
			desc =
			{
				"Metal hand coverings."
			}
			level = 50 weight = 50 cost = 2000
			allocation = { {50,1} }
			ac = 10
			flags = {
				RESIST = obvious(getter.resists{ CRUSH=4 SLASH=4 PIERCE=4 })
				SPELL_FAILURE = obvious(40)
			}
		}
	}

	[TV_SHIELD] =
	{
		color = color.UMBER
		name = 'shields'
		desc = {
			"Handheld deflection devices",
		}
		symbol = ')'
		defaults =
		{
			display = ')'
			color = color.UMBER
			flags =
			{
				WIELD_SLOT = INVEN_OFFHAND
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DA
				HARMED_BY = getter.resists{ACID=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_WOODEN_SHIELD] = {
			name = "& wooden shield~"
			desc =
			{
				"A simple slab of wood with a handle, sometimes carved",
				"with ornate designs."
			}
			level = 1 weight = 30 cost = 50
			allocation = { {1,1} }
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=3 SLASH=3 PIERCE=3 })
			}
			ac = 1
		}

		[SV_BUCKLER] = {
			name = "& buckler~"
			desc =
			{
				"Small metal shield worn on the fist."
			}
			level = 20 weight = 30 cost = 800
			allocation = { {20,1} }
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=6 SLASH=6 PIERCE=6 })
			}
			ac = 3
		}

		[SV_HOPLITE] = {
			name = "& hoplite~"
			desc =
			{
				"Moderately sized shield designed for formation combat."
			}
			level = 40 weight = 60 cost = 1500
			allocation = { {40,1} }
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=9 SLASH=9 PIERCE=9 })
			}
			ac = 5
		}

		[SV_SCUTUM] = {
			name = "& scutum~"
			desc =
			{
				"Tall, curved shield."
			}
			level = 60 weight = 80 cost = 2500
			allocation = { {60,1} }
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=12 SLASH=12 PIERCE=12 })
				SPEEDS = obvious(getter.speeds{WALK=-3})
			}
			ac = 7
		}

		[SV_PAVIS] = {
			name = "& pavis~"
			desc =
			{
				"Large, heavy shield for wearing on the back, or for ",
				"use as cover by an archer."
			}
			level = 80 weight = 200 cost = 5000
			allocation = { {80,3} }
			ac = 12
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=20 SLASH=20 PIERCE=20 })
				SPEEDS = obvious(getter.speeds{WALK=-8})
			}
		}

	}

	[TV_BOOTS] =
	{
		color = color.UMBER
		name = "boots"
		desc = {
			"Hardy footcoverings for long travels."
		}
		symbol = ']'
		defaults =
		{
			display = ']'
			color = color.UMBER
			flags =
			{
				WIELD_SLOT = INVEN_FEET
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_RELATIVE_AC | object_enchant_drop.ARMOR_ENCHANT_RELATIVE_DA
				HARMED_BY = getter.resists{ACID=true, FIRE=true}
				ID_SKILL  = getter.skill("Combat")
			}
		}

		[SV_SANDALS] = {
			name = "& pair~ of sandals"
			desc =
			{
				"Easy, light protection."
			}
			level = 1 weight = 5 cost = 50
			allocation = { {1,1} }
			ac = 1
		}

		[SV_LEATHER_BOOTS] = {
			name = "& pair~ of leather boots"
			desc =
			{
				"Hard, durable walking boots."
			}
			level = 25 weight = 30 cost = 500
			allocation = { {25,1} }
			ac = 4
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=3 SLASH=3 PIERCE=3 })
				SKILL_BONUS = obvious(getter.skills{["Stealth"] = -2000 })
			}
		}

		[SV_REINFORCED_BOOTS] = {
			name = "& pair~ of reinforced boots"
			desc =
			{
				"Loud boots for dangerous work."
			}
			level = 50 weight = 60 cost = 2000
			allocation = { {50,1} }
			ac = 8
			flags = {
				ABSORB = obvious(getter.absorbs{ CRUSH=8 SLASH=8 PIERCE=8 })
				SKILL_BONUS = obvious(getter.skills{["Stealth"] = -5000 })
			}
		}
	}


	---------------------- SCROLLS ----------------------
	[TV_SCROLL] =
	{
		color = color.WHITE
		name = "scrolls"
		desc = {
			"Scrolls are magical parchments imbued with magic ",
			"spells.  Some are good, some...are not.  When a ",
			"scroll is read, its magic is released and the ",
			"scroll is destroyed."

		}
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true}
				SCALED_QUANTITY = 20
				ID_SKILL  = getter.skill("Magic")
			}
		}
		symbol = '?'

		[SV_SCROLL_NOTHING] = {
			name = "nothing"
			display = '?' color = color.WHITE
			level = 0 weight = 5 cost = 0
			allocation = { {0,1} }
			flags = {
				ON_READ=function()
					return false, true
				end
			}
		}
		[SV_SCROLL_SATISFY_HUNGER] = {
			name = "satisfy hunger"
			display = '?' color = color.WHITE
			desc =
			{
				"Reading this scroll will transport a good portion of nourishing gruel directly into your",
				"stomach. Some people say that this is a ploy of a food producer whose food nobody wants to eat."
			}
			level = 0 weight = 5 cost = 10
			allocation = { {0,1} }
			flags = {
				ON_READ=function()
					-- Inferior to Lembas
					return true, timed_effect.inc(timed_effect.FOOD, 5000)
				end
			}
		}
		[SV_SCROLL_CONFUSE_MONSTER] = {
			name = "confuse monster"
			display = '?' color = color.WHITE
			desc =
			{
				"Reading this scroll will cause your hands to glow with a strange mesmerising light that will",
				"attempt to confuse creatures you hit with a hand or weapon attack."
			}
			level = 1 weight = 5 cost = 20
			allocation = { {1,1} }
			flags = {
				ON_READ=function()
					local dur = timed_effect.get(timed_effect.CONFUSING_TOUCH)
					if dur and dur > 0 then
						timed_effect.inc(timed_effect.CONFUSING_TOUCH, 10 + rng(10))
					else
						timed_effect.set(timed_effect.CONFUSING_TOUCH, 10 + rng(10), 20)
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_DARKNESS] = {
			name = "darkness"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll will create a blast of utter darkness, which then fades to leave the immediate",
				"surroundings in a lasting gloom. The extreme darkness can blind those who are not used to",
				"it."
			}
			level = 1 weight = 5 cost = 0
			allocation = { {1,2} }
			flags = {
				ON_READ=function()
					if player.resist(dam.BLIND) < 100 and not rng.percent(player.resist(dam.BLIND)) then
						timed_effect.inc(timed_effect.BLIND, 3 + rng(5))
					end
					unlite_room(player.py, player.px)
					return true, true
				end
			}
		}
		[SV_SCROLL_DETECT_TRAPS] = {
			name = "detect traps"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll is very helpful, because it reveals the locations of nearby snares and traps which you",
				"might otherwise blunder into at unawares."
			}
			level = 1 weight = 5 cost = 20
			allocation = { {1,1} }
			flags = {
				ON_READ=function()
					return true, traps.detect(DEFAULT_RADIUS)
				end
			}
		}
		[SV_SCROLL_LIGHT] = {
			name = "light"
			display = '?' color = color.WHITE
			desc =
			{
				"A scroll which will create a permanent magical light, illuminating the surroundings."
			}
			level = 1 weight = 5 cost = 20
			allocation = { {1,1} }
			flags = {
				ON_READ=function()
					return true, lite_room(player.py, player.px)
				end
			}
		}
		[SV_SCROLL_PHASE_DOOR] = {
			name = "phase door"
			display = '?' color = color.WHITE
			desc =
			{
				"Upon reading this scroll, you will be translocated over a short distance."
			}
			level = 1 weight = 5 cost = 20
			allocation = { {1,1} }
			flags = {
				ON_READ=function()
					teleport_player(10)
					return true, true
				end
			}
		}
		[SV_SCROLL_DETECT_OBJECTS] = {
			name = "detect objects"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll shows nearby objects to you. It only makes you aware of items sitting on the floor,",
				"however, not those carried by creatures."
			}
			level = 1 weight = 5 cost = 20
			allocation = { {1,2} }
			flags = {
				ON_READ=function()
					return true, objects.detect(DEFAULT_RADIUS)
				end
			}
		}
		[SV_SCROLL_DETECT_INVISIBLE] = {
			name = "detect invisible monsters"
			display = '?' color = color.WHITE
			desc =
			{
				"A minor detection spell is stored in this scroll, showing you the locations of otherwise",
				"unseen beings for a brief moment."
			}
			level = 5 weight = 5 cost = 50
			allocation = { {5,1} }
			flags = {
				ON_READ=function()
					return true, monsters.detect_invisible(DEFAULT_RADIUS)
				end
			}
		}
		[SV_SCROLL_LOCATE_DOORS] = {
			name = "locate doors and stairs"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll will reveal nearby passages."
			}
			level = 5 weight = 5 cost = 50
			allocation = { {5,1} }
			flags = {
				ON_READ=function()
					local obvious = dungeons.detect_doors(DEFAULT_RADIUS)
					local obvious2 = ungeons.detect_stairs(DEFAULT_RADIUS)
					return true, obvious or obvious2
				end
			}
		}
		[SV_SCROLL_VALARIN_FAVOR] = {
			name = "valarin favor"
			display = '?' color = color.WHITE
			desc =
			{
				"The recitation of this scroll will grant you a blessing of the Valar.",
			}
			level = 5 weight = 5 cost = 50
			allocation = { {5,2} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					local dur = timed_effect.get(timed_effect.VALARIN_FAVOR)
					if dur and dur > 0 then
						timed_effect.inc(timed_effect.VALARIN_FAVOR, 10 + rng(10))
					else
						timed_effect.set(timed_effect.VALARIN_FAVOR, 10 + rng(10), 1)
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_DESTROY_DOORS] = {
			name = "destroy doors and traps"
			display = '?' color = color.WHITE
			desc =
			{
				"A very specifically destructive spell is written on this scroll. It will smash all traps and all",
				"doors immediately next to you."
			}
			level = 10 weight = 5 cost = 100
			allocation = { {10,1} }
			flags = {
				ON_READ=function()
					local obvious = dungeons.destroy_doors(1)
					obvious = traps.magic_disarm(1) or obvious
					-- XXX TODO
					-- obvious = unlock_all_touch() or obvious
					return true, obvious
				end
			}
		}
		[SV_SCROLL_IDENTIFICATION] = {
			name = "identification"
			display = '?' color = color.WHITE
			desc =
			{
				"Upon reading this scroll, the identity of the item you specify will be laid open to you."
			}
			level = 10 weight = 5 cost = 100
			allocation = { {10,1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					return ident_spell(), true
				end
			}
		}
		[SV_SCROLL_SUMMON_MONSTER] = {
			name = "summon monster"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll was made by mischievous sorcerers. If it is read, a few creatures will appear to fight",
				"you."
			}
			level = 10 weight = 5 cost = 0
			allocation = { {10,1} }
			flags = {
				ON_READ=function()
					local obvious = nil
					for k = 1, rng(3) do
						is_obvious(summon_specific(player.py, player.px, dun_level, FACTION_DUNGEON, 0, 1000, true, nil, nil, nil, nil, nil, nil), obvious)
					end
					return true, obvious
				end
			}
		}
		[SV_SCROLL_REMOVE_CURSE] = {
			name = "remove curse"
			display = '?' color = color.WHITE
			desc =
			{
				"A scroll inscribed with a beneficial formula. Upon reading it, evil magics will be removed",
				"from your possessions."
			}
			level = 10 weight = 5 cost = 100
			allocation = { {10,2} }
			flags = {
				SCALED_QUANTITY = 2 -- Almost never in stacks
				ON_READ=function()
					if remove_curse() then
						message("You feel as if someone is watching over you.")
						return true, true
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_AGGRAVATE_MONSTER] = {
			name = "aggravate monster"
			display = '?' color = color.WHITE
			desc =
			{
				"This nasty scroll will make a loud noise, waking up foes in your vicinity."
			}
			level = 20 weight = 5 cost = 0
			allocation = { {20,1} }
			flags = {
				ON_READ=function()
					message("There is a high pitched humming noise.")
					aggravate_monsters(1)
					return true, true
				end
			}
		}
		[SV_SCROLL_MAGIC_MAPPING] = {
			name = "magic mapping"
			display = '?' color = color.WHITE
			desc =
			{
				"Reading this scroll will reveal the layout of your immediate surroundings to you."
			}
			level = 20 weight = 5 cost = 200
			allocation = { {20,1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					map_area(player.py, player.px, DEFAULT_RADIUS)
					return true, true
				end
			}
		}
		[SV_SCROLL_WORD_OF_RECALL] = {
			name = "word of recall"
			display = '?' color = color.WHITE
			desc =
			{
				"The spell on this scroll will slowly build an ethereal conduit to the town for you if you are in",
				"a dungeon, and into the dungeon if you are in a town. Upon completion, which takes a while, you",
				"will suddenly be translocated to the other place."
			}
			level = 20 weight = 5 cost = 200
			allocation = { {20,1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					if level_flags.flags[FLAG_NO_RECALL_OUT] then
						local msg = level_flags.flags[FLAG_NO_RECALL_OUT]
						if type(msg) == "string" then
							message(msg)
						else
							message("The scroll glows and fades.")
						end
						return false
					end
					recall_player(21, 15)
					return true, true
				end
			}
		}
		[SV_SCROLL_TELEPORTATION] = {
			name = "teleportation"
			display = '?' color = color.WHITE
			desc =
			{
				"If you read this scroll, you will immediately be transported to another place on the level."
			}
			level = 30 weight = 5 cost = 300
			allocation = { {30,1} }
			flags = {
				ON_READ=function()
					teleport_player(100)
					return true, true
				end
			}
		}
		[SV_SCROLL_BLAST_WEAPON] = {
			name = "blast weapon"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll will ruin your weapon beyond repair."
			}
			level = 30 weight = 5 cost = 0
			allocation = { {30,2} }
			flags = {
				ON_READ=function()
					return true, objects.blast_weapon()
				end
			}
		}
		[SV_SCROLL_SUMMON_SYMBIOTE] = {
			name = "summon symbiote"
			display = '?' color = color.WHITE
			desc =
			{
				"A piece of paper, inscribed with runes which will summon creature suitable for symbiosis."
			}
			level = 30 weight = 5 cost = 600
			allocation = { {30,2} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					local fl = flag_new()
					flag_set(fl, FLAG_SYMBIOTE, true)
					local obvious = summon_specific(player.py, player.px, dun_level, FACTION_PLAYER, -1000, 1000, true, fl, nil, nil, nil, nil, nil)
					flag_free(fl, true)
					return true, obvious
				end
			}
		}
		[SV_SCROLL_CREATE_TRAPS] = {
			name = "create traps"
			display = '?' color = color.WHITE
			desc =
			{
				"If you read this rather annoying scroll, snares and pitfalls will magically be planted all around",
				"you, ready to do nasty things to you once you walk onto them."
			}
			level = 40 weight = 5 cost = 0
			allocation = { {40,1} }
			flags = {
				ON_READ=function()
					return true, trap_creation(1, FACTION_DUNGEON)
				end
			}
		}
		[SV_SCROLL_RECHARGING] = {
			name = "recharging"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll is inscribed with a spell releasing the enchantments used to give wands and",
				"staves their power, allowing you to replenish their spent charges."
			}
			level = 40 weight = 5 cost = 500
			allocation = { {40,1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					return objects.recharge(60), true
				end
			}
		}
		[SV_SCROLL_DESTRUCTION] = {
			name = "destruction"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll is inscribed with a mighty conjuration which wrecks the dungeon all around you.",
				"Monsters and objects will be annihilated by this blast, only completely indestructible things can",
				"withstand it."
			}
			level = 40 weight = 5 cost = 800
			allocation = { {40,2} }
			flags = {
				SCALED_QUANTITY = 1
				ON_READ=function()
					if (is_quest(dun_level) == 0) and (dun_level ~= 0) then
						destroy_area(player.py, player.px, 15, true, false)
					else
						message("The ground trembles...")
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_GENOCIDE] = {
			name = "genocide"
			display = '?' color = color.WHITE
			desc =
			{
				"This rare and powerful scroll will annihilate all members of a specified race of monsters in",
				"your current location, but this will also exact a price of pain from you."
			}
			level = 40 weight = 5 cost = 1600
			allocation = { {40,4} }
			flags = {
				SCALED_QUANTITY = 1
				ON_READ=function()
					genocide(true)
					return true, true
				end
			}
		}
		[SV_SCROLL_BLAST_ARMOUR] = {
			name = "blast armour"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll bears a curse which will tear your armour to shreds. Don't read it!"
			}
			level = 50 weight = 5 cost = 0
			allocation = { {50,1} }
			flags = {
				ON_READ=function()
					return true, objects.blast_armour()
				end
			}
		}
		[SV_SCROLL_VALARIN_BLESSING] = {
			name = "valarin blessing"
			display = '?' color = color.WHITE
			desc =
			{
				"This blessing will give you the assistance of the Valar."
			}
			level = 50 weight = 5 cost = 1000
			allocation = { {50,1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					local dur = timed_effect.get(timed_effect.VALARIN_FAVOR)
					if dur and dur > 0 then
						timed_effect.inc(timed_effect.VALARIN_FAVOR, 15 + rng(15))
					else
						timed_effect.set(timed_effect.VALARIN_FAVOR, 15 + rng(15), 25)
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_MASS_RESURECTION] = {
			name = "& crumpled scroll~ of mass resurrection"
			display = '?' color = color.WHITE
			desc =
			{
				"This magical scroll sends a call to the halls of Mandos, issuing forth all those who have been",
				"slain by the reader."
			}
			level = 50 weight = 5 cost = 8000
			allocation = { {50,5} }
			flags = {
				SCALED_QUANTITY = 0 -- Don't want it to come in stacks at all
				ON_READ=function()
					message("You feel the souls of the dead coming back from the Halls of Mandos.")

					for k = 0, max_r_idx - 1 do
						local r_ptr = race_info_idx(k, 0)
						if has_flag(r_ptr, FLAG_UNIQUE) and not has_flag(r_ptr, FLAG_SPECIAL_GENE) then
							r_ptr.max_num = 1
						end
					end
					return true, true
				end
				BREAKAGE_CHANCE = 50
				NORM_ART = getter.object( 70 , 1 )
				FULL_NAME = true
			}
		}
		[SV_SCROLL_TELEPORT_LEVEL] = {
			name = "teleport level"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll will magically transport you to the level directly above or below, when read."
			}
			level = 50 weight = 5 cost = 1000
			allocation = { {50,1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					teleport_player_level()
					return true, true
				end
			}
		}
		[SV_SCROLL_GREATER_REMOVE_CURSE] = {
			name = "greater remove curse"
			display = '?' color = color.WHITE
			desc =
			{
				"This valuable scroll is inscribed with a powerful blessing capable of dispelling all but the",
				"mightiest curses which may have been laid on your possessions."
			}
			level = 50 weight = 5 cost = 1800
			allocation = { {50,2} }
			flags = {
				SCALED_QUANTITY = 1
				ON_READ=function()
					remove_all_curse()
					return true, true
				end
			}
		}
		[SV_SCROLL_RESET_RECALL] = {
			name = "reset recall"
			display = '?' color = color.WHITE
			desc =
			{
				"A strange formula is inscribed on this scroll, which allows you to define another place as the",
				"location to which recalls shall move you."
			}
			level = 50 weight = 5 cost = 1800
			allocation = { {50,2} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					return reset_recall(true), true
				end
			}
		}
		[SV_SCROLL_GREATER_IDENTIFICATION] = {
			name = "greater identification"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll will allow you to gain insight into an object's special properties.",
				"Only the highly magical objects, like rare rings and amulets or very unusual weapons",
				"and armour possess abilities which warrant the use of this magic."
			}
			level = 60 weight = 5 cost = 1500
			allocation = { {60,1} }
			flags = {
				SCALED_QUANTITY = 3
				ON_READ=function()
					return identify_fully(), true
				end
			}
		}
		[SV_SCROLL_PROTECTION_FROM_EVIL] = {
			name = "protection from evil"
			display = '?' color = color.WHITE
			desc =
			{
				"Upon reading the runes on this piece of paper, a faint aura of holiness will spring into existence",
				"around you, protecting you from blows of evil creatures, unless they are more powerful than you."
			}
			level = 60 weight = 5 cost = 1500
			allocation = { {60,1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					timed_effect.inc(timed_effect.PROTECTION_FROM_EVIL, 15 + rng(15))
					return true, true
				end
			}
		}
		[SV_SCROLL_SUMMON_UNDEAD] = {
			name = "summon undead"
			display = '?' color = color.WHITE
			desc =
			{
				"These spells scribed by ancient necromancers will call their horrible creatures to haunt you."
			}
			level = 60 weight = 5 cost = 0
			allocation = { {60,1} }
			flags = {
				ON_READ=function()
					local fl = flag_new()
					flag_set(fl, FLAG_UNDEAD, true)
					for k = 1, rng(3) do
						local obvious = summon_specific(player.py, player.px, dun_level, FACTION_DUNGEON, 0, 1000, true, fl, nil, nil, nil, nil, nil)
					end
					flag_free(fl, true)
					return true, obvious
				end
			}
		}
		[SV_SCROLL_AMNESIA] = {
			name = "amnesia"
			display = '?' color = color.WHITE
			desc =
			{
				"XXXXX"
			}
			level = 70 weight = 5 cost = 0
			allocation = { {70,1} }
			flags = {
				ON_READ=function()
					return true, lose_all_info()
				end
			}
		}
		[SV_SCROLL_VALARIN_STRENGTH] = {
			name = "valarin strength"
			display = '?' color = color.WHITE
			desc =
			{
				"This incantation lets you act as a warrior of Valinor for quite a while."
			}
			level = 70 weight = 5 cost = 2000
			allocation = { {70,1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_READ=function()
					local dur = timed_effect.get(timed_effect.VALARIN_FAVOR)
					if dur and dur > 0 then
						timed_effect.inc(timed_effect.VALARIN_FAVOR, 25 + rng(15))
					else
						timed_effect.set(timed_effect.VALARIN_FAVOR, 25 + rng(15), 50)
					end
					return true, true
				end
			}
		}
		[SV_SCROLL_MASS_GENOCIDE] = {
			name = "mass genocide"
			display = '?' color = color.WHITE
			desc =
			{
				"An astoundingly powerful death spell is stored in the runes of this spell.  Reading",
				"it will annihilate all nearby creatures. Only a few beings of legendary stature",
				"can withstand it."
			}
			level = 70 weight = 5 cost = 5000
			allocation = { {70,4} }
			flags = {
				SCALED_QUANTITY = 1
				ON_READ=function()
					mass_genocide(true)
					return true, true
				end
			}
		}
		[SV_SCROLL_SUMMON_DRAGONS] = {
			name = "summon dragons"
			display = '?' color = color.WHITE
			desc =
			{
				"XXXXX"
			}
			level = 80 weight = 5 cost = 0
			allocation = { {80,1} }
			flags = {
				ON_READ=function()
					local fl = flag_new()
					flag_set(fl, FLAG_DRAGON, true)
					for k = 1, rng(3) do
						local obvious = summon_specific(player.py, player.px, dun_level, FACTION_DUNGEON, 0, 1000, true, fl, nil, nil, nil, nil, nil)
					end
					flag_free(fl, true)
					return true, obvious
				end
			}
		}
		[SV_SCROLL_GREATER_RECHARGING] = {
			name = "greater recharging"
			display = '?' color = color.WHITE
			desc =
			{
				"XXXXX"
			}
			level = 80 weight = 5 cost = 4000
			allocation = { {80,2} }
			flags = {
				SCALED_QUANTITY = 1
				ON_READ=function()
					return true, true
				end
			}
		}
		[SV_SCROLL_RUNE_OF_PROTECTION] = {
			name = "rune of protection"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll is inscribed with a powerful protective incantation. When read, this will erect a",
				"strong magical ward around the location you currently stand on. Be aware that this magic is",
				"easily disturbed by already present structures and can thus not work where an object is lying or",
				"on a trap or on stairs."
			}
			level = 90 weight = 5 cost = 5000
			allocation = { {90,1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_READ=function()
					warding_glyph()
					return true, true
				end
			}
		}
		[SV_SCROLL_DEINCARNATION] = {
			name = "& old scroll~ of deincarnation"
			display = '?' color = color.WHITE
			desc =
			{
				"It allows you to leave your body to reincarnate into",
				"an other one, however your current body is lost in the process."
			}
			level = 90 weight = 5 cost = 150000
			allocation = { {90,140} }
			flags = {
				SCALED_QUANTITY = 0 -- Never in stacks
				ON_READ=function()
					if not get_check("Do you really want to leave your body? (beware, it'll be destroyed!) ") then
						return false, true
					end
					do_cmd_leave_body(false)
					return true, true
				end
				BREAKAGE_CHANCE = 50
				NORM_ART = getter.object( 70 , 51 )
				FULL_NAME = true
			}
		}
		[SV_SCROLL_DIVINATION] = {
			name = "divination"
			display = '?' color = color.WHITE
			desc =
			{
				"This scroll is inscribed with a ritual which allows you ",
				"to discern what fate holds in store for you."
			}
			level = 20 weight = 5 cost = 600
			allocation = { {30,1}, {45,1}, {55,1} }
			flags = {
				ON_READ=function()
					fates.learn_fates(1,
						{
							learned_all = "You have no unlearned fates.",
							no_fates    = "You have no unlearned fates."
						}) -- fates.learn_fates()

					return true, true
				end -- ON_READ()
				BREAKAGE_CHANCE = 50
			}
		}
	}

	[TV_POTION] =
	{
		color = color.LIGHT_BLUE
		name = "potions"
		desc =
		{
			"These mystic concoctions have a variety of strange effects",
		}
		symbol = '!'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{ COLD=true }
				SCALED_QUANTITY = 20
				BREAKAGE_CHANCE = 100
				FOUNTAIN = true
				ID_SKILL  = getter.skill("Magic")
			}
			display = '!'
			color = color.DARK
			weight = 4
		}

		[SV_POTION_SALT_WATER] = {
			name = "salt water"
			desc =
			{
				"A solution of salt in water, made for curing meat. Drinking it would cause violent nausea."
			}
			level = 0
			cost = 0
			allocation = { {0, 1} }
			flags = {
				ON_QUAFF=function()
					message("The potion makes you vomit!")
					timed_effect.set(timed_effect.FOOD, food.FOOD_STARVE - 1)
					timed_effect(timed_effect.POISON, 0)
					timed_effect.inc(timed_effect.PARALYZED, 4)
					return true
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_APPLE_JUICE] = {
			name = "apple juice"
			color = color.LIGHT_UMBER
			desc =
			{
				"A healthy fruit beverage."
			}
			level = 0
			cost = 1
			allocation = { {0, 1} }
			flags = {
				ON_QUAFF=function()
					message("You feel less thirsty.");
					return true
				end
				FOOD_VALUE = 200
			}
		}

		[SV_POTION_WATER] = {
			name = "water"
			color = color.WHITE
			desc =
			{
				"A small bottle filled with pure water."
			}
			level = 0
			cost = 1
			allocation = { {0, 1} }
			flags = {
				ON_QUAFF=function()
					message("You feel less thirsty.");
					return true
				end
				FOOD_VALUE = 100
			}
		}

		[SV_POTION_LIGHT_HEALING] = {
			name = "light healing"
			desc =
			{
				"This healthy drink heals a little damage you have taken."
			}
			level = 0
			cost = 10
			allocation = { {0, 1} }
			flags = {
				SCALED_QUANTITY = 30
				ON_QUAFF=function()
					return hp_player(rng.roll(5, 10))
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_INFRA_VISION] = {
			name = "infra-vision"
			desc =
			{
				"This potion temporarily grants or improves the ability to optically perceive heat sources."
			}
			level = 0
			cost = 10
			allocation = { {0, 1} }
			flags = {
				ON_QUAFF=function()
					if not timed_effect.get(timed_effect.INFRAVISION) then
						return timed_effect.set(timed_effect.INFRAVISION, rng(25) + 15, 3)
					else
						timed_effect.inc(timed_effect.INFRAVISION, 5)
					end
					return nil
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_SLIME_MOLD_JUICE] = {
			name = "slime mold juice"
			color = color.GREEN
			desc =
			{
				"A strange sort of yellowish-green slime, too viscous to even slosh in the bottle. And as I know",
				"you, you're going to slurp it down."
			}
			level = 0
			cost = 10
			allocation = { {0, 3} }
			flags = {
				ON_QUAFF=function()
					message("You feel less thirsty.");
					--- XXX TODO 2% chance of gaining slime mold powers
					return true
				end
				FOOD_VALUE = 400
			}
		}

		[SV_POTION_SPEED] = {
			name = "speed"
			desc =
			{
				"A magical drink which temporarily allows you to act much faster when imbibed."
			}
			level = 10
			cost = 25
			allocation = { {10, 1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_QUAFF=function()
					if not timed_effect.get(timed_effect.FAST) then
						return timed_effect.set(timed_effect.FAST, rng(25) + 15, 10)
					else
						timed_effect.inc(timed_effect.FAST, 5)
					end
					return nil
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESIST_HEAT] = {
			name = "resist heat"
			desc =
			{
				"This magical potion will make fire flow in your veins, rendering you less vulnerable to outward",
				"heat."
			}
			level = 10
			cost = 25
			allocation = { {10, 1} }
			flags = {
				ON_QUAFF=function()
					return timed_effect(timed_effect.RES_FIRE, rng(10) + 10, 50)
				end
				FOOD_VALUE = 50
				IGNORE = getter.resists { FIRE = true }
			}
		}

		[SV_POTION_RESIST_COLD] = {
			name = "resist cold"
			desc =
			{
				"This magical potion will for a short while grant you a familiarity with frost, so that you take",
				"less harm from extreme cold."
			}
			level = 10
			cost = 25
			allocation = { {10, 1} }
			flags = {
				ON_QUAFF=function()
					return timed_effect(timed_effect.RES_COLD, rng(10) + 10, 50)
				end
				FOOD_VALUE = 50
				IGNORE = getter.resists { COLD = true }
			}
		}

		[SV_POTION_LIGHT_CURING] = {
			name = "light curing"
			desc =
			{
				"This magical potion will cure blindness, somewhat clear the head, slow the effects of",
				"poison, and hasten the healing of cuts."
			}
			level = 10
			cost = 25
			allocation = { {10, 1} }
			flags = {
				ON_QUAFF=function()
					local ident = nil
					if (timed_effect(timed_effect.BLIND, 0)) then ident = true end
					if timed_effect(timed_effect.CUT, (timed_effect.get(timed_effect.CUT) or 0) / 2 - 50) then ident = true end
					if timed_effect(timed_effect.CONFUSED, (timed_effect.get(timed_effect.CONFUSED) or 0) / 2 - 50) then ident = true end
					if timed_effect(timed_effect.POISON, (timed_effect.get(timed_effect.POISON) or 0) / 2) then ident = true end
					hp_insanity(rng.roll(5, 10))
					return ident
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_UGLINESS] = {
			name = "ugliness"
			desc =
			{
				"This wicked potion slightly twists your features, making you appear less fair."
			}
			level = 10
			cost = 0
			allocation = { {10, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_CHR, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_BOLDNESS] = {
			name = "boldness"
			desc =
			{
				"This drink will improve your courage, dispelling all fear."
			}
			level = 10
			cost = 25
			allocation = { {10, 1} }
			flags = {
				ON_QUAFF=function()
					return set_afraid(0)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_LIGHT_POISON] = {
			name = "light poison"
			desc =
			{
				"This liquid will envenomate you, sapping your life away."
			}
			level = 20
			cost = 0
			allocation = { {20, 1} }
			flags = {
				ON_QUAFF=function()
					if player.resist(dam.POIS) < 50 then
						if timed_effect(timed_effect.POISON, rng(15) + 10, 1) then
							return true
						end
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_WEAKNESS] = {
			name = "weakness"
			desc =
			{
				"This nasty potion will sap your strength, making you weaker."
			}
			level = 20
			cost = 0
			allocation = { {20, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_STR, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_BLINDNESS] = {
			name = "blindness"
			desc =
			{
				"This slightly poisonous potion temporarily impedes your eyesight, making you unable to see",
				"a thing."
			}
			level = 20
			cost = 0
			allocation = { {20, 1} }
			flags = {
				ON_QUAFF=function()
					if player.resist(dam.BLIND) < 100 and not rng.percent(player.resist(dam.BLIND)) then
						if timed_effect.inc(timed_effect.BLIND, rng.number(100) + 100) then
							return true
						end
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_CURE_POISON] = {
			name = "cure poison"
			desc =
			{
				"This powerful antidote will completely negate the effect of poisons currently affecting you."
			}
			level = 20
			cost = 50
			allocation = { {20, 1} }
			flags = {
				ON_QUAFF=function()
					return set_poisoned(0)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_INVISIBILITY] = {
			name = "invisibility"
			desc =
			{
				"This magical brew will temporarily hide you from sight, and also attunes you to this state so",
				"that your eyes can still perceive your hidden form."
			}
			level = 20
			cost = 100
			allocation = { {20, 2} }
			flags = {
				SCALED_QUANTITY = 5
				ON_QUAFF=function()
					local t = 30 + rng(30)

					set_invis(player.tim_invis + t, 35)
					set_tim_invis(player.tim_invis + t)
 					return true
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_SEE_INVISIBLE] = {
			name = "see invisible"
			desc =
			{
				"This magical concoction will temporarily improve your eyesight so that you can see creatures",
				"otherwise unseen."
			}
			level = 20
			cost = 50
			allocation = { {20, 1} }
			flags = {
				ON_QUAFF=function()
					return set_tim_invis(player.tim_invis + 12 + rng(12))
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_AUGMENTATION] = {
			name = "augmentation"
			desc =
			{
				"This blessed potions will greatly improve all your abilities, if they can still be improved."
			}
			level = 30
			cost = 5000
			allocation = { {30, 8} }
			flags = {
				SCALED_QUANTITY = 2
				ON_QUAFF=function()
					local ident = nil
					if (do_inc_stat(A_STR)) then ident = true end
					if (do_inc_stat(A_INT)) then ident = true end
					if (do_inc_stat(A_WIS)) then ident = true end
					if (do_inc_stat(A_DEX)) then ident = true end
					if (do_inc_stat(A_CON)) then ident = true end
					if (do_inc_stat(A_CHR)) then ident = true end
					return ident
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
			}
		}

		[SV_POTION_RESTORE_CHARISMA] = {
			name = "restore charisma"
			desc =
			{
				"This useful potion restores drained charm."
			}
			level = 30
			cost = 300
			allocation = { {30, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_CHR, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_STRENGTH] = {
			name = "restore strength"
			desc =
			{
				"This useful potion restores drained strength."
			}
			level = 30
			cost = 300
			allocation = { {30, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_STR, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_BOOZE] = {
			name = "booze"
			desc =
			{
				"A small bottle of a dark brown distilled beverage. By its look and smell, it contains a fair",
				"amount of still muck and wood alcohol."
			}
			level = 30
			cost = 0
			allocation = { {30, 1} }
			flags = {
				ON_QUAFF=function()
					local ident = nil
					if player.resist(dam.CONFUSION) < 100 and not rng.percent(player.resist(dam.CONFUSION)) then
						if (timed_effect.inc(timed_effect.CONFUSED, rng.number(20) + 15)) then
							ident = true
						end
						if (rng(13) == 1) then
							ident = true
							if (rng(3) == 1) then
								lose_all_info()
							end
							teleport_player(100)
							wiz_dark()
							message("You wake up elsewhere with a sore head...")
							message("You can't remember a thing, or how you got here!")
						end
					end
					return ident
				end
				FOOD_VALUE = 400
			}
		}

		[SV_POTION_STUPIDITY] = {
			name = "stupidity"
			desc =
			{
				"This accursed potion will cloud your intellect, making you stupid."
			}
			level = 30
			cost = 0
			allocation = { {30, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_INT, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_SICKLINESS] = {
			name = "sickliness"
			desc =
			{
				"This unhealthy drink damages your health, reducing your physical constitution."
			}
			level = 30
			cost = 0
			allocation = { {30, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_CON, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_NAIVETY] = {
			name = "naivety"
			desc =
			{
				"This potion casts a shadow on your knowledge, making you foolish."
			}
			level = 40
			cost = 0
			allocation = { {40, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_WIS, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_CLUMSINESS] = {
			name = "clumsiness"
			desc =
			{
				"This nasty concoction will numb your nerves, making you clumsier."
			}
			level = 40
			cost = 0
			allocation = { {40, 1} }
			flags = {
				ON_QUAFF=function()
					return do_dec_stat(A_DEX, STAT_DEC_NORMAL)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_HEALING] = {
			name = "healing"
			desc =
			{
				"This sweet potion heals your bones well.",
			}
			level = 40
			cost = 300
			allocation = { {40, 1} }
			flags = {
				SCALED_QUANTITY = 7
				ON_QUAFF=function()
					return hp_player(250 + rng.roll(5, 50))
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_CURING] = {
			name = "curing"
			desc =
			{
				"This blessed potion helps to recover from a wide variety of ailments."
			}
			level = 40
			cost = 300
			allocation = { {40, 1} }
			flags = {
				SCALED_QUANTITY = 4
				ON_QUAFF=function()
					local ident = nil
					if timed_effect(timed_effect.BLIND, 0) then ident = true end
					if timed_effect(timed_effect.CONFUSED, 0) then ident = true end
					if timed_effect(timed_effect.CUT, 0) then ident = true end
					if timed_effect(timed_effect.POISON, 0) then ident = true end
					if timed_effect(timed_effect.STUN, 0) then ident = true end
					if heal_insanity(100 + rng.roll(5, 25)) then ident = true end
					return ident
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_INTELLIGENCE] = {
			name = "restore intelligence"
			desc =
			{
				"This beneficial potion will magically heal your brain, restoring drained intelligence."
			}
			level = 40
			cost = 300
			allocation = { {40, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_INT, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_CONSTITUTION] = {
			name = "restore constitution"
			desc =
			{
				"A beneficial magical concoction, restoring your damaged health."
			}
			level = 40
			cost = 300
			allocation = { {40, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_CON, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_MEMORIES] = {
			name = "restore memories"
			desc =
			{
				"Should your life force have been drained, this blessed brew will bring it back."
			}
			level = 50
			cost = 750
			allocation = { {50, 1} }
			flags = {
				SCALED_QUANTITY = 4
				ON_QUAFF=function()
					restore_level()
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_ENLIGHTENMENT] = {
			name = "enlightenment"
			desc =
			{
				"An exceptional magical drink which lets you \"remember\" your current location as if you had",
				"already seen all of it."
			}
			level = 50
			cost = 750
			allocation = { {50, 1} }
			flags = {
				SCALED_QUANTITY = 3
				ON_QUAFF=function()
					message("An image of your surroundings forms in your mind...")
					wiz_lite(true)
					return true
				end
				FOOD_VALUE = 50
			}
		}


		[SV_POTION_CORRUPTION] = {
			name = "corruption"
			desc =
			{
				"This concoction of evil substances will mold you toward the will of Morgoth."
			}
			level = 50
			cost = 0
			allocation = { {50, 3} }
			flags = {
				ON_QUAFF=function()
					message("You feel the dark corruptions of Morgoth coming over you !")
					corruption.gain()
				end
				FOOD_VALUE = 0
				FOUNTAIN = false
			}
		}

		[SV_POTION_POISON] = {
			name = "poison"
			desc =
			{
				"This liquid will badly envenomate you, sapping your life away quickly."
			}
			level = 50
			cost = 0
			allocation = { {50, 1} }
			flags = {
				ON_QUAFF=function()
					if player.resist(dam.POIS) < 50 then
						if timed_effect(timed_effect.POISON, rng(30) + 30, 3) then
							return true
						end
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_STUNNING] = {
			name = "stunning"
			desc =
			{
				"This liquid will shock your system, stunning you."
			}
			level = 50
			cost = 0
			allocation = { {50, 1} }
			flags = {
				ON_QUAFF=function()
					message("Your hair stands on end!")
					timed_effect(timed_effect.STUN, 75, 1)
					return true
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_DETONATIONS] = {
			name = "detonations"
			desc =
			{
				"This bottle is filled with a strange substance which will violently explode if strongly agitated.",
				"You don't want to know what it might do to your intestines."
			}
			level = 50
			cost = 1000
			allocation = { {50, 2} }
			flags = {
				ON_QUAFF=function()
					message("Massive explosions rupture your body!")
					take_hit(rng.roll(50, 20), "a potion of detonations")
					timed_effect(timed_effect.STUN, 75, 1)
					timed_effect(timed_effect.CUT, 4000, 1)
					return true
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_EXPERIENCE] = {
			name = "experience"
			desc =
			{
				"This exceptional drink instills into you knowledge, allowing you to advance further in your trade."
			}
			level = 60
			cost = 10000
			allocation = { {60, 3} }
			flags = {
				SCALED_QUANTITY = 2
				ON_QUAFF=function()
					local ee = (player.exp / 2) + 10
					if ee > 100000 then
						ee = 100000
					end
					if (ee + player.exp) > PY_MAX_EXP then
						ee = PY_MAX_EXP - player.exp
					end
					if ee < 10000 then
						ee = 10000
					end
					message("You feel more experienced.")
					gain_exp(ee)
					return true
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_SELF_KNOWLEDGE] = {
			name = "self knowledge"
			desc =
			{
				"A drink of insight, letting you know yourself better."
			}
			level = 60
			cost = 1500
			allocation = { {60, 1} }
			flags = {
				SCALED_QUANTITY = 3
				ON_QUAFF=function()
					message("You begin to know yourself a little better...")
					self_knowledge()
					return true
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_WISDOM] = {
			name = "restore wisdom"
			desc =
			{
				"This potion benefits the naive, bringing back their wisdom to what it once was."
			}
			level = 60
			cost = 300
			allocation = { {60, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_WIS, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_DEXTERITY] = {
			name = "restore dexterity"
			desc =
			{
				"This magical brew brings your agility back to its former glory should it have been reduced."
			}
			level = 60
			cost = 300
			allocation = { {60, 1} }
			flags = {
				SCALED_QUANTITY = 8
				ON_QUAFF=function()
					return do_res_stat(A_DEX, true)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RUINATION] = {
			name = "ruination"
			desc =
			{
				"This wicked potion will seriously damage your abilities beyond the powers of magical",
				"restoration."
			}
			level = 60
			cost = 0
			allocation = { {60, 1} }
			flags = {
				ON_QUAFF=function()
					message("Your nerves and muscles feel weak and lifeless!")
					take_hit(rng.roll(10, 10), "a potion of Ruination")
					dec_stat(A_DEX, 25, true)
					dec_stat(A_WIS, 25, true)
					dec_stat(A_CON, 25, true)
					dec_stat(A_STR, 25, true)
					dec_stat(A_CHR, 25, true)
					dec_stat(A_INT, 25, true)
					return true
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_RESISTANCE] = {
			name = "resistance"
			desc =
			{
				"This great potion infuses you with the power of the elements, so that you can better",
				"withstand their ravages."
			}
			level = 60
			cost = 1500
			allocation = { {60, 1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_QUAFF=function()
					timed_effect(timed_effect.RES_FIRE, rng(20) + 20, 50)
					timed_effect(timed_effect.RES_COLD, rng(20) + 20, 50)
					timed_effect(timed_effect.RES_ACID, rng(20) + 20, 50)
					timed_effect(timed_effect.RES_ELECTRICITY, rng(20) + 20, 50)
					return true
				end
				FOOD_VALUE = 50
				IGNORE = getter.resists
				{
					COLD = true
					FIRE = true
					ACID = true
					ELEC = true
				}
			}
		}

		[SV_POTION_LOSE_MEMORIES] = {
			name = "lose memories"
			desc =
			{
				"A wicked potion, making you less of an adventurer than by right you should be."
			}
			level = 70
			cost = 0
			allocation = { {70, 1} }
			flags = {
				ON_QUAFF=function()
					if (not player.has_intrinsic(FLAG_HOLD_LIFE) and (player.exp > 0)) then
						message("You feel your memories fade.")
						lose_exp(player.exp / 4)
						return true
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_PARALYZATION] = {
			name = "paralyzation"
			desc =
			{
				"This poisonous mixture will deprive your muscles of mobility for a time.  "
				"Its direct contact is able to penetrate any normal magical defense of free "
				"action."
			}
			level = 70
			cost = 0
			allocation = { {70, 1} }
			flags = {
				ON_QUAFF=function()
 					timed_effect.inc(timed_effect.PARALYZED, rng(5) + 5)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_BERSERK_STRENGTH] = {
			name = "berserk strength"
			desc =
			{
				"This strange drink will instill in you a wild battle rage for a while."
			}
			level = 70
			cost = 3500
			allocation = { {70, 1} }
			flags = {
				SCALED_QUANTITY = 10
				ON_QUAFF=function()
					set_afraid(0)
					if not timed_effect.get(timed_effect.FAST) then
						return timed_effect.set(timed_effect.FAST, rng(25) + 15, 10)
					else
						timed_effect.inc(timed_effect.FAST, 5)
					end
					if not timed_effect.get(timed_effect.LIFE_REGEN) then
						return timed_effect.set(timed_effect.LIFE_REGEN, rng(25) + 15, 10000)
					else
						timed_effect.inc(timed_effect.LIFE_REGEN, 5)
					end
					if not timed_effect.get(timed_effect.MANA_REGEN) then
						return timed_effect.set(timed_effect.MANA_REGEN, rng(25) + 15, 7000)
					else
						timed_effect.inc(timed_effect.MANA_REGEN, 5)
					end
					if not timed_effect.get(timed_effect.ARMOR) then
						return timed_effect.set(timed_effect.ARMOR, rng(25) + 15, 25)
					else
						timed_effect.inc(timed_effect.ARMOR, 5)
					end
					if not timed_effect.get(timed_effect.STRENGTH) then
						return timed_effect.set(timed_effect.STRENGTH, rng(25) + 15, 25)
					else
						timed_effect.inc(timed_effect.STRENGTH, 5)
					end
					return true
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_RESTORE_MANA] = {
			name = "restore mana"
			desc =
			{
				"This potion infuses the drinker with pure magic force, bringing their magical potential back to",
				"its full extent."
			}
			level = 70
			cost = 3500
			allocation = { {70, 1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_QUAFF=function()
					if (player.csp() < player.msp()) then
						player.csp(player.msp())
						message("You feel your head clear.")
						return true
					end
					return nil
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_FLYING] = {
			name = "flying"
			desc =
			{
				"This substance allows the consumer to will himself into the air."
			}
			level = 70
			cost = 3500
			allocation = { {70, 1} }
			flags = {
				SCALED_QUANTITY = 5
				ON_QUAFF=function()
					timed_effect(timed_effect.FLY, rng(15) + 10, 3)
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_GREAT_SPEED] = {
			name = "great speed"
			desc =
			{
				"An enhanced version of the traditional speed potion that gives much more speed."
			}
			level = 70
			cost = 15000
			allocation = { {70, 3} }
			flags = {
				SCALED_QUANTITY = 5
				ON_QUAFF=function()
					return timed_effect.set(timed_effect.FAST, rng(25) + 15, 25)
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
			}
		}

		[SV_POTION_SLOWNESS] = {
			name = "slowness"
			desc =
			{
				"A magical concoction which drains force from you, forcing you to move and act a lot slower,",
				"until it wears off."
			}
			level = 80
			cost = 0
			allocation = { {80, 2} }
			flags = {
				ON_QUAFF=function()
					if (not player.has_intrinsic(FLAG_FREE_ACT)) then
						return timed_effect.set(timed_effect.SLOW, rng(25) + 15, 10)
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_GREAT_HEALING] = {
			name = "great healing"
			desc =
			{
				"This blessed potion greatly heals you for a quick return to battle."
			}
			level = 80
			cost = 5000
			allocation = { {80, 1} }
			flags = {
				SCALED_QUANTITY = 3
				ON_QUAFF=function()
					return hp_player(500 + rng.roll(10, 100))
				end
				FOOD_VALUE = 50
			}
		}

		[SV_POTION_GREAT_POISON] = {
			name = "great poison"
			desc =
			{
				"This liquid will envenomate you possibly lethally."
			}
			level = 80
			cost = 0
			allocation = { {80, 1} }
			flags = {
				ON_QUAFF=function()
					if player.resist(dam) < 50 then
						if timed_effect(timed_effect.POISON, rng(30) + 30, 10) then
							return true
						end
					else
						if timed_effect(timed_effect.POISON, rng(10) + 10, 3) then
							return true
						end
					end
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_DISEASE] = {
			name = "disease"
			desc =
			{
				"This sickly drink brings an assortment of harm."
			}
			level = 80
			cost = 0
			allocation = { {80, 1} }
			flags = {
				ON_QUAFF=function()
					message("The potion makes you vomit intensely!")
					take_sanity_hit(rng.roll(20, 20), "a potion of disease")
					take_hit(rng.roll(30, 30), "a potion of disease")
					timed_effect.set(timed_effect.FOOD, food.FOOD_STARVE - 1)
					timed_effect(timed_effect.POISON, 0)
					timed_effect.inc(timed_effect.PARALYZED, 8)
					return nil
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_FIRE] = {
			name = "fire"
			desc =
			{
				"This fiery flask makes the fiercest chills harmless to you."
			}
			level = 80
			cost = 5000
			allocation = { {80, 1} }
			flags = {
				ON_QUAFF=function()
					message("It burns!")
					take_hit(rng.roll(5, 5), "a potion of fire")
					return timed_effect(timed_effect.RES_COLD, rng(10) + 10, 100)
				end
				FOOD_VALUE = 50
				IGNORE = getter.resists { FIRE = true COLD = true }
			}
		}

		[SV_POTION_COLD] = {
			name = "cold"
			desc =
			{
				"This magical mixture chills you for total immunity to heat."
			}
			level = 80
			cost = 5000
			allocation = { {80, 1} }
			flags = {
				ON_QUAFF=function()
					message("Cold!")
					take_hit(rng.roll(5, 5), "a potion of cold")
					return timed_effect(timed_effect.RES_FIRE, rng(10) + 10, 100)
				end
				FOOD_VALUE = 50
				IGNORE = getter.resists { FIRE = true COLD = true }
			}
		}

		[SV_POTION_LIFE] = {
			name = "life"
			desc =
			{
				"This kingly brew washes away your cares."
			}
			level = 90
			cost = 7500
			allocation = { {90, 1} }
			flags = {
				SCALED_QUANTITY = 1
				ON_QUAFF=function()
					local ident = nil
					if hp_player(player.mhp() - player.chp()) then ident = true end
					if hp_insanity(player.msane() - player.csane()) then ident = true end
					if timed_effect(timed_effect.BLIND, 0) then ident = true end
					if timed_effect(timed_effect.CONFUSED, 0) then ident = true end
					if timed_effect(timed_effect.CUT, 0) then ident = true end
					if timed_effect(timed_effect.POISON, 0) then ident = true end
					if timed_effect(timed_effect.STUN, 0) then ident = true end
					if timed_effect(timed_effect.SLOW, 0) then ident = true end
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
			}
		}

		[SV_POTION_DEATH] = {
			name = "death"
			desc =
			{
				"A potent and very quickly working poison, sometimes utilised by those who wish to end their",
				"lives. It is immensely toxic and will seriously hurt you even if you just get some of it on your",
				"hands."
			}
			level = 90
			cost = 0
			allocation = { {90, 1} }
			flags = {
				ON_QUAFF=function()
					message("A feeling of death flows through your body.")
					if player.undead_form.active then
						if player.undead_form.number_souls > 1 then
							player.undead_form.number_souls = player.undead_form.number_souls / 2
						end
					else
						take_hit(25000, "a potion of death")
					end
					return true
				end
				FOOD_VALUE = 0
				FOUNTAIN = false
			}
		}

		[SV_POTION_BLOOD_OF_LIFE] = {
			name = "& blood~ of life"
			desc =
			{
				"Quaffing this measure of living blood will imbue your body and soul"
				"with the power to escape death one time."
			}
			color = color.VIOLET
			level = 90
			cost = 75000
			allocation = { {90, 16} }
			flags = {
				SCALED_QUANTITY = 0  -- Never in stacks
				FULL_NAME = true
				FLAVOUR = "strangely phosphorescent"
				ON_QUAFF=function()
					message("You feel the blood of life running through your veins!")
					player.allow_one_death = player.allow_one_death + 1
					return true
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
				NORM_ART = getter.object(TV_POTION, SV_POTION_LIFE)
			}
		}

		[SV_POTION_INVULNERABILITY] = {
			name = "invulnerability"
			desc =
			{
				"This immensely powerful potion makes your physique almost indestructible for a very short",
				"time. Very rarely, an attempt to hurt you will still succeed, so don't be too confident."
			}
			level = 90
			cost = 15000
			allocation = { {90, 5} }
			flags = {
				ON_QUAFF=function()
					-- XXX
					message("Implement me!")
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
			}
		}

		[SV_POTION_GREAT_SLOWNESS] = {
			name = "great slowness"
			desc =
			{
				"This substance makes the world whirl by you."
			}
			level = 90
			cost = 0
			allocation = { {90, 1 } }
			flags = {
				ON_QUAFF=function()
					return timed_effect.set(timed_effect.SLOW, rng(50) + 15, 25)
				end
				FOOD_VALUE = 0
			}
		}

		[SV_POTION_LEARNING] = {
			name = "& shimmering potion~ of learning"
			desc =
			{
				"This old potion is supposed to grant more learning power",
				"to its user."
			}
			color = color.DARK
			level = 90
			cost = 100000
			allocation = { {90, 25} }
			flags = {
				SCALED_QUANTITY = 0   -- Never in stacks
				FULL_NAME = true
				ON_QUAFF=function()
					player.skill_points = player.skill_points + rng(4, 10 + luck( -4, 4))
					message(color.LIGHT_GREEN, "You can increase "..player.skill_points.." more skills.")
					player.redraw[FLAG_PR_STUDY] = true
					return true
				end
				FOOD_VALUE = 50
				FOUNTAIN = false
				NORM_ART = getter.object(TV_POTION, SV_POTION_LIFE)
			}
		}


	}

	---------------------- Lites ----------------------
	[TV_BOTTLE] =
	{
		color = color.WHITE
		name = "bottles"
		desc = {"A bottle."}
		symbol = '!'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{COLD=true}
			}
		}

		[SV_BOTTLE_EMPTY] = {
			name = "& empty bottle~"
			desc = {"A small, empty glass bottle, waiting to be filled."}
			display = '!' color = color.WHITE
			level = 1 weight = 2 cost = 0
			allocation = { {0,1} }
			flags = {
				EASY_KNOW = true
			}
		}
	}

	---------------------- Lites ----------------------
	[TV_LITE] =
	{
		color = color.YELLOW
		name = "lights"
		desc =
		{
			"These may enable you to see in the depths of the dungeon.",
		}
		symbol = '~'
		defaults =
		{
			flags =
			{
				WIELD_SLOT = INVEN_LITE
				EASY_KNOW = true
			}
		}

		[SV_LITE_TORCH] = {
			name = "& torch~ #"
			display = '~' color = color.UMBER
			desc =
			{
				"A piece of wood with an oily rag wrapped around its end."
				"When lit, it will give off a little light and much smoke."
			}
			level = 1 weight = 30 cost = 2
			allocation = { {1,1} }
			flags = {
				LITE = obvious(1)
				FUEL = obvious(4000)
				FUEL_MAX = 4000
				FUEL_LITE = obvious(1)
				FUEL_SOURCE = obvious(1)
				HARMED_BY = getter.resists{FIRE=true}
			}
		}
		[SV_LITE_LANTERN] = {
			name = "& lantern~ #"
			display = '~' color = color.LIGHT_UMBER
			desc =
			{
				"A brass container filled with oil, with a wick emerging"
				"from it, protected from draughts by a sheet of greased"
				"paper. It can be carried by a handle."
			}
			level = 15 weight = 50 cost = 150
			allocation = { {15,1} }
			flags = {
				LITE = obvious(2)
				FUEL = obvious(7500)
				FUEL_MAX = 15000
				FUEL_LITE = obvious(2)
				FUEL_SOURCE = obvious(2)
			}
		}
		[SV_LITE_FEANORIAN] = {
			name = "& feanorian lamp~"
			display = '~' color = color.LIGHT_BLUE
			level = 30 weight = 50 cost = 2500
			allocation = { {30,3} }
			flags = {
				LITE = obvious(3)
			}
		}
	}

	[TV_FUEL] =
	{
		color = color.YELLOW
		name = "fuel"
		desc = {
			"Flasks of fuel can be used to refuel light sources."
		}
		symbol = '!'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true}
			}
		}

		[SV_FUEL_OIL] = {
			name = "& flask~ of oil"
			desc = {"A small clay container, filled with thick oil.  The",
				"oil is flammable and can be used as lantern fuel."}
			display = '!' color = color.YELLOW
			level = 0 weight = 10 cost = 3
			allocation = { {0,1} }
			flags = {
				EASY_KNOW = true
				FUEL_SOURCE = obvious(2)
				FUEL = obvious(getter.random(2000,7000))
			}
		}
	}

	-----------------------------------------------------------
	-- Below this point has not been rewritten
	-----------------------------------------------------------

	---------------------- Magical books ----------------------
	[TV_BOOK] =
	{
		color = color.LIGHT_BLUE
		name = "books"
		desc =
		{
			"Inscribed with mystic spells, books are the mainstay of the",
			"mage classes, and may also be usefull to others.",
		}
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true}
				ID_SKILL  = getter.skill("Magic")
			}
		}
		symbol = '?'
		[SV_BOOK_RANDOM] = {
			name = "& book~ of #"
			display = '?' color = color.LIGHT_BLUE
			desc =
			{
				"The blood smudged on the cover makes you wonder how many people had it before you..."
			}
			level = 5 weight = 30 cost = 200
			allocation = { {5,1} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				FULL_NAME = true
				ON_MAKE  = function(obj, power)
					spell_books.make_random_book(obj, object_level / 2 + (power * 4))
				end
				OBJECT_VALUE = function(obj, value)
				   local spells = obj.flags[FLAG_SPELL_IDX]
				   local tmp = {}
				   foreach_flags(spells,
						function(flags, spl)
							%tmp.spl = spl
						end)
				   local level = spell(tmp.spl).level
				   return value * (level + 1) / 2
			   end -- OBJECT_VALUE
			}
		}
		[SV_BOOK_CANTRIPS] = {
			name = "& book~ of beginner cantrips"
			display = '?' color = color.WHITE
			desc =
			{
				"The blood smudged on the cover makes you wonder how many people had it before you..."
			}
			level = 5 weight = 30 cost = 100
			allocation = { {5,1} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Manathrust","Globe_of_Light","Ent-draught","Phase_Door","Sense_Monsters","Sense_Hidden","Magelock"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_BASIC_ELEMENTS] = {
			name = "& book~ of elemental cantrips"
			display = '?' color = color.RED
			desc =
			{
				"The blood smudged on the cover makes you wonder how many people had it before you..."
			}
			level = 5 weight = 30 cost = 100
			allocation = { {5,1} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Geyser","Vapor","Globe_of_Light","Stone Skin"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_MANA] = {
			name = "& tome~ of magical energy"
			display = '?' color = color.LIGHT_BLUE
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Manathrust","Remove_Curses","Elemental_Shield","Disruption_Shield"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_FIRE] = {
			name = "& tome~ of the eternal flame"
			display = '?' color = color.LIGHT_RED
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				IGNORE = getter.resists{FIRE=true}
				SPELL_IDX = getter.spells{"Globe_of_Light","Fireflash","Fire_Golem","Firewall","Fiery_Shield"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_AIR] = {
			name = "& tome~ of the blowing wind"
			display = '?' color = color.BLUE
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Noxious_Cloud","Poison_Blood","Invisibility","Sterilize","Wings_of_Winds","Thunderstorm"}
				IGNORE = getter.resists{ELEC=true}
				FULL_NAME = true
			}
		}
		[SV_BOOK_EARTH] = {
			name = "& tome~ of the impenetrable earth"
			display = '?' color = color.LIGHT_UMBER
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Stone_Skin","Dig","Stone_Prison","Shake","Strike"}
				IGNORE = getter.resists{ACID=true}
				FULL_NAME = true
			}
		}
		[SV_BOOK_WATER] = {
			name = "& tome~ of the everrunning wave"
			display = '?' color = color.LIGHT_BLUE
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Geyser","Vapor","Ent-draught","Tidal_Wave","Ice_Storm"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_TRANSLOCATION] = {
			name = "& tome~ of translocation"
			display = '?' color = color.LIGHT_BLUE
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Disarm","Phase_Door","Teleportation","Teleport_Away","Recall","Probability_Travel"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_NATURE] = {
			name = "& tome~ of the tree"
			display = '?' color = color.LIGHT_GREEN
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Grow_Trees","Healing","Recovery","Regeneration","Summon_Animal"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_DIVINATION] = {
			name = "& tome~ of knowledge"
			display = '?' color = color.LIGHT_DARK
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Sense_Monsters","Sense_Hidden","Reveal_Ways","Identify","Vision","Greater_Identify"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_TIME] = {
			name = "& tome~ of the ages"
			display = '?' color = color.BLUE
			desc =
			{
				"This tome seems to have trouble to decide whether it really exists now. Its flickering pages",
				"contain all that is known about the currents of time."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Magelock","Slow Monster","Essence of Speed","Banishment","Timeweave Folding"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_META] = {
			name = "& tome~ of meta spells"
			display = '?' color = color.VIOLET
			desc =
			{
				"This tome gives you deeper insights on the works of magic."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				ATTR_MULTI = true
				SPELLBOOK = true
				EASY_KNOW = true
				IGNORE = getter.resists{FIRE=true}
				SPELL_IDX = getter.spells{"Recharge","Disperse_Magic","Spellbinder","Tracker","Inertia Control","Manaflow"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_MIND] = {
			name = "& tome~ of the mind"
			display = '?' color = color.LIGHT_BLUE
			desc =
			{
				"This tome has no pages, just by holding it , it transfers knowledge."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Charm","Neural Blast","Confuse","Armor_of_Fear","Stun","Mindwave"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_UDUN] = {
			name = "& unholy tome~ of the hellflame"
			display = '?' color = color.VIOLET
			desc =
			{
				"This singed book smells like burned flesh.  Its power is as evident",
				"as its thirst for your blood."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				ATTR_MULTI = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Drain","Genocide","Wraithform","Flame_of_Udun"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_ERU] = {
			name = "& holy tome~ of eru iluvatar"
			display = '?' color = color.LIGHT_GREEN
			desc =
			{
				"This dusty tome is filled with ancient rituals ,",
				"designed to uncover all that is hidden."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"See_the_Music","Listen_to_the_Music","Know_the_Music","Lay_of_Protection"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_MANWE] = {
			name = "& holy tome~ of manwe sulimo"
			display = '?' color = color.LIGHT_BLUE
			desc =
			{
				"A large jewel encrusted tome that transfers",
				"wisdom and understanding to its wearer."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Manwe's_Blessing","Wind_Shield","Manwe's_Call","Avatar"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_TULKAS] = {
			name = "& war tome~ of tulkas"
			display = '?' color = color.LIGHT_RED
			desc =
			{
				"This tome fills you with glorious visions of total devastation.",
				"Anyone in you way shall be destroyed."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Divine_Aim","Whirlwind","Wave_of_Power"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_MELKOR] = {
			name = "& corrupted tome~ of melkor"
			display = '?' color = color.LIGHT_DARK
			desc =
			{
				"A black and scarlet flame springs from this tome, issuing",
				"a thunderous roar under which you think you hear the screams of tormented souls."
			}
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Curse","Corpse_Explosion","Mind_Steal"}
				FULL_NAME = true
			}
		}
		[SV_BOOK_YAVANNA] = {
			name = "& forest tome~ of yavanna"
			display = '?' color = color.LIGHT_GREEN
			level = 50 weight = 30 cost = 25000
			allocation = { {50,4} }
			dice = {1, 1}
			flags = {
				SPELLBOOK = true
				EASY_KNOW = true
				SPELL_IDX = getter.spells{"Charm_Animal","Grow_Grass","Tree_Roots","Nature's_Wrath","Uproot"}
				FULL_NAME = true
			}
		}
	}

	---------------------- Parchments ----------------------
	[TV_PARCHMENT] =
	{
		color = color.ORANGE
		name = "parchments"
		desc =
		{
			"A fragment of lamb's skin, inscribed with information.",
			"somebody thought it was important enought to write down,",
			"but that doesn't mean that you or I would agree with them.",
		}
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true}
			}
		}
		symbol = '?'
		[20] = {
			define_sval_as = "SV_BEGINNER_PARCHMENT"
			name = "Adventurer's guide to Middle-earth"
			display = '?' color = color.ORANGE
			desc =
			{
				"Read it !"
			}
			level = 0 weight = 5 cost = 1
			allocation = { {0,1} }
			flags = { ON_READ=display_parchement }
		}
	}

	---------------------- Corpses ----------------------
	[TV_CORPSE] =
	{
		color = color.LIGHT_UMBER
		name = "corpses"
		desc =
		{
			"Something has died, leaving this empty husk behind,",
			"and going on to someplace better than this foul world.",
		}
		symbol = '~'
		 -- 001
		[SV_CORPSE_CORPSE] = {
			name = "corpse"
			display = '~' color = color.LIGHT_UMBER
			desc =
			{
				"Whatever happened with this one, it wasn't pretty."
			}
			level = 20 weight = 80 cost = 0
			allocation = { {30,1} }
			flags = {
--				ON_EAT     = food.eat_corpse
--				FOOD_VALUE = 2500
				DECAY      = true
			}
		}
		[SV_CORPSE_SKELETON] = {
			name = "skeleton"
			display = '~' color = color.WHITE
			desc =
			{
				"Whatever happened to this one, it didn't leave much behind.",
			}
			level = 20 weight = 80 cost = 0
			allocation = { {30,1} }
			flags = {
--				ON_EAT     = food.eat_corpse
--				FOOD_VALUE = 500
			}
		}
		[SV_CORPSE_MEAT] = {
			name = "raw meat"
			display = '~' color = color.LIGHT_UMBER
			desc =
			{
				"Yuck, just looking at it makes your stomach upset..",
			}
			level = 20 weight = 10 cost = 0
			allocation = { {30,1} }
			flags = {
				-- DECAY flag will be copied over (or not) from corpse object
--				ON_EAT     = food.eat_corpse
--				FOOD_VALUE = 500
			}
		}
	}

	---------------------- Food ----------------------
	[TV_FOOD] =
	{
		color = color.LIGHT_UMBER
		name = "food"
		desc =
		{
			"Edible items may have mysteriouss effects. Or maybe",
			"just something to much on when you get hungry",
		}
		symbol = ','
		defaults = {
			display = ','
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true ACID=true}
			}
		}
		 -- 035
		[SV_FOOD_RATION] = {
			name = "& ration~ of food"
			display = ',' color = color.LIGHT_UMBER
			desc = {"Lightweight and filling. Not an incredible taste",
				"experience, but that'd be asking a bit much.",
				"You can 'E'at it."}
			level = 0 weight = 10 cost = 3
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				EASY_KNOW  = true
				ON_EAT     = food.eat_bland
				FOOD_VALUE = 5000
			}
		} -- SV_FOOD_RATION

		[SV_FOOD_BISCUIT] = {
			name = "& hard biscuit~"
			display = ',' color = color.LIGHT_UMBER
			desc = {"It doesn't look great, and 'E'ating it will only",
				"fill your stomacha bit, for a short time."}
			level = 0 weight = 2 cost = 1
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				EASY_KNOW  = true
				ON_EAT     = food.eat_bland
				FOOD_VALUE = 100
			}
		} -- SV_FOOD_BISCUIT

		[SV_FOOD_VENISON] = {
			name = "& strip~ of venison"
			display = ',' color = color.UMBER
			desc = {"It looks greats, and 'E'ating it will",
				"fill your stomacha well."}
			level = 0 weight = 2 cost = 2
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				EASY_KNOW  = true
				ON_EAT     = food.eat_bland
				FOOD_VALUE = 500
			}
		} -- SV_FOOD_VENISON

		[SV_FOOD_ALE] = {
			name = "& pint~ of fine ale"
			display = ',' color = color.YELLOW
			desc = {"A bottle of a dark beer-like beverage.  You can",
				"drink it by pressing 'E'.  Drinking it or destroying",
				"it will produce an empty bottle which can be used to",
				"extract potions from fountains."}
			level = 0 weight = 5 cost = 1
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				EASY_KNOW  = true
				ON_EAT     = food.eat_bottled
				FOOD_VALUE = 500

				PLAYER_DESTROY_POST = food.destroy_bottled
			}
		} -- SV_FOOD_ALE

		[SV_FOOD_WINE] = {
			name = "& pint~ of fine wine"
			display = ',' color = color.RED
			desc = {"A bottle of fine wine.  You can",
				"drink it by pressing 'E'.  Drinking it or destroying",
				"it will produce an empty bottle which can be used to",
				"extract potions from fountains."}
			level = 0 weight = 10 cost = 2
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				EASY_KNOW  = true
				ON_EAT     = food.eat_bottled
				FOOD_VALUE = 500

				PLAYER_DESTROY_PRE = food.destroy_bottled
			}
		} -- SV_FOOD_WINE
	} -- TV_FOOD
	---------------------- Mushrooms ----------------------
	[TV_MUSHROOM] =
	{
		color = color.LIGHT_UMBER
		name = "mushrooms"
		desc =
		{
			"The fruiting body of a fungus.  Some are poisonous, some ",
			"are healtful, and all of them just plain taste good."
		}
		symbol = ','
		defaults = {
			display = ','
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true ACID=true}
			}
		}
		[SV_MUSHROOM_TEST] = {
			name = "testing"
			display = ',' color = color.DARK
			level = 0 weight = 10 cost = 3
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
				ON_EAT     = food.eat_bland
				FOOD_VALUE = 100
			}
		}
	}
	---------------------- MONEY ----------------------
	[TV_GOLD] =
	{
		color = color.YELLOW
		name = "gold"
		desc =
		{
			"It's the *love* of money that's the root of all evil",
			"So if you don't love it, it's safe to collect as",
			"much as you can, right?",
		}
		symbol = '$'
		 -- 001
		[1] = {
			name = "copper"
			display = '$' color = color.UMBER
			level = 1 weight = 0 cost = 3
			flags = {
				GOLD_VALUE = 24
			}
		}
		 -- 002
		[2] = {
			name = "copper"
			display = '$' color = color.UMBER
			level = 1 weight = 0 cost = 4
			flags = {
				GOLD_VALUE = 32
			}
		}
		 -- 003
		[3] = {
			name = "copper"
			display = '$' color = color.UMBER
			level = 1 weight = 0 cost = 5
			flags = {
				GOLD_VALUE = 40
			}
		}
		 -- 004
		[4] = {
			name = "silver"
			display = '$' color = color.SLATE
			level = 1 weight = 0 cost = 6
			flags = {
				GOLD_VALUE = 48
			}
		}
		 -- 005
		[5] = {
			name = "silver"
			display = '$' color = color.SLATE
			level = 1 weight = 0 cost = 7
			flags = {
				GOLD_VALUE = 56
			}
		}
		 -- 006
		[6] = {
			name = "silver"
			display = '$' color = color.SLATE
			level = 1 weight = 0 cost = 8
			flags = {
				GOLD_VALUE = 64
			}
		}
		 -- 007
		[7] = {
			name = "garnets"
			display = '$' color = color.RED
			level = 1 weight = 0 cost = 9
			flags = {
				GOLD_VALUE = 72
			}
		}
		 -- 008
		[8] = {
			name = "garnets"
			display = '$' color = color.RED
			level = 1 weight = 0 cost = 10
			flags = {
				GOLD_VALUE = 80
			}
		}
		 -- 009
		[9] = {
			name = "gold"
			display = '$' color = color.YELLOW
			level = 1 weight = 0 cost = 12
			flags = {
				GOLD_VALUE = 96
			}
		}
		 -- 010
		[10] = {
			name = "gold"
			display = '$' color = color.YELLOW
			level = 1 weight = 0 cost = 14
			flags = {
				GOLD_VALUE = 112
			}
		}
		 -- 011
		[11] = {
			name = "gold"
			display = '$' color = color.YELLOW
			level = 1 weight = 0 cost = 16
			flags = {
				GOLD_VALUE = 128
			}
		}
		 -- 012
		[12] = {
			name = "opals"
			display = '$' color = color.LIGHT_WHITE
			level = 1 weight = 0 cost = 18
			flags = {
				GOLD_VALUE = 144
			}
		}
		 -- 013
		[13] = {
			name = "sapphires"
			display = '$' color = color.BLUE
			level = 1 weight = 0 cost = 20
			flags = {
				GOLD_VALUE = 160
			}
		}
		 -- 014
		[14] = {
			name = "rubies"
			display = '$' color = color.RED
			level = 1 weight = 0 cost = 24
			flags = {
				GOLD_VALUE = 192
			}
		}
		 -- 015
		[15] = {
			name = "diamonds"
			display = '$' color = color.WHITE
			level = 1 weight = 0 cost = 28
			flags = {
				GOLD_VALUE = 224
			}
		}
		 -- 016
		[16] = {
			name = "emeralds"
			display = '$' color = color.GREEN
			level = 1 weight = 0 cost = 32
			flags = {
				GOLD_VALUE = 256
			}
		}
		 -- 017
		[17] = {
			name = "mithril"
			display = '$' color = color.LIGHT_BLUE
			level = 1 weight = 0 cost = 40
			flags = {
				GOLD_VALUE = 320
			}
		}
		 -- 018
		[18] = {
			name = "pure mithril"
			display = '$' color = color.LIGHT_GREEN
			level = 1 weight = 0 cost = 80
			flags = {
				GOLD_VALUE = 640
			}
		}
	}

	---------------------- Amulets ----------------------
	[TV_AMULET] =
	{
		color = color.RED
		name = "amulets"
		desc = {"Amulets are fine pieces of jewelry, usually imbued with",
			"arcane magics."}
		symbol = '"'
		defaults = {
			display = '"'
			flags = {
				WIELD_SLOT = obvious(INVEN_NECK)
				HARMED_BY = getter.resists{ELEC=true}
				ID_SKILL  = getter.skill("Magic")
			}
		}
		[SV_TEST_AMULET] = {
			name = "testing amulet"
			desc = {"This magical bauble slows your metabolism,",
				"allowing you to subsist on less food."}
			color = color.DARK
			level = 1 weight = 2 cost = 250
			allocation = { {1, 100} }
			flags = {
				MIN_EGO_LEVEL = 1
				MAX_EGO_LEVEL = 10
				FORCE_EGO = 100
			}
		}
		[SV_POWERFUL_AMULET] = {
			name = "powerful amulet"
			desc = {"This magical bauble slows your metabolism,",
				"allowing you to subsist on less food."}
			color = color.DARK
			level = 2 weight = 2 cost = 250
			allocation = { {2, 100} }
			flags = {
				MIN_EGO_LEVEL = 10
				MAX_EGO_LEVEL = 20
				FORCE_EGO = 100
			}
		}
		--[[
		[SV_AMULET_SLOW_DIGEST] = {
			name = "slow digestion"
			desc = {"This magical bauble slows your metabolism,",
				"allowing you to subsist on less food."}
			color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {5, 1} }
			flags = {
				METAB_PCT = obvious(50)
			}
		}

		[SV_AMULET_RESIST_ACID] = {
			name = "acid resistance"
			desc = {"This magical talisman will make the corroding forces",
				"of acid less threatening to your health."}
			color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {10, 1} }
			flags = {
				RESIST = obvious(getter.resists{ACID=66})
			}
		}

		[SV_AMULET_RESIST_ELEC] = {
			name = "lightning resistance"
			desc = {"This amulet will protect you from electrical",
				"discharges and storms."}
			color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {10, 1} }
			flags = {
				RESIST = obvious(getter.resists{ELEC=66})
			}
		}
		[SV_AMULET_PORTABLE_HOLE] = {
			name = "& pitch black amulet~"
			desc = {"This actually looks like a hole, a very deep and dark hole..."}
			color = color.DARK_KHAKI
			level = 70 weight = 2 cost = 50000
			allocation = { {70, 90} }
			flags = {
				FLAVOUR = "pitch black"
				INSTA_ART = true
			}
		}
		[SV_AMULET_EYE_OF_SMAUG] = {
			name = "& fiery eye~"
			desc = {"This is the eye of a mighty red dragon."}
			color = color.CRIMSON
			level = 70 weight = 2 cost = 50000
			allocation = { {70, 90} }
			flags = {
				SPECIAL_GENE = true
				FLAVOUR = "fiery"
				INSTA_ART = true
			}
		}  
		--]]
	} -- TV_AMULET


	---------------------- Rings ----------------------
	[TV_RING] =
	{
		color = color.RED
		name = "rings"
		desc = {"Rings are fine pieces of jewelry, usually imbued with",
			"arcane magics."}
		symbol = '='
		defaults =
		{
			flags =
			{
				WIELD_SLOT=obvious(INVEN_RING)
				HARMED_BY = getter.resists{ELEC=true}
				ID_SKILL  = getter.skill("Magic")
			}
		}

		[SV_RING_SEARCHING] = {
			name = "searching"
			desc = {"This ring magically improves your attention, so you",
				"can detect hidden things better."}

			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {5, 1} }
			flags = {
				SKILL_BONUS = obvious(getter.skills{["Sneakiness"] =
										  getter.random(1000, 12000)})
			}
		}

		[SV_RING_SLOW_DIGEST] = {
			name = "slow digestion"
			desc = {"This magical bauble grants you some sustenance,",
				"allowing you to subsist on less food."}
			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {5, 1} }
			flags = {
				NUTRI_MOD = obvious(5)
			}
		}

		[SV_RING_RESIST_FEAR] = {
			name = "fear resistance"
			desc = {"This ring grants courage, so that you will never ",
				"become afraid."}

			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 300
			allocation = { {10, 2} }
			flags = {
				RESIST = obvious(getter.resists{FEAR=100})
			}
		}

		[SV_RING_RESIST_FIRE] = {
			name = "fire resistance"
			desc = {"This piece of jewellery grants you some protection"
				"from the burning heat of fire."}

			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {10, 1} }
			flags = {
				RESIST = obvious(getter.resists{FIRE=66})
			}
		}

		[SV_RING_RESIST_COLD] = {
			name = "cold resistance"
			desc = {"This piece of jewellery grants you some protection"
				"from the chilling forces of cold."}

			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 250
			allocation = { {10, 1} }
			flags = {
				RESIST = obvious(getter.resists{COLD=66})
			}
		}

		[SV_RING_RESIST_LITE_AND_DARK] = {
			name = "light and darkness resistance"
			desc = {"This ring protects against fluctuations in the light."}

			display = '=' color = color.DARK
			level = 30 weight = 2 cost = 3000
			allocation = { {30, 2} }
			flags = {
				RESIST = obvious(getter.resists{LITE=66, DARK=66})
			}
		}

		[SV_RING_RESIST_SOUND] = {
			name = "sound resistance"
			desc = {"This ring projects an aura of quiet around you,",
				"protecting you from loud noise."}

			display = '=' color = color.DARK
			level = 26 weight = 2 cost = 3000
			allocation = { {26, 2} }
			flags = {
				RESIST = obvious(getter.resists{SOUND=66})
			}
		}

		[SV_RING_RESIST_CONFUSION] = {
			name = "confusion resistance"
			desc = {"This ring stabilises your mind, protecting you ",
				"from all kinds of befuddlement."}

			display = '=' color = color.DARK
			level = 25 weight = 2 cost = 3000
			allocation = { {25, 2} }
			flags = {
				RESIST = obvious(getter.resists{CONFUSION=66})
			}
		}

		[SV_RING_RESIST_SHARDS] = {
			name = "shard resistance"
			desc = {"This piece of jewellery magically toughens your skin,"
				"protecting you from flying shrapnel."}

			display = '=' color = color.DARK
			level = 25 weight = 2 cost = 3000
			allocation = { {25, 2} }
			flags = {
				RESIST = obvious(getter.resists{SHARDS=66})
			}
		}

		[SV_RING_RESIST_NEXUS] = {
			name = "nexus resistance"
			desc = {"This ring of stability protects you from the ",
				"warping forces of nexus."}

			display = '=' color = color.DARK
			level = 24 weight = 2 cost = 3000
			allocation = { {24, 2} }
			flags = {
				RESIST = obvious(getter.resists{NEXUS=66})
			}
		}

		[SV_RING_RESIST_CHAOS] = {
			name = "chaos resistance"
			desc = {"This ring protects you from the horribly warping ",
				"forces of chaos."}

			display = '=' color = color.DARK
			level = 50 weight = 2 cost = 13000
			allocation = { {50, 2} }
			flags = {
				RESIST = obvious(getter.resists{CHAOS=66, CONFUSION=66})
			}
		}

		[SV_RING_RESIST_BLINDNESS] = {
			name = "blindness resistance"
			desc = {"This ring magically preserves your eyesight, making ",
				"you impervious to any attempt to blind you."}

			display = '=' color = color.DARK
			level = 60 weight = 2 cost = 7500
			allocation = { {60, 2} }
			flags = {
				RESIST = obvious(getter.resists{BLIND=100})
			}
		}

		[SV_RING_RESIST_DISENCHANT] = {
			name = "disenchantment resistance"
			desc = {"This rare ring of preservation protects your ",
				"equipment from attempts to sap its magic, also",
				"causing you to suffer less pain from such attacks"}

			display = '=' color = color.DARK
			level = 90 weight = 2 cost = 15000
			allocation = { {90, 10} }
			flags = {
				RESIST = obvious(getter.resists{DISENCHANT=66})
			}
		}

		[SV_RING_LEVITATION] = {
			name = "levitation"
			desc = {"When you put on this ring, you will be able to float",
				"just above the floor.  It prevents you from drowning, ",
				"and all your falls will be painless."}
			display = '=' color = color.DARK
			level = 5 weight = 2 cost = 200
			allocation = { {5, 1} }
			flags = {
				FLY = obvious(1)
			}
		}

		[SV_RING_FLYING] = {
			name = "flying"
			desc = {"This ring is imbued with the power of eagles. It",
				"grants you the power of flight."}
			display = '=' color = color.DARK
			level = 20 weight = 2 cost = 16000
			allocation = { {20, 3} }
			flags = {
				FLY = obvious(3)
			}
		}

		[SV_RING_PROTECTION] = {
			name = "protection"
			desc = {"This ring creates a magical aura around you, protecting you against the blows of your enemies."}
			display = '=' color = color.DARK
			level = 10 weight = 2 cost = 500
			allocation = { {10, 1} }
			-- XXX
			--to_a = getter.random(1,12)
			to_a = 12
			flags = {
				-- Force to_a to be shown, even if it's 0
				SHOW_AC_MODS = true
				ARMOR_ENCHANT = object_enchant_drop.ARMOR_ENCHANT_ABSOLUTE_AC
			}
		}

		[SV_RING_SLAYING] = {
			name = "slaying"
			desc = {"This ring magically improves your fighting prowess, allowing to hit more often and harder."}
			display = '=' color = color.DARK
			level = 40 weight = 2 cost = 1000
			allocation = { {40, 1} }
			flags = {
				-- Force to_h and to_d to be shown, even if
				-- one (or both) is 0
				SHOW_COMBAT_MODS = true
				WEAPON_ENCHANT = object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TH | object_enchant_drop.WEAPON_ENCHANT_ABSOLUTE_TD
			}
		}

		[SV_RING_SPEED] = {
			name = "speed"
			desc = {"This wonderful ring grants you additional energy, allowing you to act faster."}
			display = '=' color = color.DARK
			level = 75 weight = 2 cost = 100000
			allocation = { {75, 1} }
			flags = {
				SPEEDS = obvious(getter.speeds{GLOBAL=getter.random(1,12)})
			}
		}

		[SV_RING_SPELL_MASTERY] = {
			name = "spell mastery"
			desc = {"This ring allows the wearer a reduced spell failure rate."}
			display = '=' color = color.DARK
			level = 20 weight = 2 cost = 1000
			allocation = { {20, 1}, {40, 1}, {60, 2} }
			flags = {
				SPELL_FAILURE = obvious(getter.random(-15,-5))
			}
		}

		[SV_RING_ONE] = {
			name = "& plain gold ring~"
			desc = {"This seems to be a plain unadorned gold ring."}
			display = '=' color = color.GOLD
			level = 100 weight = 2 cost = 100000
			allocation = { {100, 100} }
			flags = {
				FLAVOUR = "plain gold"
				INSTA_ART = true
				SPECIAL_GENE = true
			}
		}

		[SV_RING_NARYA] = {
			name = "& ruby and gold ring~"
			display = '=' color = color.LIGHT_RED
			level = 90 weight = 2 cost = 100000
			allocation = { {90, 50} }
			flags = {
				FLAVOUR = "ruby and gold"
				INSTA_ART = true
				SPECIAL_GENE = true
			}
		}
	}
--[[ -- DGDGDGDG -- make them latter
	---------------------- Chests ----------------------
	[TV_TRAPKIT] = {
		name = "trap set"
		color = color.LIGHT_DARK
		desc = {"Trapping kits are used with the trapping ability",
			"to set deadly monster traps."}
		symbol = '`'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true}
			}
		}

		-- 001
		[1] = {
			name = "& catapult trap set~"
			desc = {"It must be loaded with sling bullets,",
				"which wil be fired at the monster who",
				"triggers the trap"}

			display = '`' color = color.RED
			level = 1 weight = 50 cost = 40
			allocation = { {1, 2}, {20, 2} }

			flags = {
				MULTIPLIER       = 2
				AMMO_PER_TRIGGER = 1
				SHOW_COMBAT_MODS = true

				SELECT_AMMO =
				function(trapkit)
					return traps.ammo_by_tval(trapkit,
								  TV_SHOT)
				end
				TRAPKIT_TRIP =
				function(trap, trapkit, ammo, who, y, x)
					return traps.fire_missile_ammo(trap,
								       trapkit,
								       ammo,
								       who,
								       y,
								       x)
				end
			} -- flags
		} -- 001
	} -- TV_TRAPKIT
-- ]]

	---------------------- Chests ----------------------
	[TV_CHEST] =
	{
		color = color.SLATE
		name = "chests"
		desc = {"Chests contain treasures, but are usually trapped."}
		symbol = '~'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{FIRE=true ACID=true}
				ID_SKILL  = getter.skill("Disarming")
			}
		}

		-- 001
		[1]  = {
			name = "& small wooden chest~"
			desc = {"A small wooden box, locked and possibly",
				"trapped. You wonder what might be inside."}

			display = '~' color = color.SLATE
			level = 5 weight = 250 cost = 0
			allocation = { {5, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_small
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 2
			}
		} -- 001

		-- 002
		[2]  = {
			name = "& large wooden chest~"
			desc = {"A large wooden box.  It doesn't seem",
				"to be locked -- why not risk a look insise?"}

			display = '~' color = color.SLATE
			level = 15 weight = 500 cost = 0
			allocation = { {15, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_large
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 2
			}
		} -- 002

		-- 003
		[3]  = {
			name = "& small iron chest~"
			desc = {"A small rectangular container made of wood",
				"and reinforced with iron corners and",
				"latches."}

			display = '~' color = color.SLATE
			level = 25 weight = 300 cost = 0
			allocation = { {25, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_small
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 4
			}
		} -- 003

		-- 004
		[4]  = {
			name = "& large iron chest~"
			desc = {"A big container made of wood, with a",
				"heavy iron lock."}

			display = '~' color = color.SLATE
			level = 35 weight = 1000 cost = 0
			allocation = { {35, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_large
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 4
			}
		} -- 004

		-- 005
		[5]  = {
			name = "& small steel chest~"
			desc = {"A small wooden box with strong steel",
				"locks and reinforcements."}

			display = '~' color = color.SLATE
			level = 45 weight = 500 cost = 0
			allocation = { {45, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_small
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 6
			}
		} -- 005

		-- 006
		[6]  = {
			name = "& large steel chest~"
			desc = {"A nearly indestructible chest of wood",
				"and steel. The lock doesn't look",
				"impenetrable, but it might be trapped."}

			display = '~' color = color.SLATE
			level = 55 weight = 1000 cost = 0
			allocation = { {55, 1} }

			flags = {
				ON_MAKE   = chests.setup
				OPEN_POST = chests.open_large
				LOCKED    = obvious(true)
				CLOSED    = obvious(true)
				AMOUNT    = 6
			}
		} -- 006

		-- 007
		[7]  = {
			name = "& ruined chest~"
			desc = {"A broken, empty chest."}

			display = '~' color = color.SLATE
			level = 0 weight = 1000 cost = 0
			allocation = { {0, 0} }

		} -- 007

		-- 008
		-- For testing pruposes
		[8] = {
			name = "& closeable chest~"
			desc = {"A chest which can be closed."}
			display = '~' color = color.RED
			level = 0 weight = 1 cost = 0
			allocation = { {0, 0} }

			flags = {
				OPEN = true
			}
		} -- 008
	} -- TV_CHEST


	---------------------- Junk ----------------------
	[TV_JUNK] =
	{
		color = color.WHITE
		name = "junk"
		desc = {
			"Junk is usually worthless, though experienced ",
			" archers can create ammo with them."
		}
		symbol = '~'

		[SV_BROKEN_HILT] = {
			name = "& broken hilt~"
			desc =
			{
				"The hilt of a former sword."
			}
			display = '~' color = color.WHITE
			level = 5 weight = 5 cost = 1
			allocation = { {5,50} }
			flags = {
				SPECIAL_GENE=true
			}
		}
		[SV_BROKEN_BLADE] = {
			name = "& broken blade~"
			desc =
			{
				"The blade of a former sword."
			}
			display = '~' color = color.WHITE
			level = 5 weight = 5 cost = 1
			allocation = { {5,50} }
			flags = {
				SPECIAL_GENE=true
			}
		}

		-- 001
		[SV_JUNK_SLIMY_THING] = {
			name = "& slimy thing~"
			desc = {"A disgusting... *thing* covered in slime"}

			display = '~' color = color.LIGHT_DARK
			level = 0 weight = 1 cost = 0
			allocation = { {0, 1} }

			flags = {
				EASY_KNOW= true
				GET_PRE=function()
					message("The slippery slimy thing slips from your grasp.")
					return true
				end
			}
		}

		-- 002
		[SV_JUNK_STICKY_WOOD] = {
			name = "& sticky piece~ of wood"
			desc = {"A piece of wood covered in resin"}

			display = '~' color = color.LIGHT_UMBER
			level = 0 weight = 1 cost = 0
			allocation = { {0, 1} }

			flags = {
				EASY_KNOW= true
				GET_POST=function()
					 message("The wood sticks to your hands.")
				 end
				DROP_PRE=function()
					 message("The resin covered wood is too sticky to let go of.")
					 return true
				 end
			}
		}
		[SV_JUNK] = {
			name = "& some junk piece~"
			display = '~' color = color.WHITE
			level = 5 weight = 5 cost = 1
			allocation = { {5,50} }
			flags = {
				SPECIAL_GENE=true
			}
		}
	} -- TV_JUNK

	[TV_SPIKE] =
	{
		color = color.SLATE
		name = "spikes"
		desc = {
			"A small spur of iron. Ramming one between a door ",
			"and its frame might jam it."
		}
		symbol = '~'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{ACID=true}
			}
		}

		[1] = {
			name = "& iron spike~"
			desc = {"A small spur of iron. Ramming one ",
				"between a door and its frame might jam it."}

			display = '~' color = color.SLATE
			level = 1 weight = 1 cost = 1
			allocation = { {1, 1} }

			flags = {
				EASY_KNOW= true
			}
		}
	} -- TV_SPIKE

	---------------------- Sticks ----------------
	[TV_WAND] =
	{
		color = color.GREEN
		name = "wands"
		desc =
		{
			"Wands are made by thaumaturgist alchemists to hold some of their spells.",
		}
		symbol = '-'
		defaults = {
			flags = {
				PARSE_WISH_STR = sticks.wand.parse_wish_str
				GRANT_WISH = sticks.wand.grant_wish
				GEM_SOCKET = obvious(true)
				HARMED_BY = getter.resists{ELEC=true}
				ID_SKILL  = getter.skills{["Magic-device"]=0 ["Magic"]=0}
			}
		}
		[SV_WAND_LESSER] = {
			name = "& lesser wand~ of #"
			display = '-' color = color.GREEN
			level = 4 weight = 10 cost = 100
			allocation = { {3,1}, {13,1}, {23,1}, {43,1}, {63,1}, {83,1} }
			flags = {
				ON_MAKE = sticks.wand.make_lesser
			}
		}
		[SV_WAND] = {
			name = "& wand~ of #"
			display = '-' color = color.GREEN
			level = 20 weight = 10 cost = 1000
			allocation = { {20,1}, {40,1}, {60,1}, {80,1} }
			flags = {
				ON_MAKE = sticks.wand.make_normal
			}
		}
		[SV_WAND_GREATER] = {
			name = "& greater wand~ of #"
			display = '-' color = color.GREEN
			level = 40 weight = 10 cost = 3000
			allocation = { {45,1}, {65,1}, {85,1} }
			flags = {
				ON_MAKE = sticks.wand.make_greater
			}
		}
	}
	[TV_GEMSTONE] =
	{
		color = color.YELLOW
		name = "gems"
		desc =
		{
			"Gems can be used to store ambient energy.  When plugged into",
			"a wand or an orb they will power it.",
		}
		symbol = '*'
		defaults =
		{
			flags =
			{
				HARMED_BY = getter.resists{ELEC=true}
				ID_SKILL  = getter.skills{["Magic-device"]=0 ["Magic"]=0}
			}
		}

		[SV_GEM_AGATE] = {
			name = "& agate gemstone~"
			desc = {"A basic gem for powering sticks."}
			display = '*' color = color.GREEN
			level = 4 weight = 10 cost = 100
			allocation = { {3,1}, {13,1}, {23,1}, }
			flags = {
				MANA_CURR = getter.mbonus(30, 200)
				GEMSTONE  = true
				ON_MAKE = function(obj)
						-- Gemstones start out at 1/2 max capacity
						obj.flags[FLAG_MANA_MAX] = 2 * obj.flags[FLAG_MANA_CURR]
					end
			}
		}
		[SV_GEM_RUBIS] = {
			name = "& rubis gemstone~"
			desc = {"A powerful gem for powering sticks."}
			display = '*' color = color.RED
			level = 34 weight = 10 cost = 1000
			allocation = { {33,2}, {43,1}, {53,1}, {63,1},  }
			flags = {
				MANA_CURR = getter.mbonus(100, 500)
				GEMSTONE  = true
				ON_MAKE = function(obj)
						-- Gemstones start out at 1/2 max capacity
						obj.flags[FLAG_MANA_MAX] = 2 * obj.flags[FLAG_MANA_CURR]
					end
			}
		}
		[SV_GEM_DIAMOND] = {
			name = "& diamond gemstone~"
			desc = {"A rare and extremely potent gem for powering sticks."}
			display = '*' color = color.WHITE
			level = 74 weight = 10 cost = 5000
			allocation = { {73,4}, {83,3}, {93,2},  }
			flags = {
				MANA_CURR = getter.mbonus(200, 1100)
				GEMSTONE  = true
				ON_MAKE = function(obj)
						-- Gemstones start out at 1/2 max capacity
						obj.flags[FLAG_MANA_MAX] = 2 * obj.flags[FLAG_MANA_CURR]
					end
			}
		}
	}
	[TV_ORB] =
	{
		color = color.UMBER
		symbol = '_'
		name = "obrs"
		desc =
		{
			"These store mystic powers, and can be very useful.",
		}
		defaults = {
			flags = {
				PARSE_WISH_STR = sticks.orb.parse_wish_str
				GRANT_WISH = sticks.orb.grant_wish
				GEM_SOCKET = obvious(true)
				HARMED_BY = getter.resists{ELEC=true}
				ID_SKILL  = getter.skills{["Magic-device"]=0 ["Magic"]=0}
			}
		}
		[SV_ORB_LESSER] = {
			name = "& lesser orb~ of #"
			display = '_' color = color.DARK
			level = 5 weight = 10 cost = 100
			allocation = { {5,1}, {15,1} }
			dice = {1, 2}
			flags = {
				ON_MAKE = sticks.orb.make
			}
		}
		[SV_ORB] = {
			name = "& orb~ of #"
			display = '_' color = color.DARK
			level = 35 weight = 10 cost = 100
			allocation = { {35,1}, {45,1}, {65,1} }
			dice = {1, 2}
			flags = {
				ON_MAKE = sticks.orb.make
			}
		}
		[SV_ORB_GREATER] = {
			name = "& greater orb~ of #"
			display = '_' color = color.DARK
			level = 75 weight = 10 cost = 100
			allocation = { {75,1}, {85,1}, {95,1} }
			dice = {1, 2}
			flags = {
				ON_MAKE = sticks.orb.make
			}
		}
	}

	---------------------- Mount ----------------------
	[TV_MOUNT] =
	{
		color = color.WHITE
		desc = {
			"The animal you are riding."
		}
		symbol = '~'

		[SV_MOUNT] = {
			name = "& mount~"
			display = '~' color = color.LIGHT_DARK
			level = 1 weight = 1 cost = 0
			allocation = { {1, 1} }

			flags = {
				SPECIAL_GENE = true
				TAKEOFF_PRE = function() message("You must dismount. Press 'm'.") return true end
			}
		}
	}

	---------------------- Symbiotes ----------------------
	[TV_SYMBIOTE] =
	{
		color = color.WHITE
		desc = {
			"The animal you are in symbiosis with."
		}
		symbol = '~'

		[SV_SYMBIOTE] = {
			name = "& smybiote~"
			display = '~' color = color.LIGHT_DARK
			level = 1 weight = 60 cost = 1
			allocation = { {1, 1} }

			flags = {
				SPECIAL_GENE  = true
				EASY_KNOW     = true
				WIELD_SLOT    = obvious(INVEN_SYMBIOTE)
				SHOW_AC_MODS  = true

				WIELD_PRE = function(obj)
					local monst = obj.flags[FLAG_MONSTER_OBJ]
					local name, pronoun  = symbiosis.get_symbiote_name(obj)

					if not monst then
						message(color.LIGHT_RED,
								"ERROR: Symbiote has no " ..
									"FLAG_MONSTER_OBJ!!")
						return true, false
					end

					if not symbiosis.can_symbiotize("enter into") then
						return true, false
					end
					if or_msg("You cannot enter into symbiosis with " ..
							  object_desc(obj, 0) .. "for some reason.",
							  obj.flags[FLAG_NO_SYMBIOSIS],
							  monst.flags[FLAG_NO_SYMBIOSIS])
					then
						return true, false
					end

					if monst.flags[FLAG_WIELD_PRE] then
						local func
						func = get_func_from_flag_or_registry(monst.flags,
															  FLAG_WIELD_PRE)
						if func(obj) then
							return true, false
						end
					end

					-- Warn player if the symbtioe might attack him/her
					if symbiosis.get_rebellion_chance(obj) > 0 then
						if wizard then
							message("Rebellion chance is " ..
									symbiosis.get_rebellion_chance(obj) ..
										" in 1000 per turn.")
						end
						message("You feel you cannot completely " ..
								"synchronize with " .. name .. ", and " ..
									pronoun .. " might attack you while " ..
									"in symbiosis.")
						local ret, key =
							get_com("Still wear " .. pronoun .. "? (y/n)")

						if not ret then
							return true, false
						end

						local key = strlower(strchar(key))

						if key ~= "y" then
							return true, false
						end
					end
				end, -- WIELD_PRE

				WIELD_POST = function(obj)
					local monst = obj.flags[FLAG_MONSTER_OBJ]

					if monst.flags[FLAG_WIELD_POST] then
						local func
						func = get_func_from_flag_or_registry(monst.flags,
															  FLAG_WIELD_POST)

						func(obj, monst)
					end
					player.redraw[FLAG_PR_MH] = true
				end -- WIELD_POST

				TAKEOFF_PRE = function(obj)
					local monst = obj.flags[FLAG_MONSTER_OBJ]

					if monst.flags[FLAG_TAKEOFF_PRE] then
						local func
						func = get_func_from_flag_or_registry(monst.flags,
															  FLAG_TAKEOFF_PRE)

						if func(obj, monst) then
							return true, false
						end
					end
				end -- TAKEOFF_PRE

				TAKEOFF_POST = function(obj)
					local monst = obj.flags[FLAG_MONSTER_OBJ]

					if monst.flags[FLAG_TAKEOFF_POST] then
						local func
						func = get_func_from_flag_or_registry(monst.flags,
														FLAG_TAKEOFF_POST)

						func(obj, monst)
					end
					player.redraw[FLAG_PR_MH] = true
				end -- TAKEOFF_POST
			}
		} -- [SV_SYMBIOTE]
	} -- [TV_SYMBIOTE]

	---------------------- Totems ----------------------
	[TV_TOTEM] =
	{
		color = color.VIOLET
		desc = {
			"A totem."
		}
		symbol = '"'

		[SV_TOTEM_PARTIAL] = {
			name = "& partial totem~ of #"
			desc = {"An item which a Summoner can use animate a copy of a ",
				"creature that exists only by the will of its master."}
			display = '"' color = color.VIOLET
			level = 1 weight = 10 cost = 1
			allocation = { {1, 1} }

			flags = {
				EASY_KNOW    = true
				SPECIAL_GENE = true
			}
		} -- [SV_TOTEM_PARTIAL]

		[SV_TOTEM_TRUE] = {
			name = "& true totem~ of #"
			desc = {"An item which a Summoner can use to revive a true ",
				"copy of a creatue."}
			display = '"' color = color.VIOLET
			level = 1 weight = 10 cost = 1
			allocation = { {1, 1} }

			flags = {
				EASY_KNOW    = true
				SPECIAL_GENE = true
			}
		} -- [SV_TOTEM_TRUE]
	} -- [TV_TOTEM]

	---------------------- Runes ----------------------
	[TV_RUNE1] = {
		color = color.WHITE
		name = "primary runes"
		desc = {"A primary rune, which specifies the effect " ..
			"of your runespell."}
		symbol = '?'

		defaults = {
			flags = {
				EASY_KNOW = true
				HARMED_BY = getter.resists{ELEC=true}
			}
		}

		[SV_RUNE_FIRE] = {
			name = "fire"
			display = '?' color = color.RED
			level = 10 weight = 2 cost = 1
			allocation = { {10, 1} }

			flags = {
				IGNORE        = getter.resists{FIRE=true}
				RUNE_DAM      = obvious(dam.FIRE)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_FIRE

		[SV_RUNE_COLD] = {
			name = "cold"
			display = '?' color = color.BLUE
			level = 12 weight = 2 cost = 1
			allocation = { {12, 1} }

			flags = {
				IGNORE        = getter.resists{COLD=true}
				RUNE_DAM      = obvious(dam.COLD)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_COLD

		[SV_RUNE_LIGHTNING] = {
			name = "lightning"
			display = '?' color = color.WHITE
			level = 13 weight = 2 cost = 1
			allocation = { {13, 1} }

			flags = {
				IGNORE        = getter.resists{ELEC=true}
				RUNE_DAM      = obvious(dam.ELEC)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_LIGHTNING

		[SV_RUNE_ACID] = {
			name = "acid"
			display = '?' color = color.LIGHT_BLUE
			level = 16 weight = 2 cost = 1
			allocation = { {16, 1} }

			flags = {
				IGNORE        = getter.resists{FIRE=true}
				RUNE_DAM      = obvious(dam.ACID)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_ACID

		[SV_RUNE_ELEMENT] = {
			name = "element"
			display = '?' color = color.GREEN
			level = 23 weight = 2 cost = 1
			allocation = { {23, 1} }

			flags = {
				IGNORE = getter.resists{
					FIRE = true
					COLD = true
					ACID = true
					ELEC = true
				}
				RUNE_DAM      = obvious(dam.PURE)
				RUNE_DAM_NAME = "elemental"
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_ELEMENT

		[SV_RUNE_KNOWLEDGE] = {
			name = "knowledge"
			display = '?' color = color.BLUE
			level = 6 weight = 2 cost = 200
			allocation = { {6, 1} }

			flags = {
				RUNE_DAM      = obvious(dam.KNOWLEDGE)
				RUNE_DAM_INFO = function() end
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_KNOWLEDGE

		[SV_RUNE_UNDEATH] = {
			name = "undeath"
			display = '?' color = color.LIGHT_GREEN
			level = 35 weight = 2 cost = 1000
			allocation = { {35, 1} }

			flags = {
				IGNORE        = getter.resists{ACID = true}
				RUNE_DAM_INFO = function() end
				RUNE_DAM      = obvious(dam.RAISE_DEAD)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_UNDEATH

		[SV_RUNE_LIFE] = {
			name = "life"
			display = '?' color = color.SLATE
			level = 3 weight = 2 cost = 200
			allocation = { {3, 1} }

			flags = {
				RUNE_DAM      = obvious(dam.HEAL)
				RUNE_HARDNESS = 1
			}
		} -- SV_RUNE_LIFE

		[SV_RUNE_PROTECTION] = {
			name = "protection"
			display = '?' color = color.LIGHT_GREEN
			level = 45 weight = 2 cost = 1500
			allocation = { {35, 1} }

			flags = {
				IGNORE        = getter.resists{ACID = true}
				RUNE_DAM      = obvious(dam.PROTECTION)
				RUNE_HARDNESS = 1
				RUNE_PROJ     = PROJECT_GRID
				RUNE_PROJ_NOT = (PROJECT_THRU | PROJECT_STOP | PROJECT_KILL)
				RUNE_EXCL_SHAPES = getter.flags{RUNE_SHAPE_VIEW=true}
				RUNE_DAM_INFO = function(spell, min_dam, max_dam)
					local is_self = false
					for i = 1, getn(spell.rune_shapes) do
						local shape = spell.rune_shapes[i].shape
						if shape == FLAG_RUNE_SHAPE_SELF then
							is_self = true
							break
						end
					end

					if not is_self then
						return ""
					end

					local min_ac = sroot(min_dam)
					local max_ac = sroot(max_dam)
					local min_dur = sroot(min_dam) + 1
					local max_dur = sroot(max_dam) + 10

					return format("AC %d to %d, duration %d to %d",
								  min_ac, max_ac, min_dur, max_dur)
				end
			}
		} -- SV_RUNE_PROTECTION

	} -- TV_RUNE1

	[TV_RUNE2] = {
		color = color.WHITE
		name = "secondary runes"
		desc = {"A secondary rune, which modifies a primary rune."}
		symbol = '?'

		defaults = {
			flags = {
				EASY_KNOW = true
				HARMED_BY = getter.resists{ELEC=true}
			}
		}

		[SV_RUNE_ARROW] = {
			name = "arrow"
			display = '?' color = color.BLUE
			level = 1 weight = 2 cost = 1
			allocation = { {10, 1} }

			flags = {
				IGNORE = getter.resists{FIRE=true}
				RUNE_SHAPE  = obvious(getter.runeshape("ARROW"))
				RUNE_WEIGHT = 2
			}
		} -- SV_RUNE_ARROW

		[SV_RUNE_RAY] = {
			name = "ray"
			display = '?' color = color.BLUE
			level = 10 weight = 2 cost = 300
			allocation = { {10, 1} }

			flags = {
				IGNORE      = getter.resists{FIRE=true, COLD=true}
				RUNE_SHAPE  = obvious(getter.runeshape("RAY"))
				RUNE_WEIGHT = 1
			}
		} -- SV_RUNE_RAY

		[SV_RUNE_SPHERE] = {
			name = "sphere"
			display = '?' color = color.BLUE
			level = 15 weight = 2 cost = 1000
			allocation = { {15, 1} }

			flags = {
				IGNORE      = getter.resists{FIRE=true, COLD=true, ELEC=true}
				RUNE_SHAPE  = obvious(getter.runeshape("SPHERE"))
				RUNE_WEIGHT = 1
			}
		} -- SV_RUNE_SPHERE

		[SV_RUNE_WAVE] = {
			name = "wave"
			display = '?' color = color.LIGHT_BLUE
			level = 30 weight = 2 cost = 5000
			allocation = { {30, 1} }

			flags = {
				IGNORE = getter.resists{FIRE=true}
				RUNE_SHAPE  = obvious(getter.runeshape("WAVE"))
				RUNE_WEIGHT = 2
			}
		} -- SV_RUNE_WAVE

		[SV_RUNE_SWARM] = {
			name = "swarm"
			display = '?' color = color.LIGHT_RED
			level = 30 weight = 2 cost = 5000
			allocation = { {30, 1} }

			flags = {
				IGNORE      = getter.resists{FIRE=true}
				RUNE_SHAPE  = obvious(getter.runeshape("SWARM"))
				RUNE_WEIGHT = 2
			}
		} -- SV_RUNE_SWARM

		[SV_RUNE_VIEW] = {
			name = "view"
			display = '?' color = color.LIGHT_GREEN
			level = 60 weight = 2 cost = 10000
			allocation = { {60, 1} }

			flags = {
				IGNORE      = getter.resists{FIRE=true}
				RUNE_SHAPE  = obvious(getter.runeshape("VIEW"))
				RUNE_WEIGHT = 2
			}
		} -- SV_RUNE_VIEW

		[SV_RUNE_SELF] = {
			name = "self"
			display = '?' color = color.BLUE
			level = 3 weight = 2 cost = 40
			allocation = { {3, 1} }

			flags = {
				IGNORE        = getter.resists{FIRE=true}
				RUNE_SHAPE    = obvious(getter.runeshape("SELF"))
				RUNE_WEIGHT   = 2
				RUNE_PROJ_NOT = PROJECT_GRID
			}
		} -- SV_RUNE_WAVE

		[SV_RUNE_FUTURE] = {
			name = "future"
			display = '?' color = color.VIOLET
			level = 20 weight = 2 cost = 1000
			allocation = { {20, 4} }

			flags = {
				IGNORE      = getter.resists{FIRE=true}
				RUNE_SHAPE  = obvious(getter.runeshape("FUTURE"))
				RUNE_WEIGHT = 3
			}
		} -- SV_RUNE_FUTURE

		[SV_RUNESTONE] = {
			name = "runestone"
			desc = {"Can be used to store a runespell, for easier casting."}
			display = '?' color = color.VIOLET
			level = 1 weight = 2 cost = 1
			allocation = { {15, 1} }

			flags = {
				IGNORE     = getter.resists{FIRE=true, COLD=true, ELEC=true}
				RUNE_STONE = obvious(true)
			}
		} -- SV_RUNESTONE
	} -- TV_RUNE2

	[TV_TOOL] = {
		color = color.SLATE
		name = "tools",

		defaults = {
			flags = {
				EASY_KNOW  = true
				WIELD_SLOT = obvious(INVEN_TOOL)
			} -- flags
		} -- defaults

		[SV_SHOVEL] = {
			name = "& shovel~"
			desc = {"A simple digging tool for shovelling away rubble and",
				"maybe even soft rock."}
			display = '\\' color = color.SLATE
			level = 1 weight = 60 cost = 10
			allocation = { {5, 2} }

			flags = {
				DIG_POWER = obvious(getter.flags{
					SUBST_ROCK=20
					SUBST_SAND=20
					SUBST_TREE=20
					SUBST_ICE=20
				})
			}
		} -- SV_SHOVEL

		[SV_PICK] = {
			name = "& pick~"
			desc = {"A heavy digging tool, primarily for earthworking,",
				"but also good for tunnelling through stone, if one is",
				"willing to perform the time-consuming labour."}

			display = '\\' color = color.SLATE
			level = 1 weight = 150 cost = 50
			allocation = { {10, 2} }

			flags = {
				DIG_POWER = obvious(getter.flags{
					SUBST_ROCK=40
					SUBST_SAND=40
					SUBST_TREE=40
					SUBST_ICE=40
				})
			}
		} -- SV_PICK
		[SV_THIEVES_TOOLS] = {
			name = "& set~ of thieves tools"
			desc = {"A set of picks, probes and tools to aid in picking of locks and the disarmament",
				"of traps. While extremely helpful, they do require some skill to be able to use.",
			}
			display = '\\' color = color.SLATE
			level = 1 weight = 10 cost = 250
			allocation = { {10, 2} }

			flags = {
				SKILL_BONUS = obvious(getter.skills{["Disarming"] = getter.random(1000, 5000)})
				WIELD_PRE = function(obj)
					if skill(SKILL_DISARMING).value < 5000 then
						message("You must have at least five disarming skill to use these.")
						return true, false
					end
				end
			}
		} -- SV_PICK
	} -- TV_TOOL

	[TV_BACKPACK] = {
		color = color.RED
		name = "backpacks"
		desc = {"A backpack, to store stuff inside."}
		symbol = '~'

		defaults = {
			flags = {
				WIELD_SLOT = obvious(INVEN_BACKPACK)
			} -- flags
		} -- defaults

		[SV_BACKPACK_DEFAULT] = {
			name = "& backpack~"
			desc = {"Your classic backpack."}
			display = '~' color = color.WHITE
			level = 1 weight = 20 cost = 10
			allocation = { {1, 5}, {15, 3}, {30, 2}, {45, 1} }
		}
		[SV_BACKPACK_LARGE] = {
			name = "& large backpack~"
			desc = {"A bigger than normal backpack."}
			display = '~' color = color.YELLOW
			level = 1 weight = 30 cost = 1000
			allocation = { {80, 50} }

			flags = {
				EQUIPMENT_SIZE = obvious(getter.body_parts{ INVEN=3 })
			}
		}
	} -- TV_BACKPACK

	[TV_QUIVER] = {
		color = color.YELLOW
		name = "quivers"
		desc = {"A quiver, to store ammo inside."}
		symbol = '~'

		defaults = {
			flags = {
				WIELD_SLOT = obvious(INVEN_QUIVER)
				ID_SKILL  = getter.skill("Archery")
			} -- flags
		} -- defaults

		[SV_QUIVER_SMALL] = {
			name = "& quiver~"
			desc = {"A quiver."}
			display = '~' color = color.YELLOW
			level = 1 weight = 10 cost = 50
			allocation = { {1, 5} }

			flags = {
				EQUIPMENT_SIZE = obvious(getter.body_parts{ AMMO=2 })
			}
		}
	} -- TV_QUIVER
}

