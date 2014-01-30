-- Ingame contextual help

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-----------------------Here comes the definition of help-----------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

ingame_help
{
	["hook"]  = hook.MOVE_PRE,
	["event"] = function(y, x) if cave(y, x).feat == FEAT_BETWEEN then return true end end,
	["desc"] =
	{
		"Void Jumpgates can be entered by pressing the > key. They will",
		"transport you to another jumpgate, but beware the monsters that",
		"might wait on the other side."
	}
}

ingame_help
{
	["hook"]  = hook.MOVE_PRE,
	["event"] = function(y, x) if cave(y, x).feat == FEAT_FOUNTAIN then return true end end,
	["desc"] =
	{
		"Fountains are always magical, you can quaff from them or store",
		"the liquid in emtpy bottlesby pressing H, or V with the Rogue-like",
		"keyset.  Beware that unlike potions they cannot be identified.",
	}
}

ingame_help
{
	["hook"]  = hook.MOVE_PRE,
	["event"] = function(y, x) if (flag_used(cave(y, x).inventory) > 1) then return true end end,
	["desc"] =
	{
		"So you found your first item, nice eh? Now when you stumble across",
		"objects you can pick them up by pressing g, and if you are wondering",
		"what they do press I to get some basic information.",
		"You may also want to identify them with scrolls, obrs or spells.",
	}
}
--[[DGDGDGDG
ingame_help
{
	["hook"] =      hook.MOVE_PRE,
	["event"] =     function(y, x) if (cave(y, x).feat >= FEAT_ALTAR_HEAD) and (cave(y, x).feat <= FEAT_ALTAR_TAIL) then return true end end,
	["desc"] =
	{
		"Altars are the way to reach the Valar, powers of the world,",
		"usualy called Gods. You can press O to become a follower.",
		"Beware that once you follow a god it is not allowed to change.",
		"For an exact description of what gods do and want, read the documentation."
	}
}
]]
-- Beware this one, if Bree is moved from 21, 35 (y, x) on the wilderness
-- map it will break
ingame_help
{
	["hook"]  = hook.END_TURN,
	["event"] = function(y, x)
		if ((player.wilderness_x ~= 35) or (player.wilderness_y ~= 21) and
			(player.astral == nil)) then return true end
	end,
	["desc"] =
	{
		"Ahh wilderness travel... The overview mode will allow you to travel",
		"fast, but that comes to the cost of GREATLY increased food cunsumption.",
		"So you should really watch your hungriness status.",
		"To enter the overview mode, press < while in the wilderness or ",
		"in a surface town."
	}
}

ingame_help
{
	["hook"] =      hook.PLAYER_LEVEL,
	["event"] =     function(y, x) if player.lev > 1 then return true end end,
	["desc"] =
	{
		"Okay, so you now gained a level, and you got skill points to spend.",
		"To do so simply press G to learn skills. Reading the documentation",
		"about skills and abilities is also very recommended.",
	}
}

ingame_help
{
	["hook"] =      hook.MOVE_PRE,
	["event"] =     function(y, x) if cave_feat_is(cave(y, x), FLAG_LEVEL_CHANGER) then return true end end,
	["desc"] =
	{
		"Ah, this is a way to a different place(dungeon, forest, ...). Press > or < to enter it.",
		"But be ready to fight what lies within for it could be not too friendly.",
	}
}

ingame_help
{
	["callback"] =  "monster_chat",
	["desc"] =
	{
		"Somebody is speaking at you it seems. You can talk back with the Y",
		"key, or ( in the Rogue-like keyset.",
		"This can lead to quests. You can also give items to 'monsters' ",
		"with the y key, or ' in the Rogue-like keyset.",
	}
}

ingame_help
{
	["hook"]  = hook.END_TURN,
	["event"] = function(y, x) return true end,
	["desc"] =
	{
		"Welcome to ToME! I am the spirit of knowledge and my task is to help you",
		"to get used to how to play. I have prepared a #vparchment#y for you to #vread#y.",
		"Press r, and select it. You can also check the documentation",
		"by pressing ? at (nearly) any time.",
		"The first place you can explore is Barrow-downs, go to the west of town",
		"you should see a #v>#y there.",
		"",
		"You have been sent here by Gandalf the grey on behalf of Elrond, to locate",
		"and secure the shards of Narsil.",
		"Press ctrl+q to see your quests.",
		"",
		"If you miss any of this you can press ctrl+p to see your message log.",
		"If you do not want me to bother you anymore with tips, press = to,",
		"edit the options, then in the User Interface section deactivate",
		"'Ingame Contexual Help'.  Also use that section of the game options",
		"to turn on Rogue-like commands if you are familiar with the command"
		"keys of games like Nethack.",
		"Now go to your destiny!",
	}
}

