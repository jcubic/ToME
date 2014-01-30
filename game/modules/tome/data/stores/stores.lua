-- Stores-only themes :

-- For the general store :
rand_obj.themed.register_new("THEME_STORE_FOOD", 
{
	[TV_FOOD] = 1 
})
rand_obj.themed.register_new("THEME_STORE_TOOLS",
{
	[TV_SPIKE] = 1
	[TV_TOOL] = 1
	[TV_LITE] = 1
	[TV_TRAPKIT] = 1
	[TV_BACKPACK] = 1
})
rand_obj.themed.register_new("THEME_STORE_AMMUNITION",
{
	[TV_SHOT] = 1
	[TV_ARROW] = 1
})

-- For the armory :
rand_obj.themed.register_new("THEME_STORE_ARMOR",
{
	[TV_BOOTS] = 1
	[TV_GLOVES] = 1
	[TV_LIGHT_HELM] = 1
	[TV_HEAVY_HELM] = 1
	[TV_SHIELD] = 1
	[TV_CLOAK] = 1
	[TV_LIGHT_ARMOUR] = 1
	[TV_HEAVY_ARMOUR] = 1
})

-- For the weaponsmith :
rand_obj.themed.register_new("THEME_STORE_WEAPON",
{
	[TV_BOW] = 1
	[TV_SLING] = 1
	[TV_HAFTED] = 1
	[TV_POLEARM] = 1
	[TV_SWORD] = 1
	[TV_KNIFE] = 1
	[TV_AXE] = 1
})

-- A theme which always return darkstone weapons
rand_obj.themed.register_new("THEME_STORE_DARKSTONE_WEAPON",
	function(obj_level, theme_flags, extra_filter, egos, artifact_chance, add_chances)
		local okay = 
		{
			[TV_POLEARM] = true
			[TV_AXE] = true
			[TV_HAFTED] = true
			[TV_KNIFE] = true
			[TV_SWORD] = true
		}
		local obj = rand_obj.get_obj(obj_level, {function(k_idx) return %okay[k_info(k_idx).tval] end, extra_filter}, egos, artifact_chance, add_chances, true)

		-- Add the darkstone ego and apply the magic
		obj.ego_id[5] = EGO_WEAPON_DARKSTONE

		if type(obj_level) == "table" then
			obj_level = obj_level.max
		end
		apply_magic(obj, obj_level, true, false, false)

		return obj
	end
)

-- For the temple :
rand_obj.themed.register_new("THEME_STORE_HAFTED",
{
	[TV_HAFTED] = 1
})
rand_obj.themed.register_new("THEME_STORE_POTION",
{
	[TV_POTION] = 1
})

-- For the alchemist :
rand_obj.themed.register_new("THEME_STORE_SCROLL",
{
	[TV_SCROLL] = 1
})


-- Todo:
-- * 'Flavorful' stores by race
-- * Make stores in towns far from Bree generally stock
--   better items than stores close to Bree?

-------------------------- Basic shops --------------------------
new_store
{
	define_as = "store.STORE_GENERAL"
	name = "General Store"
	display = '1' color = color.LIGHT_UMBER
	max_obj = 15
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "theme_and_list"
	item_kinds =
	{
		{ chance=100 item="& Ration~ of Food" },
		{ chance=100 item="& Hard Biscuit~" },
		{ chance=70 item="& Shovel~" },
		{ chance=30 item="& Pick~" },
		{ chance=100 item="& Iron Spike~" },
		{ chance=100 item="& torch~ #" },
		{ chance=100 item="& torch~ #" },
		{ chance=100 item="& filthy rag~" },
		{ chance=100 item="& filthy rag~" },
		{ chance=15 item="& lantern~ #" },
		{ chance=100 item="& flask~ of oil" },
		{ chance=100 item="& flask~ of oil" },
		{ chance=100 item="& flask~ of oil" },
	}
	flags =
	{
		LEVEL=function() return 2 end
		STORE_MAINTAIN_TURNS = 500
		STORE_THEME = getter.flags {
			THEME_STORE_FOOD = 40
			THEME_STORE_AMMUNITION = 30
			THEME_STORE_TOOLS = 30
		}
		STORE_THEME_CHANCE = 30

	},
}

