getter.auto_share_flags(true)

--------------
-- Weapon Egos
--------------

-- Brands
new_item_ego
{
	name = { "before", "fiery" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 1
	allocation = 100
	value_add = 2000
	rating = 20
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.FIRE] = 66})
			BRAND = obvious(getter.array{[dam.FIRE] = 3})
			LITE = obvious(1)
		}
	}
}

new_item_ego
{
	name = { "before", "icy" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 1
	allocation = 100
	value_add = 2000
	rating = 20
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.COLD] = 66})
			BRAND = obvious(getter.array{[dam.COLD] = 3})
		}
		[10] =
		{
			BLOW_RESPONSE = getter.array{[AURA_COLD] = {2,4}}
		}
	}
}

new_item_ego
{
	name = { "after", "of melting" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 15
	allocation = 100
	value_add = 2500
	rating = 25
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.ACID] = 66})
			BRAND = obvious(getter.array{[dam.ACID] = 3})
		}
		[10] =
		{
			BLOW_RESPONSE = getter.array{[AURA_ACID] = {2,4}}
		}
	}
}

new_item_ego
{
	name = { "after", "of shocking" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 15
	allocation = 100
	value_add = 2500
	rating = 25
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.ELEC] = 66})
			BRAND = obvious(getter.array{[dam.ELEC] = 3})
			LITE = obvious(1)
		}
	}
}

new_item_ego
{
	name = { "before", "elemental" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 50
	allocation = 100
	value_add = 7500
	rating = 50
	flags =
	{
		{ CHANCE = 100
			RESIST = obvious(getter.array
			{
				[dam.FIRE] = 66
				[dam.COLD] = 66
				[dam.ACID] = 66
				[dam.ELEC] = 66
			})
			BRAND = obvious(getter.array
			{
				[dam.FIRE] = 3
				[dam.COLD] = 3
				[dam.ACID] = 3
				[dam.ELEC] = 3
			})
		}
		{ CHANCE = 10
			BLOW_RESPONSE = getter.array{[AURA_FIRE] = {2,4}}
		}
		{ CHANCE = 10
			BLOW_RESPONSE = getter.array{[AURA_COLD] = {2,4}}
		}
		{ CHANCE = 10
			BLOW_RESPONSE = getter.array{[AURA_ACID] = {2,4}}
		}
		{ CHANCE = 10
			BLOW_RESPONSE = getter.array{[AURA_ELEC] = {2,4}}
		}
	}
}

-- Slays
new_item_ego
{
	name = { "before", "bug-slaying" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 15
	allocation = 100
	value_add = 2500
	rating = 25
	flags =
	{
		[100] =
		{
			SPEED_FURY = obvious(getter.array{[SLAY_INSECT] = {1,2}})
		}
	}
}

new_item_ego
{
	name = { "after", "that fells beasts" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 15
	allocation = 100
	value_add = 2500
	rating = 25
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_MAMMAL] = 2})
		}
	}
}

new_item_ego
{
	name = { "before", "orcsbane" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 25
	allocation = 100
	value_add = 3500
	rating = 35
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_ORC] = 3})
		}
	}
}

new_item_ego
{
	name = { "after", "of grounding" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 25
	allocation = 100
	value_add = 3500
	rating = 35
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_AERIAL] = 2})
		}
	}
}

new_item_ego
{
	name = { "after", "of troll slaying" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 25
	allocation = 100
	value_add = 4000
	rating = 40
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_TROLL] = 3})
		}
	}
}

new_item_ego
{
	name = { "before", "fisherman's" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 25
	allocation = 100
	value_add = 4000
	rating = 40
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_AQUATIC] = 3})
		}
	}
}

new_item_ego
{
	name = { "before", "peace-bringing" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 35
	allocation = 100
	value_add = 4500
	rating = 45
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_UNDEAD] = 3})
		}
	}
}

new_item_ego
{
	name = { "after", "of demon destruction" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 35
	allocation = 100
	value_add = 4500
	rating = 45
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_DEMON] = 3})
		}
	}
}

