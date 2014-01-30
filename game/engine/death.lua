--
-- Ohh the player died .. too bad
-- Well ask for some char sheet, register scores, whatever
--

constant("deathface", {})

deathface.EXIT = 1

hook.new_hook_type("PLAYER_DEATH")

deathface.background =
{
[[#G ____    __     _  _     ____     ]]
[[#G(_  _)  /  \   ( \/ )   (  __)    ]]
[[#G  )(  _(  O )_ / \/ \ _  ) _)  _      #yYou have died it seems.]]
[[#G (__)(_)\__/(_)\_)(_/(_)(____)(_)     #yThat is bad, but you can try again!]]
[[                                    ]]
[[and the #rT#w-#REngine                  ]]
}

function deathface.char_sheet()
	local name = input_box("Save character sheet?", 40, player_name())
	if name == "" then
		return
	end
	character_sheet.write_to_file(name)
end

function deathface.show()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local exec_menu = {}
	for i = 1, getn(deathface.menu) do exec_menu[deathface.menu[i][1]] = deathface.menu[i][3] end

	while not nil do
		term.clear()
		for i = 1, getn(deathface.background) do tbaby.display(deathface.background[i], i - 1, 0) end
		tbaby.menu(deathface.menu, sel, hgt)

		local raw = term.inkey()
		local key = strchar(raw)

		local ret, aux = nil
		if key == '2' then
			sel = sel + 1
			if sel > getn(deathface.menu) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(deathface.menu) end
		elseif key == '\r' then
			ret, aux = deathface.menu[sel][3]()
		elseif exec_menu[key] then
			ret, aux = exec_menu[key]()
		end
		if ret then
			if ret == deathface.EXIT then return nil end
		end
	end
end

function player.on_died_show_info()
	hook.process(hook.PLAYER_DEATH)
	if not noscore then
		local s, desc = player.register_score()
		deathface.add_score{score=s desc=desc}
	end
	deathface.show()
end


------ Score
deathface.score_background = deathface.background

function deathface.add_score(score)
	local name, id = nil, 0
	repeat
		name = score.score.."."..id
		id = id + 1
	until not fs.file_exist("/scores/score."..name)
	local fff = fs.open("/scores/score."..name, "w")
	if fff then
		fs.write(fff, score.desc)
		fs.close(fff)
		deathface.__score_recently_added = true
	end
end

function deathface.get_scores()
	local scores = {}
	local files = fs.enumerate_files("/scores/", false)
	for file in files do
		local a, b, score = strfind(file, "^score\.([^.]*)\..*$")
		if a and b and score then
			local fff = fs.open("/scores/"..file, "r")
			if fff then
				local desc = fs.read(fff)
				if desc then
					local last_modified = fs.last_modified("/scores/"..file)
					tinsert(scores, {score=tonumber(score) desc=desc last_modified=last_modified})
				end
				fs.close(fff)
			end
		end
	end

	-- Find the latest
	if deathface.__score_recently_added then
		local latest_id, latest_date = -1, -1
		for i = 1, getn(scores) do
			if latest_date < scores[i].last_modified then
				latest_date = scores[i].last_modified
				latest_id = i
			end
		end
		if latest_id >= 1 then scores[latest_id].latest = true end
	end

	-- Sort by score
	sort(scores, function(a, b) return a.score > b.score end)
	return scores
end

function deathface.show_score()
	local score = deathface.get_scores()
	if getn(score) == 0 then
		msg_box("No score to show")
		return
	end

	local ret, wid, hgt = term.get_size()
	local sel = 1

	while not nil do
		term.clear()
		local y = 0
		for i = 1, getn(deathface.score_background) do tbaby.display(deathface.score_background[i], i - 1, 0) y = i end
		local max = min(sel + hgt - 2 - y, getn(score))
		for i = sel, max do
			tbaby.display(score[i].latest and color.LIGHT_BLUE or color.LIGHT_GREEN, format("%2d: [%10d] %s", i, score[i].score, score[i].desc), y + i - sel, 0)
		end
		tbaby.display("Press escape to exit the score list", hgt - 1, wid / 2  - 18)

		local raw = term.inkey()
		local key = strchar(raw)

		local ret, aux = nil
		if raw == ESCAPE then
			break
		elseif key == '2' then
			sel = sel + 1
			if sel > getn(score) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(score) end
		end
	end
end

function player.show_score()
	deathface.show_score()
end

--- @fn
-- @brief Registers the score of a dead character. (can and should be overwriten by modules)
function player.register_score()
	local score = player.exp
	local desc = player_name().." (Level "..player.lev..", Experience "..player.exp..")"
	return score, desc
end

-- And last but not least the menu, this can be overwriten by modules
deathface.menu =
{
	{ 'a', "Character sheet", deathface.char_sheet }
	{ 'b', "Show score", deathface.show_score }
	{ 'e', "Exit", function() return deathface.EXIT end }
}
