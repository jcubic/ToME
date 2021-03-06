-- Combat system

declare_global_constants {
	"__do_aura",
	"add_aura"
}

-- Now load the various combat systems
load_subsystem("combat_weapon_default",
{
	strength_blow_modifier = function(mul, div)
		return (player.stat(A_STR) * 3 * mul) / div
	end
	dexterity_blow_modifier = function()
		return player.stat(A_DEX) / 5
	end
	mainhand_slot = INVEN_MAINHAND
	offhand_slot = INVEN_OFFHAND
	base_skill_name = "Weaponmastery"

	-- Can we dual wield ?
	dual_wield = function(obj)
		return true
	end

	skill_bonus = function()
		local lev = get_skill(combat.default_weapon.get_weaponmastery_skill())

		player.to_h = player.to_h + lev / 2
		player.to_d = player.to_d + (lev / 4)
	end

	energy_use = function(moved, args)
		local mult = args.speed_mult or 1
		local div = args.speed_div or 1
		
		-- Swift kill ability
		if args.mdeath and ability(AB_SWIFT_KILL).acquired then
			div = div * 2
		end

		return (get_player_energy(SPEED_WEAPON) * mult) / div
	end

	-- Determine if the player "hits" a monster (normal combat).
	-- Note -- Always miss 5%, always hit 5%, otherwise random.
	test_hit = function(obj, mon, chance)
		-- Percentile dice
		local k = rng.number(100)

		combat.k = k
		if hook.process(hook.TEST_HIT, obj, nil, mon, chance) then
			return false
		end
		k = combat.k

		k = item_hooks.process_all_chain(obj, FLAG_TEST_HIT, k,
										 nil, mon, chance)

		if not k then
			return false
		end

		-- Instant miss or hit
		if k < 10 then
			if k < 5 then
				return true
			else
				return false
			end
		end

		-- Wimpy attack never hits
		if chance <= 0 then return false end

		-- Penalize invisible targets
		if not mon.ml then chance = (chance + 1) / 2 end

		-- Power must defeat armor
		if rng.number(chance + luck(-10, 10)) < (mon.ac * 3) / 4 then
			return false
		end

		-- Assume hit
		return true
	end -- test_hit()

	---

	-- Critical hits (by player)
	-- Factor in weapon weight, total plusses, player skills.
	critical_hit = function(obj, mon, skill)
		local i, k, num
		local crit_mult, crit_div, bonus = 1, 1, 0

		num = rng(5000)

		-- Extract "blow" power
		i = combat.default_weapon.calc_crit_chance(obj, mon, skill)

		if not i then
			return crit_mult, crit_div, bonus
		end

		-- Chance
		if num <= i then
			k = obj.weight + rng(650)

			if obj.tval == TV_KNIFE then
				k = k + get_skill_scale(SKILL_CRITS, 400)
			end

			combat.critical_type = k
			if hook.process(hook.CRIT_HIT_TYPE, obj, nil, mon, skill) then
				return crit_mult, crit_div, bonus
			end
			k = combat.critical_type

			k = item_hooks.process_all_chain(obj, FLAG_CRIT_HIT_TYPE, k,
											 nil, mon, skill)

			if not k then
				return crit_mult, crit_div, bonus
			end

			if k < 400 then
				message("It was a good hit!")
				crit_mult = 2
				bonus     = 5
			elseif k < 700 then
				message("It was a great hit!")
				crit_mult = 2
				bonus     = 10
			elseif k < 900 then
				message("It was a superb hit!")
				crit_mult = 3
				bonus     = 15
			elseif k < 1300 then
				message("It was a *GREAT* hit!")
				crit_mult = 3
				bonus     = 20
			else
				message("It was a *SUPERB* hit!")
				crit_mult = 7
				crit_div  = 2
				bonus     = 25
			end
			
			-- Account for stunning blows when using heavy hafted weapons
			if obj.tval == TV_HAFTED and obj.weight > 50 then
				local value = (get_skill_scale(SKILL_STUN, 1000) * crit_mult) / crit_div + bonus * 10

				if value > 0 then
					-- Use the already defined STUN damage type
					project(WHO_PLAYER, 0, mon.fy, mon.fx, value, dam.STUN, PROJECT_JUMP | PROJECT_HIDE | PROJECT_STOP | PROJECT_KILL | PROJECT_NO_REFLECT)
				end
			end
		end

		return crit_mult, crit_div, bonus
	end -- critical_hit()

	---

	on_hit = function(obj, m_ptr, args)
		message("You hit "..monster_desc(m_ptr, 0))

		local y, x = m_ptr.fy, m_ptr.fx
		
		-- Project some more nasty stuff?
		if timed_effect.get(timed_effect.MELEE_PROJECT) then
			local effect = timed_effect.MELEE_PROJECT
			local rad    = timed_effect.get(effect, "rad")
			local dam    = timed_effect.get(effect, "dam")
			local typ    = timed_effect.get(effect, "type")
			local flags  = timed_effect.get(effect, "flags")

			project(0, rad, y, x, dam, typ,
					flags | PROJECT_JUMP | PROJECT_NO_REFLECT)

			-- Stop, stop, he's already dead
			if (cave(y, x).m_idx == 0) then
				return true
			end
		end

		if player.has_intrinsic(FLAG_MELEE_CONFUSE) then
			local confuse_power = player.intrinsic(FLAG_MELEE_CONFUSE)
			project(0, 0, y, x, confuse_power, dam.CONFUSE,
			        PROJECT_HIDE | PROJECT_KILL | PROJECT_JUMP | PROJECT_NO_REFLECT)
			-- Jim, he's dead
			if (cave(y, x).m_idx == 0) then
				return true
			end
		end
	end

	---

	max_blows = function()
		local num = 0
		-- Count bonus abilities
		if has_ability(AB_MAX_BLOW1) then num = num + 1 end
		if has_ability(AB_MAX_BLOW2) then num = num + 1 end
		return num
	end
})

