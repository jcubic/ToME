-- Handle timed temporary effects

constant("timed_effect", {})
settag(timed_effect, TAG_NAMESPACE) -- Tag as a namespace

timed_effect.__state = {}
-- The counters of all the effects
timed_effect.__state.counter = {}
-- The effects additional parameters, if any
timed_effect.__state.aux = {}
-- The effects are silent ?
timed_effect.__state.silent = {}
add_loadsave("timed_effect.__state", {})

-- The currently executing effect
timed_effect.__next = 1

-- The currently executing effect
timed_effect.__cur_effect = nil

--- @fn
-- @brief Makes a new timed effect
function timed_effect.create(t)
	assert(t.name, "No effect name")
	assert(t.parameters, "No effect parameters")
	timed_effect[timed_effect.__next] = t
	timed_effect[t.name] = timed_effect.__next
	timed_effect.__next = timed_effect.__next + 1
end

-- Gain the effect
function timed_effect.on_gain(effect, counter, ...)
	local notice = false

	if timed_effect.__state.silent[effect] then return false end
	if not timed_effect[effect].on_gain then return true end
	if tag(timed_effect[effect].on_gain) == TAG_FUNCTION then
		notice = timed_effect[effect].on_gain(counter, unpack(arg))
	else
		message(timed_effect[effect].on_gain)
		notice = true
	end
	return notice
end

-- Change the effect
function timed_effect.on_change(effect, counter)
	local notice = false

	if timed_effect.__state.silent[effect] then return false end
	if not timed_effect[effect].on_change then return true end
	if tag(timed_effect[effect].on_change) == TAG_FUNCTION then
		notice = timed_effect[effect].on_change(counter)
	else
		message(timed_effect[effect].on_change)
		notice = true
	end
	return notice
end

-- Lose the effect
function timed_effect.on_lose(effect)
	local notice = false

	-- Do we have an effect to lose in the first place?
	if timed_effect.__state.counter[effect] == nil then
		return false
	end

	local was_silent = timed_effect.__state.silent[effect]
	timed_effect.__state.silent[effect] = false

	if timed_effect[effect].on_lose and not was_silent then
		if tag(timed_effect[effect].on_lose) == TAG_FUNCTION then
			notice = timed_effect[effect].on_lose()
		else
			message(timed_effect[effect].on_lose)
			notice = true
		end
	end
	-- Remove from the list
	timed_effect.__state.counter[effect] = nil
	return notice
end

--- @fn
-- @brief Call when the effects time must decrease
function timed_effect.timeout()
	for k, _ in timed_effect.__state.counter do
		timed_effect.__cur_effect = k
		local amt = 1
		if timed_effect[k].amount_from_intrinsic then amt = player.intrinsic(timed_effect[k].amount_from_intrinsic) end

		-- Changed message ?
		timed_effect.on_change(k, timed_effect.__state.counter[k] - amt)

		timed_effect.__state.counter[k] = timed_effect.__state.counter[k] - amt
		if timed_effect[k].on_timeout then timed_effect[k].on_timeout() end
		if not timed_effect.__state.counter[k] or timed_effect.__state.counter[k] <= 0 then
			timed_effect.on_lose(k)
			player.update = player.update | PU_BONUS
			if timed_effect[k].redraw then player.redraw[timed_effect[k].redraw] = true end
		end
		timed_effect.__cur_effect = nil
	end
end

--- @fn
-- @brief Call when the bonuses shouldbe applied
function timed_effect.execute()
	for k, _ in timed_effect.__state.counter do
		timed_effect.__cur_effect = k
		if timed_effect[k].bonus then timed_effect[k].bonus() end
		timed_effect.__cur_effect = nil
	end
end

--- @fn
-- @brief Sets an effect timeout and its parameters
-- @param effect Effect Effect to set
-- @param counter Number The time to have the effect last
-- @param ... List a list of additional parameters
function timed_effect.set_real(effect, counter, ...)
	local notice = false
	assert(timed_effect[effect], "Unknown effect")
	local old = timed_effect.__state.counter[effect]

	if counter == 0 and not timed_effect.__state.counter[effect] then return false end
	if timed_effect.__state.counter[effect] == counter then return false end

	-- Message if it was previsouly set to 0
	if not timed_effect.__state.counter[effect] and counter > 0 then
		timed_effect.on_gain(effect, counter, unpack(arg))
	end

	-- Changed message ?
	timed_effect.on_change(effect, counter, unpack(arg))

	timed_effect.__state.counter[effect] = counter

	local params = {}
	for i = 1, getn(timed_effect[effect].parameters) do
		if arg[i] then
			-- Use the new value
			params[timed_effect[effect].parameters[i][1]] = arg[i]
		elseif (not timed_effect.__state.aux[effect]) or (not timed_effect.__state.aux[effect][timed_effect[effect].parameters[i][1]]) then
			-- If no previous value, use the default
			params[timed_effect[effect].parameters[i][1]] = timed_effect[effect].parameters[i][2]
		else
			-- Use the old value
			params[timed_effect[effect].parameters[i][1]] = timed_effect.__state.aux[effect][timed_effect[effect].parameters[i][1]]
		end
	end
	timed_effect.__state.aux[effect] = params

	-- Do we lose the effect already ?
	if timed_effect.__state.counter[effect] <= 0 and old then
		timed_effect.on_lose(effect)
	end

	if timed_effect[effect].redraw then player.redraw[timed_effect[effect].redraw] = true end
	player.update = player.update | PU_BONUS
	return notice