new_store
{
	define_as = "store.STORE_ARMOURY"
	name = "Armoury"
	display = '2' color = color.SLATE
	max_obj = 24
	store_make_num = 15
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "theme_and_list"
	item_kinds =
	{
		{ chance=100 item="& robe~" },
		{ chance=100 item="& soft leather jacket~" },
		{ chance=100 item="& splint mail~" },
		{ chance=100 item="& metal scale mail~" },
		{ chance=100 item="& chain mail~" },
		{ chance=100 item="& leather cap~" },
		{ chance=100 item="& partial helm~" },
		{ chance=100 item="& set~ of leather gloves" },
		{ chance=100 item="& wooden shield~" },
		{ chance=100 item="& buckler~" },
	}
	flags =
	{
		LEVEL=function() return 5 end
		STORE_MAINTAIN_TURNS = 3000
		STORE_THEME = getter.flags {
			THEME_STORE_ARMOR = 100
		}
		STORE_THEME_CHANCE = 50

	},
}


new_store
{
	define_as = "store.STORE_WEAPONS"
	name = "Weaponsmith"
	display = '3' color = color.WHITE
	max_obj = 24
	store_make_num = 15
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "theme_and_list"
	item_kinds =
	{
		{ chance=100 item="& pike~" },
		{ chance=50 item="& glaive~" },
		{ chance=100 item="& flint axe~" },
		{ chance=50 item="& biscayan axe~" },
		{ chance=100 item="& club~" },
		{ chance=50 item="& hand hammer~" },
		{ chance=100 item="& knife~" },
		{ chance=50 item="& misericorde~" },
		{ chance=100 item="& rapier~" },
		{ chance=50 item="& schiavona~" },
		{ chance=100 item="& short bow~" },
		{ chance=100 item="& bodkin arrow~" },
		{ chance=100 item="& short sling~" },
		{ chance=100 item="& sling stone~" },
	}
	flags =
	{
		LEVEL=function() return 5 end
		STORE_MAINTAIN_TURNS = 3000
		STORE_THEME = getter.flags {
			THEME_STORE_WEAPON = 50
			THEME_STORE_AMMUNITION = 40
			THEME_STORE_DARKSTONE_WEAPON = 10
		}
		STORE_THEME_CHANCE = 50

	},
}


new_store
{
	define_as = "store.STORE_TEMPLE"
	name = "Temple"
	display = '4' color = color.GREEN
	max_obj = 24
	store_make_num = 20
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "theme_and_list"
	item_kinds =
	{
		{ chance=100 item="valarin favor" },
		{ chance=100 item="valarin favor" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="remove curse" },
		{ chance=100 item="remove curse" },
		{ chance=100 item="greater remove curse" },
		{ chance=100 item="light healing" },
		{ chance=100 item="light healing" },
		{ chance=100 item="light healing" },
		{ chance=100 item="light healing" },
		{ chance=100 item="light curing" },
		{ chance=100 item="light curing" },
		{ chance=100 item="light curing" },
		{ chance=100 item="restore memories" },
		{ chance=100 item="restore memories" },
		{ chance=100 item="healing" },
		{ chance=100 item="curing" },
		{ chance=100 item="curing" },
	}
	flags =
	{
		LEVEL=function() return 10 end
		STORE_MAINTAIN_TURNS = 1000
		STORE_THEME = getter.flags {
			THEME_STORE_POTION = 70
			THEME_STORE_HAFTED = 30
		}
		STORE_THEME_CHANCE = 30

	},
}

new_store
{
	define_as = "store.STORE_ALCHEMY"
	name = "Alchemy shop"
	display = '5' color = color.BLUE
	max_obj = 24
	store_make_num = 20
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	item_kinds =
	{
		{ chance=100 item="identification" },
		{ chance=100 item="identification" },
		{ chance=100 item="identification" },
		{ chance=100 item="identification" },
		{ chance=100 item="light" },
		{ chance=100 item="phase door" },
		{ chance=100 item="phase door" },
		{ chance=100 item="confuse monster" },
		{ chance=100 item="magic mapping" },
		{ chance=100 item="detect objects" },
		{ chance=100 item="detect traps" },
		{ chance=100 item="detect traps" },
		{ chance=100 item="detect traps" },
		{ chance=100 item="detect invisible monsters" },
		{ chance=100 item="recharging" },
		{ chance=100 item="recharging" },
		{ chance=100 item="satisfy hunger" },
		{ chance=100 item="satisfy hunger" },
		{ chance=100 item="satisfy hunger" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="word of recall" },
		{ chance=100 item="greater identification" },
		{ chance=100 item="restore strength" },
		{ chance=100 item="restore intelligence" },
		{ chance=100 item="restore dexterity" },
		{ chance=100 item="restore wisdom" },
		{ chance=100 item="restore constitution" },
		{ chance=50 item="restore charisma" },
	}
	flags =
	{
		LEVEL=function() return 10 end
		STORE_MAINTAIN_TURNS = 1000
		STORE_THEME = getter.flags {
			THEME_STORE_POTION = 40
			THEME_STORE_SCROLL = 60
		}
		STORE_THEME_CHANCE = 50

	},
}

