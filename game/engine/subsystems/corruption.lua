--
-- The corruptions subsystem
--
-- Load it with: load_subsystem("corruption")
--

constant("corruption", {})
settag(corruption, TAG_NAMESPACE) -- Tag game as a namespace

corruption.__corruptions_aux = {}
add_loadsave("corruption.__corruptions_aux", {})
hook(hook.BIRTH, function() corruption.__corruptions_aux = {} end)
corruption.__corruptions = {}
corruption.__corruptions_max = 0
corruption.__corruptions_callbacks_max = 0

-- Get the corruption
function corruption.__exec_module(c, set)
	if set then
		corruption.__corruptions_aux[c] = set
		flag_set(player.redraw, FLAG_PR_BASIC, true)
		player.update = player.update | PU_BONUS | PU_TORCH | PU_BODY | PU_POWERS
		if (set == true) and (corruption.__corruptions[c].gain) then
			corruption.__corruptions[c].gain()
		end
		if (set == false) and (corruption.__corruptions[c].lose) then
			corruption.__corruptions[c].lose()
		end
	else
		return corruption.__corruptions_aux[c]
	end
end

-- Test if we have that corruption
-- We must:
-- 1) have it or be willing to get it
-- 2) have all its dependancies
-- 3) have none of its opposing corruptions
-- 4) pass the possible tests
function corruption.test_depend_corrupt(corrupt, can_gain)
	local i, c

	if can_gain then
		if corruption(corrupt) then
			return false
		end
	else
		if not corruption(corrupt) then
			return false
		end
	end

	for c, i in corruption.__corruptions[corrupt].depends do
		if corruption.test_depend_corrupt(c) ~= true then
			return false
		end
	end

	for c, i in corruption.__corruptions[corrupt].oppose do
		if corruption.test_depend_corrupt(c) ~= false then
			return false
		end
	end

	-- are we even allowed to get it?
	if corruption.__corruptions[corrupt].can_gain and (not corruption.__corruptions[corrupt].can_gain()) then
		return false
	end

	return true
end

-- Gain a new corruption
function corruption.gain(group)
	local i, max
	local pos = {}

	-- Get the list of all possible ones
	max = 0
	group = group or "default"
	for i = 0, corruption.__corruptions_max - 1 do
		if corruption.__corruptions[i].group == group and corruption.test_depend_corrupt(i, true) == true and corruption.__corruptions[i].random == true and corruption.__corruptions[i].allow() then
			pos[max] = i
			max = max + 1
		end
	end

	-- Ok now get one of them
	if (max > 0) then
		local ret = rng.number(max)

		corruption(pos[ret], true)
		message(color.LIGHT_RED, corruption.__corruptions[pos[ret]].get_text)

		return pos[ret]
	else
		return -1
	end
end

-- Lose an existing corruption
function corruption.lose()
	local i, max
	local pos = {}

	-- Get the list of all possible ones
	max = 0
	for i = 0, corruption.__corruptions_max - 1 do
		if corruption.test_depend_corrupt(i) == true and corruption.__corruptions[i].removable == true then
			pos[max] = i
			max = max + 1
		end
	end

	-- Ok now get one of them
	if (max > 0) then
		local ret = rng.number(max)

		corruption(pos[ret], FALSE)
		message(color.LIGHT_RED, corruption.__corruptions[pos[ret]].lose_text)

		-- Ok now lets see if it broke some dependancies
		for i = 0, max - 1 do
			if corruption(pos[i]) ~= corruption.test_depend_corrupt(pos[i]) then
				corruption(pos[i], FALSE)
				message(color.LIGHT_RED, __corruptions[pos[i]].lose_text)
			end
		end

		return pos[ret]
	else
		return -1
	end
end

-- Lose all corruptions (for e.g. Potion of New Life)
function corruption.lose_all()
	local i;
	for i = 0, corruption.__corruptions_max - 1 do
		corruption.lose()
	end
	return -1
end

-- Creates a new corruption
function corruption.new(c)
	assert(c.color, "No corruption color")
	assert(c.name, "No corruption name")
	assert(c.get_text, "No corruption get_text")
	assert(c.lose_text, "No corruption lose_text")
	assert(c.desc, "No corruption desc")
	assert(c.hooks, "Nothing to do for corruption")
	if not c.group then c.group = "default" end
	if not c.random then c.random = true end
	if not c.removable then c.removable = true end
	if not c.allow then c.allow = function() return not nil end end

	if c.depends == nil then c.depends = {} end
	if c.oppose == nil then c.oppose = {} end

	-- We must make sure the other ones opposes too
	local o, i
	for o, i in c.oppose do
		corruption.__corruptions[o].oppose[corruption.__corruptions_max] = true
	end

	local index, h
	for index, h in c.hooks do
		local hookname = "__lua__corrupt_callback"..corruption.__corruptions_callbacks_max
		local id = corruption.__corruptions_max
		hook.add_script(index, hookname, hookname)
		declare_global_constants{hookname}
		setglobal(hookname,
			function (...)
				if corruption.test_depend_corrupt(%id) then
					return call(%h, arg)
				end
			end
		)
		corruption.__corruptions_callbacks_max = corruption.__corruptions_callbacks_max + 1
	end

	if type(c.desc) == "table" then
		local new_desc = ""
		for index, h in c.desc do
			new_desc = new_desc..h.."\n"
		end
		c.desc = new_desc
	end

	corruption.__corruptions[corruption.__corruptions_max] = c
	corruption.__corruptions_max = corruption.__corruptions_max + 1
	return corruption.__corruptions_max
end

-- Now load player corruptions
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/corruptions.lua", false)