ingame_help
{
	["no_test"] =   true,
	["callback"] =  "select_context",
	["fct"] =       function(typ, name)
			-- list of files for classes, { filename, anchor }
			local t =
			{
				["race"] =
				{
					["Beorning"] = { "r_beorn.txt", 0 },
					["DeathMold"] = { "r_deathm.txt", 0 },
					["Avari"] = { "r_drkelf.txt", 0 },
					["Dunadan"] = { "r_dunad.txt", 0 },
					["Dwarf"] = { "r_dwarf.txt", 0 },
					["Quendi"] = { "r_elf.txt", 0 },
					["Ent"] = { "r_ent.txt", 0 },
					["Gnome"] = { "r_gnome.txt", 0 },
					["Half-Elf"] = { "r_hafelf.txt", 0 },
					["Half-Ogre"] = { "r_hafogr.txt", 0 },
					["Eldar"] = { "r_hielf.txt", 0 },
					["Hobbit"] = { "r_hobbit.txt", 0 },
					["Human"] = { "r_human.txt", 0 },
					["Kobold"] = { "r_kobold.txt", 0 },
					["Maia"] = { "r_maia.txt", 0 },
					["Orc"] = { "r_orc.txt", 0 },
					["Petty-Dwarf"] = { "r_pettyd.txt", 0 },
					["RohanKnight"] = { "r_rohank.txt", 0 },
					["Thunderlord"] = { "r_thlord.txt", 0 },
					["Troll"] = { "r_troll.txt", 0 },
					["Nandor"] = { "r_wodelf.txt", 0 },
					["Yeek"] = { "r_yeek.txt", 0 },
				},
				["subrace"] =
				{
					["Barbarian"] = { "rm_barb.txt", 0 },
					["Classical"] = { "rm_class.txt", 0 },
					["Corrupted"] = { "rm_corru.txt", 0 },
					["Hermit"] = { "rm_herm.txt", 0 },
					["LostSoul"] = { "rm_lsoul.txt", 0 },
					["Skeleton"] = { "rm_skel.txt", 0 },
					["Spectre"] = { "rm_spec.txt", 0 },
					["Vampire"] = { "rm_vamp.txt", 0 },
					["Zombie"] = { "rm_zomb.txt", 0 },
				},
				["class"] =
				{
					["Alchemist"] = { "c_alchem.txt", 0 },
					["Archer"] = { "c_archer.txt", 0 },
					["Assassin"] = { "c_assass.txt", 0 },
					["Axemaster"] = { "c_axemas.txt", 0 },
					["Bard"] = { "c_bard.txt", 0 },
					["Dark-Priest"] = { "c_pr_drk.txt", 0 },
					["Demonologist"] = { "c_demono.txt", 0 },
					["Druid"] = { "c_druid.txt", 0 },
					["Geomancer"] = { "c_geoman.txt", 0 },
					["Haftedmaster"] = { "c_hafted.txt", 0 },
					["Loremaster"] = { "c_lorema.txt", 0 },
					["Mage"] = { "c_mage.txt", 0 },
					["Mimic"] = { "c_mimic.txt", 0 },
					["Mindcrafter"] = { "c_mindcr.txt", 0 },
					["Monk"] = { "c_monk.txt", 0 },
					["Necromancer"] = { "c_necro.txt", 0 },
					["Paladin"] = { "c_palad.txt", 0 },
					["Polearmmaster"] = { "c_polear.txt", 0 },
					["Possessor"] = { "c_posses.txt", 0 },
					["Priest"] = { "c_priest.txt", 0 },
					["Priest(Eru)"] = { "c_pr_eru.txt", 0 },
					["Priest(Manwe)"] = { "c_pr_man.txt", 0 },
					["Ranger"] = { "c_ranger.txt", 0 },
					["Rogue"] = { "c_rogue.txt", 0 },
					["Runecrafter"] = { "c_runecr.txt", 0 },
					["Sorceror"] = { "c_sorcer.txt", 0 },
					["Summoner"] = { "c_summon.txt", 0 },
					["Swordmaster"] = { "c_swordm.txt", 0 },
					["Symbiant"] = { "c_symbia.txt", 0 },
					["Thaumaturgist"] = { "c_thaum.txt", 0 },
					["Unbeliever"] = { "c_unbel.txt", 0 },
					["Warper"] = { "c_warper.txt", 0 },
					["Warrior"] = { "c_warrio.txt", 0 },
				},
				["god"] =
				{
					["Eru Iluvatar"] = { "g_eru.txt", 0 },
					["Manwe Sulimo"] = { "g_manwe.txt", 0 },
					["Tulkas"] = { "g_tulkas.txt", 0 },
					["Melkor Bauglir"] = { "g_melkor.txt", 0 },
					["Yavanna Kementari"] = { "g_yavann.txt", 0 },
				},
				["skill"] =
				{
					["Air"] = { "skills.txt", 27 },
					["Alchemy"] = { "skills.txt", 49 },
					["Antimagic"] = { "skills.txt", 50 },
					["Archery"] = { "skills.txt", 08 },
					["Axe-mastery"] = { "skills.txt", 05 },
					["Backstab"] = { "skills.txt", 18 },
					["Barehand-combat"] = { "skills.txt", 13 },
					["Boomerang-mastery"] = { "skills.txt", 12 },
					["Boulder-throwing"] = { "skills.txt", 58 },
					["Bow-mastery"] = { "skills.txt", 10 },
					["Combat"] = { "skills.txt", 01 },
					["Conveyance"] = { "skills.txt", 30 },
					["Corpse-preservation"] = { "skills.txt", 44 },
					["Critical-hits"] = { "skills.txt", 04 },
					["Crossbow-mastery"] = { "skills.txt", 11 },
					["Demonology"] = { "skills.txt", 52 },
					["Disarming"] = { "skills.txt", 16 },
					["Divination"] = { "skills.txt", 31 },
					["Dodging"] = { "skills.txt", 20 },
					["Druidistic"] = { "skills.txt", 40 },
					["Earth"] = { "skills.txt", 28 },
					["Fire"] = { "skills.txt", 25 },
					["Geomancy"] = { "skills.txt", 60 },
					["Hafted-mastery"] = { "skills.txt", 06 },
					["Magic"] = { "skills.txt", 21 },
					["Magic-device"] = { "skills.txt", 54 },
					["Mana"] = { "skills.txt", 24 },
					["Meta"] = { "skills.txt", 29 },
					["Mimicry"] = { "skills.txt", 47 },
					["Mind"] = { "skills.txt", 33 },
					["Mindcraft"] = { "skills.txt", 41 },
					["Monster-lore"] = { "skills.txt", 42 },
					["Music"] = { "skills.txt", 59 },
					["Nature"] = { "skills.txt", 34 },
					["Necromancy"] = { "skills.txt", 35 },
					["Polearm-mastery"] = { "skills.txt", 07 },
					["Possession"] = { "skills.txt", 45 },
					["Prayer"] = { "skills.txt", 39 },
					["Runecraft"] = { "skills.txt", 36 },
					["Sling-mastery"] = { "skills.txt", 09 },
					["Sneakiness"] = { "skills.txt", 14 },
					["Spell-power"] = { "skills.txt", 22 },
					["Spirituality"] = { "skills.txt", 38 },
					["Sorcery"] = { "skills.txt", 23 },
					["Stealing"] = { "skills.txt", 19 },
					["Stealth"] = { "skills.txt", 15 },
					["Stunning-blows"] = { "skills.txt", 53 },
					["Summoning"] = { "skills.txt", 43 },
					["Sword-mastery"] = { "skills.txt", 03 },
					["Symbiosis"] = { "skills.txt", 46 },
					["Temporal"] = { "skills.txt", 32 },
					["Thaumaturgy"] = { "skills.txt", 37 },
					["Udun"] = { "skills.txt", 48 },
					["Weaponmastery"] = { "skills.txt", 02 },
					["Water"] = { "skills.txt", 26 },
				},
				["ability"] =
				{
				    ["Swift Kill"] = { "ability.txt", 02 },
				    ["Tree walking"] = { "ability.txt", 03 },
				    ["Perfect casting"] = { "ability.txt", 04 },
				    ["Extra Max Blow(1)"] = { "ability.txt", 05 },
				    ["Extra Max Blow(2)"] = { "ability.txt", 06 },
				    ["Ammo creation"] = { "ability.txt", 07 },
				    ["Touch of death"] = { "ability.txt", 08 },
				    ["Artifact Creation"] = { "ability.txt", 09 },
				    ["Far reaching attack"] = { "ability.txt", 10 },
				    ["Trapping"] = { "ability.txt", 11 },
				    ["Undead Form"] = { "ability.txt", 12 },
				},
			}

			if t[typ][name] then ingame_help_doc(t[typ][name][1], t[typ][name][2])
			else ingame_help_doc("help.hlp", 0)
			end
	end,
}

