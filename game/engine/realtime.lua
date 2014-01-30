-- Handles realtime stuff
-- ****** EXPERIMENTAL ******
-- T-Engine cannot be held responsible if this stuff eats your module or destroys the universe

--- @fn
-- @brief Are we in realtime mode?
realtime.__exec = function()
	return realtime.millis_per_turn > 0
end

--- @fn
-- @brief Switch to realtime mode
function realtime.switch(turns_per_second)
	if turns_per_second then
		realtime.__switch(1000 / turns_per_second)
	else
		realtime.__switch(0)
	end
end

--- @fn
-- @brief Enables realtime and sets some parameters
function realtime.enable(t)
	assert(t.turns_per_second, "No turns_per_second given for realtime")
	assert(t.run_factor, "No run_factor for realtime")
	realtime.switch(t.turns_per_second)
	realtime.run_factor = t.run_factor
end

--[[
realtime.enable
{
	turns_per_second = 20
	run_factor = 10
}
]]