end

--- @fn
-- @brief Sets an effect timeout and its parameters
-- @param effect Effect Effect to set
-- @param counter Number The time to have the effect last
-- @param ... List a list of additional parameters
function timed_effect.set_silent(effect, counter, ...)
	timed_effect.__state.silent[effect] = true
	timed_effect.set_real(effect, counter, unpack(arg))
end

--- @fn
-- @brief Sets an effect timeout and its parameters
-- @param effect Effect Effect to set
-- @param counter Number The time to have the effect last
-- @param ... List a list of additional parameters
function timed_effect.set(effect, counter, ...)
	timed_effect.__state.silent[effect] = false
	timed_effect.set_real(effect, counter, unpack(arg))
end

--- @fn
-- @brief Set a parameter from the currently executing effect, or any effect
-- @param effect Effect (Optionnal) If not set the current effect will be used(if any)
-- @param param String The effect parameter to set
-- @param v Value The value to set
function timed_effect.set_param(effect, param, v)
	-- If no effect given, use default one
	if tag(effect) == TAG_STRING then
		effect, param, v = timed_effect.__cur_effect, effect, param
	end
	-- Otherwise return the parameter value
	if not timed_effect.__state.aux[effect] then return end
	timed_effect.__state.aux[effect][param] = v
end

--- @fn
-- @brief Get a parameter from the currently executing effect, or any effect
-- @param effect Effect (Optionnal) If not set the current effect will be used(if any)
-- @param param String The effect parameter to get
function timed_effect.get(effect, param)
	-- If no effect given, use default one
	if tag(effect) == TAG_STRING then
		effect, param = timed_effect.__cur_effect, effect
	else
		-- If the first param is not a string, and there is no second param
		-- return the counter value
		if not param then
			if not effect then effect = timed_effect.__cur_effect end
			return timed_effect.__state.counter[effect]
		end
	end
	-- Otherwise return the parameter value
	if not timed_effect.__state.aux[effect] then return nil end
	return timed_effect.__state.aux[effect][param]
end

--- @fn
-- @brief Increases the counter of an effect
function timed_effect.inc(effect, inc)
	return timed_effect.set(effect, (timed_effect.get(effect) or 0)+ inc)
end

--- @fn
-- @brief Resets counters(like, upon character generation)
function timed_effect.reset()
	timed_effect.__state.counter = {}
	timed_effect.__state.aux = {}
	timed_effect.__state.silent = {}
	for k = 1, timed_effect.__next - 1 do
		timed_effect.__cur_effect = k

		local params = {}
		for i = 1, getn(timed_effect[k].parameters) do
			-- Default value
			params[timed_effect[k].parameters[i][1]] = timed_effect[k].parameters[i][2]
		end
		timed_effect.__state.aux[k] = params

		if timed_effect[k].on_reset then timed_effect[k].on_reset() end
		timed_effect.__cur_effect = nil
	end
end

--- @fn
-- Calls callback function for every timed effect.
function timed_effect.foreach_effect(type, func)
	for k, _ in timed_effect.__state.counter do
		timed_effect.__cur_effect = k

		if (type == "all" or type == timed_effect[k].type) then
			func(k, timed_effect[k])
		end
	end
end

-- Calling timed_effects sets one
timed_effect.__exec_module = timed_effect.set

-- Every turn, decrease the effects
hook(hook.PROCESS_WORLD, timed_effect.timeout)


--- @fn
-- Displays the list
function timed_effect.display_effects(status, type, full)
	local status_color = {
		beneficial = "G"
		detrimental = "R"
		hidden = "W"
		all = "b"
	}

	term.save()

	local filename = "/tmp/te.txt"
	local file = fs.open(filename, "w")

	fs.write(file, "Status effects list :")
	fs.write(file, "")

	for k, _ in timed_effect.__state.counter do
		timed_effect.__cur_effect = k
		if (status == "all" or status == timed_effect[k].status) and (type == "all" or type == timed_effect[k].type) then
			fs.write(file, "#####"..status_color[timed_effect[k].status]..timed_effect[k].desc.." ("..timed_effect[k].status.."/"..timed_effect[k].type..")")
			fs.write(file, " * duration: "..timed_effect.get())
			for i = 1, getn(timed_effect[k].parameters) do
				local data = timed_effect[k].parameters[i]
				fs.write(file, " * "..data[1]..": "..timed_effect.get(data[1]))
			end
		end
		timed_effect.__cur_effect = nil
	end

	fs.close(file)
	show_file(filename, "Effects status", 0, 0)
	fs.delete(filename)

	term.load()
end

--- @fn
-- Returns a random effect of the specified status/type
function timed_effect.get_random(status, type)
	local tbl = {}
	for k, _ in timed_effect.__state.counter do
		if (status == "all" or status == timed_effect[k].status) and (type == "all" or type == timed_effect[k].type) then
			tinsert(tbl, k)
		end
	end
	if getn(tbl) == 0 then return nil end
	return tbl[rng(1, getn(tbl))]
end

-- Register on the ! key to see beneficial effects
hook(hook.KEYPRESS, function(key)
	if key==strbyte('!') then timed_effect.display_effects("beneficial", "all", true) return true end
end)
