-- handle the meta school

declare_global_constants {
	"DISPERSEMAGIC",
	"RECHARGE",
	"SPELLBINDER",
	"TRACKER",
	"TIMER_INERTIA_CONTROL",
	"INERTIA_CONTROL",
	"MANAFLOW",
	"get_spellbinder_max",
	"spellbinder_is_ok_spell",
}

RECHARGE = add_spell
{
	["name"] = 	"Recharge",
	["school"] = 	{SCHOOL_META},
	["level"] = 	9,
	["mana"] = 	10,
	["mana_max"] = 	100,
	["fail"] = 	20,
	["spell"] = 	function()
			message("IMPLEMENT RECHARGE")
			return recharge(60 + get_cast_level(140))
	end,
	["info"] = 	function()
			return "power "..(60 + get_cast_level(140))
	end,
	["desc"] =	{
			"Taps on the ambient mana to recharge an object's power",
	}
}

function get_spellbinder_max()
	local i

	i = get_level(SPELLBINDER, 4)
	if i > 4 then i = 4 end
	return i
end

SPELLBINDER = add_spell
{
	["name"] = 	"Spellbinder",
	["school"] = 	{SCHOOL_META},
	["level"] = 	39,
	["mana"] = 	100,
	["mana_max"] = 	300,
	["fail"] = 	85,
	["spell"] = 	function()
			local i, ret, c

			if player.spellbinder.active then
				message("The spellbinder is already active.")
				message("It will trigger at "..player.spellbinder.trigger.."% HP.")
				message("With the spells: ")
				for i = 1, getn(player.spellbinder.spells) do
					message(spell(player.spellbinder.spells[i]).name)
				end

				if get_check("Replace these bindings? ") then
					player.spellbinder.active = false
				else
					return true
				end
			end

			ret, c = get_com("Trigger at [a]75% hp [b]50% hp [c]25% hp?", strbyte("a"))
			if not ret then return SPELL_NOTHING end

			if c == strbyte("a") then
				player.spellbinder.trigger = 75
			elseif c == strbyte("b") then
				player.spellbinder.trigger = 50
			elseif c == strbyte("c") then
				player.spellbinder.trigger = 25
			else
				return true
			end

			local max = get_spellbinder_max()
			player.spellbinder.spells = {}
			for i = 1, max do
				local s

				s = get_school_spell("bind", "spellbinder_is_ok_spell")
				if s == -1 then
					player.spellbinder.active = false
					return true
				end
				tinsert(player.spellbinder.spells, s)
			end
			player.energy = player.energy - (31 * get_player_energy(SPEED_CAST));
			player.spellbinder.active = true
			message("Spellbinder ready.")
			return true
	end,
	["info"] = 	function()
			return "number "..(get_spellbinder_max()).." max level "..(7 + get_cast_level(35))
	end,
	["desc"] =	{
			"Stores spells in a trigger.",
			"When the condition is met all spells fire off at the same time",
			"This spell takes a long time to cast so you are advised to prepare it",
			"in a safe area.",
			"Also it will use the mana for the Spellbinder and the mana for the",
			"selected spells"
	}
}

-- Spellbinder stuff
player.spellbinder =
{
	active = false
	spells = {}
	trigger = 25
}
add_loadsave("player.spellbinder", {})
hook(hook.BIRTH, function() player.spellbinder.active = false end)

hook(hook.PLAYER_TAKE_HIT, function(value, modif)
	if not player.spellbinder.active then return end
	local new_hp = value + modif
	local perc = new_hp * 100 / player.mhp()
	if perc < player.spellbinder.trigger then
		message("Spellbinder triggered!")
		for i = 1, getn(player.spellbinder.spells) do
			local spl = player.spellbinder.spells[i]
			cast_school_spell(spl, spell(spl), true)
		end
		player.spellbinder.active = false
	end
end)

function spellbinder_is_ok_spell(s, obj)
	if not is_ok_spell(s, obj) then return false end
	return spell(s).skill_level < 7 + get_level(SPELLBINDER, 35)
end


DISPERSEMAGIC = add_spell
{
	["name"] = 	"Disperse Magic",
	["school"] = 	{SCHOOL_META},
	["level"] = 	29,
	["mana"] = 	30,
	["mana_max"] = 	60,
	["fail"] = 	40,
	-- Unaffected by blindness
	["blind"] =     FALSE,
	-- Unaffected by confusion
	["confusion"] = FALSE,
	["stick"] =
	{
			["charge"] =    { 5, 5 },
			[TV_ORB] =
			{
				["rarity"] = 		25,
				["base_level"] =	{ 1, 15 },
				["max_level"] =		{ 5, 40 },
			},
	},
	["inertia"] = 	{ 1, 5 },
	["spell"] = 	function()
			local nb = get_cast_level(5)
			for i = 1, nb do
				local effect = timed_effect.get_random(iif(rng.percent(50 + get_cast_level(25)), "detrimental", "beneficial"), "all")
				if effect then
					timed_effect.set(effect, 0)
				end
			end
			return true
	end,
	["info"] = 	function()
			return get_cast_level(5).." effects"
	end,
	["desc"] =	{
			"Dispels a lot of magic that can affect you, be it good or bad",
	}
}

