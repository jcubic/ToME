--
-- The shapeshifting subsystem
--
-- Load it with: load_subsystem("mimicry")
--
constant("mimic", {})

new_flag("PR_MIMIC")

mimic.__mimics = {}
mimic.__mimics_max = 1
mimic.__mimics_names = {}

function mimic.add_shape(t)
	assert(t.name, "no mimic name")
	assert(t.desc, "no mimic desc")
	assert(t.level, "no mimic level")
	assert(t.duration, "no mimic duration")

	if not t.limit then t.limit = 0 end

	if not t.obj_name then
		t.obj_name = t.name
	end

	t.show_name = '['..t.name..']'

	-- if it needs hooks, add them
	if t.hooks then
		hook(t.hooks)
	end

	-- Add it in a name to index hash table
	mimic.__mimics_names[t.name] = mimic.__mimics_max

	mimic.__mimics[mimic.__mimics_max] = t
	if t.define then mimic[t.define] = mimic.__mimics_max end
	mimic.__mimics_max = mimic.__mimics_max + 1
end

function mimic.resolve_name(name)
	if mimic.__mimics_names[name] then
		return mimic.__mimics_names[name]
	else
		return -1
	end
end

function mimic.find_random_shape(level, limit, realm)
	local mimic, tries

	tries = 1000
	while tries > 0 do
		tries = tries - 1
		mimic = rng(1, mimic.__mimics_max - 1)
		if (not realm) or (mimic.__mimics[mimic].realm == realm) then
			if limit >= mimic.__mimics[mimic].limit then
				if (rng.number(mimic.__mimics[mimic].level * 3) < level) and (mimic.__mimics[mimic].rarity < 100)  and (rng.percent(100 - mimic.__mimics[mimic].rarity)) then
					break
				end
			end
		end
	end
	if tries > 0 then
		return mimic
	end
end

function mimic.get_info(m, info)
	if not mimic.__mimics[m] then return end
	return mimic.__mimics[m][info]
end

function mimic.get_rand_dur(m)
	return rng(mimic.__mimics[m].duration[1], mimic.__mimics[m].duration[2])
end

function mimic.set_shape(m, level)
	level = level or player.lev
	player.mimic_level = level
	player.mimic_form = m
	player.update = 4294967295
	player.redraw[FLAG_PR_MIMIC] = true
end

function mimic.get_shape()
	return player.mimic_form, player.mimic_level
end

hook
{
	[hook.CALC_BONUS_BEGIN] = function()
		if player.mimic_form > 0 and mimic.__mimics[player.mimic_form].calc_begin then
			mimic.__mimics[player.mimic_form].calc_begin()
		end
	end
	[hook.CALC_BONUS] = function()
		if player.mimic_form > 0 and mimic.__mimics[player.mimic_form].calc then
			mimic.__mimics[player.mimic_form].calc()
		end
	end
	[hook.CALC_BONUS_END] = function()
		if player.mimic_form > 0 and mimic.__mimics[player.mimic_form].calc_end then
			mimic.__mimics[player.mimic_form].calc_end()
		end
	end
}

-- No mimic shape
mimic.NONE = 0

-- Add the mimic shapes
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/mimicry.lua", false)
