-- Define all ToME's timed effects

-- When cut, people start to bleed, bleeding is bad for their health
-- it may lead to dangerous conditions, even death
timed_effect.create
{
	name = "CUT"
	desc = "Bleeding"
	type = "physical"
	status = "detrimental"
	parameters = { {"power",0} }
	redraw = FLAG_PR_CUT
	on_gain = "You start to bleed."
	on_lose = "You stop bleeding."
	on_timeout = function()
		local power = min(40, max(1, timed_effect.get() / 10)) * timed_effect.get("power")
		take_hit(power, "bleeding")
	end
}

-- Poison is nasty,
-- it may lead to dangerous conditions, even death
timed_effect.create
{
	name = "POISON"
	desc = "Poisoned"
	type = "physical"
	status = "detrimental"
	parameters = { {"power",0} }
	redraw = FLAG_PR_POISONED
	on_gain = "You are poisoned!"
	on_lose = "You are no longer poisoned."
	on_timeout = function()
		local power = min(40, max(1, timed_effect.get() / 10)) * timed_effect.get("power")
		take_hit(power, "poison")
	end
}

-- Stuns make it harder to move around and to cast spells
timed_effect.create
{
	name = "STUN"
	desc = "Stunned"
	type = "physical"
	status = "detrimental"
	parameters = { {"power",0} }
	redraw = FLAG_PR_STUN
	on_gain = "You have been stunned."
	on_lose = "You are no longer stunned."
	on_timeout = function()
		-- Each turn we must recompute speed bonus
		-- This is slow, if this is too slow, other stun speed reducing
		-- technics will need to be found
		player.update = player.update | PU_BONUS
	end
	bonus = function()
		local power = min(20, max(1, timed_effect.get() / 10)) * timed_effect.get("power")
		player.inc_speed(SPEED_WALK, -power)
		player.inc_intrinsic(FLAG_SPELL_FAILURE, power)
	end
}

-- Blind make it harder to see things
timed_effect.create
{
	name = "BLIND"
	desc = "Blind"
	type = "physical"
	status = "detrimental"
	parameters = {}
	redraw = FLAG_PR_BLIND
	on_gain = "You are blind!"
	on_lose = "You can see again."
	bonus = function()
		player.set_intrinsic(FLAG_BLIND, true)
	end
}

-- Confused makes you .. confused
timed_effect.create
{
	name = "CONFUSED"
	desc = "Confused"
	type = "mental"
	status = "detrimental"
	parameters = {}
	redraw = FLAG_PR_CONFUSED
	on_gain = "You are confused!"
	on_lose = "You feel less confused now."
	bonus = function()
		player.set_intrinsic(FLAG_CONFUSED, true)
	end
}

-- Hallucinate makes you .. hallucinate
timed_effect.create
{
	name = "HALLUCINATE"
	desc = "Hallucinating"
	type = "mental"
	status = "detrimental"
	parameters = {}
	redraw = FLAG_PR_CONFUSED
	on_gain = "Oh, wow! Everything looks so cosmic now!"
	on_lose = "You can see clearly again."
	bonus = function()
		player.set_intrinsic(FLAG_HALLUCINATE, true)
		player.redraw[FLAG_PR_MAP] = 1
		player.window[FLAG_PW_M_LIST] = true
		player.window[FLAG_PW_OVERHEAD] = true
		player.update = player.update | PU_MONSTERS
	end
}

-- Afraid makes you scared
timed_effect.create
{
	name = "AFRAID"
	desc = "Scared"
	type = "mental"
	status = "detrimental"
	parameters = {}
	redraw = FLAG_PR_AFRAID
	on_gain = "You are terrified!"
	on_lose = "You feel bolder now."
	bonus = function()
		player.set_intrinsic(FLAG_FEAR, true)
	end
}

-- Paralyzed makes lose turns
timed_effect.create
{
	name = "PARALYZED"
	desc = "Paralyzed"
	type = "physical"
	status = "detrimental"
	parameters = {}
	redraw = FLAG_PR_STATE
	on_gain = "You are paralyzed!"
	on_lose = "You can move again."
	bonus = function()
		player.set_intrinsic(FLAG_PARALYZED, true)
	end
}

-- When invisibile, monsters have an harder time seeing you
-- But "wraith" like monsters(invisibile ones) see you easily
-- and you are more vulnerable to them
timed_effect.create
{
	name = "INVISIBILITY"
	desc = "Invisible"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your body fades away!"
	on_lose = "Your body is once again visible."
	bonus = function()
		player.inc_intrinsic(FLAG_INVIS, timed_effect.get("power"))
	end
}