new_item_ego
{
	name = { "before", "dragon-felling" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 45
	allocation = 100
	value_add = 5000
	rating = 50
	flags =
	{
		[100] =
		{
			SLAY = obvious(getter.array{[SLAY_DRAGON] = 3})
		}
	}
}

-- Type-specific bonuses
new_item_ego
{
	name = { "before", "sharp" }
	item_kinds =
	{
		[TV_SWORD] = true
		[TV_AXE] = true
	}
	level = 10
	allocation = 100
	value_add = 5000
	rating = 30
	flags =
	{
		[100] =
		{
			-- Boost the damage dice for slash and pierce damage
			ON_MAKE = function(obj, power)
				local mul1 = rng(150) + 100
				local mul2 = rng(150) + 100
				local slash1 = flag_get(obj.flags(FLAG_DAM), dam.SLASH)
				local pierce1 = flag_get(obj.flags(FLAG_DAM), dam.PIERCE)
				local slash2 = flag_get2(obj.flags(FLAG_DAM), dam.SLASH)
				local pierce2 = flag_get2(obj.flags(FLAG_DAM), dam.PIERCE)
				flag_set_full(obj.flags[FLAG_DAM], dam.SLASH, FLAG_FLAG_BOTH,
				              mul1 * slash1 / 100, mul1 * slash2 / 100)
				flag_set_full(obj.flags[FLAG_DAM], dam.PIERCE, FLAG_FLAG_BOTH,
				              mul2 * pierce1 / 100, mul2 * pierce2 / 100)
				flag_learn(obj.flags, FLAG_DAM, true)
			end
		}
	}
}

new_item_ego
{
	name = { "before", "penetrating" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_KNIFE] = true
	}
	level = 10
	allocation = 100
	value_add = 5000
	rating = 30
	flags =
	{
		[100] =
		{
			WOUNDING = true
		}
	}
}

new_item_ego
{
	name = { "before", "heavy" }
	item_kinds =
	{
		[TV_HAFTED] = true
	}
	level = 10
	allocation = 100
	value_add = 5000
	rating = 30
	flags =
	{
		[100] =
		{
			IMPACT = true
		}
	}
}

new_item_ego
{
	name = { "after", "etched in venom" }
	item_kinds =
	{
		[TV_KNIFE] = true
	}
	level = 30
	allocation = 100
	value_add = 2000
	rating = 30
	flags =
	{
		[100] =
		{
			BRAND = obvious(getter.array{[dam.POISONING] = 2})
		}
	}
}

new_item_ego
{
	name = { "after", "etched in red" }
	item_kinds =
	{
		[TV_SWORD] = true
		[TV_POLEARM] = true
	}
	level = 30
	allocation = 100
	value_add = 2000
	rating = 30
	flags =
	{
		[100] =
		{
			VAMPIRIC = obvious(100)
		}
	}
}

new_item_ego
{
	name = { "after", "etched in runes" }
	item_kinds =
	{
		[TV_AXE] = true
		[TV_HAFTED] = true
	}
	level = 30
	allocation = 100
	value_add = 2000
	rating = 30
	flags =
	{
		[100] =
		{
			BRAND = obvious(getter.array{[dam.CONFUSE] = 2})
		}
	}
}

