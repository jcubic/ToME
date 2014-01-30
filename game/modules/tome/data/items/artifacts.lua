getter.auto_share_flags(true)

----------------------------------------------------
---                   Lites                      ---
----------------------------------------------------

-- The phial of galadriel, the only lite source with a radius of 4
new_artifact
{
	define_as = "ART_PHIAL"
	name = "& glowing phial~ of galadriel"
	unknown_name = "& glowing phial~"

	desc = {
		"A small crystal phial, with the light of Earendil's Star ",
		"contained inside.  Its light is imperishable, and near it ",
		"darkness cannot endure.",
	}
	base_object = { tval=TV_LITE, sval=SV_LITE_FEANORIAN }
	color = color.YELLOW
	level = 20 rarity = 10 weight = 10 cost = 10000
	flags =	{
		INSTA_ART = true LITE = obvious(4)
		LITE_SUN = obvious(true) LUCK = 4
		SKILL_BONUS = getter.skills{["Sneakiness"] = 4000}

		WIELD_POST=function(obj) message(color.GREEN, "You feel the courrage to go in the dark places of the world where nobody else will go.") end
		TAKEOFF_POST=function(obj) message(color.RED, "Your confidence in yourself seems to wither away just by thinking of dark places.") end

		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			-- ACT_SPELL   = "activations.artifacts.galadriel"
			ACT_SPELL   =
				function(obj, spell_flags, skill,
						 mana_cost, who)
					if not obj then
						return "lite", "to lite up the room"
					end
					message(color.YELLOW, "It wells with clear light...")
					lite_room(player.py, player.px)
					return true
				end

			-- Every 5 - 15 turns (4 + 1d11)
			ACT_COST_BASE = 4 ACT_COST_DICE=1 ACT_COST_SIDES=11
		}) -- DEVICE_USE

		-- Mana and autocharging needed for activation
		MANA_CURR = obvious(15) MANA_MAX = obvious(15)
		AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
		AUTO_CHARGE_RATE=obvious(1) AUTO_CHARGE_WIELD=obvious(true)
	}
}

-- A lite source AND a powerful gemstone
new_artifact
{
	name = "& arkenstone~ of thrain"
	unknown_name = "& arkenstone~"
	desc = {
		"A great globe seemingly filled with moonlight, the famed Heart ",
		"of the Mountain, which splinters the light that falls upon it ",
		"into a thousand glowing shards.  It is also a powerful gemstone ",
		"that can power other objects.",
	}
	-- TODO: pick different base type
	base_object = { tval=TV_LITE, sval=SV_LITE_LANTERN }
	color = color.LIGHT_RED
	level = 50 rarity = 50 weight = 5 cost = 50000
	flags =
	{
		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			ACT_SPELL   =
				function(obj, spell_flags, skill,
						 mana_cost, who)
					if not obj then
						return "lite level", "to up the whole level"
					end
					message("You hold the Arkenstone and summon " ..
							"bright beams of light in all directions!")
					wiz_lite_extra()
					return true
				end

			-- Every 75 - 150 turns (148 + 1d152, charge rate = 2)
			ACT_COST_BASE = 148 ACT_COST_DICE=1 ACT_COST_SIDES=152
		}) -- DEVICE_USE

		ESP = getter.flags{ ORC=90 } HIDE_TYPE = true
		HOLD_LIFE = true INSTA_ART = true LUCK = 3
		SEE_INVIS = true SPEED = 2 LITE = obvious(3) LITE_SUN = obvious(true)
		RESIST=obvious(getter.resists{DARK=33 LITE=33})

		GEMSTONE = obvious(true)
		AUTO_CHARGE_PACK  = true
		AUTO_CHARGE_WIELD = true
		AUTO_CHARGE_TYPE = obj_mana.auto_charge_types.TIME
		AUTO_CHARGE_RATE = 2
		MANA_CURR = 300
		MANA_MAX = 300
		MANA_USE_POST =	function(obj, amnt, sink)
			-- When used it fires a weak light based attack and lights the room
			fire_ball(dam.LITE, 0, amnt + get_skill_scale(SKILL_DEVICE, 25), 3)
			lite_room(player.py, player.px)
		end
	}
}