-- Allows you to see invisible things
timed_effect.create
{
	name = "SEE_INVISIBILITY"
	desc = "Invisible detection"
	type = "magical"
	status = "beneficial"
	parameters = {}
	on_gain = "Your eyes seem much more perceptive."
	on_lose = "Your eyes get back to their normal sensory level."
	bonus = function()
		player.set_intrinsic(FLAG_SEE_INVIS, true)
	end
}

-- Speed up
timed_effect.create
{
	name = "FAST"
	desc = "Fast"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "You feel yourself moving faster!"
	on_lose = "You feel yourself slow down."
	bonus = function()
		player.inc_speed(timed_effect.get("power"))
	end
}

-- Speed down
timed_effect.create
{
	name = "SLOW"
	desc = "Slowed"
	type = "magical"
	status = "detrimental"
	parameters = { {"power",0} }
	on_gain = "You feel yourself moving slower!"
	on_lose = "You feel yourself speed up."
	bonus = function()
		player.inc_speed(-timed_effect.get("power"))
	end
}

-- Timed projection melee damage
timed_effect.create
{
	name = "MELEE_PROJECT"
	desc = "Imbue weapon"
	type = "magical"
	status = "beneficial"
	parameters = { {"type",0}, {"dam",0}, {"rad",0}, {"flags",0} }
	on_gain = "Your weapon starts glowing."
	on_lose = "Your weapon stops glowing."
}

-- Timed projection archery damage
timed_effect.create
{
	name = "ARCHERY_PROJECT"
	desc = "Imbue bow"
	type = "magical"
	status = "beneficial"
	parameters = { {"type",0}, {"dam",0}, {"rad",0}, {"flags",0} }
	on_gain = "Your missile thrower starts glowing."
	on_lose = "Your missile thrower stops glowing."
}

-- Probability travel
timed_effect.create
{
	name = "PROB_TRAVEL"
	desc = "Probability travel"
	type = "magical"
	status = "beneficial"
	parameters = {}
	on_gain = "Your body destabilizes!",
	on_lose = "Your body grows stable again.",
	bonus = function()
		player.set_intrinsic(FLAG_PROB_TRAVEL_UPDOWN)
		player.set_intrinsic(FLAG_PROB_TRAVEL_WALLS)
	end
}

-- Setup a damaging aura on the player
timed_effect.create
{
	name = "AURA"
	desc = "Aura"
	type = "magical"
	status = "beneficial"
	parameters = { {"type",0}, {"dice",0}, {"side", 1} }
	on_gain = "A magical aura erects around you!"
	on_lose = "The magical aura surrounding you collapses."
	bonus = function()
		local type = timed_effect.get("type")
		local dice = timed_effect.get("dice")
		local side = timed_effect.get("side")
		local blow_res = player.intrinsic_flags[FLAG_BLOW_RESPONSE]
		if not blow_res then
			blow_res = getter.array{[type] = {dice, side}}
		else
			blow_res[type] = {dice, side}
		end
		player.intrinsic_flags[FLAG_BLOW_RESPONSE] = blow_res
	end
}

-- Speed up
timed_effect.create
{
	name = "FLY"
	desc = "Fly"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "You feel like you can fly!"
	on_lose = "You no longer feel like you can fly."
	bonus = function()
		player.add_intrinsic_higher(FLAG_CAN_FLY, timed_effect.get("power"))
	end
}

-- Regen mana
timed_effect.create
{
	name = "LIFE_REGEN"
	desc = "Regeneration"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your body regeneration abilities greatly increase!"
	on_lose = "Your body regeneration abilities becomes normal again."
	bonus = function()
		player.inc_intrinsic(FLAG_REGEN_LIFE, timed_effect.get("power"))
	end
}

-- Strength
timed_effect.create
{
	name = "STRENGTH"
	desc = "Strength"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	redraw = FLAG_PR_STATS
	on_gain = "Your muscles grow more powerful!"
	on_lose = "Your muscles return to normal."
	bonus = function()
		player.modify_stat(A_STR, timed_effect.get("power"))
	end
}

-- Infravision
timed_effect.create
{
	name = "INFRAVISION"
	desc = "Infra-vision"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your eyes tingle softly."
	on_lose = "Your eyes stop tingling."
	bonus = function()
		player.see_infra = player.see_infra + timed_effect.get("power") * 10
	end
}

