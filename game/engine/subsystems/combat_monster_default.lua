-- The default monster combat subsystem
--
-- Load it with: load_subsystem("combat_monster_default")
--
-- This is the "default" monster combat system as it exists for ToME(a derivate of angband one)
--
-- Parameters:
-- * on_hit: function which will be called when a blow successfully landed (default to nothing)

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat_monster")

-- Make the namespace
combat_monster.default = {}

combat_monster.__project_flags = 0

-- Extra stuff to do when a weapon hits
combat_monster.default.on_hit = get_subsystem_param("combat_monster_default", "on_hit") or function() return false end

-- Determine if a monster attack against the player succeeds.
-- Always miss 5% of the time, Always hit 5% of the time.
-- Otherwise, match monster power against player armor.
combat_monster.default.check_hit = get_subsystem_param("combat_monster_default", "test_hit") or
function(power, level, ac, luck)
	local i, k

	-- Percentile dice
	k = rng.number(100)

	-- Hack -- Always miss or hit
	if (k < 10) then
		if k < 5 then
			return true
		else
			return FALSE
		end
	end

	-- Calculate the "attack quality"
	i = (power + (level * 3))

	-- Power and Level compete against Armor
	if ((i > 0) and (rng(i - luck) > ((ac * 3) / 4))) then return (true) end

	-- Assume miss
	return (FALSE);
end

-- Critical blow.  All hits that do 95% of total possible damage,
-- and which also do at least 20 damage, or, sometimes, N damage.
-- This is used only to determine "cuts" and "stuns" and such.
combat_monster.default.critical_hit = get_subsystem_param("combat_monster_default", "critical_hit") or
function(dice, sides, dam)
	local max = 0
	local total = dice * sides

	-- Must do at least 95% of perfect
	if (dam < (total * 19) / 20) then return (0) end

	-- Weak blows rarely work
	if ((dam < 20) and (rng.number(100) >= dam)) then return (0) end

	-- Perfect damage
	if (dam == total) then max = max + 1 end

	-- Super-charge
	if (dam >= 20) then
		while (rng.number(100) < 2) do
			max = max + 1
		end
	end

	-- Critical damage
	if (dam > 45) then return (6 + max) end
	if (dam > 33) then return (5 + max) end
	if (dam > 25) then return (4 + max) end
	if (dam > 18) then return (3 + max) end
	if (dam > 11) then return (2 + max) end
	return (1 + max)
end

-- Dead monster explodes
function combat_monster.default.explode(m_idx)
	local monst = monster(m_idx)

	-- Scan through all four blows
	if (flag_exists(monst.blow,0)) then
		for ap_cnt = 0, flag_max_key(monst.blow) do
			local blow = flag_get_blow(monst.blow, ap_cnt)

			-- Extract the attack infomation
			local effect_idx = blow.effect
			local method_idx = blow.method

			-- no more attacks
			if (method_idx == 0) then break end

			local effect = __monster_attack_types[effect_idx]
			local method = __monster_attack_methods[method_idx]

			local d_dice = blow.d_dice
			local d_side = blow.d_side

			if method.explode then
				project(m_idx, 3, monst.fy, monst.fx,
						rng.roll(d_dice, d_side), effect.type,
						PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL)
			end
		end
	end
end
monster_explode = combat_monster.default.explode

