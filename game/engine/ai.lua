-- The AI support lib
-- Some basic AIs

constant("ai", {})

ai.__next = 0

ai.action =
{
	MOVE = 1
	MOVE_DIR = 2
	REST = 3
	OTHER = 4
}

function ai.new(t)
	assert(t.name, "No AI name")
	assert(t.exec, "No AI exec action")

	if t.state then
		for k, e in t.state do
			safe_new_flag(k)
		end
		t.state = grab_flags(t.state, flag_new(), nil)
	else
		t.state = flag_new(2)
	end

	t.index = ai.__next
	ai[t.index] = t
	ai[t.name] = t.index

	ai.__next = ai.__next + 1
end

function ai.new_state(idx)
	local state = flag_dup(ai[idx].state)
	return state
end

-- Make a new state and init it withthe default values
function ai.init(monst, idx)
	flag_add_new(monst.ai_states, ai[idx].state)
	if ai[idx] and ai[idx].init then ai[idx].init(monst, monst.ai_states) end
end

--- @fn
-- @brief Go to bed...
function ai.setup_sleep(monst)
	monst.ai_states[FLAG_SLEEP_NEXT_AI] = monst.ai
	set_monster_ai(monst, ai.SLEEP)
end

--- @fn
-- @brief Run an other AI as a sub-AI
function ai.exec(idx, m_idx, monst)
	ai[idx].exec(m_idx, monst, monst.ai_states)
end

--- @fn
-- @brief Is the target adjacent ?
function ai.adjacent(monst, y, x)
	local dy, dx = y - monst.fy, x - monst.fx
	return dy >= -1 and dy <= 1 and dx >= -1 and dx <= 1
end

--- @fn
-- @brief Returns the distance to the monster target
function ai.distance(m)
	if m.target > 0 then local m2 = monster(m.target) return distance(m.fy, m.fx, m2.fy, m2.fx)
	else return m.cdis end
end

--- @fn
-- @brief Returns the y, x coordinates of the monster target
function ai.target(m)
	if m.target > 0 and monster(m.target) then local m2 = monster(m.target) return m2.fy, m2.fx
	else return player.py, player.px end
end

--- @fn
-- @brief Returns true if we are enemy(neitehr friendly or neuter) to the target
function ai.target_enemy(m)
	if m.target > 0 and monster(m.target) then return is_enemy(m, monster(m.target))
	else return is_friend(m) < 0 end
end

--- @fn
-- @brief Loads an AI from either /scripts/ai/ or /engine/ai/
function ai.load(name)
	if fs.file_exist(path_build(TENGINE_DIR_SCPT, "ai/"..name..".lua")) then
		tome_dofile_anywhere(TENGINE_DIR_SCPT, "ai/"..name..".lua")
	elseif fs.file_exist(path_build(TENGINE_DIR_ENGINE, "ai/"..name..".lua")) then
		tome_dofile_anywhere(TENGINE_DIR_ENGINE, "ai/"..name..".lua")
	else
		quit("AI "..name.." not found!")
	end
end

---------------
-- Spellcasting

ai.cast = {}

ai.cast._cur = {}

--- @fn
-- @brief Prepares a spell for analysis and possible casting
ai.cast.use_spell = function(m_idx, monst, spl, y, x)
	ai.cast._cur.y = y
	ai.cast._cur.x = x
	ai.cast._cur.m_idx = m_idx
	ai.cast._cur.monst = monst
	ai.cast._cur.spell = spl
	ai.cast._cur.level = flag_get(monst.spells, spl)
	ai.cast._cur.fail = flag_get2(monst.spells, spl)
	set_auto_cast(m_idx, ai.cast._cur.level, y, x)
	ai.cast._cur.mana = get_mana(spl)
	unset_auto_cast()
end

ai.action.NO_CAST = 100
ai.action.NO_LOS = 101
ai.action.NO_MANA = 102
ai.action.NO_COUNT = 103

--- @fn
-- @brief Checks if the prepared spell can be cast right now, returns nil or error action
ai.cast.can = function(need_los)
	if need_los then
		set_current_spell_info{index = ai.cast._cur.spell, source = ai.cast._cur.m_idx faction = ai.cast._cur.monst.faction }
		project_range = get_cast_range()
		local ret = projectable(ai.cast._cur.monst.fy, ai.cast._cur.monst.fx, ai.cast._cur.y, ai.cast._cur.x)
		project_range = -1
		unset_current_spell_info()
		if not ret then return ai.action.NO_LOS end
	end
	if ai.cast._cur.mana > flag_get(ai.cast._cur.monst.flags, FLAG_MANA_CURR) then return ai.action.NO_MANA end
	local counts = flag_get_flags(ai.cast._cur.monst.flags, FLAG_LIMIT_SPELLS)
	if counts and flag_exists(counts, ai.cast._cur.spell) and flag_get(counts, ai.cast._cur.spell) <= 0 then return ai.action.NO_COUNT end
	return nil
end

--- @fn
-- @brief Attempt to cast the spell, and return the action taken
ai.cast.cast = function()
	if cast_monster_spell(ai.cast._cur.m_idx, ai.cast._cur.spell, ai.cast._cur.level, ai.cast._cur.fail, ai.cast._cur.y, ai.cast._cur.x) then
		ai.cast._cur.monst.ai_speed = SPEED_CAST
		flag_inc(ai.cast._cur.monst.flags, FLAG_MANA_CURR, -ai.cast._cur.mana)

		local counts = flag_get_flags(ai.cast._cur.monst.flags, FLAG_LIMIT_SPELLS)
		if counts and flag_exists(counts, ai.cast._cur.spell) then flag_inc(counts, ai.cast._cur.spell, -1) end

		return ai.action.OTHER
	else
		return ai.action.NO_CAST
	end
end

----------------
-- Do-nothing AI
ai.new
{
	name = "NONE"
	exec = function() end
}

-----------------
-- Load stock AIs
tome_dofile_anywhere("/engine/ai/", "init.lua")