--[[
new_store
{
	define_as = "store.STORE_MAGIC"
	name = "Magic shop"
	display = '6' color = color.RED
	max_obj = 24
	actions = {	store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_SELL,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	item_kinds =
	{
		{ chance=100 item="Protection" },
		{ chance=100 item="Levitation" },
		{ chance=100 item="Protection" },
		{ chance=100 item="Fire Resistance" },
		{ chance=100 item="Cold Resistance" },
		{ chance=100 item={TV_RING, SV_RING_SLOW_DIGEST} },
		{ chance=100 item={TV_AMULET, SV_AMULET_SLOW_DIGEST} },
		{ chance=100 item="Acid Resistance" },
		{ chance=100 item="Lightning Resistance" },
		{ chance=100 item="Searching" },
		{ chance=100 item="& book~ of #" },
		{ chance=100 item="& lesser wand~ of #" },
		{ chance=100 item="& lesser wand~ of #" },
		{ chance=100 item="& lesser wand~ of #" },
		{ chance=100 item="& lesser wand~ of #" },
		{ chance=100 item="& wand~ of #" },
--		{ chance=100 item={TV_ORB,3} },
--		{ chance=100 item={TV_ORB,5} },
--		{ chance=100 item={TV_ORB,8} },
--		{ chance=100 item={TV_ORB,9} },
--		{ chance=100 item={TV_ORB,14} },
--		{ chance=60 item={TV_ORB,15} },
--		{ chance=60 item={TV_ORB,16} },
--		{ chance=60 item={TV_ORB,17} },
	}
	buy = function (obj)
		local buy =
		{
			[TV_AMULET] = true,
			[TV_RING] = true,
			[TV_ORB] = true,
			[TV_WAND] = true,
			[TV_SCROLL] = true,
			[TV_POTION] = true,
			[TV_MSTAFF] = true,
			--[TV_RANDART] = true,
		}

		if obj.tval == TV_BOOK and obj.sval == 255 and (can_spell_random(obj.pval) == "magic") then return true
		elseif obj.tval == TV_BOOK and obj.sval ~= 255 then return true
		elseif buy[obj.tval] == true then return true
		end
	end
}
-- Book
-- Wands
-- Staves
--]]

new_store
{
	define_as = "store.STORE_MAGIC"
	name = "Magic shop"
	display = '6' color = color.RED
	max_obj = 24
	actions = {	store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_SELL,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "random_theme"
	flags =
	{
		LEVEL=function() return rng.range(25, 50) end
		STORE_MAINTAIN_TURNS = 500
		STORE_THEME = getter.flags {
			THEME_DEVICE = 100
		} 

	},
	buy = function (obj)
		local buy =
		{
			[TV_AMULET] = true,
			[TV_RING] = true,
			[TV_ORB] = true,
			[TV_WAND] = true,
			[TV_SCROLL] = true,
			[TV_POTION] = true,
			[TV_MSTAFF] = true,
			--[TV_RANDART] = true,
		}

		if obj.tval == TV_BOOK and obj.sval == 255 and (can_spell_random(obj.pval) == "magic") then return true
		elseif obj.tval == TV_BOOK and obj.sval ~= 255 then return true
		elseif buy[obj.tval] == true then return true
		end
	end
}


new_store
{
	define_as = "store.STORE_MARKET"
	name = "Black Market"
	display = '7' color = color.LIGHT_DARK
	max_obj = 24
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "full_random"
	flags =
	{
		LEVEL=function() return rng.range(25, 50) end
		STORE_MAINTAIN_TURNS = 500
	}
}

new_store
{
	define_as = "store.STORE_HOME"
	name = "Home"
	display = '8' color = color.YELLOW
	max_obj = 24
	store_make_num = 0
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_DROP,	store.ACTION_GET,
			store.ACTION_NONE,	store.NONE }
	owners = store.OWNER_PLAYER
	flags = { FREE=1 STORE_MAINTAIN_TURNS=0 }
}

new_store
{
	define_as = "store.STORE_BOOKS"
	name = "Book Store"
	display = '9' color = color.ORANGE
	max_obj = 24
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	item_kinds = {
		{ chance=100 item="& book~ of beginner cantrips"},
		{ chance=100 item="& book~ of #"},
		{ chance=100 item="& book~ of #"},
		{ chance=100 item="& book~ of #"},
	}
	flags = {
		LEVEL = function() return 30 end
	}
}

new_store
{
	define_as = "store.STORE_PETS"
	name = "Pet Shop"
	display = '+' color = color.BLUE
	max_obj = 12
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	flags =
	{
		LEVEL=function() return rng.range(25, 50) end
	}
	item_kinds =
	{
		{ chance=100 item="summon symbiote" },
		{ chance=100 item="& Hard Biscuit~" },
	}
}

new_store
{
	define_as = "store.STORE_BEASTMASTER"
	name = "Beastmaster Shanty"
	display = '+' color = color.GREEN
	max_obj = 0
	actions = { store.ACTION_BOUNTY_QUEST,	store.ACTION_NONE,
			store.ACTION_RESEARCH_MON,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_NONE }
	owners = store.OWNER_BEASTMASTER
}

-------------------------- Bree shops --------------------------
new_store
{
	define_as = "store.BREE_MAYOR"
	name = "Mayor's Office"
	display = '+' color = color.ORANGE
	max_obj = 0
	actions = { store.ACTION_CHAT,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_NONE }
	owners = store.OWNER_MARLO
}

new_store
{
	define_as = "store.SOOTHSAYER"
	name = "The Soothsayer"
	display = '+' color = color.LIGHT_BLUE
	max_obj = 2
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	flags =
	{
		RANDOM=1
	}
	item_kinds =
	{
		{ chance=100 item="Divination" },
	}

}

new_store
{
	define_as = "store.INN_PRANCING_PONY"
	name = "The Prancing Pony"
	display = '+' color = color.WHITE
	max_obj = 5
	store_make_num = 5 * 99
	actions = { store.ACTION_REST,	store.ACTION_NONE,
			store.ACTION_NONE,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	item_kinds =
	{
		{ chance=100 item="& Ration~ of Food" },
		{ chance=100 item="& Hard Biscuit~" },
		{ chance=100 item="& Strip~ of Venison" },
		{ chance=100 item="& Pint~ of Fine Wine" },
		{ chance=100 item="& Pint~ of Fine Ale" },
	}
}

new_store
{
	define_as = "store.MUSEUM_MATHOM"
	name = "The Mathom-house"
	display = '+' color = color.GREEN
	max_obj = 1200
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_DROP,	store.ACTION_NONE,
			store.ACTION_EXAMINE,	store.ACTION_NONE }
	owners = store.OWNER_DEFAULT
	flags =
	{
		MUSEUM=1 FREE=1 STORE_MAINTAIN_TURNS=0
	}

}

-------------------------- Amulet: Portable Hole --------------------------
new_store
{
	define_as = "store.STORE_MAGIC_HOME"
	name = "Portable Hole"
	display = '8' color = color.YELLOW
	max_obj = 5
	store_make_num = 0
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_DROP,	store.ACTION_GET,
			store.ACTION_NONE,	store.ACTION_NONE }
	owners = store.OWNER_PLAYER
	flags = { FREE=1 STORE_MAINTAIN_TURNS=0 }
}


