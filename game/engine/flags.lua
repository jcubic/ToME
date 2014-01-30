-- No price display in stores
new_flag("FREE")

-- Where to place subrace names
new_flag("PLACE")

-- Increase spell failure
new_flag("SPELL_FAILURE")

-- Object theme flagset
new_flag("OBJ_THEME")

-- Object wield slots
new_flag("WIELD_SLOT")

-- Player was unable to destroy an object
new_flag("COULDNT_DESTROY")

-- Substance flags, for walls
new_flag("SUBST_DOOR")
new_flag("SUBST_ROCK")
new_flag("SUBST_GRANITE")
new_flag("SUBST_RUBBLE")

--
-- WEAPON flag means that the object deals damage, and that the
-- "(XtoY)" information should be displayed
--
new_flag("WEAPON")

--
-- SHOW_MODS forces the showing of all mods, even if they're 0.
-- SHOW_AC_MODS forces only the showing of AC mods
-- SHIW_COMBAT_MOD only forces the showing of to_h and to_d mods
new_flag("SHOW_AC_MODS")
new_flag("SHOW_COMBAT_MODS")

-- Various ways in which monsters might pass over/through various features
new_flag("PASS_INCORP")
new_flag("PASS_STONE")
new_flag("PASS_LIQUID")
new_flag("PASS_GASEOUS")
new_flag("PASS_CLIMB")
new_flag("PASS_DOOR")

-- A dungeon level flagset that gets added to the player intrinsic_flags,
-- as if it was a item the player was wearing.
new_flag("LEVEL_BONUS")

-- A monster flagset that acts like an tiem flagset if the player is
-- in that monster's body.
new_flag("BODY_BONUS")

-- For dungeons that exit you onto a specific wilderness location,
-- but you can't get back into from the surface once you leave.
new_flag("NO_SURFACE_ENTRY")

-- Where the player first starts at the begining of a game
new_flag("STARTING_DUNGEON")
new_flag("STARTING_LEVEL")

new_flag("ENTRANCE_MIMIC")

-- Objects dam types harm and protections
new_flag("HARMED_BY")
new_flag("PROTECT_FROM")

new_flag("EQUIPMENT_SIZE")

new_flag("MANA_CURR")
new_flag("MANA_MAX")
new_flag("MANA_REGENERATE")

new_flag("LIMIT_SPELLS")

-- Flavour id for multiflavoured objects
new_flag("FLAVOUR_ID")

-- Original weight and decay counter for corpses
new_flag("ORIG_WEIGHT")
new_flag("ORIG_DECAY")