TRACKER = add_spell
{
	["name"] = 	"Tracker",
	["school"] = 	{SCHOOL_META, SCHOOL_CONVEYANCE},
	["level"] = 	59,
	["mana"] = 	50,
	["mana_max"] = 	50,
	["fail"] = 	95,
	["spell"] = 	function()
			if last_teleportation_y == -1 then
				message("There has not been any teleportation here.")
				return true
			end
			teleport_player_to(last_teleportation_y, last_teleportation_x)
			return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Tracks down the last teleportation that happened on the level and teleports",
			"you to it",
	}
}

-- Loadsaves for the TIMER
time.save_timer("TIMER_INERTIA_CONTROL")
add_loadsave("player.inertia_controlled_spell", -1)
hook(hook.BIRTH, function() player.inertia_controlled_spell = -1 end)
player.inertia_controlled_spell = -1

-- Automatically cast the inertia controlled spells
TIMER_INERTIA_CONTROL = time.new_timer
{
	["delay"] = 	10,
	["enabled"] = 	nil,
	["callback"] = 	function()
			if player.inertia_controlled_spell ~= -1 then
				__spell_spell[player.inertia_controlled_spell]()
			end
	end,
}

hook(hook.CALC_MANA,
	function()
		if player.inertia_controlled_spell ~= -1 then
			player.msp(player.msp() - (get_mana(player.inertia_controlled_spell) * 4))
			if player.msp() < 0 then player.msp(0) end
		end
	end
)

INERTIA_CONTROL = add_spell
{
	["name"] = 	"Inertia Control",
	["school"] = 	{SCHOOL_META},
	["level"] = 	73,
	["mana"] = 	300,
	["mana_max"] = 	700,
	["fail"] = 	95,
	["spell"] = 	function()
			if player.inertia_controlled_spell ~= -1 then
				message("You cancel your inertia flow control.")
				player.inertia_controlled_spell = -1
				TIMER_INERTIA_CONTROL.enabled = nil
				player.update = player.update | PU_MANA
				return true
			end

			local s = get_school_spell("control", "is_ok_spell")
			if s == -1 then
				player.inertia_controlled_spell = -1
				TIMER_INERTIA_CONTROL.enabled = nil
				player.update = player.update | PU_MANA
				return true
			end

			local inertia = __tmp_spells[s].inertia

			if inertia == nil then
				message("This spell inertia flow can not be controlled.")
				player.inertia_controlled_spell = -1
				TIMER_INERTIA_CONTROL.enabled = nil
				player.update = player.update | PU_MANA
				return true
			end
			if inertia[1] > get_cast_level(10) then
				message("This spell inertia flow("..inertia[1]..") is too strong to be controlled by your current spell.")
				player.inertia_controlled_spell = -1
				TIMER_INERTIA_CONTROL.enabled = nil
				player.update = player.update | PU_MANA
				return true
			end

			player.inertia_controlled_spell = s
			TIMER_INERTIA_CONTROL.enabled = true
			TIMER_INERTIA_CONTROL.delay = inertia[2]
			TIMER_INERTIA_CONTROL.countdown = TIMER_INERTIA_CONTROL.delay
			player.update = player.update | PU_MANA
			message("Inertia flow controlling spell "..spell(s).name..".")
			return true
	end,
	["info"] = 	function()
			return "level "..get_cast_level(10)
	end,
	["desc"] =	{
			"Changes the energy flow of a spell to be continuously recasted",
			"at a given interval. The inertia controlled spell reduces your",
			"maximum mana by four times its cost.",
	}
}

MANAFLOW = add_spell
{
	["name"] = 	"Manaflow",
	["school"] = 	{SCHOOL_META},
	["level"] = 	99,
	["mana"] = 	10,
	["mana_max"] = 	10,
	["fail"] = 	100,
	["cooldown"] = 	400,
	["action"] =    "@Source@ is engulfed in a storm of pure mana.",
	tactical =
	{
		[ai.tactics.MANA] = 8
		[ai.tactics.NO_LOS] = true
	}
	["spell"] = 	function()
			if spell_caster == WHO_PLAYER then
				timed_effect(timed_effect.MANA_REGEN, 10, ((80 + get_cast_level(80)) * player.msp()))
				timed_effect(timed_effect.PARALYZED, 5)
			else
				local monst = monster(spell_caster)
				monst.flags[FLAG_MANA_CURR] = monst.flags[FLAG_MANA_MAX]
				monst.energy = monst.energy - (get_monster_energy(monst, SPEED_CAST) * 5)
			end
			return true
	end,
	["info"] = 	function()
			return ((player.msp() * (80 + get_cast_level(80)) / 100)) .. "mana/10turns"
	end,
	["desc"] =	{
			"Taps on the ambient mana to increase your own mana regeneration rate.",
			"For 10 turns your mana regeneration is greatly increased but the power",
			"of the forces at work will paralyze you for the first 5 turns.",
			"It is greatly advised to cast this spell while away from immediate danger.",
	}
}