-- Regen mana
timed_effect.create
{
	name = "MANA_REGEN"
	desc = "Manaflow"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your body pulses with power!"
	on_lose = "Your body no longer pulses with power."
	bonus = function()
		player.inc_intrinsic(FLAG_REGEN_MANA, timed_effect.get("power"))
	end
}

-- Increase AC
timed_effect.create
{
	name = "ARMOR"
	desc = "Magical Armour(Deflect)"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your skin turns to stone!"
	on_lose = "Your skin reverts to normal state."
	bonus = function()
		player.to_a = player.to_a + timed_effect.get("power")
		player.dis_to_a = player.dis_to_a + timed_effect.get("power")
	end
}
timed_effect.create
{
	name = "ARMOR_DR"
	desc = "Magical Armour(Resist)"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your skin turns to stone!"
	on_lose = "Your skin reverts to normal state."
	bonus = function()
		player.resists[dam.CRUSH] = (player.resists[dam.CRUSH] or 0) + timed_effect.get("power")
		player.resists[dam.PIERCE] = (player.resists[dam.PIERCE] or 0) + timed_effect.get("power")
		player.resists[dam.SLASH] = (player.resists[dam.SLASH] or 0) + timed_effect.get("power")
	end
}
timed_effect.create
{
	name = "ARMOR_DA"
	desc = "Magical Armour(Absorb)"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your skin turns to stone!"
	on_lose = "Your skin reverts to normal state."
	bonus = function()
		player.absorbs[dam.CRUSH] = (player.absorbs[dam.CRUSH] or 0) + timed_effect.get("power")
		player.absorbs[dam.PIERCE] = (player.absorbs[dam.PIERCE] or 0) + timed_effect.get("power")
		player.absorbs[dam.SLASH] = (player.absorbs[dam.SLASH] or 0) + timed_effect.get("power")
	end
}

-- Absorb souls into health
timed_effect.create
{
	name = "ABSORB_SOUL"
	desc = "Absorb Soul"
	type = "magical"
	status = "beneficial"
	parameters = { {"chance",0} }
	on_gain = "#DYou start absorbing souls of your foes."
	on_lose = "#DYou stop absorbing souls of dead foes."
}

-- Takes hits on mana instead of HP
timed_effect.create
{
	name = "MANA_SHIELD"
	desc = "Mana Shield"
	type = "magical"
	status = "beneficial"
	parameters = { {"ratio",0} }
	on_gain = "A protective shield of mana forms around you."
	on_lose = "The protective shield of mana around you collapses."
}

-- Handle manashield
hook
{
	[hook.PLAYER_TAKE_HIT] = function(value, modif)
		if timed_effect.get(timed_effect.MANA_SHIELD) then
			modif = -(modif * timed_effect.get(timed_effect.MANA_SHIELD, "ratio")) / 100
			if player.csp() > modif then
				increase_mana(-modif)
				modif = 0
			else
				modif = modif - player.csp()
				player.csp(0)
				timed_effect(timed_effect.MANA_SHIELD, 0)
			end
			return true, -modif
		end
	end
	[hook.SPELL_CAST_PRE] = function()
		if current_spell_info.faction == WHO_PLAYER and timed_effect.get(timed_effect.MANA_SHIELD) then
			timed_effect(timed_effect.MANA_SHIELD, 0)
		end
	end
}


timed_effect.create
{
	name = "CONFUSING_TOUCH"
	desc = "Confusing Touch"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Your hands take on a glow."
	on_lose = "Your hands stop glowing."
	bonus = function()
		player.inc_intrinsic(FLAG_MELEE_CONFUSE, timed_effect.get("power"))
	end
}

timed_effect.create
{
	name = "VALARIN_FAVOR"
	desc = "Valarin Favor"
	type = "magical"
	status = "beneficial"
	parameters = { {"level",1} }
	on_gain = "Your apply to the Valar for their favor."
	on_lose = "Your application expires."
	bonus = function()
		hook.process(hook.VALARIN_FAVOR, timed_effect.get("level"))
	end
}

timed_effect.create
{
	name = "HEROISM"
	desc = "Heroism"
	type = "magical"
	status = "beneficial"
	parameters = { {"level",1} }
	on_gain = function()
		message("You feel like a hero!")
		timed_effect.on_lose(timed_effect.AFRAID)
		player.hp_mod = player.hp_mod + 10
		player.calc_hitpoints()
    end
	on_lose = function()
		message("Your heroism wears off.")
		player.hp_mod = player.hp_mod - 10
		player.calc_hitpoints()
	end
	bonus = function()
		timed_effect.on_lose(timed_effect.AFRAID)
		player.resists[dam.FEAR] = 100
		player.to_h     = player.to_h     + 10
		player.dis_to_h = player.dis_to_h + 10
	end
}