----------------------------------------------------
---                   Jewelry                    ---
----------------------------------------------------
new_artifact
{
	define_as = "ART_NARYA"
	name = "& Narya, the elven ring of power"
	desc = {
		"The Ring of Fire, set with a ruby that glows like flame."
		"Narya is one of the three Rings of Power created by the Elves"
		"and hidden by them from Sauron."
	}
	base_object = { tval=TV_RING sval=SV_RING_NARYA }
	to_h = 12 to_d = 12
	level = 90 rarity = 50 weight = 2 cost = 300000
	flags =
	{
		PROTECT_FROM = getter.resists{FIRE=true}
		RESIST = getter.resists{FIRE=100 POIS=70 }
		FREE_ACT = true LUCK = 3 SEE_INVIS = true FLY = 1 HOLD_LIFE = true
		STATS = getter.stats{[A_INT] = 3 [A_WIS] = 6 [A_CHR] = 2}
		SUST_STATS = getter.stats{[A_INT]=true [A_WIS]=true [A_CHR]=true }
		SPEED = 3
	}

}

new_artifact
{
	define_as = "ART_ONE_RING"
	name = "& one ring"
	desc = {
		'"Ash nazg durbatuluk, ash nazg gimbatul, ash nazg thrakatuluk agh',
		'burzum-ishi krimpatul". Unadorned, made of massive gold,',
		'set with runes in the foul speech of Mordor, with power so great that it',
		'inevitably twists and masters any earthly being who wears it.',
	}
	base_object = { tval=TV_RING, sval=SV_RING_ONE }
	to_h = 0 to_d = 0
	level = 100 rarity = 100 weight = 2 cost = 5000000
	flags =	{
		SPECIAL_GENE = true
		DRAIN_EXP = true
		HIDE_TYPE = true
		INSTA_ART = true
		INVIS = 30
		SEE_INVIS = true
		OBJ_EXP_PLAYER_EXP = true
		PROTECT_FROM = getter.resists{FIRE=true ACID=true ELEC=true ACID=true}
		ESP = getter.flags{ORC=50 TROLL=50 DRAGON=50 GIANT=50 DEMON=50 UNDEAD=50 EVIL=50 ANIMAL=50 GOOD=50 NONLIVING=50 UNIQUE=50 SPIDER=50}
		RESIST = getter.resists{FIRE=2 ELEC=2 ACID=2 COLD=2 POIS=2}
		SUST_STATS = getter.stats{[A_STR]=true [A_DEX]=true [A_CON]=true [A_INT]=true [A_WIS]=true [A_CHR]=true }

		AUTO_PICKUP = "#vYou feel the urge to pick up that plain gold ring you see.#w"

		-- The One Ring starts quite weak, but gains power as it goes
		LEVELS = function(obj)
			-- Every levels lose 1% HP & gain 1% mana
			inc_flag(obj, FLAG_LIFE, -1)
			inc_flag(obj, FLAG_MANA, 1)

			if imod(obj.elevel, 7) == 0 then inc_flag(obj, FLAG_SPEED, 1) end
			if imod(obj.elevel, 7) == 0 then inc_flag(obj, FLAG_SPELL, 1) end
			if imod(obj.elevel, 3) == 0 then obj.to_h = obj.to_h + 1 end
			if imod(obj.elevel, 3) == 1 then obj.to_d = obj.to_d + 1 end

			if imod(obj.elevel, 6) == 0 then
				local stats = obj.flags[FLAG_STATS]
				if not stats then stats = flag_new() obj.flags[FLAG_STATS] = stats end
				flag_inc(stats, A_STR, 1)
				flag_inc(stats, A_DEX, 1)
				flag_inc(stats, A_CON, 1)
				flag_inc(stats, A_INT, 1)
				flag_inc(stats, A_WIS, 1)
				flag_inc(stats, A_CHR, 1)
			end

			local res = obj.flags[FLAG_RESIST]
			flag_inc(res, dam.FIRE, 2)
			flag_inc(res, dam.ELEC, 2)
			flag_inc(res, dam.COLD, 2)
			flag_inc(res, dam.ACID, 2)
			if imod(obj.elevel, 3) == 0 then flag_inc(res, dam.ACID, 5) end

			if obj.elevel == 10 then set_flag(obj, FLAG_RES_BLIND, true) end
			if obj.elevel == 20 then set_flag(obj, FLAG_FREE_ACT, true) end
			if obj.elevel == 15 then inc_flag(obj, FLAG_REGEN_LIFE, 7000) end

			player.update = player.update | PU_BONUS
		end

		WIELD_PRE = function(obj)
			if not get_check("You were warned not to wear it, are you sure?") then return true, false end
			if not get_check("You were warned not to wear it, are you *REALLY* sure?") then return true, false end
			if not get_check("You were *WARNED* not to wear it, are you *R*E*A*L*L*Y* sure?") then return true, false end
		end
		WIELD_POST = function(obj)
			message(color.LIGHT_DARK, "As you put on the ring you feel as if your soul was torn apart.")
			message(color.LIGHT_DARK, "You begin to regret wielding such power.")
			change_quest_status(QUEST_ONE_RING, QUEST_STATUS_FAILED_DONE)
			acquire_quest(QUEST_SAURON)

			for i = 1, max_towns - 1 do
				town(i).destroyed = true
			end
		end
		TAKEOFF_PRE = function(obj) message(color.LIGHT_RED, "You can not bring yourself to remove it. The Ring will not allow it.") return true,false end
		DROP_PRE = function(obj, item)
			if cave(player.py, player.px).feat == FEAT_GREAT_FIRE then
				if not get_check("Are you sure you wish to destroy the One Ring?") then return true end
				message(color.YELLOW, "You cast the ring into the fiery core of the mountain")
				message(color.YELLOW, "and watch it as it slowly disolves.")
				item_increase(item, -99)
				item_describe(item)
				item_optimize(item)
				message(color.YELLOW, "Your hear a lound and powerful shriek coming from Dol Guldur, Sauron knows.")
				change_quest_status(QUEST_ONE_RING, QUEST_STATUS_COMPLETED)
				acquire_quest(QUEST_SAURON)
				message(color.YELLOW, "You should go to Caras Galadhon in Lothlorien and inform Lady Galadriel of the destruction fo the One Ring.")
			else
				message(color.LIGHT_RED, "You can not bring yourself to part from it. The Ring will not allow it.")
			end
			return true
		end
	}
}
--[[
new_artifact
{
	define_as = "ART_PORTABLE_HOLE"
	name = "& portable hole amulet"
	desc = {
		'This amulet seems empty, very empty. Profoundly empty.',
		'It makes you shiver.',
	}
	base_object = { tval=TV_AMULET, sval=SV_AMULET_PORTABLE_HOLE }
	to_h = 0 to_d = 0
	level = 50 rarity = 90 weight = 2 cost = 25000
	flags =	{
		AUTO_CURSE = true
		INSTA_ART = true
		ON_MAKE = function(obj)
			obj.flags[FLAG_STORE] = store.create(store.STORE_MAGIC_HOME)
		end

		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			ACT_SPELL   =
				function(obj, spell_flags, skill,
						 mana_cost, who)
					if not obj then
						return "open portable hole",
						"to open the portable hole (this takes a long time)"
					end

					store.display(obj.flags[FLAG_STORE])
					player.energy = player.energy -
						(4 * get_player_energy(SPEED_GLOBAL));
					return true
				end

			-- Every 10 - 20 turns (9 + 1d11)
			ACT_COST_BASE = 9 ACT_COST_DICE=1 ACT_COST_SIDES=11
		}) -- DEVICE_USE

		-- Mana and autocharging needed for activation
		MANA_CURR = obvious(20) MANA_MAX = obvious(20)
		AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
		AUTO_CHARGE_RATE=obvious(1) AUTO_CHARGE_WIELD=obvious(true)
	}
}