-------------------------- Lothlorien shops --------------------------


--------------------------- Dwarven shops ----------------------------

-- Not yet implemented
--[[
new_store
{
	define_as = "store.STORE_MITHRIL_SMITH"
	name = "Mithril Smith"
	display = '2' color = color.RED
	max_obj = 24
	actions = {	store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_SELL,	store.ACTION_BUY,
			store.ACTION_NONE,	store.ACTION_EXAMINE }
	owners = store.OWNER_DEFAULT
	store_maker = "mithril"
	flags =
	{
		LEVEL=30
	}
	buy = function(obj)
		if strfind("mithril", object_desc(obj)) then
			return true
		end
	end
}
]]

-- I figure the dwarves need a dedicated drinking hall.
-- Not a 'prancing' inn.
new_store
{
	define_as = "store.PUB_DRUNKEN_MARMOT"
	name = "The Drunken Marmot"
	display = '+' color = color.WHITE
	max_obj = 5
	store_make_num = 5 * 99
	actions = { store.ACTION_NONE,	store.ACTION_NONE,
			store.ACTION_BUY,		store.ACTION_EXAMINE,
			store.ACTION_NONE,	store.ACTION_NONE }
	owners = store.OWNER_DEFAULT
	item_kinds =
	{
		{ chance=100 item="& Pint~ of Fine Ale" },
		{ chance=100 item="& Pint~ of Fine Ale" },
		{ chance=100 item="& Pint~ of Fine Ale" },
		{ chance=100 item="& Pint~ of Fine Wine" },
		{ chance=100 item="& Pint~ of Fine Wine" },
	}

}