-- Define the monster combat itself
combat_monster.default.COMBAT = combat_monster.register
{
	name = "Default monster combat"
	energy	= function() return get_player_energy(SPEED_WEAPON) end

	attack = function(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags)
		combat_monster.__exploded = false

		-- Not allowed to attack
		if not combat_monster.init_combat_turn(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags) then return false end

		-- Total armor
		local ac
		if t_ptr ~= player then
			ac = t_ptr.ac
		else
			ac = player.ac + player.to_a
		end

		-- Extract the effective monster level
		local rlev = m_ptr.level
		if rlev < 1 then rlev = 1 end

		-- Get the monster name (or "it")
		local m_name = combat_monster.desc_self(m_ptr)
		local t_name = combat_monster.desc_target(t_ptr)

		-- Assume no blink
		local blinked = FALSE

		-- Parameters
		local params = { ac = ac }

		-- Scan through all blows
		for ap_cnt = 0, flag_max_key(m_ptr.blow) do
			-- Special effects
			local special = {}

			local visible = false
			local obvious = false

			local power = 0
			params.dam = 0

			local act = ""

			-- Extract the attack infomation
			local blow = flag_get_blow(m_ptr.blow, ap_cnt)
			if not blow then break end
			local effect_idx = blow.effect
			local method_idx = blow.method

			-- no more attacks
			if (method_idx == 0) then break end

			local effect = __monster_attack_types[effect_idx]
			local method = __monster_attack_methods[method_idx]

			local d_dice = blow.d_dice
			local d_side = blow.d_side

			-- Extract visibility (before blink)
			if (m_ptr.ml) then visible = true end

			-- Extract the attack "power"
			local power = effect.power

			-- Calculate luck for players only
			local l = 0
			if t_ptr == player then l = luck(-10, 10) end

			params.m_ptr = m_ptr
			params.m_name = m_name
			params.effect = effect
			params.method = method
			params.t_ptr = t_ptr
			params.t_name = t_name
			params.t_flags = t_flags
			params.t_luck = l
			local done = hook.process(hook.HANDLE_MONSTER_BLOW, params)

			-- Monster hits
			if (not done) and (combat_monster.__force_hit or (effect == 0) or
							   (combat_monster.default.check_hit(power, rlev,
																 ac, l) ==true))
			then
				-- Always disturbing
				if game_options.disturb_other and
					(m_ptr.ml or t_ptr == player or t_ptr.ml)
				then
					term.disturb(1, 0)
				end

				-- Describe the attack method
				if method.fct then
					method.fct(special, effect, m_idx)
				else
					if t_ptr == player then
						method.player(special, effect, m_idx)
					else
						method.monster(special, effect, m_idx)
					end
				end

				-- Get the action
				if special.action then act = special.action else act = method.action end

				-- Message
				combat_monster.action_message(m_ptr, m_name, t_ptr, t_name, act)

				-- Roll out the damage
				params.dam = rng.roll(d_dice, d_side)

				-- Execute effect
				if effect.fct then
					effect.fct(params, special, method, m_idx)
				else
					if t_ptr == player then
						effect.player(params, special, method, m_idx)
					else
						effect.monster(params, special, method, m_idx)
					end
				end

				-- Get obviousness
				if special.obvious then obvious = special.obvious else obvious = effect.obvious end

				if (effect.type > 0) then
					-- Do damage if not exploding
					if not special.explode then
						project(m_idx, 0, y, x, params.dam, effect.type,
								PROJECT_KILL | PROJECT_STOP | PROJECT_HIDE |
								PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT |
								combat_monster.__project_flags)
					end
				end

				if (special.explode) then
					local hp
					-- Hehehhehe Evil me .. evil me !
					if t_ptr == player then hp = player.chp()
					else hp = t_ptr.hp end
					project(m_idx, 0, m_ptr.fy, m_ptr.fx, hp + 1, effect.type,
							PROJECT_KILL | PROJECT_STOP | PROJECT_HIDE |
							PROJECT_HIDE_BLAST | PROJECT_CASTER |
							PROJECT_NO_REFLECT |
							combat_monster.__project_flags)
					combat_monster.__exploded = true
					return true
				end

				-- Blow responses (passive attacks)
				if flag_exists(t_flags, FLAG_BLOW_RESPONSE) and
					not combat_monster.__no_response
				then
					local responses = t_flags[FLAG_BLOW_RESPONSE]

					for i = 1, flag_max_key(responses) do
						if responses[i] then
							local blow_response = get_blow_response(i)

							blow_response.monster_blow(m_idx, t_idx, effect,
													   method, params, special,
													   responses[i],
													   flag_get2(responses, i))
						end

						-- Blow response killed monster?
						if m_ptr.r_idx and m_ptr.r_idx == 0 then
							return
						end
					end
				end

				-- Blink away
				if special.blinked and not combat_monster.__no_blink then
					if (m_ptr.ml) then
						monster_msg(special.blinked)
					end

					teleport_away(m_idx, 20 * 2 + 5)
					break
				end

				-- Anything else ?
				if combat_monster.default.on_hit(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags, blow, params, special) then break end

			-- Monster missed player
			elseif not done then
				-- Visible monsters
				if (m_ptr.ml) then
					-- Disturbing
					term.disturb(1, 0)

					-- Message
					monster_player_msg(strcap(m_name).." misses "..t_name..".", (t_ptr == player) or combat_monster.__force_name, t_ptr.ml)
				end
			end

			-- Analyze "visible" monsters only
			if not done and visible == true and (t_ptr == player or t_ptr.ml) then
				local blowmem = deep_get{memory.get_entry(race_info(m_ptr),m_ptr.ego),RT_BLOWS}
				-- The blowmem array indexing start at 1
				if not blowmem[ap_cnt+1] then blowmem[ap_cnt+1] = 0 end
				-- Count "obvious" attacks (and ones that cause damage)
				if (obvious == true) or (params.dam > 0) or (blowmem[ap_cnt+1] > 10) then
					-- Count attacks of this type
					if (blowmem[ap_cnt+1] < 255) then
						blowmem[ap_cnt+1] = blowmem[ap_cnt+1] + 1
					end
				end
			end

			-- Don't keep hitting if the player is dead
			if (t_ptr == player) and (player.chp() < 0) then
				return
			end
		end
		return false
	end
}
