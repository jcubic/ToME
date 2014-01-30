-- Colors handling

color.__next = 17

function color.new(name, char, default, r, g, b)
	if color.can_redefine_color then
		color.init_tome_color(color.__next, name, strbyte(char), r, g, b)
		color[name] = color.__next
		color.__next = color.__next + 1
		if color.__next == 256 then
			quit("Too many colors defined!(max 256)")
		end
	else
		color[name] = default
	end
end

function color.test()
	-- Make up the reverse looking table
	local tbl = {}
	for k, e in color do
		if tag(k) == TAG_STRING and tag(e) == TAG_NUMBER then
			tbl[e] = k
		end
	end

	local screen = term.save_to()
	local sel = 0
	while true do
		local y = 0
		term.clear()
		for i = sel, color.__next - 1 do
			if i ~= 16 then
				term.print(format("%024s", tbl[i]..":"), y, 0)
				term.print(i, "###############################################", y, 25)
				y = y + 1
			end
		end
		local key = term.inkey()
		local c = strchar(key)
		if key == ESCAPE then
			break
		elseif c == "8" then
			-- Up key
			sel = sel - 1
			if sel < 0 then sel = 0 end
		elseif c == "2" then
			-- Down key
			sel = sel + 1
		end
	end
	term.load_from(screen)
end
