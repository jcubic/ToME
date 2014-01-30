-- Time and calendar stuff

-- Set your own date for turn 1 with these for use with set_calendar
time.start_year = 1
time.start_day = 1
-- Set time of day
time.start_minute = 360

time.dawnhour = 6
time.duskhour = 18

-- If you have a 365 day year with a set of months, set_calendar should work fine
-- otherwise reimplement four functions:
--      time.get_date_string(turn)
--      time.get_note_date_string(turn)
--      time.get_long_note_date_string(turn)
--      time.display_time(turn)

-- Control daylight hours with these functions:
--      time.is_daylight(turn)
--      time.is_dawn(turn)
--      time.is_dusk(turn)

-- Eventually, bst should be moved to lua and made reimplementable, and the
-- C code using it should be made flexible enough to handle something other than
-- a 24-hour day.
function time.set_calendar(t)
	local days = 0

	time.calendar = {}
	for _, e in t do
		if not e[3] then e[3] = 0 end
		tinsert(time.calendar, { days=days, name=e[2], length=e[1], offset=e[3] })
		days = days + e[1]
	end
	assert(days == 365, "Calendar incomplete, days ends at "..days.." instead of 365")
end

-- Converts a T-Engine turn to our display turns
-- This allows us to set our custom start date
function time.display_turn(t)
	return t +
	       (time.start_day - 1) * time.DAY * 10 +
	       time.start_minute * time.MINUTE * 10
end

function time.display_year(t)
	local turn = time.display_turn(t)
	return time(time.YEAR, turn) + time.start_year
end

function time.get_month_num(dayofyear)
	local i = getn(time.calendar)

	-- Find the period name
	while ((i > 1) and (dayofyear < time.calendar[i].days)) do
		i = i - 1
	end

	return i
end

function time.get_month_name(dayofyear)
	local month = time.get_month_num(dayofyear)
	return time.calendar[month].name
end

function time.get_day_of_month(dayofyear)
	local month = time.get_month_num(dayofyear)
	return dayofyear - time.calendar[month].days + 1 + time.calendar[month].offset
end

function time.get_month_length(dayofyear)
	local month = time.get_month_num(dayofyear)
	return time.calendar[month].length
end

function time.ordinal(number)
	local remainder = imod(number, 10)
	local suffix = "th"
	if remainder == 1 then
		suffix = "st"
	elseif remainder == 2 then
		suffix = "nd"
	elseif remainder == 3 then
		suffix = "rd"
	end

	return format("%d%s", number, suffix)
end

function time.get_date_string(t)
	local turn = time.display_turn(t)
	local year = time.display_year(t)
	local dayofyear = time(time.DAY, turn)
	local month = time.get_month_name(dayofyear)
	local month_length = time.get_month_length(dayofyear)

	if month_length > 1 then
		local day = time.ordinal(time.get_day_of_month(dayofyear))
		return "the "..day.." "..month.." of the year "..year
	else
		return month.." of the year "..year
	end
end

function time.get_twelve_hour_time(turn)
	local hour = imod(time(time.HOUR, turn), 12)
	local minute = time(time.MINUTE, turn)
	if hour == 0 then
		hour = 12
	end
	local ampm = "AM"
	if time(time.HOUR, turn) >= 12 then
		ampm = "PM"
	end
	return format("%d:%02d %s", hour, minute, ampm)
end

function time.get_note_date_string(t)
	local turn = time.display_turn(t)
	local timeofday = time.get_twelve_hour_time(turn)
	local dayofyear = time(time.DAY, turn)
	local month = time.get_month_name(dayofyear)
	local day = time.ordinal(time.get_day_of_month(dayofyear))

	return format("%s, %s %s", timeofday, day, month)
end

function time.get_long_note_date_string(t)
	local turn = time.display_turn(t)
	local timeofday = time.get_twelve_hour_time(turn)
	local date = time.get_date_string(t)

	return format("%s %s", timeofday, date)
end

function time.display_time(t)
	local turn = time.display_turn(t)
	local timeofday = time.get_twelve_hour_time(turn)
	local date = time.get_date_string(t)

	msg_format("Today is %s.", date)
	msg_format("The time is %s.", timeofday)
end

function time.is_daylight(t)
	local turn = time.display_turn(t)
	local hour = time(time.HOUR, turn)

	if hour < time.dawnhour then
		return nil
	elseif hour < time.duskhour then
		return 1
	else
		return nil
	end
end

function time.is_dawn(t)
	local turn = time.display_turn(t)
	local hour = time(time.HOUR, turn)
	local minute = time(time.MINUTE, turn)

	if hour == time.dawnhour and minute == 0 and imod(turn, time.MINUTE) == 0
	then
		return true
	else
		return false
	end
end

function time.is_dusk(t)
	local turn = time.display_turn(t)
	local hour = time(time.HOUR, turn)
	local minute = time(time.MINUTE, turn)

	if hour == time.duskhour and minute == 0 and imod(turn, time.MINUTE) == 0
	then
		return true
	else
		return false
	end
end