new_artifact
{
	define_as = "ART_EYE_OF_SMAUG"
	name = "& fiery eye of Smaug the golden"
	desc = {
		'The Eye of Smaug himself, taken from his dead body.',
		'It conveys all of the beast\'s fiery and powerful magic.',
	}
	base_object = { tval=TV_AMULET, sval=SV_AMULET_EYE_OF_SMAUG }
	to_h = 0 to_d = 0
	level = 70 rarity = 90 weight = 2 cost = 60000
	flags =	{
		SPECIAL_GENE = true
		AUTO_CURSE = true
		INSTA_ART = true

		ESP = getter.flags{ DRAGON=40 }
		RESIST = obvious(getter.resists{FIRE=100 COLD=-33})
		PROTECT_FROM = getter.resists{FIRE=true}
		SPELL_FAILURE = -5
		STATS = getter.stats{[A_INT] = 4 [A_WIS] = 6 [A_CON] = 3}
		MANA = 6
		SPELL = 4

		GEMSTONE = true
		MANA_CURR = 2000
		MANA_MAX = 2000
	}
}
--]]

----------------------------------------------------
---                   Weapons                    ---
----------------------------------------------------

-- Mage staff of absorption
-- Steals spell levels to beef up the owner's spells
new_artifact
{
	define_as = "ART_ABSORPTION"
	name = "& magestaff of absorption"
	desc = {
		"Light seems to dim around it.",
	}
	base_object = { tval=TV_MSTAFF, sval=SV_MAGE_STAFF }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 20 rarity = 40 weight = 20 cost = 30000
	flags =
	{
		ABSORBED_LEVELS=0
		ON_HIT=getter.spell_chance("Spell Absorption", 5) ON_HIT_LEVEL=25

		-- Eats up all mana when wielded
		WIELD_POST=function(obj)
			message(color.LIGHT_RED, "You feel the dark power of this staff taping into your very soul.")
			player.csp(-player.csp())
		end
		TAKEOFF_POST=function(obj) message(color.LIGHT_RED, "You feel relieved as the staff grasp on your soul weakens.") end

		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			ACT_SPELL   =
				function(obj, spell_flags, skill,
						 mana_cost, who)
					if not obj then
						return "release spell levels",
						"to release spell levels"
					end

					local used = obj.flags[FLAG_ABSORBED_LEVELS]
					used = get_quantity("How much much levels to be " ..
										"used (max " ..
											used .. ")?", used)

					local old = player.to_s
					player.to_s = player.to_s + used

					local spl = get_school_spell("cast", "is_ok_spell")
					if spl ~= -1 then
						cast_school_spell(spl, spell(spl))
					end

					-- Reset
					obj.flags[FLAG_ABSORBED_LEVELS] =
						obj.flags[FLAG_ABSORBED_LEVELS] - used
					player.to_s = old
					return true
				end

			-- Every 350 - 650 turns (349 + 1d301)
			ACT_COST_BASE = 349 ACT_COST_DICE=1 ACT_COST_SIDES=301
 		}) -- DEVICE_USE

		DEVICE_CAN_USE =
			function(obj, spell_flags, who)
				if get_skill(SKILL_MAGIC) < 20 then
					message("You need a magic skill of at least 20 " ..
							"to release spell levels..")
					return true
				end
				if obj.flags[FLAG_ABSORBED_LEVELS] == 0 then
					message("The staff does not contain any spell levels.")
					return true
				end

				return false
			end

		-- Mana and autocharging needed for activation
		MANA_CURR = obvious(650)  MANA_MAX = obvious(650)
		AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
		AUTO_CHARGE_RATE=obvious(1) AUTO_CHARGE_WIELD=obvious(true)
	}
}

