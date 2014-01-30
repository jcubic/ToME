--
-- The items flavours subsystem
--

constant("flavour", {})

new_flag("FLAVOUR")

flavour.generators = {}
flavour.table      = {}

function flavour.add_generator(tval, generator)
	if not flavour.generators[tval] then
		flavour.generators[tval] = {}
	end

	tinsert(flavour.generators[tval], generator)
end

function flavour.make_from_list(out, number, list, prefix, suffix)
	if getn(out) >= number then
		return true
	end

	for i = 1, getn(list) do
		local str = list[i][1]

		if prefix then
			str = prefix .. " " .. str
		end

		if suffix then
			str = str .. " " .. suffix
		end

		tinsert(out, {str, list[i][2]})

		if getn(out) == number then
			return true
		end
	end

	return false
end -- flavour.make_from_list()

function flavour.title_generator(flavs, syllables, color, number)
	local generated_titles = {}

	for i = 1, number do
		-- Get a new title
		while not nil do
			-- Start a new title
			local buf = ""

			-- Collect words until done
			while not nil do
				local nb_syllables = iif(rng.percent(30), 1, 2)

				local tmp = ""

				-- Add a one or two syllable word
				for q = 1, nb_syllables do
					-- Add the syllable
					tmp = tmp .. syllables[rng(getn(syllables))]
				end

				-- Stop before getting too long
				if strlen(buf) + 1 + strlen(tmp) > 15 then break end

				-- Add a space and the word
				buf = buf .. iif(buf == "", "", " ") .. tmp
			end

			-- Check for "duplicate" scroll titles
			if not generated_titles[buf] then
				-- Save the title
				flavs[i] = { buf, color }
				break
			end
		end

		-- Memorize this title to avoid generating it again
		generated_titles[flavs[i]] = true
	end
end -- flavour.title_generator()

--------------------------------------------------------

function flavour.init()
	local k_idx_list = {}
	local reserved   = {}

	for tval, generator_list in flavour.generators do
		k_idx_list[tval]    = {}
		reserved[tval]      = {}
		flavour.table[tval] = {}
	end

	-- Collect list of k_idx's of flavoured objects
	for i = 1, max_k_idx - 1 do
		local item = k_info(i)

		if item.name and k_idx_list[item.tval] then
			tinsert(k_idx_list[item.tval], i)

			if item.flags[FLAG_FLAVOUR] then
				reserved[item.tval][item.flags[FLAG_FLAVOUR]] = true
			end
		end
	end

	-- Switch on the simple RNG with a saved seed to ensure the same
	-- flavours for the same cahracter
	local was_simple = rng.is_simple()
	rng.simple(seed_flavor)

	-- Generate randomized flavour lists
	for tval, generator_list in flavour.generators do
		-- Get the higest sval
		local number = 1
		for i = 1, getn(k_idx_list[tval]) do if number < k_info[1 + k_idx_list[tval][i]].sval then number = k_info[1 + k_idx_list[tval][i]].sval end end

		local flavs  = {}

		for i = 1, getn(generator_list) do
			generator_list[i](flavs, number)

			if getn(flavs) >= number then
				break
			end
		end

		-- Remove reserved flavours
		local i          = 1
		local reserv_num = 0
		while i < getn(flavs) do
			if reserved[tval][flavs[i][1]] then
				tremove(flavs, i)
				reserv_num = reserv_num + 1
			else
				i = i + 1
			end
		end

		if (getn(flavs) + reserv_num) < number then
			error("Not enough flavours for " .. tval_to_str(tval))
		end

		-- Randomize list
		for i = 1, getn(flavs) do
			local j = rng(1, getn(flavs))
			flavs[i], flavs[j] = flavs[j], flavs[i]
		end

		-- Insert reserved flavours into the list if the list isn't
		-- big enough to cover all the item kinds, or replace the
		-- flavour if there is enough
		for i = 1, getn(k_idx_list[tval]) do
			local k_idx = k_idx_list[tval][i]
			local item  = k_info[k_idx + 1]
			local sval  = item.sval

			if item.flags[FLAG_FLAVOUR] then
				local fl = {item.flags[FLAG_FLAVOUR]}

				if item.x_attr ~= 0 then
					fl[2] = item.x_attr
				else
					fl[2] = item.d_attr
				end

				if getn(flavs) < number then
					tinsert(flavs, sval, fl)
				else
					flavs[sval] = fl
				end
			end
		end -- for i = 1, number do

		-- Done!
		flavour.table[tval] = flavs
	end -- for tval, generator_list in flavour.generators do

	-- Swich off the simple RNG
	if not was_simple then rng.simple() end

	-- Analyze every object
	for i = 1, max_k_idx - 1 do
		local item = k_info(i)

		-- Skip "empty" objects
		if item.name then
			-- Extract "flavor" (if any)
			local col = flavour.get_color(i)

			-- No flavor yields aware
			if not col or col == 0 then
				item.aware = true
			else
				item.d_attr = col
				item.x_attr = col
			end

			-- Check for "easily known"
			item.easy_know = flavour.easy_know(i)
		end
	end
end -- flavour.init()

--------------------------------------------------------------------------

--- @fn
-- @brief Returns the flavour name and colors
-- @param type String The flavour type.
-- @param nb Number The flavour index
-- @return name Number Name of the flavour
-- @return color Number Color of the flavour
function flavour.get(type, nb)
	nb = nb - 1
	nb = imod(nb, getn(flavour.table[type])) + 1
	return flavour.table[type][nb][1], flavour.table[type][nb][2]
end

-- Tell if an object is "flavourable" by returning nil or the flavour
function flavour.get_flavour(k_idx)
	local item = k_info[k_idx + 1]

	if flavour.table[item.tval] then
		return flavour.table[item.tval][item.sval][1]
	else
		return nil
	end
end

-- Tell if an object is "flavourable" by returning 0 or the color
function flavour.get_color(k_idx)
	local item = k_info[k_idx + 1]

	if flavour.table[item.tval] then
		assert(flavour.table[item.tval][item.sval],
			   "There is a " .. tval_to_str(item.tval) .. " item " ..
				   "whose sval is larger than the number of items of " ..
				   "that tval.")
		return flavour.table[item.tval][item.sval][2]
	else
		return 0
	end
end

-- Tell if an object is easily known
-- This function should be overrided by modules that want to change
-- the behaviour.
function flavour.easy_know(k_idx)
	return nil
end