-- Cursed
new_item_ego
{
	name = { "before", "unlucky" }
	item_kinds = {
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 1
	allocation = 100
	value_add = -2500
	rating = 5
	flags =
	{
		[100] =
		{
			TEST_HIT = function(obj)
				if rng.percent(50) then
					message(strcap(object_desc(obj, nil, 0)) ..
				               " misses by a mile!")
					return nil
				else
					return true
				end
			end
			LUCK = getter.random(-10, -5)
			CURSED = true
		}
	}
}

new_item_ego
{
	name = { "before", "of pacifism" }
	item_kinds = {
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	level = 25
	allocation = 100
	value_add = -2000
	rating = 5
	flags =
	{
		[100] =
		{
			NEVER_BLOW = true
			CURSED = true
		}
	}
}

-------------------
-- Weapon Materials
-------------------

declare_global_constants { "EGO_WEAPON_MITHRIL" }
EGO_WEAPON_MITHRIL = new_item_ego
{
	name = { "before", "mithril" }
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	ego_group = "material"
	level = 1
	allocation = 100
	value_add = 500
	rating = 10
	flags =
	{
		[100] =
		{
			ON_MAKE = function(obj, power)
				obj.to_h = obj.to_h + rng(5)
				obj.to_d = obj.to_d + rng(5)
			end
			HARMED_BY = getter.resists{}
		}
	}
}

declare_global_constants { "EGO_WEAPON_DARKSTONE" }
EGO_WEAPON_DARKSTONE = new_item_ego
{
	name = { "before", "darkstone"}
	item_kinds =
	{
		[TV_POLEARM] = true
		[TV_AXE] = true
		[TV_HAFTED] = true
		[TV_KNIFE] = true
		[TV_SWORD] = true
	}
	ego_group = "material"
	level = 1
	allocation = 25
	value_add = 2500
	rating = 100
	flags =
	{
		[100] =
		{
			ON_MAKE = function(obj, power)
				obj.to_h = 0
				obj.to_d = 0
			end
			ANTIMAGIC = obvious(getter.mbonus(50, 500))
		}
	}
}

------------------
-- Mage Staff Egos
------------------

new_item_ego
{
	name = { "after", "of power" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 1
	allocation = 100
	value_add = 500
	rating = 25
	flags =
	{
		[100] =
		{
			SPELL = obvious(getter.random(1, 5))
		}
	}
}

new_item_ego
{
	name = { "after", "of might" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 30
	allocation = 100
	value_add = 2500
	rating = 40
	flags =
	{
		[100] =
		{
			SPELL = obvious(getter.random(5, 15))
		}
	}
}

new_item_ego
{
	name = { "before", "shimmering" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 5
	allocation = 100
	value_add = 1000
	rating = 25
	flags =
	{
		[100] =
		{
			MANA = obvious(getter.random(5, 20))
		}
	}
}

new_item_ego
{
	name = { "before", "pulsating" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 40
	allocation = 100
	value_add = 10000
	rating = 50
	flags =
	{
		[100] =
		{
			MANA = obvious(getter.random(10, 100))
		}
	}
}

new_item_ego
{
	name = { "before", "mnemonic" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 1
	allocation = 100
	value_add = 250
	rating = 10
	flags =
	{
		[100] =
		{
			SPELL_CONTAIN = true
		}
	}
}

new_item_ego
{
	name = { "after", "with experience" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 15
	allocation = 100
	value_add = 1000
	rating = 20
	flags =
	{
		[100] =
		{
			SPELL_FAILURE = obvious(getter.random(-20, -5))
		}
	}
}

new_item_ego
{
	name = { "before", "wild" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 10
	allocation = 100
	value_add = -5000
	rating = 5
	flags =
	{
		[100] =
		{
			SPELL_FAILURE = obvious(getter.random(5, 20))
			CURSED = true
		}
	}
}

new_item_ego
{
	name = { "after", "etched in darkstone" }
	item_kinds =
	{
		[TV_MSTAFF] = true
	}

	level = 30
	allocation = 100
	value_add = -5000
	rating = 5
	flags =
	{
		[100] =
		{
			MANA = obvious(getter.random(-100, -50))
			CURSED = true
		}
	}
}

-----------------------------
-- Light source and fuel egos
-----------------------------

new_item_ego
{
	name = { "before", "ever-burning" }
	item_kinds =
	{
		[TV_FUEL] = true
		[TV_LIGHT_ARMOUR] = {SV_FILTHY_RAG, SV_FILTHY_RAG}
	}

	level = 50
	allocation = 100
	value_add = 5000
	rating = 30
	flags =
	{
		[100] =
		{
			ON_FUEL = function(light, fuel)
				flag_remove(light.flags, FLAG_FUEL)
				message("Your " .. object_desc(light) .. " will now burn forever.")
			end
			-- Don't wear it, stupid, use it as fuel!
			RESIST = obvious(getter.array{[dam.FIRE] = -100})
		}
	}
}

new_item_ego
{
	name = { "before", "bright" }
	item_kinds =
	{
		[TV_LITE] = true
	}
	level = 1
	allocation = 100
	value_add = 500
	rating = 10
	flags =
	{
		[100] =
		{
			ON_MAKE = function(obj, power)
				set_flag(obj, FLAG_LITE, obj.flags[FLAG_LITE] + 2)
			end
		}
	}
}

new_item_ego
{
	name = { "after", "of clear seeing" }
	item_kinds =
	{
		[TV_LITE] = true
	}
	level = 25
	allocation = 100
	value_add = 5000
	rating = 50
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array
			{
				[dam.CONFUSION] = 50
				[dam.BLIND] = 100
			})
		}
	}
}

new_item_ego
{
	name = { "after", "of bold leadership" }
	item_kinds =
	{
		[TV_LITE] = true
	}
	level = 25
	allocation = 100
	value_add = 5000
	rating = 50
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array
			{
				[dam.CONFUSION] = 50
				[dam.FEAR] = 100
			})
		}
	}
}

------------------
-- Armor Materials
------------------

new_item_ego
{
	name = { "before", "uruloki" }
	item_kinds =
	{
		[TV_LIGHT_ARMOUR] = {SV_DRAGON_SCALE_MAIL, SV_DRAGON_SCALE_MAIL}
		[TV_HEAVY_ARMOUR] = {SV_DRAGON_PLATE_MAIL, SV_DRAGON_PLATE_MAIL}
		[TV_LIGHT_HELM]   = {SV_DRAGON_LEATHER_CAP, SV_DRAGON_LEATHER_CAP}
		[TV_HEAVY_HELM]   = {SV_DRAGON_HELM, SV_DRAGON_HELM}
	}
	ego_group = "material"
	level = 1
	allocation = 100
	value_add = 0
	rating = 0
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.FIRE] = 66})

			DEVICE_USE = obvious(getter.single_device_spell
			{
				DEVICE_TYPE = "activate"
				ACT_SPELL = function(obj, spell_flags, skill, mana_cost, who)
					if not obj then
						return "breathe", "to breathe and resist fire"
					end
					message("A fiery blast wells up from inside you! UNIMPLEMENTED")
					return true
				end

				ACT_COST_BASE = 10
			})

			MANA_CURR = obvious(10)
			MANA_MAX = obvious(10)
			AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
			AUTO_CHARGE_RATE=obvious(1)
			AUTO_CHARGE_WIELD=obvious(true)
		}
	}
}

