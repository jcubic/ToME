-- Mindcraft, new version
-- Allows to learn some school spells "in your mind"
-- to be cast when blind/out of lite/without any book

constant("mindcraft", {})

mindcraft.cast_check = function(s, s_ptr)
	-- Not when confused
	if (check_affect(s, "confusion")) and player.has_intrinsic(FLAG_CONFUSED) then
		message("You are too confused!")
		return false
	end
        return true
end

add_loadsave("player.mindcraft_spells", {})
player.mindcraft_spells = {}
hook(hook.BIRTH, function() player.mindcraft_spells = {} end)

add_mkey
{
        mkey	= "Mindcraft",
	type	= "skill",
        fct	= function()
                local book = make_book(unpack(player.mindcraft_spells))

		local spl = nil
		local ret, rep = repeat_keys.pull()
		if ret then
			if is_ok_spell(rep, book) then
				spl = rep
			end
		end

                -- select a spell from that book
                mindcraft.in_use = true
                spl = spl or get_school_spell("cast", "is_ok_spell", book)
                mindcraft.in_use = false

                -- and delete it.
                delete_object(book)

                if spl == -1 then return end

                -- Now to actually cast the spell...
                mindcraft.in_use = true
                cast_school_spell(spl, spell(spl), nil, mindcraft.cast_check)
                mindcraft.in_use = false
        end,
}

function mindcraft.learn(to_learn)
	local schools = {}
        local schools_desc = {}
        local found = false

        -- Compute which spells are usable
	for i = 0, __tmp_spells_num - 1 do
        	local spl = spell(i)
                local ok = true
                for j = 1, getn(spl.school) do
                	if not school(spl.school[j]).mindcraft then
                        	ok = false
                                break
                        end
                        if spl.skill_level > (get_skill_scale(SKILL_MINDCRAFT, 50) * school(spl.school[j]).mindcraft.mul) / school(spl.school[j]).mindcraft.div then
				ok = false
                                break
                        end
                end
                for j = 1, getn(player.mindcraft_spells) do
                	if player.mindcraft_spells[j] == i then ok = false break end
                end
                if ok then
                	found = true
	                for j = 1, getn(spl.school) do
                        	if not schools[spl.school[j]] then schools[spl.school[j]] = {} end
                                tinsert(schools[spl.school[j]], i)
                	end
                end
        end
        -- If none, then abord
        if not found then return end

        -- Sort spells/schools
        for sch, spells in schools do
        	sort(spells, function(a, b) return spell(a).skill_level < spell(b).skill_level end)
                tinsert(schools_desc, { school=sch })
        end
        sort(schools_desc, function(a, b) return school(a.school).name < school(b.school).name end)

        -- And display everything
	local old_term = term.save_to()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local scroll = 1
	while not nil do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)
		draw_box(0, 0, 4, wid - 1)
		term.c_put_str(color.LIGHT_BLUE, " Mindcraft spell screen ", 0, (wid - 22) / 2)
       		term.display("#GYou can learn "..to_learn.." new mindcraft spell(s).", 1, 1)
       		term.display("Select the school you wish to randomly gain a spell from.", 2, 1)

		display_list(4, 0, hgt - 5, 20, "Schools to learn", schools_desc, scroll, sel, color.LIGHT_GREEN, function(e)
			return school(e.school).name
		end)

		draw_box(4, 21, hgt - 5, wid - 22)
		term.c_put_str(color.LIGHT_BLUE, " "..school(schools_desc[sel].school).name.." ", 4, 23)

		local y = 5
       		term.display("In this school you could learn:", y, 22)
                y = y + 1
                for i = 1, getn(schools[schools_desc[sel].school]) do
			term.display(" - "..spell(schools[schools_desc[sel].school][i]).name, y + i - 1, 22)
                end

		local raw = term.inkey()
		local key = strchar(raw)

		if key == '2' then
			repeat
				sel = sel + 1
				if sel > getn(schools_desc) then sel = getn(schools_desc) end
				if sel >= scroll + hgt then scroll = scroll + 1 end
			until sel ~= getn(schools_desc) + 1
		elseif key == '8' then
			repeat
				sel = sel - 1
				if sel < 1 then sel = 1 end
				if sel < scroll then scroll = scroll - 1 end
			until sel ~= getn(schools_desc) + 1
		elseif key == '\r' then
                	local s = schools[schools_desc[sel].school][rng(getn(schools[schools_desc[sel].school]))]
                        msg_box("You learn the spell: "..spell(s).name..".")
                        tinsert(player.mindcraft_spells, s)
                        sort(player.mindcraft_spells, function(a, b) return spell(a).skill_level < spell(b).skill_level end)
                        break
		end
	end

	term.load_from(old_term, true)
        return true
end

-- Learn spells
hook(hook.RECALC_SKILLS, function()
	if get_skill(SKILL_MINDCRAFT) < 1 then return end

	local nb = 1 + (get_skill_scale(SKILL_MINDCRAFT, 50) / 3)
        local cur = getn(player.mindcraft_spells)
        while nb > cur do
        	if not mindcraft.learn(nb - cur) then
                	break
                end
                cur = getn(player.mindcraft_spells)
        end
end)
