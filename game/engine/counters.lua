-- Counters system
-- A counter can be anything that needs counting
-- Typical counters are life, mana, sanity, ...

constant("counter", {})

counter.__next = 1
counter.__counters = {}
counter.__state = {}

add_loadsave("counter.__state", {})

function counter.create(t)
	assert(t.name, "No counter name")
	assert(t.min, "No counter min")
	assert(t.max, "No counter max")
	assert(t.reset, "No counter reset method")

	counter.__state[counter.__next] = counter.__state[counter.__next] or
	{
		min = t.min
		cur = iif(t.reset == "max", t.max, t.min)
		max = t.max
	}

	counter.__counters[counter.__next] = t
	counter[t.name] = counter.__next
	counter.__next = counter.__next + 1
end

--- @fn
-- @brief Reset one or all counters to their default value
function counter.reset(cnt)
	if not cnt then
		for i = 1, counter.__next - 1 do counter.reset(i) end
	else
		if counter.__counters[cnt].reset == "max" then
			counter.__state[cnt].cur = counter.__state[cnt].max
		elseif counter.__counters[cnt].reset == "min" then
			counter.__state[cnt].cur = counter.__state[cnt].min
		end
	end
end

function counter.inc(cnt, val, ...)
	-- Check
	if val < 0 then
		if counter.__counters[cnt].on_decrease then
			local ret, new_val
			ret, new_val = counter.__counters[cnt].on_decrease(counter.__state[cnt].cur, val, unpack(arg))
			if ret then val = new_val end
		end
	else
		if counter.__counters[cnt].on_increase then
			local ret, new_val
			ret, new_val = counter.__counters[cnt].on_increase(counter.__state[cnt].cur, val, unpack(arg))
			if ret then val = new_val end
		end
	end

	if val == 0 then return end

	-- Ok do it
	counter.__state[cnt].cur = counter.__state[cnt].cur + val
	if counter.__state[cnt].cur < counter.__state[cnt].min then
		counter.__state[cnt].cur = counter.__state[cnt].min
	elseif counter.__state[cnt].cur > counter.__state[cnt].max then
		counter.__state[cnt].cur = counter.__state[cnt].max
	end
	return true
end

function counter.dec(cnt, val, ...)
	return counter.inc(cnt, -val, unpack(arg))
end

function counter.set(cnt, val, ...)
	local diff = val - counter.get(cnt)
	counter.inc(cnt, diff, unpack(arg))
end

function counter.min(cnt, val)
	if val then
		counter.__state[cnt].min = val
	end
	return counter.__state[cnt].min
end

function counter.max(cnt, val)
	if val then
		counter.__state[cnt].max = val
	end
	return counter.__state[cnt].max
end

function counter.get(cnt)
	return counter.__state[cnt].cur
end

function counter.state(cnt, field, val)
	if val then
		counter.__state[cnt][field] = val
	end
	return counter.__state[cnt][field]
end

--- @fn
-- @brief Call when the counters may change automatically(regen)
function counter.regen()
	for k, _ in counter.__counters do
		if counter.__counters[k].auto_regen then
			local inc = counter.__counters[k].auto_regen()
			if inc then counter.inc(k, inc) end
		end
	end
end

-- Every turn, regen teh counters
hook(hook.PROCESS_WORLD, counter.regen)