timed_effect.create
{
	name = "PROTECTION_FROM_EVIL"
	desc = "Protection from evil"
	type = "magical"
	status = "beneficial"
	parameters = { }
	on_gain = "You feel protected from evil."
	on_lose = "You no longer feel protected from evil."
}

-- Handle protectection from evil
hook(hook.HANDLE_MONSTER_BLOW, function(params)
	if params.t_ptr == player and timed_effect.get(timed_effect.PROTECTION_FROM_EVIL) and has_flag(params.m_ptr, FLAG_EVIL) then
		if (params.m_ptr.level < player.lev or rng(0, params.m_ptr < player.lev) == 0) and rng.percent(50) then
			message(strcap(params.m_name).." attack is deflected.")
			return true
		end
	end
	return false
end)

-- Time stop, decreases all energy used by a factor
timed_effect.create
{
	name = "TIME_STOP"
	desc = "Timeweave Folding"
	type = "magical"
	status = "beneficial"
	parameters = { {"power",0} }
	on_gain = "Everything seems to freeze around you!"
	on_lose = "The flow of time returns to normal."
}

-- Handle time stop
hook
{
	[hook.PLAYER_USED_ENERGY] = function()
		if timed_effect.get(timed_effect.TIME_STOP) then
			energy_use = energy_use / timed_effect.get(timed_effect.TIME_STOP, "power")
			if energy_use < 1 then energy_use = 1 end
			-- Since we will only pass one turn every 1000 actions when under this effect
			-- we instead decrease the counter manually
			timed_effect.inc(timed_effect.TIME_STOP, -1)
		end
	end
}

-- When diseased, people start to suffer, suffering is bad for their health
-- it may lead to dangerous conditions, even death
timed_effect.create
{
	name = "DISEASE"
	desc = "Disease"
	type = "magical"
	status = "detrimental"
	parameters = { {"power",0} }
	on_gain = "You get very sick."
	on_lose = "You are no longer sick."
	on_timeout = function()
		local time = timed_effect.get()
		local power = timed_effect.get("power")
		if time > 1 then power = timed_effect.get("power") / time end
		take_hit(power, "disease")
		timed_effect.set_param("power", timed_effect.get("power") - power)
	end
}

timed_effect.create
{
	name = "ESP"
	desc = "Esp"
	type = "magical"
	status = "beneficial"
	parameters = { {"flags", {} } }
	on_gain = "You feel your consciousness expand!"
	on_lose = "Your consciousness contracts again."
	bonus = function()
		local flags = getter.flags(timed_effect.get("flags"))
		player.add_intrinsic_higher(FLAG_ESP, flags)
	end
}

-- Randomly projects stuff at monsters
-- It really needs a better name
timed_effect.create
{
	name = "RANDOMPROJECT"
	desc = "Magical Storm"
	type = "magical"
	status = "beneficial"
	parameters = { {"type",0}, {"dice",0}, {"side",0}, {"rad",0}, 
	               {"flags", PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID |
				             PROJECT_HIDE | PROJECT_JUMP} }
	on_gain = "The air charges up around you."
	on_lose = "The air is no longer charged."
	on_timeout = function()
		local type = timed_effect.get("type")
		local dice = timed_effect.get("dice")
		local side = timed_effect.get("side")
		local rad = timed_effect.get("rad")
		local flags = timed_effect.get("flags")
		
		-- Scan for monsters in LOS, and select one at random
		local counter = 0
		local selection = nil
		for iter = 1, monst_list.size do
    		if (monst_list.node[iter].flags & FLAG_FLAG_USED) ~= 0 then
        		local m_ptr = monst_list[monst_list.node[iter].key]
				-- A friendliness test could be added here to avoid hitting
				-- neutral or allied monsters
				if m_ptr.ml then
					counter = counter + 1
					if rng(1, counter) == 1 then
						selection = m_ptr
					end
				end
    		end
		end

		-- If we got one, blast him!
		if selection then
			local dam = rng.roll(dice,side)
			local ty, tx = selection.fy, selection.fx
			local m_name = monster_desc(selection, 0)
			local dam_desc = strcap(get_dam_type_info(type, "desc"))

			message(dam_desc.." hits "..m_name)
			project(WHO_PLAYER, rad, ty, tx, dam, type, flags)
		end
	end
}