new_item_ego
{
	name = { "before", "cold-drake" }
	item_kinds =
	{
		[TV_LIGHT_ARMOUR] = {SV_DRAGON_SCALE_MAIL, SV_DRAGON_SCALE_MAIL}
		[TV_HEAVY_ARMOUR] = {SV_DRAGON_PLATE_MAIL, SV_DRAGON_PLATE_MAIL}
		[TV_LIGHT_HELM]   = {SV_DRAGON_LEATHER_CAP, SV_DRAGON_LEATHER_CAP}
		[TV_HEAVY_HELM]   = {SV_DRAGON_HELM, SV_DRAGON_HELM}
	}
	ego_group = "material"
	level = 1
	allocation = 100
	value_add = 0
	rating = 0
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.COLD] = 66})

			DEVICE_USE = obvious(getter.single_device_spell
			{
				DEVICE_TYPE = "activate"
				ACT_SPELL = function(obj, spell_flags, skill, mana_cost, who)
					if not obj then
						return "breathe", "to breathe and resist cold"
					end
					message("A cold blast wells up from inside you! UNIMPLEMENTED")
					return true
				end

				ACT_COST_BASE = 10
			})

			MANA_CURR = obvious(10)
			MANA_MAX = obvious(10)
			AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
			AUTO_CHARGE_RATE=obvious(1)
			AUTO_CHARGE_WIELD=obvious(true)
		}
	}
}

new_item_ego
{
	name = { "before", "long-worm" }
	item_kinds =
	{
		[TV_LIGHT_ARMOUR] = {SV_DRAGON_SCALE_MAIL, SV_DRAGON_SCALE_MAIL}
		[TV_HEAVY_ARMOUR] = {SV_DRAGON_PLATE_MAIL, SV_DRAGON_PLATE_MAIL}
		[TV_LIGHT_HELM]   = {SV_DRAGON_LEATHER_CAP, SV_DRAGON_LEATHER_CAP}
		[TV_HEAVY_HELM]   = {SV_DRAGON_HELM, SV_DRAGON_HELM}
	}
	ego_group = "material"
	level = 1
	allocation = 100
	value_add = 0
	rating = 0
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.ELEC] = 66})

			DEVICE_USE = obvious(getter.single_device_spell
			{
				DEVICE_TYPE = "activate"
				ACT_SPELL = function(obj, spell_flags, skill, mana_cost, who)
					if not obj then
						return "breathe", "to breathe and resist electricity"
					end
					message("A stormy blast wells up from inside you! UNIMPLEMENTED")
					return true
				end

				ACT_COST_BASE = 10
			})

			MANA_CURR = obvious(10)
			MANA_MAX = obvious(10)
			AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
			AUTO_CHARGE_RATE=obvious(1)
			AUTO_CHARGE_WIELD=obvious(true)
		}
	}
}