ingame_help
{
	["hook"]  = hook.IDENTIFY_POST,
	["event"] = function(obj, slot, mode)
					return known_flag(obj, FLAG_SPELL_CONTAIN)
			end,
	["desc"] =
	{
		"Ah, an item that can contain a spell. When it contains a spell, you",
		"can use it like a spellbook, but only when it is equiped."
		"To copy a spell into it you must have some levels of the Magic",
		"skill and then you get the option to copy a spell when pressing m.",
		"Then just select which spell to copy and to which object.  Note",
		"that doing so is permanent; the spell cannot be removed or",
		"changed later.",
	}
}

ingame_help
{
	["hook"] =      hook.GET_POST,
	["event"] =     function(obj, idx) if obj.tval == TV_TRAPKIT then return true end end,
	["desc"] =
	{
		"Ooooh, a trapping kit. If you have ability in the trapping skill, ",
		"you can lay this trap (via the 'm' key) to harm unsuspecting foes. ",
		"You'll generally need either some ammo or magic device depending  ",
		"on the exact type of traps.",
	}
}

ingame_help
{
	["hook"] =      hook.RECALC_SKILLS,
	["event"] =     function() if game.started and (get_combat_num() > 1) then return true end end,
	["desc"] =
	{
		"Ah, you now possess more than one melee type, to switch between them press 'm'",
		"and select the switch melee type option.",
	}
}

