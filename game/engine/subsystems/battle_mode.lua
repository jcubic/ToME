-- The battle mode subsystem
--
-- Load it with: load_subsystem("battle_mode")
--
-- This subsystem adds the basic building blocks for making big scale battles with the engine
-- like a recreation of Helm's Deep, the Black Gate assault, the storming of Isengard by the Ents
-- and so on.
--
-- Basically it provides a custom AI for such battles, grouping monsters in "battle groups"
-- allowing each group to have orders and objectives, formations, ...

constant("battle_mode", {})

-- Tells which battle group a monster belongs to
new_flag("BATTLE_MODE_GROUP")
new_flag("BATTLE_MODE_GROUP_LEADER")

-- The orders that groups can follow
battle_mode.orders = {}
battle_mode.__next_order = 1

-- List of possible events that can happen in a battle
battle_mode.events =
{
	GROUP_MEMBER_DEATH	= 1
	GROUP_LEADER_DEATH	= 2
	GROUP_END		= 3
	GROUP_ORDER_DONE	= 4
	GROUP_MEMBER_HURT	= 5
}


-- The list of groups and their properties/orders
-- A group must have:
-- * list of monsters it contains, updated when one dies
-- * a battle order
battle_mode.groups = {}
battle_mode.group = {}
add_loadsave("battle_mode.groups", {})
add_loadsave("battle_mode.group", {})
hook(hook.BIRTH, function() battle_mode.groups = {} battle_mode.group = {} end)

--- @fn
-- @brief Make a new battle group
function battle_mode.create_group(t)
	assert(t.define, "No battle group define")
	assert(t.name, "No battle group name")
	t.list = t.list or {}
	t.active_order = t.active_order or -1
	t.passive_orders = t.passive_orders or {}
	tinsert(battle_mode.groups, t)
	battle_mode.group[t.define] = getn(battle_mode.groups)
end

--- @fn
-- @brief Add a member to the group
function battle_mode.add_group_member(id, m_idx)
	local group = battle_mode.groups[id]
	if not group then return end
	if group.list[m_idx] then return end
	group.list[m_idx] =
	{
		target = -1
	}
	monster(m_idx).flags[FLAG_BATTLE_MODE_GROUP] = id
	return true
end

--- @fn
-- @brief Delete a member from a group(but does not remove the monster from the game)
function battle_mode.del_group_member(id, m_idx)
	local group = battle_mode.groups[id]
	if not group then return end
	if not group.list[m_idx] then return end
	group.list[m_idx] = nil
	monster(m_idx).flags[FLAG_BATTLE_MODE_GROUP] = nil
	return true
end

--- @fn
-- @brief Sets a group order
function battle_mode.set_group_order(id, order_id)
	local group = battle_mode.groups[id]
	if not group then return end
	local order = battle_mode.orders[order_id]
	if not order then return end

	if order.type == "active" then
		group.active_order = order_id
	else
		group.passive_orders[order_id] = true
	end
	return true
end

function battle_mode.new_order(t)
	assert(t.name, "No battle order name")
	assert(t.type and (t.type == "active" or t.type == "passive"), "No or wrong battle order type in "..t.name)
	t.index = battle_mode.__next_order
	battle_mode.orders[t.name] = t.index
	battle_mode.__next_order = battle_mode.__next_order + 1
end

function battle_mode.process_event(event, ...)
	print("Battle event "..event)
	for k, e in arg do
		print(" * "..tostring(k).." :=: "..tostring(e))
	end
end

-- Register deaths to update the global state
hook
{
	[hook.MONSTER_DEATH] = function(m_idx, who)
		local monst = monster(m_idx)
		if monst.flags[FLAG_BATTLE_MODE_GROUP] then
			local group = battle_mode.groups[monst.flags[FLAG_BATTLE_MODE_GROUP]]
			if monst.flags[FLAG_BATTLE_MODE_GROUP_LEADER] then
				battle_mode.process_event(battle_mode.events.GROUP_LEADER_DEATH, { group=monst.flags[FLAG_BATTLE_MODE_GROUP] m_idx=m_idx source=who })
			else
				battle_mode.process_event(battle_mode.events.GROUP_MEMBER_DEATH, { group=monst.flags[FLAG_BATTLE_MODE_GROUP] m_idx=m_idx source=who })
			end
			group.list[m_idx] = nil
		end
	end
	[hook.MONSTER_TAKE_HIT] = function(who, m_idx, monst, dam)
		if monst.flags[FLAG_BATTLE_MODE_GROUP] then
			battle_mode.process_event(battle_mode.events.GROUP_MEMBER_HURT, { group=monst.flags[FLAG_BATTLE_MODE_GROUP] m_idx=m_idx source=who })
		end
	end
}

-- The AI using the battle mode global tactical state to give orders to each monsters
ai.new
{
	name	= "BATTLE_MODE"
	state	=
	{
		BATTLE_MODE_NEXT_AI = ai.NONE
	}
	init	= function(monst, state)
		ai.init(monst, flag_get(state, FLAG_BATTLE_MODE_NEXT_AI))
	end
	exec	= function(m_idx, monst, state)
		local group = battle_mode.groups[monst.flags[FLAG_BATTLE_MODE_GROUP]]
		if group.list[m_idx].target ~= -1 then
			monst.target = group.list[m_idx].target
			ai.exec(flag_get(state, FLAG_BATTLE_MODE_NEXT_AI), m_idx, monst)
		else
			monst.ai_action = ai.action.REST
			return
		end
	end
}




battle_mode.new_order
{
	name = "STOP"
	type = "active"
}

battle_mode.new_order
{
	name = "ATTACK_GROUP"
	type = "active"
}

battle_mode.new_order
{
	name = "GUARD_LOCATION"
	type = "active"
}

battle_mode.new_order
{
	name = "ATTACK_ATTACKER"
	type = "passive"
}