new_artifact
{
	define_as = "ART_NARTHANC"
	name = "& dagger~ 'narthanc'"
	desc = {"A fiery dagger finely balanced for deadly throws.",
		"It is one of the 3 legendary daggers."}
	base_object = { tval=TV_KNIFE, sval=SV_DAGGER }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 15 rarity = 40 weight = 25 cost = 30000
	flags =	{
		OBJ_EXP_MON_KILL_ACTIVE = true
		SENTIENT                = obvious("melee")
		LEVELS                  = true

		BRAND       = obvious(getter.array{[dam.FIRE] = 3})
		LITE        = obvious(1)
		RESIST      = obvious(getter.resists{FIRE=66})
		SKILL_BONUS = getter.skills{["Fire"] = 10000}
	}
} -- Narthanc

new_artifact
{
	define_as = "ART_TAXIDERMY"
	name = "& dagger~ of the taxidermist"
	desc = {"Monsters slain by this weapon will always leave behind a corpse."}
	base_object = { tval=TV_KNIFE, sval=SV_DAGGER }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 15 rarity = 40 weight = 25 cost = 10000
	flags =	{
		WEAP_KILLED_MON = function(obj, launcher, m_ptr)
				m_ptr.flags[FLAG_DROP_CORPSE_FORCE] = true
			end
		SKILL_BONUS = getter.skills{["Corpse-preservation"] = 5000}
	}
} -- Taxidermy