combat.default_weapon.calc_crit_chance = function(obj, mon, skill)
	-- Extract "blow" power
	local i = obj.weight + ((player.to_h + obj.to_h) * 5) +
		get_skill_scale(skill, 150)
	i = i + (50 * player.intrinsic(FLAG_CRIT))
	i = i + luck(-100, 100)
	if obj.tval == TV_KNIFE then
		i = i + get_skill_scale(SKILL_CRITS, 30 * 50)
	end

	combat.critical_chance = i
	if hook.process(hook.CRIT_HIT_CHANCE, obj, nil, mon, skill) then
		return crit_mult, crit_div, bonus
	end
	i = combat.critical_chance

	i = item_hooks.process_all_chain(obj, FLAG_CRIT_HIT_CHANCE, i, nil, mon, skill)

	return i
end

-----------------------------------------

load_subsystem("combat_barehand_default",
{
	stat_str = A_STR
	stat_dex = A_DEX
	mainhand_slot = INVEN_MAINHAND
	offhand_slot = INVEN_OFFHAND
	base_skill_name = "Barehand-combat"

	available =
		function()
			return (player.inventory_limits(INVEN_HANDS) >= 1)
		end
	
	energy_use = function(moved, args)
		local mult = args.speed_mult or 1
		local div = args.speed_div or 1
			
		-- Swift kill ability
		if args.mdeath and ability(AB_SWIFT_KILL).acquired then
			div = div * 2
		end

		return (get_player_energy(SPEED_BAREHAND) * mult) / div
	end


	-- Blows to use, race dependant
	blow_table = function()
		-- If the current mimic shape can do special barehand combat, do it
		if mimic.get_shape() ~= mimic.NONE and mimic.get_info(mimic.get_shape(), "barehand_combat") then
			return mimic.get_info(mimic.get_shape(), "barehand_combat")
		-- Otherise plain old monkish stuff
		else
			return
			{
				{
					desc = "You punch %s.",
					level = 1,
					chance = 0,
					dice = { 2, 4, },
					type = dam.CRUSH
				},
				{
					desc = "You kick %s.",
					level = 2,
					chance = 0,
					dice = { 2, 6, },
					type = dam.CRUSH
				},
				{
					desc = "You strike %s.",
					level = 3,
					chance = 0,
					dice = { 2, 7, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with your elbow.",
					level = 7,
					chance = 5,
					dice = { 2, 8, },
					type = dam.CRUSH
				},
				{
					desc = "You butt %s.",
					level = 9,
					chance = 10,
					dice = { 4, 5, },
					type = dam.CRUSH
				},
				{
					desc = "You kick %s.",
					level = 11,
					chance = 10,
					dice = { 6, 4, },
					type = dam.CRUSH_SLOW
				},
				{
					desc = "You uppercut %s.",
					level = 13,
					chance = 12,
					dice = { 8, 4, },
					type = dam.CRUSH_STUN
				},
				{
					desc = "You double-kick %s.",
					level = 16,
					chance = 15,
					dice = { 10, 4, },
					type = dam.CRUSH_STUN
				},
				{
					desc = "You hit %s with a Cat's Claw.",
					level = 20,
					chance = 20,
					dice = { 10, 5, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with a jump kick.",
					level = 25,
					chance = 25,
					dice = { 10, 6, },
					type = dam.CRUSH_STUN
				},
				{
					desc = "You hit %s with an Eagle's Claw.",
					level = 29,
					chance = 25,
					dice = { 12, 6, },
					type = dam.CRUSH_STUN
				},
				{
					desc = "You hit %s with a circle kick.",
					level = 33,
					chance = 30,
					dice = { 12, 8, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with an Iron Fist.",
					level = 37,
					chance = 35,
					dice = { 16, 8, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with a flying kick.",
					level = 41,
					chance = 35,
					dice = { 16, 10, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with a Dragon Fist.",
					level = 45,
					chance = 35,
					dice = { 20, 10, },
					type = dam.CRUSH
				},
				{
					desc = "You hit %s with a Crushing Blow.",
					level = 48,
					chance = 35,
					dice = { 20, 12, },
					type = dam.CRUSH
				},
			}
		end
	end

	-- Determine if the player "hits" a monster (normal combat).
	-- Note -- Always miss 5%, always hit 5%, otherwise random.
	test_hit = function(mon, chance)
		-- Percentile dice
		local k = rng.number(100)

		-- Instant miss or hit
		if k < 10 then
			if k < 5 then
				return true
			else
				return false
			end
		end

		-- Wimpy attack never hits
		if chance <= 0 then return false end

		-- Penalize invisible targets
		if not mon.ml then chance = (chance + 1) / 2 end

		-- Power must defeat armor
		if rng.number(chance + luck(-10, 10)) < (mon.ac * 3) / 4 then
			return false
		end

		-- Assume hit
		return true
	end -- test_hit()

	---

	-- Critical hits (by player)
	-- Factor in weapon weight, total plusses, player skills.
	critical_hit = function(mon, skill)
		local i, k, num
		local crit_mult, crit_div, bonus = 1, 1, 0

		num = rng(5000)

		-- Extract "blow" power
		i = (player.stat(A_STR) * 3) + ((player.to_h) * 5) + get_skill_scale(skill, 150)
		i = i + (50 * player.intrinsic(FLAG_CRIT))
		i = i + luck(-100, 100)

		-- Chance
		if num <= i then
			k = (player.stat(A_STR) * 3) + rng(650)

			if k < 400 then
				message("It was a good hit!")
				crit_mult = 2
				bonus     = 5
			elseif k < 700 then
				message("It was a great hit!")
				crit_mult = 2
				bonus     = 10
			elseif k < 900 then
				message("It was a superb hit!")
				crit_mult = 3
				bonus     = 15
			elseif k < 1300 then
				message("It was a *GREAT* hit!")
				crit_mult = 3
				bonus     = 20
			else
				message("It was a *SUPERB* hit!")
				crit_mult = 7
				crit_div  = 2
				bonus     = 25
			end
		end

		return crit_mult, crit_div, bonus
	end -- critical_hit()

	---

	on_hit = function(m_ptr)
		-- Project some more nasty stuff?
		local y, x = m_ptr.fy, m_ptr.fx
		if timed_effect.get(timed_effect.MELEE_PROJECT) then
			local effect = timed_effect.MELEE_PROJECT
			local rad    = timed_effect.get(effect, "rad")
			local dam    = timed_effect.get(effect, "dam")
			local typ    = timed_effect.get(effect, "type")
			local flags  = timed_effect.get(effect, "flags")

			project(0, rad, y, x, dam, typ,
					flags | PROJECT_JUMP | PROJECT_NO_REFLECT)

			-- Stop, stop, he's already dead
			if (cave(y, x).m_idx == 0) then
				return true
			end
		end

		if player.has_intrinsic(FLAG_MELEE_CONFUSE) then
			local confuse_power = player.intrinsic(FLAG_MELEE_CONFUSE)
			project(0, 0, y, x, confuse_power, dam.CONFUSE,
			        PROJECT_HIDE | PROJECT_KILL | PROJECT_JUMP | PROJECT_NO_REFLECT)
			-- Jim, he's dead
			if (cave(y, x).m_idx == 0) then
				return true
			end
		end

		hook.process(hook.PLAYER_HIT_MON_POST, cave(y, x).m_idx, m_ptr,
					  combat.default_barehand.SKILL_BAREHAND)
	end
})

-----------------------------------------

load_subsystem("combat_archery",
{
	bow_slot = INVEN_MAINHAND
	ammo_slot = INVEN_AMMO
	base_skill_name = "Archery"

	-- Determine if the player "hits" a monster (normal combat).
	-- Note -- Always miss 5%, always hit 5%, otherwise random.
	test_hit = function(obj, launcher, mon, chance)
		local k

		-- Percentile dice
		k = rng.number(100)

		combat.k = k
		if hook.process(hook.TEST_HIT, obj, launcher, mon, chance) then
			return false
		end
		k = combat.k

		k = item_hooks.process_all_chain(obj, FLAG_TEST_HIT, k,
										 launcher, mon, chance)

		if not k then
			return false
		end

		k = item_hooks.process_all_chain(launcher, FLAG_TEST_HIT, k,
										 obj, mon, chance)

		if not k then
			return false
		end

		-- Instant miss or hit
		if k < 10 then
			if k < 5 then
				return true
			else
				return false
			end
		end

		-- Never hit
		if chance <= 0 then return (false) end

		-- Invisible monsters are harder to hit
		if not mon.ml then chance = (chance + 1) / 2 end

		-- Power competes against armor
		if rng.number(chance + luck( -10, 10)) < (mon.ac * 3 / 4) then
			return false
		end

		-- Assume hit
		return true
	end -- test_hit()

	---

	-- Critical hits (from objects thrown by player)
	-- Factor in item weight, total plusses, and player level.
	critical_hit = function(obj, launcher, mon)
		local i, k
		local crit_mult, crit_div, bonus = 1, 1, 0

		-- Extract "shot" power
		i = (obj.weight + ((player.to_h + obj.to_h) * 4) +
			 get_skill_scale(combat.archery.SKILL_ARCHERY, 100))
		i = i + (50 * player.intrinsic(FLAG_CRIT))
		i = i + luck(-100, 100)

		combat.critical_chance = i
		if hook.process(hook.CRIT_HIT_CHANCE, obj, launcher, mon, dam,
						combat.archery.SKILL_ARCHERY) then
			return crit_mult, crit_div, bonus
		end
		i = combat.critical_chance

		i = item_hooks.process_all_chain(obj, FLAG_CRIT_HIT_CHANCE, i,
										 launcher, mon,
										 combat.archery.SKILL_ARCHERY)

		if not i then
			return crit_mult, crit_div, bonus
		end

		i = item_hooks.process_all_chain(launcher, FLAG_CRIT_HIT_CHANCE, i,
										 obj, mon,
										 combat.archery.SKILL_ARCHERY)

		if not i then
			return crit_mult, crit_div, bonus
		end

		-- Critical hit
		if rng(5000) <= i then
			k = obj.weight + rng(500)

			combat.critical_type = k
			if hook.process(hook.CRIT_HIT_TYPE, obj, launcher, mon,
							combat.archery.SKILL_ARCHERY) then
				return crit_mult, crit_div, bonus
			end
			k = combat.critical_type

			k = item_hooks.process_all_chain(obj, FLAG_CRIT_HIT_TYPE, k,
											 launcher, mon,
											 combat.archery.SKILL_ARCHERY)

			if not k then
				return crit_mult, crit_div, bonus
			end

			k = item_hooks.process_all_chain(launcher, FLAG_CRIT_HIT_TYPE, k,
											 obj, mon,
											 combat.archery.SKILL_ARCHERY)

			if not k then
				return crit_mult, crit_div, bonus
			end

			if k < 500 then
				message("It was a good hit!")
				crit_mult = 2
				bonus     = 5
			elseif k < 1000 then
				message("It was a great hit!")
				crit_mult = 2
				bonus     = 10
			else
				message("It was a superb hit!")
				crit_mult = 3
				bonus     = 15
			end
		end

		return crit_mult, crit_div, bonus
	end -- critical_shot()

	---

	on_hit = function(obj, launcher, m_ptr)
		-- Project some more nasty stuff?
		local y, x   = m_ptr.fy, m_ptr.fx
		if timed_effect.get(timed_effect.ARCHERY_PROJECT) then
			local effect = timed_effect.ARCHERY_PROJECT
			local rad    = timed_effect.get(effect, "rad")
			local dam    = timed_effect.get(effect, "dam")
			local typ    = timed_effect.get(effect, "type")
			local flags  = timed_effect.get(effect, "flags")

			project(0, rad, y, x, dam, typ,
					flags | PROJECT_JUMP | PROJECT_NO_REFLECT)
			if (cave(y, x).m_idx == 0) then
				return true
			end
		end
	end -- on_hit()

	---

	test_pierce = function(obj, launcher, m_ptr)
		return rng.percent(45 + get_skill_scale(combat.archery.SKILL_ARCHERY, 50))
	end

	---

	piercing_shots = function()
		if not (combat.archery.on or
				combat.archery.piercing_can()) then
			return 0
		end

		return combat.archery.piercing_max()
	end -- piercing_shots()

	---

	breakage_chance = function(obj)
		local chance = get_flag(obj, FLAG_BREAKAGE_CHANCE)

		if has_flag(obj, FLAG_BREAKAGE_CHANCE) == nil then chance = 10 end

		if has_flag(obj, FLAG_SKILL) then
			local reducer = get_skill_scale(get_flag(obj, FLAG_SKILL), 10)
			if reducer < 1 then reducer = 1 end

			chance = chance / reducer
		end
		return chance
	end -- breakage_chance()
})

combat.archery.piercing_on = false

function combat.archery.piercing_can()
	local skill = combat.archery.get_archery_skill()

	-- Relevent skill not high enough, hide mkey
	return (get_skill(skill) >= 52)
end

function combat.archery.piercing_max()
	local shots = get_skill(SKILL_COMBAT) / 20 - 1
	if shots < 0 then
		shots = 0
	end
	return shots
end

combat.archery.piercing_MKEY = add_mkey
{
	fct =
		function(type)
			if type == BROWSE_SKILL then
				message("Piercing shot browsing not implemented")
				return
			end

			while true do
				local ret, key =
					get_com("Fire piercing shots (currently " ..
							iif(combat.archery.on, "on", "off") ..
								") [y/n]?")

				key = strlower(strchar(key))

				if not ret then
					break
				end

				if key == 'y' then
					combat.archery.on = true
					message("Piercing shots now on")
					break
				elseif key == 'n' then
					combat.archery.on = false
					message("Piercing shots now off")
					break
				end
			end -- while
		end -- fct

	hide =
		function(type)
			if type == BROWSE_SKILL then
				-- Don't hide when browsing
				return false
			end

			-- Can't turn it on
			if not combat.archery.piercing_can() then
				return true
			end

			-- Can't piercing through anythingm even with it n
			if combat.archery.piercing_max() == 0 then
				return true
			end

			-- Okay to display
			return false
		end -- hide
}

------------------------------------------------

-- Monster combat
tome_dofile("monster.lua")

------------------------------------------------------------
-- Slays
------------------------------------------------------------
combat.new_slay("DRAGON", "is especially deadly against dragons",
				"dragons",
function(mon)
	return mon.flags[FLAG_DRAGON]
end)

combat.new_slay("ORC", "is especially deadly against orcs",
				"orcs",
function(mon)
	return mon.flags[FLAG_ORC]
end)

combat.new_slay("TROLL", "is especially deadly against trolls",
				"trolls",
function(mon)
	return mon.flags[FLAG_TROLL]
end)

combat.new_slay("GIANT", "is espcially deadly against giants",
				"giants",
function(mon)
	return mon.flags[FLAG_GIANT]
end)

combat.new_slay("DEMON", "is especially deadly against demons",
				"demons",
function(mon)
	return mon.flags[FLAG_DEMON]
end)

combat.new_slay("UNDEAD", "is especially deadly against the undead",
				"the undead",
function(mon)
	return mon.flags[FLAG_UNDEAD]
end)

combat.new_slay("EVIL", "is especially deadly against evil creatures",
				"evil creatures",
function(mon)
	return mon.flags[FLAG_EVIL]
end)

combat.new_slay("ANIMAL", "is especially deadly against natural creatures",
				"animals",
function(mon)
	return mon.flags[FLAG_ANIMAL]
end)

combat.new_slay("MAMMAL", "is especially deadly against mammals",
				"mammals",
function(mon)
	return mon.flags[FLAG_MAMMAL]
end)

combat.new_slay("AERIAL", "is especially deadly against flying creatures",
				"flying creatures",
function(mon)
	return mon.flags[FLAG_AERIAL]
end)

combat.new_slay("AQUATIC", "is especially deadly against aquatic creatures",
				"aquatic creatures",
function(mon)
	return mon.flags[FLAG_AQUATIC]
end)

combat.new_slay("INSECT", "is especially deadly against insects",
				"insects",
function(mon)
	return mon.flags[FLAG_INSECT]
end)

------------------------------------------------------------
-- Vampirism
------------------------------------------------------------
hook(hook.WEAP_DAM_DEALT,
function(obj, launcher, mon, r_idx, re_idx, dam_dealt)
	if dam_dealt == 0 then
		return
	end

	if mon.r_idx == 0 then
		-- Monster is dead, so the mon structure has been wiped; get
		-- info directly from race info and race ego info.
		if r_info[r_idx + 1].flags[FLAG_UNDEAD] or
			r_info[r_idx + 1].flags[FLAG_NONLIVING] then
			return
		end
		if re_info[re_idx + 1].flags[FLAG_UNDEAD] or
			re_info[re_idx + 1].flags[FLAG_NONLIVING] then
			return
		end
	elseif mon.flags[FLAG_UNDEAD] or mon.flags[FLAG_NONLIVING] then
		return
	end

	if obj.flags[FLAG_VAMPIRIC] then
		if mon.r_idx == 0 then
			message("Your weapon suck the last of the life out of " ..
					"your fallen foe!")
		else
			message("Your weapon drains life from " ..
					monster_desc(mon, 0) .. "!")
		end

		if dam_dealt <= obj.flags[FLAG_VAMPIRIC] then
			hp_player(dam_dealt)
		else
			hp_player(obj.flags[FLAG_VAMPIRIC])
		end
	end
end
)

-----------------------------------------------------------
-- Auras
-----------------------------------------------------------
memory.flag_aura = {}
memory.flag_aura.surround = {}
memory.flag_aura.cover    = {}
memory.flag_aura.radiate  = {}
memory.flag_aura.shroud   = {}
memory.flag_aura.shield   = {}

function __do_aura(aura, attacker, target, mindam, maxdam)
	local a_ptr, t_ptr
	local a_name, t_name, a_possesive, t_possesive, isare, sss
	local y, x
	local msg

	if attacker == WHO_PLAYER then
		a_ptr       = player
		a_name      = "you"
		a_possesive = "your"
		isare       = "are"
		sss         = ""

		msg = aura.player_msg or aura.msg

		y = player.py
		x = player.px
	else
		a_ptr        = monster(attacker)
		a_name       = monster_desc(a_ptr, 4)
		a_possesive  = monster_desc(a_ptr, 6)
		isare        = "is"
		sss          = "s"

		msg = aura.monster_msg or aura.msg

		y = a_ptr.fy
		x = a_ptr.fx
	end

	if target == WHO_PLAYER then
		t_ptr       = player
		t_name      = "you"
		t_possesive = "your"
	else
		t_ptr        = monster(target)
		t_name       = monster_desc(t_ptr, 4)
		t_possesive  = monster_desc(t_ptr, 6)
	end

	-- Player learns about non-visible auras when s/he attacks a monster
	if attacker == WHO_PLAYER and aura.not_visible and t_ptr.ml then
		local r_ptr = race_info_idx(t_ptr.r_idx, t_ptr.ego)
		local entry = memory.get_entry(r_ptr, t_ptr.ego)

		memory.check(entry, {{FLAG_BLOW_RESPONSE, aura.index}}, RT_MISC,
					 memory.make_flag_checker(r_ptr))
	end

	if msg and (attacker == WHO_PLAYER or
				(a_ptr.ml and player_has_los_bold(y, x) and
				 not player.has_intrinsic(FLAG_BLIND) and
					 not aura.not_visible)) then
		msg = gsub(msg, "(@Attacker@)", strcap(a_name))
		msg = gsub(msg, "(@attacker@)", a_name)
		msg = gsub(msg, "(@Target@)", strcap(t_name))
		msg = gsub(msg, "(@target@)", t_name)
		msg = gsub(msg, "(@attacker_possesive@)", a_possesive)
		msg = gsub(msg, "(@target_possesive@)", t_possesive)
		msg = gsub(msg, "(@isare@)", isare)
		msg = gsub(msg, "(@s@)", sss)

		message(msg)
		if game_options.disturb_other or attacker == WHO_PLAYER then
			term.disturb(1, 0)
		end
	end

	local proj_flags = (PROJECT_JUMP | PROJECT_HIDE | PROJECT_STOP |
						PROJECT_KILL | PROJECT_NO_REFLECT | PROJECT_SILENT)

	proj_flags = proj_flags | aura.proj_flags

	project(target, aura.rad, y, x, rng.range(mindam, maxdam),
 			aura.dam_type, proj_flags)
end

function add_aura(t)
	assert(t.name, "Aura has no name")
	assert(t.dam_type,  t.name .. " has no damage type")
	assert(t.msg or (t.player_msg and t.monster_msg),
		   t.name .. " has no message")
	assert(not t.color or color[t.color],
		   "Unknown color '" .. t.color .. "'")


	t.is_aura = true

	t.proj_flags = t.proj_flags or 0
	t.rad        = t.rad        or 0

	t.monster_blow = function(attacker, target, effect, method, params,
							  special, mindam, maxdam)
						 if special.touched then
							 __do_aura(%t, attacker, target,
									   mindam, maxdam)
						 end
					 end

	t.weapon_blow = function(attacker, target, weapon, launcher,
							 damns, computed_dams, crit_mult, crit_div,
							 max_weap_dam, mindam, maxdam)
						if not launcher then
							__do_aura(%t, attacker, target,
									  mindam, maxdam)
						end
					end

	add_blow_response(t)

	if not t.color_desc and t.desc then
		if t.color then
			t.color_desc = "#" .. strchar(conv_color[color[t.color] + 1]) ..
				t.desc .. "#w"
		else
			t.color_desc = t.desc
		end
	end

	if not t.self_know and t.desc and t.type then
		if t.type == "radiate" then
			t.self_know = "You radiate " .. t.desc .. "."
		elseif t.type == "shield" then
			t.self_know = "You are envolvoped in a mystic shield which " ..
				t.desc .. " your foes."
		elseif t.type == "surround" then
			t.self_know = "You are surrounded by " .. t.desc .. "."
		elseif t.type == "cover" then
			t.self_know = "You are covered with " .. t.desc .. "."
		elseif t.type == "shroud" then
			t.self_know = "You are shrouded in " .. t.desc .. "."
		else
			print("Unknown type '" .. t.type .. "'")
		end
	end

	if not t.memory and t.desc and t.type then
		if t.color then
			t.memory = {t.type, "<" .. t.color .. " " .. t.desc .. ">"}
		else
			t.memory = {t.type, t.desc}
		end
	end

	if t.memory then
		local flags = memory.flagdesc_add{
			{FLAG_BLOW_RESPONSE, t.index, t.memory[2]}
		}

		tinsert(memory.flag_aura[t.memory[1]], flags[1])

		if not t.not_visible then
			memory.obviousflag_add({FLAG_BLOW_RESPONSE, t.index})
		end
	end

	return t.index
end

-----------------------

add_aura{
	name     = "AURA_FIRE",
	dam_type = dam.FIRE,
	msg      = "@Attacker@ @isare@ suddenly very hot!",
	type     = "surround",
	desc     = "flames",
	color    = "ORANGE",
}

add_aura{
	name     = "AURA_VAL_FIRE",
	dam_type = dam.VALINOREAN_FIRE,
	msg      = "@Attacker@ @isare@ suddenly very hot!",
	type     = "surround"
	desc     = "Valinorian flames",
	color    = "ORANGE",
}

add_aura{
	name     = "AURA_DARK_FIRE",
	dam_type = dam.DARK_FIRE,
	msg      = "@Attacker@ @isare@ suddenly very hot!",
	type     = "surround"
	desc     = "dark flames",
	color    = "LIGHT_DARK",
}

add_aura{
	name     = "AURA_SUN_FIRE",
	dam_type = dam.SUN_FIRE,
	msg      = "@Attacker@ @isare@ suddenly *VERY* hot!",
	type     = "surround"
	desc     = "sun fire",
	color    = "LIGHT_RED",
}

add_aura{
	name     = "AURA_ELEC",
	dam_type = dam.ELEC,
	msg      = "@Attacker@ get@s@ zapped!"
	type     = "surround"
	desc     = "electricity",
	color    = "LIGHT_BLUE",
}

add_aura{
	name     = "AURA_POIS",
	dam_type = dam.POIS,
	msg      = "@Attacker@ @isare@ smothered in noxious fumes!",
	type     = "surround"
	desc     = "noxious fumes",
	color    = "GREEN",
}

add_aura{
	name     = "AURA_COLD",
	dam_type = dam.COLD,
	msg      = "@Attacker@ @isare@ suddenly very cold!",
	type     = "cover"
	desc     = "frost",
	color    = "BLUE",
}

add_aura{
	name     = "AURA_ACID",
	dam_type = dam.ACID,
	msg      = "@Attacker@ @isare@ splashed with acid!",
	type     = "cover"
	desc     = "acid",
	color    = "LIGHT_GREEN",
}

add_aura{
	name     = "AURA_SHARDS",
	dam_type = dam.SHARDS,
	msg      = "@Attacker@ @isare@ slashed by shards!",
	type     = "cover"
	desc     = "shards",
	color    = "UMBER",
}

---

add_aura{
	name     = "AURA_SLASH",
	dam_type = dam.SLASH,
	msg      = "@Attacker@ @isare@ slashed by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "slashes",
	color    = "YELLOW",

	not_visible = true
}

add_aura{
	name     = "AURA_CRUSH",
	dam_type = dam.CRUSH,
	msg      = "@Attacker@ @isare@ crushed by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "crushes",
	color    = "YELLOW",

	not_visible = true
}

add_aura{
	name     = "AURA_PIERCE",
	dam_type = dam.PIERCE,
	msg      = "@Attacker@ @isare@ pierced by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "pierces",
	color    = "YELLOW",

	not_visible = true
}

add_aura{
	name     = "AURA_CUT",
	dam_type = dam.CUT,
	msg      = "@Attacker@ @isare@ cut by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "cuts",
	color    = "YELLOW",

	not_visible = true
}

add_aura{
	name     = "AURA_STUN",
	dam_type = dam.STUN,
	msg      = "@Attacker@ @isare@ stunned by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "stuns",
	color    = "YELLOW",

	not_visible = true
}

add_aura{
	name     = "AURA_FEAR",
	dam_type = dam.FEAR,
	msg      = "@Attacker@ @isare@ frightened by @target_possesive@ " ..
		"mystic shield!",
	type     = "shield"
	desc     = "frightens",
	color    = "SLATE",
}

-----------------------------

function memory.aura(r_ptr,entry)
	local str = memory.flags2str(r_ptr, entry, memory.flag_aura.surround,
								 RT_MISC, memory.prop2)
	if str ~= "" then str = " @It@ is surrounded by"..str.."." end

	local str2 = memory.flags2str(r_ptr, entry, memory.flag_aura.cover,
								  RT_MISC, memory.prop2)
	if str2 ~= "" then str2 = " @It@ is covered with"..str2.."." end

	local str3 = memory.flags2str(r_ptr, entry, memory.flag_aura.radiate,
								  RT_MISC, memory.prop2)
	if str3 ~= "" then str3 = " @It@ radiates"..str3.."." end

	local str4 = memory.flags2str(r_ptr, entry, memory.flag_aura.shroud,
								  RT_MISC, memory.prop2)
	if str4 ~= "" then str4 = " @It@ is shrouded in"..str4.."." end

	local str5 = memory.flags2str(r_ptr, entry, memory.flag_aura.shield,
								  RT_MISC, memory.prop2)
	if str5 ~= "" then
		str5 = " @It@ is enveloped in a mystic shield which"..str5..
			" @its@ foes."
	end

	return str .. str2 .. str3 .. str4 .. str5
end

-- In monster memory, display aura info after exp info
memory.entrydesc_add_after("aura", memory.aura, "exp")

---------------------------

-- Describe objects which grant auras
function desc_obj.desc_auras(obj, to_file, trim_down)
	if not known_flag(obj, FLAG_BLOW_RESPONSE) then
		return
	end

	local responses = obj.flags[FLAG_BLOW_RESPONSE]

	local aura_list = {}

	for i = 1, flag_max_key(responses) do
		if responses[i] then
			local aura = get_blow_response(i)
			if flag_is_known(responses, i) and (aura.is_aura and aura.type and
												aura.color_desc) then
				aura_list[aura.type] = aura_list[aura.type] or {}
				tinsert(aura_list[aura.type], aura.color_desc)
			end
		end
	end

	if aura_list.surround then
		desc_obj.text_out("It surrounds you with " ..
					  strjoin(aura_list.surround, ", ", " and ") .. ". ")
	end

	if aura_list.cover then
		desc_obj.text_out("It covers you in " ..
					  strjoin(aura_list.cover, ", ", " and ") .. ". ")
	end

	if aura_list.shroud then
		desc_obj.text_out("It shrouds you in " ..
					  strjoin(aura_list.shroud, ", ", " and ") .. ". ")
	end

	if aura_list.radiate then
		desc_obj.text_out("It makes your radiate " ..
					  strjoin(aura_list.radiate, ", ", " and ") .. ". ")
	end

	if aura_list.shield then
		desc_obj.text_out("It envelops you in a mystic shield which " ..
					  strjoin(aura_list.shield, ", ", " and ") ..
						  " your foes. ")
	end
end -- desc_obj.desc_auras

-- Allow a quiver when the player paid for it
hook(hook.BODY_PARTS, function()
	if has_ability(AB_QUIVER) then
		player.add_body_part(INVEN_QUIVER, 1)
	end
end)
