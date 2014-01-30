-- Need a dummy skill
new_skill{ name = "" desc = "" __index__ = 0 }

-- Skill definitions
new_skill
{
	define_as = "SKILL_COMBAT"
	name = "Combat"
	desc =
	{
		"General ability to fight and to pseudo-id armours and weapons.",
		"It also allows to use heavier armours without penalties.",
		"Finally it makes you stronger, increasing your hit points.",
	}
	flags = { RANDOM_GAIN=100 }
}

new_skill
{
	define_as = "SKILL_MASTERY"
	name = "Weaponmastery"
	desc = "General ability to use melee weapons"
	flags = { RANDOM_GAIN=100 }
	friends = { ["Combat"]=50, }
}

new_skill
{
	define_as = "SKILL_SWORD"
	name = "Sword-mastery"
	desc = "Ability to use swords"
	friends = { ["Weaponmastery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_KNIFE"
	name = "Knife-mastery"
	desc = "Ability to use knives"
	friends = { ["Weaponmastery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_CRITS"
	name = "Critical-hits"
	desc = "Ability to deal critical hits with knives"
	friends = { ["Knife-mastery"]=5, }
}

new_skill
{
	define_as = "SKILL_AXE"
	name = "Axe-mastery"
	desc = "Ability to use axes"
	friends = { ["Weaponmastery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_HAFTED"
	name = "Hafted-mastery"
	desc = "Ability to use hafted weapons"
	friends = { ["Weaponmastery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_STUN"
	name = "Stunning-blows"
	desc = "Ability to stun opponents when doing critical hits with hafted weapons > 5 lb"
	friends = { ["Hafted-mastery"]=5, }
}

new_skill
{
	define_as = "SKILL_POLEARM"
	name = "Polearm-mastery"
	desc = "Ability to use polearms"
	friends = { ["Weaponmastery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_ARCHERY"
	name = "Archery"
	desc = "General ability to use ranged weapons"
	flags = { RANDOM_GAIN=100 }
	friends = { ["Combat"]=50, }
}

new_skill
{
	define_as = "SKILL_SLING"
	name = "Sling-mastery"
	desc = "Ability to use slings"
	action = { combat.archery.piercing_MKEY, "Fire piercing shots" }
	browse = { combat.archery.piercing_MKEY, "Check piercing shots status" }
	friends = { ["Archery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_BOW"
	name = "Bow-mastery"
	desc = "Ability to use bows"
	action = { combat.archery.piercing_MKEY, "Fire piercing shots" }
	browse = { combat.archery.piercing_MKEY, "Check piercing shots status" }
	friends = { ["Archery"]=25, ["Combat"]=7, }
}

new_skill
{
	define_as = "SKILL_HAND"
	name = "Barehand-combat"
	desc = "Ability to fight barehanded"
	flags = { RANDOM_GAIN=70 }
	friends = { ["Combat"]=50, }
}

new_skill
{
	define_as = "SKILL_BEAR"
	name = "Bearform-combat"
	desc = "Ability to fight in bear form"
	flags = { HIDDEN=1 AUTO_HIDE=1 }
}

new_skill
{
	define_as = "SKILL_BOULDER"
	name = "Boulder-throwing"
	desc = "Ability to make and throw boulders"
	action = { nil, "Tear down a wall to create boulders" }
	friends = { ["Combat"]=40, }
}

new_skill
{
	define_as = "SKILL_ANTIMAGIC"
	name = "Antimagic"
	desc = "Ability to generates an antimagic field"
	action = { nil, "Use antimagic" }
	flags = { RANDOM_GAIN=80 }
	exclude = { "Divination", "Mana", "Magic-device", "Nature", "Earth", "Udun", "Demonology", "Air", "Geomancy", "Prayer", "Meta", "Necromancy", "Music", "Mindcraft", "Temporal", "Runecraft", "Thaumaturgy", "Water", "Sorcery", "Fire", "Conveyance", "Mind", }
}

new_skill
{
	define_as = "SKILL_SNEAK"
	name = "Sneakiness"
	desc =
	{
		"General ability at the sneakiness skills.",
		"It also affects the searching abilities",
	}
	flags = { RANDOM_GAIN=100 }
}

new_skill
{
	define_as = "SKILL_STEALTH"
	name = "Stealth"
	desc = "Ability to move unnoticed, silently"
	flags = { RANDOM_GAIN=100 }
	friends = { ["Sneakiness"]=15, }
}

new_skill
{
	define_as = "SKILL_DISARMING"
	name = "Disarming"
	desc = "Ability to disarm the various traps"
	flags = { RANDOM_GAIN=100 }
	friends = { ["Sneakiness"]=10, }
}

new_skill
{
	define_as = "SKILL_BACKSTAB"
	name = "Backstab"
	desc = "Ability to backstab fleeing and sleeping monsters to increase damage"
	friends = { ["Sneakiness"]=5, }
}

new_skill
{
	define_as = "SKILL_STEALING"
	name = "Stealing"
	desc = "Ability to steal objects"
	action = { nil, "Steal object" }
	friends = { ["Sneakiness"]=15, }
}

new_skill
{
	define_as = "SKILL_DODGE"
	name = "Dodging"
	desc = "Ability to dodge blows and bolts"
	action = { 16, "Check dodge chance" }
	friends = { ["Sneakiness"]=10, }
}

new_skill
{
	define_as = "SKILL_MAGIC"
	name = "Magic"
	desc =
	{
		"General ability to do magic, also affect mana reserves and",
		"magic device ability. Helps pseudo-id of magic objects",
	}
	action = { nil, "Copy a spell" }
	flags = { RANDOM_GAIN=100 }
}

new_skill
{
	define_as = "SKILL_DEVICE"
	name = "Magic-device"
	desc =
	{
		"Ease the use of magical devices, such as wands, staves and rods.",
		"It also helps pseudo-id of magic objects",
	}
	action = { nil, "Manipulate gemstones" }
	flags = { RANDOM_GAIN=100 }
	friends = { ["Magic"]=7, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_ALCHEMY"
	name = "Alchemy"
	desc = "Ability to use essences to modify/create magic items"
	action = { nil, "Use Alchemy" }
	browse = { nil, "Browse formulas" }
	flags = { RANDOM_GAIN=50 }
	friends = { ["Magic-device"]=7, }
}

new_skill
{
	define_as = "SKILL_SPELL"
	name = "Spell-power"
	desc = "Ability to increase the power of spells"
	friends = { ["Magic"]=20, }
}

new_skill
{
	define_as = "SKILL_SORCERY"
	name = "Sorcery"
	desc =
	{
		"Ability to use all the magic schools as if their skill was sorcery",
		"But the price to channel that much magic is your health",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=20, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_MANA"
	name = "Mana"
	desc =
	{
		"Ability to learn and use spells from the Mana school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_GEOMANCY"
	name = "Geomancy"
	desc =
	{
		"Ability to understand the raw elemental forces of nature and use",
		"them to your advantage. Most spells need Fire/Water/Earth/Air skills",
	}
	action = { nil, "Use Geomancy" }
	friends = { ["Air"]=45, ["Fire"]=45, ["Earth"]=45, ["Water"]=45, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_FIRE"
	name = "Fire"
	desc =
	{
		"Ability to learn and use spells from the Fire school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_WATER"
	name = "Water"
	desc =
	{
		"Ability to learn and use spells from the Water school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_AIR"
	name = "Air"
	desc =
	{
		"Ability to learn and use spells from the Air school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_EARTH"
	name = "Earth"
	desc =
	{
		"Ability to learn and use spells from the Earth school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_META"
	name = "Meta"
	desc =
	{
		"Ability to learn and use spells from the Meta school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_CONVEYANCE"
	name = "Conveyance"
	desc =
	{
		"Ability to learn and use spells from the Conveyance school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	flags = { RANDOM_GAIN=100 }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_DIVINATION"
	name = "Divination"
	desc =
	{
		"Ability to learn and use spells from the Divination school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	flags = { RANDOM_GAIN=100 }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_TEMPORAL"
	name = "Temporal"
	desc =
	{
		"Ability to learn and use spells from the Temporal school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_MIND"
	name = "Mind"
	desc =
	{
		"Ability to learn and use spells from the Mind school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_NATURE"
	name = "Nature"
	desc =
	{
		"Ability to learn and use spells from the Nature school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_UDUN"
	name = "Udun"
	desc =
	{
		"Ability to learn and use spells from the Udun school",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	flags = { HIDDEN=1 }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_DAEMON"
	name = "Demonology"
	desc =
	{
		"Ability to use incantations from the Demonblades",
		"Spells use the intelligence stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_NECROMANCY"
	name = "Necromancy"
	desc =
	{
		"Ability to harness the powers of the dead",
		"Spells use the intelligence stat",
	}
	action = { nil, "Use Necromancy" }
	flags = { RANDOM_GAIN=60 }
	friends = { ["Magic"]=4, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_RUNECRAFT"
	name = "Runecraft"
	desc =
	{
		"Ability to combine magic runes to create your own spells",
		"Runespells use the dexterity stat",
	}
	action = { nil, "Use Runespells" }
	friends = { ["Magic"]=12, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_THAUMATURGY"
	name = "Thaumaturgy"
	desc =
	{
		"Ability to gain and cast innate spells",
		"Spells use the intelligence stat",
	}
	action = { nil, "Cast a thaumaturgy spell" }
	flags = { RANDOM_GAIN=100 }
	friends = { ["Magic"]=6, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_SPIRITUALITY"
	name = "Spirituality"
	desc = "General ability to use spiritual skills and also influence your Saving Throw"
	flags = { RANDOM_GAIN=100 }
}

new_skill
{
	define_as = "SKILL_PRAY"
	name = "Prayer"
	desc =
	{
		"Ability to learn and use spells from the gods schools",
		"Spells use the wisdom stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Spirituality"]=10, ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_MINDCRAFT"
	name = "Mindcraft"
	desc =
	{
		"Ability to focus the powers of the mind",
		"Mindpowers use the wisdom stat",
	}
	action = { nil, "Use Mindcraft" }
	flags = { RANDOM_GAIN=50 }
	friends = { ["Spirituality"]=10, ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_MUSIC"
	name = "Music"
	desc =
	{
		"Ability to learn and sing songs",
		"Songs use the charisma stat",
	}
	action = { easy_cast.MKEY_CAST, "Cast a spell" }
	browse = { easy_cast.MKEY_CAST, "Browse spells" }
	friends = { ["Spirituality"]=10, ["Magic"]=10, }
	exclude = { "Antimagic", }
}

new_skill
{
	define_as = "SKILL_LORE"
	name = "Monster-lore"
	desc =
	{
		"General ability at the monster related skills, ability to gain experience",
		"from friendly kills. It also affects the number of companions you can have",
	}
	action = { 22, "Turn pet into companion" }
	flags = { RANDOM_GAIN=100 }
}

new_skill
{
	define_as = "SKILL_SUMMON"
	name = "Summoning"
	desc = "Ability to create totems from monsters and use them to summon monsters"
	action = { nil, "Manipulate totems" }
	flags = { RANDOM_GAIN=60 }
	friends = { ["Monster-lore"]=10, }
}

new_skill
{
	define_as = "SKILL_PRESERVATION"
	name = "Corpse-preservation"
	desc = "Ability to not destroy the monster corpse when killing them"
	friends = { ["Monster-lore"]=10, }
}

new_skill
{
	define_as = "SKILL_POSSESSION"
	name = "Possession"
	desc = "Ability to incarnate into monsters"
	action = { nil, "Use the possession skill" }
	friends = { ["Monster-lore"]=10, }
}

new_skill
{
	define_as = "SKILL_SYMBIOTIC"
	name = "Symbiosis"
	desc =
	{
		"Ability to enter in symbiosis with monsters unable to move by themselves",
		"Spells use the intelligence stat",
	}
	action = { nil, "Use symbiotic powers" }
	flags = { RANDOM_GAIN=70 }
	friends = { ["Monster-lore"]=10, }
}

new_skill
{
	define_as = "SKILL_RIDING"
	name = "Riding"
	desc =
	{
		"Ability to mount a friendly horse, wolf, ...",
	}
	action = { riding.MKEY, "Ride" }
	flags = { RANDOM_GAIN=30 }
	friends = { ["Monster-lore"]=10, }
}

new_skill
{
	define_as = "SKILL_MIMICRY"
	name = "Mimicry"
	desc = "Ability to use special cloaks to change form"
	action = { nil, "Shapeshift(Mimicry)" }
	flags = { RANDOM_GAIN=80 }
	friends = { ["Monster-lore"]=10, }
}

-- Skill tree
set_skill_tree
{
	["Combat"] =
	{
		["Weaponmastery"] =
		{
			["Axe-mastery"] = {}
			["Hafted-mastery"] =
			{
				["Stunning-blows"] = {}
			}
			["Knife-mastery"] =
			{
				["Critical-hits"] = {}
			}
			["Polearm-mastery"] = {}
			["Sword-mastery"] = {}
		}
		["Archery"] =
		{
			["Bow-mastery"] = {}
			["Sling-mastery"] = {}
		}
		["Barehand-combat"] = {}
		["Bearform-combat"] = {}
		["Boulder-throwing"] = {}
		["Antimagic"] = {}
	}
	["Sneakiness"] =
	{
		["Stealth"] = {}
		["Disarming"] = {}
		["Backstab"] = {}
		["Stealing"] = {}
		["Dodging"] = {}
	}
	["Magic"] =
	{
		["Magic-device"] =
		{
			["Alchemy"] = {}
		}
		["Spell-power"] = {}
		["Sorcery"] = {}
		["Mana"] = {}
		["Geomancy"] =
		{
			["Fire"] = {}
			["Water"] = {}
			["Air"] = {}
			["Earth"] = {}
		}
		["Meta"] = {}
		["Conveyance"] = {}
		["Divination"] = {}
		["Temporal"] = {}
		["Mind"] = {}
		["Nature"] = {}
		["Udun"] = {}
		["Demonology"] = {}
		["Necromancy"] = {}
		["Runecraft"] = {}
		["Thaumaturgy"] = {}
	}
	["Spirituality"] =
	{
		["Prayer"] = {}
		["Mindcraft"] = {}
		["Music"] = {}
	}
	["Monster-lore"] =
	{
		["Riding"] = {}
		["Summoning"] = {}
		["Corpse-preservation"] = {}
		["Possession"] = {}
		["Symbiosis"] = {}
		["Mimicry"] = {}
	}
}