--
-- The hilt and the blade of the broken sword narsil
--
new_artifact
{
	define_as = "ART_NARSIL_HILT"
	name = "& shard~ of Narsil(hilt)"
	desc = {"The sword that was broken shall be reforged..."}
	base_object = { tval=TV_JUNK, sval=SV_BROKEN_HILT }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 25 rarity = 40 weight = 25 cost = 1000
	flags =	{
		SPECIAL_GENE=true
	}
}
new_artifact
{
	define_as = "ART_NARSIL_BLADE"
	name = "& shard~ of Narsil(the blade)'"
	desc = {"The sword that was broken shall be reforged..."}
	base_object = { tval=TV_JUNK, sval=SV_BROKEN_BLADE }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 25 rarity = 40 weight = 25 cost = 1000
	flags =	{
		SPECIAL_GENE=true
	}
}

----------------------------------------------------
---                   Armours                    ---
----------------------------------------------------

new_artifact
{
	define_as = "ART_GREAT_LUCK"
	name = "& robe of great luck"
	desc = {
		"Once created by a powerful wizard this robe to grant him incredible luck...",
		"It seems he forgot to wear it.",
	}
	base_object = { tval=TV_LIGHT_ARMOUR sval=SV_ROBE }
	ac = -30
	to_a = -20
	level = 50 rarity = 100 weight = 20 cost = 6000
	flags =
	{
		DRAIN_LIFE=1000 DRAIN_MANA=1000
		FREE_ACT = true
		LUCK = 60
	}

}

-- A mage armour, that can absord the costof some spells, if powered
new_artifact
{
	define_as = "ART_DRAGON_SCALE_GLAURUNG1"
	name = "& dragon scale mail of Glaurung"
	unknown_name = "& golden dragon scale mail~" -- Glaurung was golden
	desc = {
		"This armour was forged from the scales of Glaurung himself.",
		"It shimmers with pure power, but it seems it should be possible",
		"to insert a gemstone to power some incredible magic powers."
	}
	base_object = { tval=TV_LIGHT_ARMOUR, sval=SV_DRAGON_SCALE_MAIL }
	ac = 18 to_a = 0
	level = 90 rarity = 200 weight = 65 cost = 200000
	flags =
	{
		SPELL_FAILURE = -5
		STATS = getter.stats{[A_INT] = 4 [A_WIS] = 3 [A_CON] = 1}
		MANA = 18
		SPELL = 3
		PLAYER_SCHOOL_SPELL_CAST_POST=function(obj, item, inven, slot, s, spend_sp)
			local gem = obj.flags[FLAG_MANA_SOURCE_OBJ]
			if gem and spend_sp and obj_mana.get_curr(obj) >= get_mana(s) * 2 and rng.percent(10) then
				message("The dragon scale mail of Glaurung glows a golden color for a second!")

				-- Take the mana in the gemstone
				obj_mana.use(obj, get_mana(s) * 2)

				-- Do not use sp or cooldowns, but still take a turn
				energy_use = get_player_energy(SPEED_CAST)
				return true, false
			end
		end

		GEM_SOCKET = obvious(true)

		SPECIAL_GENE=true
		WIELD_POST=function(obj) message(color.CRIMSON, "You feel the power of Glaurung spirit strengthening your magical abilities..")	end
		TAKEOFF_POST=function(obj) message(color.CRIMSON, "You feel the power of Glaurung spirit withering away.") end

		-- Glaurung breathed fire; should this provide fire resistance?
	}

}

----------------------------------------------------
---                   Head gear                  ---
----------------------------------------------------