ingame_help
{
	["hook"] =      hook.GET_POST,
	["event"] =     function(obj, idx) if obj.tval == TV_WAND then return true end end,
	["desc"] =
	{
		"You've found a wand. Each wand contains a random Thaumaturgy spell;",
		"check out the help file on Thaumaturgy for more about these.",
		"Increasing your Magic Device skill will make using these wands",
		"easier.  There is lots of information you can find about this",
		"object if you identify it and 'I'nspect it."
	}
}

ingame_help
{
	["hook"]  = hook.GET_POST,
	["event"] = function(obj, idx) if obj.tval == TV_ORB then return true end end,
	["desc"] =
	{
		"You've found a orb.  Each orb has a spell from one of the",
		"primary magic schools; check the help file on Magic for more about",
		"these.  Increasing your Magic Device skill will make using staves",
		"easier, and also increase the power of the spell that it casts."
		"There is lots of information you can find about this object if",
		"you identify it and 'I'nspect it.",
	}
}

ingame_help
{
	["hook"]  = hook.MOVE_PRE,
	-- User is unlikely to pick up a treasure chest, so check if its
	-- on the ground
	["event"] = function(y, x)
					local ret = {["ret"] = false}
					for_inventory_inven(cave(y, x).inventory,
										function(obj)
											if obj.tval == TV_CHEST then
												%ret.ret = true
											end
										end)
					return ret.ret
				end
	["desc"] =
	{
		"A treasure chest!  You can 'o'pen it up to get its contents, but",
		"be wary of traps!"
	}
}

ingame_help
{
	["hook"]  = hook.MOVE_PRE,
	-- User is unlikely to pick up junk, so check if its on the ground
	["event"] = function(y, x)
					local ret = {["ret"] = false}
					for_inventory_inven(cave(y, x).inventory,
										function(obj)
											if obj.tval == TV_JUNK then
												%ret.ret = true
											end
										end)
					return ret.ret
				end
	["desc"] =
	{
		"A useless bit of junk lying on the ground!  Or maybe not so",
		"useless, since it can be converted into ammo by those with the",
		"Forge Ammo ability."
	}
}

ingame_help
{
	["hook"]  = hook.GET_POST,
	["event"] = function(obj, idx)
					if obj.tval == TV_RUNE1 then
						return true
					end
					if obj.tval == TV_RUNE2 and obj.sval ~= SV_RUNESTONE then
						return true
					end
				end
	["desc"] =
	{
		"You've found a rune.  Those with the Runecrafting skill can",
		"combine runes to create hand-crafted spells."
	}
}


ingame_help
{
	["hook"] =      hook.PLAYER_LEVEL,
	["event"] =     function(y, x) if player.lev >= 30 then return true end end,
	["desc"] =
	{
		"I see you are now at least level 30, nice. If you want to gloat about your",
		"character you could press 'C' then 'f' to make a character dump and post it to",
		"http://angband.oook.cz/ where it will end up in the ladder.",
	}
}
