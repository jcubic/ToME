-- Subsystem for filling unordered nests and ordered pits.
--
-- Load it with: load_subsystem("nests_and_pits")
-- Good functions to override:
-- * nest_fill
-- * pit_fill

constant("nest_pit", {})

nest_pit.ongoing = false
nest_pit.template_race = 2
nest_pit.type = nil

nest_pit.clone = function(r_idx)
	return (r_idx == nest_pit.template_race)
end

nest_pit.symbol_clone = function(r_idx)
	return (r_info[r_idx + 1].d_char == r_info[nest_pit.template_race + 1].d_char)
end

hook(hook.INIT_DONE, function()
	nest_pit.ongoing = false
	nest_pit.template_race = 2
	nest_pit.type = nest_pit.symbol_clone
end)


-- Choosing a random monster: converted from C.
nest_pit.choose_template = function(level)
	while true do
		nest_pit.template_race = rng(2, max_r_idx)

		-- Lua is stupid for not having a "continue" construct
		-- Reject uniques
		if not has_flag(r_info[nest_pit.template_race + 1], FLAG_UNIQUE) then
			-- Reject OoD monsters in a loose fashion
			if r_info[nest_pit.template_race + 1].level + rng(5) <= level + rng(5) then
				-- Monster ok
				return
			end
		end
	end
end

-- Nests and pits do not have to have the same possible inhabitants but they can easily.

nest_pit.nest_fill = function()
	nest_pit.choose_template(dun_level)
 	if dun_level >= 25 + rng(15) and rng(2) == 1 then
		nest_pit.type = nest_pit.symbol_clone
		return "symbol clones"
	else
		nest_pit.type = nest_pit.clone
		return "clones"
	end
end

nest_pit.pit_fill = function()
	return nest_pit.nest_fill()
end

hook(hook.NEW_NEST,
function()
	nest_pit.nest_fill()
	nest_pit.ongoing = true
	return true, false
end)

hook(hook.NEW_PIT,
function(level)
	nest_pit.pit_fill()
	nest_pit.ongoing = true
	return true, false
end)

hook(hook.MONSTER_FORBID,
function(r_idx)
	if nest_pit.ongoing == true then
		-- Result inverted for convenience at both ends.
		-- If this hook is not executed, the code defaults to allow.
		if has_flag(r_info[r_idx + 1], FLAG_UNIQUE) then return true end
		if nest_pit.type(r_idx) then
			return false, false
		else
			return true, false
		end
	end
end)

hook(hook.NEST_PIT_POST,
function()
	-- Turn off forbidding monsters.
	nest_pit.ongoing = nil
end)
