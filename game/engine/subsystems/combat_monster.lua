-- The monster combat subsystem
--
-- Load it with: load_subsystem("combat_monster")
--
-- Most module authors wont load this module as it it automatically included
-- by other monster combat subsystems. One wants to include it only when they are creating
-- a completly new monster combat subsystem.

constant("combat_monster", {})

-- Combat system
safe_new_flag("MONSTER_COMBAT_SYSTEM")
hook.new_hook_type("HANDLE_MONSTER_BLOW")

combat_monster.__list = {}
combat_monster.__default = 1

function combat_monster.register(t)
	assert(t.name, "No monster combat name")
	assert(t.energy, "No monster combat energy")
	assert(t.attack, "No monster combat attack")

	tinsert(combat_monster.__list, t)
	t.__index__ = getn(combat_monster.__list)
	return t.__index__
end

function combat_monster.desc_target(t_ptr)
	return (t_ptr == player) and "you" or monster_desc(t_ptr, 0)
end

function combat_monster.desc_self(m_ptr)
	local m_name = monster_desc(m_ptr, 0)
	if combat_monster.__force_name then m_name = force_name end
	return m_name
end

function combat_monster.action_message(m_ptr, m_name, t_ptr, t_name, act)
	if act then
		act = gsub(act, "(@target@)", t_name)
		if t_ptr == player then
			act = gsub(act, "(@target_possesive@)", "your")
		else
			act = gsub(act, "(@target_possesive@)", t_name)
		end

		monster_player_msg(strcap(m_name).." "..act..".", (t_ptr == player) or combat_monster.__force_name, m_ptr.ml and t_ptr.ml)
	end
end

function combat_monster.init_combat_turn(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags)
	-- Not allowed to attack
	if (has_flag(m_ptr, FLAG_NEVER_BLOW)) then return false end

	-- Some extra cehcks for player targets
	if t_ptr == player then
		-- ...nor if friendly
	        if is_friend(m_ptr) >= 0 then
                	if player.control == m_idx then swap_position(m_ptr.fy, m_ptr.fx) end
        	        return false
	        end
	end

	if t_ptr == m_ptr then -- Paranoia
		if wizard then message(color.VIOLET, "Monster attacking self? "..m_idx) end
		return false
	end

	return true
end

-- The main attack function called by the C engine
monster_attack = function(m_idx, y, x, force_name)
	local c_ptr = cave(y, x)
	local m_ptr = monster(m_idx)
	local r_ptr = race_info(m_ptr)
	local t_idx = c_ptr.m_idx

	local t_ptr = player
	local tr_ptr = nil

	-- Get the monster if it is a monster
	if c_ptr.m_idx > 0 then
		t_ptr = monster(t_idx)
		tr_ptr = race_info(t_ptr)
	end

	local t_flags = t_ptr.intrinsic_flags or t_ptr.flags

	combat_monster.__force_name = force_name

	-- Call the real subsystem for that monster, or the default one
	local combat_type = m_ptr.flags[FLAG_MONSTER_COMBAT_SYSTEM] or combat_monster.__default
	return combat_monster.__list[combat_type].attack(y, x, c_ptr, m_idx, m_ptr, t_idx, t_ptr, t_flags)
end