new_item_ego
{
	name = { "before", "were-worm" }
	item_kinds =
	{
		[TV_LIGHT_ARMOUR] = {SV_DRAGON_SCALE_MAIL, SV_DRAGON_SCALE_MAIL}
		[TV_HEAVY_ARMOUR] = {SV_DRAGON_PLATE_MAIL, SV_DRAGON_PLATE_MAIL}
		[TV_LIGHT_HELM]   = {SV_DRAGON_LEATHER_CAP, SV_DRAGON_LEATHER_CAP}
		[TV_HEAVY_HELM]   = {SV_DRAGON_HELM, SV_DRAGON_HELM}
	}
	ego_group = "material"
	level = 1
	allocation = 100
	value_add = 0
	rating = 0
	flags =
	{
		[100] =
		{
			RESIST = obvious(getter.array{[dam.ACID] = 66})

			DEVICE_USE = obvious(getter.single_device_spell
			{
				DEVICE_TYPE = "activate"
				ACT_SPELL = function(obj, spell_flags, skill, mana_cost, who)
					if not obj then
						return "breathe", "to breathe and resist acid"
					end
					message("A corrosive blast wells up from inside you! UNIMPLEMENTED")
					return true
				end

				ACT_COST_BASE = 10
			})

			MANA_CURR = obvious(10)
			MANA_MAX = obvious(10)
			AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
			AUTO_CHARGE_RATE=obvious(1)
			AUTO_CHARGE_WIELD=obvious(true)
		}
	}
}

-----------------------------------------------------------
-- Below this point has not been rewritten
-----------------------------------------------------------

---------------------------------------------------------------
-- Bow and sling egos
---------------------------------------------------------------
new_item_ego
{
	name = { "after", "of extra might"}
	item_kinds = {
		[TV_BOW] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 2 to_d = 3
	flags =
	{
		[100] =
		{
			XTRA_MIGHT = getter.random(1, 3)
		}
	}
}

new_item_ego
{
	name = { "after", "of extra shots"}
	item_kinds = {
		[TV_BOW] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =
	{
		[100] =	{
			XTRA_SHOTS = getter.rand_mbonus(1, 3)
		}
	}
}

------------------------------------------------------------------------
-- Trapkit egos
------------------------------------------------------------------------

new_item_ego
{
	name = { "after", "of extra shots"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =
	{
		[100] =	{
			AMMO_PER_TRIGGER = getter.rand_mbonus(2, 10)
		}
	}
}

-- Trapkits which are set off by the player, and not by monsters
new_item_ego
{
	name = { "before", "treacherous"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = -1000 rating = -20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			TRAP_PLACE_POST =
				function(trapkit, trap, ammo, y, x)
					trap[FLAG_FACTION] = FACTION_DUNGEON
					trap[FLAG_APPARENT_FACTION] =
						FACTION_PLAYER
				end
			DESC_DETAIL = "It will be triggered by you, " ..
				"rather than by monsters."
		}
	}
}

-- Trapkit which returns itself and leftover ammo to the player after
-- being unset
new_item_ego
{
	define_as = "EGO_RE"
	name = { "before", "returning"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			TRAPKIT_UNSET =
				function(trapkit, trap, ammo, y, x)
					y = player.py
					x = player.px
					drop_near(trapkit, -1, y, x)
					if ammo then
						drop_near(ammo, -1 , y, x)
					end
					return true
				end
			DESC_DETAIL = "It magically returns itself and unused ammo " ..
				"to you when it becomes unset."
		}
	}
}

new_item_ego
{
	name = { "before", "sometimes automatic"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			REARM_CHANCE = 20
		}
	}
}

new_item_ego
{
	name = { "before", "automatic"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			REARM_CHANCE = 40
		}
	}
}

new_item_ego
{
	name = { "before", "often automatic"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			REARM_CHANCE = 60
		}
	}
}

new_item_ego
{
	name = { "before", "very often automatic"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			REARM_CHANCE = 80
		}
	}
}

new_item_ego
{
	name = { "before", "fully automatic"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			REARM_CHANCE = 100
		}
	}
}

new_item_ego
{
	name = { "before", "well hidden"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			HIDDENNESS = getter.rand_mbonus(2, 3)
		}
	}
}

new_item_ego
{
	name = { "before", "obvious"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = -1000 rating = -20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			HIDDENNESS = getter.rand_mbonus(-2,-3)
		}
	}
}

