-- Define stats

player.stats = {}
player.stats.def = {}
player.stats.from_name = {}
player.stats.from_short = {}

function player.define_stat(t)
	assert(t.name, "No stat name")
	assert(t.short, "No stat short name")
	assert(t.reduced, "No stat reduced short name")
	assert(t.desc_positive, "No stat desc_positive")
	assert(t.desc_negative, "No stat desc_negative")

	stats.MAX = stats.MAX + 1
	player.stats.def[stats.MAX] = t
	t.__index__ = stats.MAX

	player.stats.from_name[strlower(t.name)]   = stats.MAX
	player.stats.from_short[strlower(t.short)] = stats.MAX

	if t.define_as then
		define_as(t)
		dostring(t.load_script)
	end
end

function player.get_stat_info(stat, info)
	return player.stats.def[stat][info]
end

function player.get_stat_idx(stat, quiet)
	stat = strlower(stat)
	if player.stats.from_name[stat] then
		stat = player.stats.from_name[stat]
	elseif player.stats.from_short[stat] then
		stat = player.stats.from_short[stat]
	else
		if not quiet then
			message("*ERROR* No such stat as '" .. stat .. "'")
		end
		return
	end

	return stat
end

function player.define_experience_ladder(exps)
	assert(getn(exps) == PY_MAX_LEVEL - 1, "Defined exp ladder is not in line with number of player levels. "..getn(exps).." != ".. (PY_MAX_LEVEL-1))
	local last = 0
	for i = 1, getn(exps) do
		player_exp[i] = exps[i]
	end
end


--
-- Things get 100 energy per game turn
-- When things have more than 1000 energy they can act
-- This tells how much energy is used by which speed setting
--
function player.define_speed_ladder(speeds)
	if getn(speeds) ~= 210 then error("Speed ladder must be exactly 210 entries long, it is "..getn(speeds)) end
	for i = 1, 210 do
		__extract_energy[i + 10] = speeds[i]
	end
end

-- Define the default speed ladder
player.define_speed_ladder
{
	--[[ S-100]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-90 ]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-80 ]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-70 ]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-60 ]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-50 ]] 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
	--[[ S-40 ]]  5000,  5000,  5000,  5000,  5000,  5000,  5000,  5000,  5000,  5000,
	--[[ S-30 ]]  5000,  5000,  5000,  5000,  5000,  5000,  5000,  3330,  3330,  3330,
	--[[ S-20 ]]  3330,  3330,  3330,  3330,  3330,  2500,  2500,  2500,  2500,  2500,
	--[[ S-10 ]]  2000,  2000,  2000,  2000,  1670,  1670,  1430,  1430,  1250,  1110,
	--[[ Norm ]]  1000,   910,   830,   770,   710,   670,   630,   590,   560,   530,
	--[[ F+10 ]]   500,   480,   450,   430,   420,   400,   380,   370,   360,   340,
	--[[ F+20 ]]   330,   320,   310,   300,   290,   290,   280,   280,   270,   270,
	--[[ F+30 ]]   260,   260,   260,   260,   250,   250,   250,   240,   240,   240,
	--[[ F+40 ]]   240,   240,   240,   230,   230,   230,   230,   230,   230,   230,
	--[[ F+50 ]]   220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
	--[[ F+60 ]]   210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
	--[[ F+70 ]]   200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
	--[[ F+80 ]]   200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
	--[[ V+90 ]]   200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
	--[[ V+100]]   200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
}