-- The Steel Helm 'Lebohaum'
-- The name comes from a french parody of dungeon dwelling in mp3
-- http://penofchaos.com/warham.htm
new_artifact
{
	define_as = "ART_LEBOHAUM"
	name = "& helm 'Lebohaum'"
	desc = {
		"With the Helm 'Lebohaum' your head is safe!",
	}
	base_object = { tval=TV_HEAVY_HELM, sval=SV_PARTIAL_HELM }
	ac = 0 to_a = 0
	level = 40 rarity = 15 weight = 50 cost = 5000
	flags =
	{
		ABSORB = getter.absorbs{ SLASH=15 CRUSH=15 PIERCE=15 }

		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			ACT_SPELL   =
				function (obj, spell_flags, skill,
						  mana_cost, who)
					if not obj then
						return "sing a cheerful song",
						"to sing a cheerful song"
					end

					message("You hear a little song in your head in some unknown tongue:")
					message("'Avec le casque Lebohaum y a jamais d'anicroches, je parcours les dongeons,")
					message("j'en prend plein la caboche. Avec le casque Lebohaum, tout ces monstres a la")
					message("con, je leur met bien profond: c'est moi le maitre du dongeon!'")
					return true
				end

			-- Free use (zero cost)
			ACT_COST_BASE = 0
		}) -- DEVICE_USE

		-- Mana and autocharging needed for activation
		MANA_CURR = obvious(1) MANA_MAX = obvious(1)
		AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
		AUTO_CHARGE_RATE=obvious(1) AUTO_CHARGE_WIELD=obvious(true)
	}
}


-- The helm of knowledge
new_artifact
{
	define_as = "ART_KNOWLEGDE"
	name = "& helm of knowledge"
	unknown_name = "& glass helm~"
	desc = {
		"This helm, designed by Petty-Dwarves ages ago to act as the brain ",
		"of a long lost project, is made of finest glass.  Its light ",
		"banishes all secrets, and makes audible whispers from the deceased.",
	}
	base_object = { tval=TV_LIGHT_HELM, sval=SV_HARD_CAP }
	ac = 3 to_a = 6
	level = 30 rarity = 10 weight = 50 cost = 10000
	flags =
	{
		-- Activation
		DEVICE_USE = obvious(getter.single_device_spell
		{
			DEVICE_TYPE = "activate"
			ACT_SPELL   =
               function(obj, spell_flags, skill, mana_cost, who)
				   if not obj then
					   return "whispers from beyond",
					   "whispers from beyond (fully identify + sanity drain)"
				   end

				   identify_fully()
				   take_sanity_hit(rng.roll(10, 7), "the sounds of the dead")
			   end

		   -- Every 100 - 300 turns (99 + 1d201)
		   ACT_COST_BASE = 99 ACT_COST_DICE=1 ACT_COST_SIDES=201
	    }) -- DEVICE_USE

		-- Mana and autocharging needed for activation
		MANA_CURR = obvious(300) MANA_MAX = obvious(300)
		AUTO_CHARGE_TYPE=obvious(obj_mana.auto_charge_types.TIME)
		AUTO_CHARGE_RATE=obvious(1) AUTO_CHARGE_WIELD=obvious(true)

		AUTO_ID = true LITE = obvious(1)
		LUCK = -6
	}
}

----------------------------------------------------
---                     Cloaks                   ---
----------------------------------------------------

-- ever oiled cloak, to pass nasty spider webs
new_artifact
{
	define_as = "ART_OILY_CLOAK"
	name = "& ever-oiled cloak"
	desc = {
		"This cloak looks normal, except that it seems to be coated in oil"
		"that never goes off."
	}
	base_object = { tval=TV_CLOAK sval=SV_LEATHER_CLOAK }
	ac = 4 to_a = 6
	level = 30 rarity = 4 weight = 40 cost = 3000
	flags =
	{
		PROTECT_FROM = getter.resists{ACID=true FIRE=true COLD=true}
		RESIST = getter.resists{COLD=60}
		FREE_ACT = true LUCK = 5 HOLD_LIFE = true
		SPEED = 4
		PASS_WALL = getter.flags{ PASS_WEB=1 }
	}

}


----------------------------------------------------
---               Sticks and gems                ---
----------------------------------------------------

----------------------------------------------------
---                Junk and quest                ---
----------------------------------------------------

--
-- The heirloom for Farlina Snowfoot
--
new_artifact
{
	define_as = "ART_FARLINA_HEIRLOOM"
	name = "& heirloom~ of Farlina Snowfoot"
	desc = {"Some kind of family ring"}
	base_object = { tval=TV_JUNK, sval=SV_JUNK }
	dice = {1, 1}
	to_h = 0 to_d = 0
	level = 12 rarity = 40 weight = 25 cost = 1000
	flags =	{
		SPECIAL_GENE = true
		GET_POST = function()
			 message(color.YELLOW, "This seems to be the heirloom of Farlina Snowfoot, bring it back to her.")
		end
	}
}


---------------------------------------------------------------------------
---------------------------------------------------------------------------

getter.auto_share_flags(false)