-- Magic disarming does nothing, and manually disarming (by either player
-- or monster) triggers it instead
new_item_ego
{
	name = { "before", "undisarmable"}
	item_kinds = {
		[TV_TRAPKIT] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	to_h = 1 to_d = 2
	flags =	{
		[100] = {
			TRAP_DISARM_PRE =
				function(trapkit, manual, trap,
					 ammo, who, y, x)
					if manual then
						return DISARM_TRIGGERED
					else
						return DISARM_FAILURE
					end
				end
			DESC_DETAIL = "It can't be disarmed by magic, and any " ..
				"attempt to manually disarm it, by a monster or " ..
				"by you will set it off."
		}
	}
}

-------------------------------------------------------------------------
-- Stick and gem egos
-------------------------------------------------------------------------

--
-- Vampricic gem ego: the gem expends only 1/3 the normal amount of
-- mana when used, but makes up for the other 2/3 by draing the
-- equivalent amount of HP out of the player.
--
-- Suggested by Phineas Butterfat on the wiki.
--
new_item_ego
{
	name = { "before", "vampiric"}
	item_kinds = {
		[TV_GEMSTONE] = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			CHEAPNESS = 4
			MANA_USE_POST =
				function(obj, amnt, sink)
					if amnt < 1 then
						return
					end

					if not sink then
						message("Non-embeded gemstone being used???")
						return
					end

					local tval_name

					tval_name = __k_info.get_tval_info("name", sink.tval)

					message("The " .. tval_name .. " drains life from " ..
							"you to power its magic!")

					take_hit(amnt * 3, "a life draining " .. tval_name)
				end
			DESC_DETAIL = "It supplies mana by draining life from you."
		}
	}
} -- "vampiric" gem ego

new_item_ego
{
	name = { "after", "of cheapness"}
	item_kinds = {
		[TV_GEMSTONE] = {0, 255}
		[TV_WAND]     = {0, 255}
		[TV_ORB]     = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			CHEAPNESS = 2
		}
	}
} -- "of cheapness" stick/gem ego

new_item_ego
{
	name = { "before", "auto-charging"}
	item_kinds = {
		[TV_GEMSTONE] = {0, 255}
		[TV_WAND]     = {0, 255}
		[TV_ORB]     = {0, 255}
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			AUTO_CHARGE_PACK = true
			AUTO_CHARGE_TYPE = obj_mana.auto_charge_types.TIME
			AUTO_CHARGE_RATE = 1

			ON_MAKE =
				function(obj)
					obj = obj_mana.to_source(obj)

					if not obj or not obj.flags[FLAG_MANA_CURR] then
						return
					end

					if not obj.flags[FLAG_MANA_MAX] then
						-- Can't auto-recharge something with no
						-- mana maximum, since the mana would just
						-- keep going up and up.
						obj.flags[FLAG_MANA_MAX] = 2 *
							obj.flags[FLAG_MANA_CURR]
					end

					-- If an auto-charging item has been laying around
					-- for a long time before being found by the player,
					-- then it's bound to fully charged up.
					obj.flags[FLAG_MANA_CURR] = obj.flags[FLAG_MANA_MAX]
				end
		}
	}
} -- "auto-charing" gem/stick ego

-------------------------------------------------------------------------
-- Ring and amulet egos
-------------------------------------------------------------------------
new_item_ego
{
	name = { "before", "stealthy"}
	item_kinds = {
		[TV_RING] = SV_RING_SEARCHING
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			SKILL_BONUS = obvious(getter.skills{["Stealth"] =
									  getter.random(1000, 12000)})
		}
	}
}

new_item_ego
{
	name = { "after", "of Water"}
	item_kinds = {
		[TV_AMULET] = true
	}
	level = 1 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			SKILL_BONUS = obvious(getter.skills{["Water"] =
									  getter.random(1000, 12000)})
		}
	}
}

new_item_ego
{
	name = { "after", "of Fire"}
	item_kinds = {
		[TV_AMULET] = true
	}
	level = 15 allocation = 100 value_add = 1000 rating = 20
	flags =	{
		[100] = {
			SKILL_BONUS = obvious(getter.skills{["Fire"] =
									  getter.random(1000, 12000)})
		}
	}
}

--------------------------------------------------------------------------
-- Scroll and spellbook egos
--------------------------------------------------------------------------
new_item_ego
{
	name = { "before", "fireproof"}
	item_kinds = {
		[TV_SCROLL] = {0, 255}
		[TV_BOOK]   = {0, 255}
	}
	level = 1 allocation = 100 base_value_pct = 200  rating = 1
	flags =	{
		[100] = {
			IGNORE = obvious(getter.resists{FIRE=true})
		}
	}
}

--------------------------------------------------------------------------
--------------------------------------------------------------------------
getter.auto_share_flags(false)
