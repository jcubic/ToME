-- The elven calendar of the reckoning of Rivendell

-- 43rd Yavie in the
-- Bilbo birthday
time.start_day = 1 + 54 + 72 + 43

-- Sunrise
time.start_minute = 360

time.dawnhour = 6
time.duskhour = 18

-- Rewrite the formating of a date(the default doesnt include the third age part)
time.get_date_string = function(t)
	local turn = time.display_turn(t)
	local year = time.ordinal(time.display_year(t))
	local dayofyear = time(time.DAY, turn)
	local month = time.get_month_name(dayofyear)
	local month_length = time.get_month_length(dayofyear)

	if month_length > 1 then
		local day = time.ordinal(time.get_day_of_month(dayofyear))
		return "the "..day.." "..month.." of the "..year.." year of the Third Age"
	else
		return month.." of the "..year.." year of the Third Age"
	end
end

-- Redefine the passing of time
time.MINUTE = 3
time.HOUR = time.MINUTE * 60
time.DAY = time.HOUR * 24
time.YEAR = time.DAY * 365
time.DAY_START = time.HOUR * 6

-- Delay calendar definition until player is born, so we can set a calendar
-- specific to her/his race if needed
hook(hook.GAME_START, function()
	local race = player.get_descriptor("race").id
	-- Hobbits use the Shire's reckoning calendar
	if race == RACE_HOBBIT then
		-- Bilbo birthday year
		time.start_year = 2890

		time.set_calendar
		{
			-- Notice it is not in order, this is to correspond year start day with the other calendars
			-- All calendars are aligned to the start day of Rivendell's reckoning
			{ 24, "Astron", 5 },
			{ 30, "Thrimidge" },
			{ 30, "Forelithe" },
			{ 1,  "Lithe" },
			{ 1,  "Midyear's Day" },
			{ 1,  "Lithe" },
			{ 30, "Afterlithe" },
			{ 30, "Wedmath" },
			{ 30, "Halimath" },
			{ 30, "Winterfilth" },
			{ 30, "Blotmath" },
			{ 30, "Foreyule" },
			{ 2, "Yule", },
			{ 30, "Afteryule" },
			{ 30, "Solmath" },
			{ 30, "Rethe" },
			{ 6, "Astron" },
		}
	-- Elves use the Rivendell's reckoning calendar
	elseif race == RACE_ELF then
		-- Bilbo birthday year
		time.start_year = 2890

		time.set_calendar
		{
			{ 1, "Yestare", },
			{ 54, "Tuile" },
			{ 72, "Laire" },
			{ 54, "Yavie" },
			{ 3, "Enderi" },
			{ 54, "Quelle" },
			{ 72, "Hrive" },
			{ 54, "Coire" },
			{ 1, "Mettare" },
		}
	-- Others(Human, ..) use the Stewards' reckoning calendar(calendar of Gondor)
	else
		-- Bilbo birthday year
		time.start_year = 2890

		time.set_calendar
		{
			{ 25, "Viresse", 4 },
			{ 30, "Lotesse" },
			{ 30, "Narie" },
			{ 1,  "Loende" },
			{ 30, "Cermie" },
			{ 30, "Urime" },
			{ 30, "Yavannie" },
			{ 1,  "Yaviere" },
			{ 30, "Narquelie" },
			{ 30, "Hisime" },
			{ 30, "Ringare" },
			{ 1,  "Mettare" },
			{ 1, "Yestare", },
			{ 30, "Narvinye" },
			{ 30, "Nenime" },
			{ 30, "Sulime" },
			{ 1,  "Tuliere" },
			{ 5, "Viresse" },
		}
	end
end)
