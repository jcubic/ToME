-- Dialog system engine

constant("dialog", {})

dialog.__next = 1
dialog.__registered = {}

function dialog.sub(t)
	assert(t.say, "no dialog say")
	assert(t.answers, "no dialog answers")

	for k, a in t.answers do
		if type(a) == "string" then t.answers[k] = { a, dialog.stop, function() return true end }
		elseif not a[3] then a[3] = function() return true end
		end
	end

	t.__index__ = dialog.__next
	dialog.__registered[t.__index__] = t
	dialog.__next = dialog.__next + 1

	return t
end

function dialog.new(t)
	return dialog.sub(t).__index__
end

function dialog.stop()
	return nil
end

function dialog.exec(who, act, screen, args)
	message.add(1, player_name()..": #w"..act[1], color.LIGHT_BLUE)
	if type(act[2]) == "function" then
		term.load_from(screen, true)
		return call(act[2], args)
	elseif type(act[2]) == "table" then
		term.load_from(screen, false)
		dialog.display(who, act[2], act[1], nil, unpack(args))
	else
		error("unimplemented answer action type "..type(act[2]))
	end
end

function dialog.display_aux(who, d, old)
	term.gotoxy(1, 0)
	if old then
	        term.text_out(color.LIGHT_BLUE, player_name()..": ")
		term.text_out(old)
		term.text_out("\n")
	end
        term.text_out(color.LIGHT_BLUE, who..": ")
	term.text_out(d.say)
	term.text_out("\n")
	term.text_out("\n")
	for k, a in d.answers do
		term.text_out(k..") "..a[1].."\n")
	end
end

function dialog.prune(d)
	local new =
	{
		say = d.say
		answers = {}
	}

	local idx = 1
	for k, a in d.answers do
		if a[3]() then
			new.answers[idx] = a
			idx = idx + 1
		end
	end
	return new
end

function dialog.display(who, d, old, screen, ...)
	if tag(d) == TAG_STRING then
		d = dostring("return "..d)
	elseif tag(d) == TAG_NUMBER then
		d = dialog.__registered[d]
	end
	screen = screen or term.save_to()

	-- Prune forbiden answers
	d = dialog.prune(d)

	message.add(1, who..": #w"..d.say, color.LIGHT_BLUE)
	dialog.display_aux(who, d, old)

	while not nil do
		local c = strchar(term.inkey())

		if c == strchar(ESCAPE) then
			break
		elseif c >= '1' and c <= tostring(getn(d.answers)) then
			if not dialog.exec(who, d.answers[tonumber(c)], screen, arg) then return end
		end
	end

	term.load_from(screen, true)
end

function dialog.on_quest_status(q, ...)
	return function()
		local statuses = %arg
		for i = 1, getn(statuses) do
			if quest(%q).status == statuses[i] then return true end
		end
		return false
	end
end
