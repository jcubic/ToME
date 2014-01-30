--
-- Birth subsystem
--

function descriptor_info(i)
	return %descriptor_info[i + 1]
end

-- Handler to generate names
-- Random Name Generator
-- based on a Javascript by Michael Hensley
-- "http://geocities.com/timessquare/castle/6274/"
function birth.create_random_name(race)
	local name_parts = birth.random_names

	if not name_parts[race] then
		race = name_parts.DEFAULT
	end

	local name = ""

	for i = 1, getn(name_parts[race]) do
		local part = name_parts[race][i]
		name = name..part[rng(getn(part))]
	end

	return name
end

function birth.set_random_names(t)
	birth.random_names = t
end


-- Return a list of descriptors of the given type
function birth.grab_descriptors(typ, descriptors)
	local choices = {}
	for i = 1, getn(descriptors) do
		local desc = descriptor_info(descriptors[i])
		for j = 1, flag_max_key(desc.choice) do
			tinsert(choices, desc.choice[j])
		end
	end

	local descs = {}
	for i = 0, max_dp_idx - 1 do
		local desc = descriptor_info(i)
		if desc.type == typ then
			local ok, default_ok = nil, "allow"
			for j = 1, getn(choices) do
				-- Is the rule for this type?
				if choices[j][0] == typ then
					-- Try to find either the name or __ALL__(if no name)
					local allow = choices[j][1]
					local nogood = choices[j][2]
					local never = choices[j][3]
					for z = 1, flag_max_key(allow) do
						if allow[z] == "__ALL__" then default_ok = "allow"
						elseif allow[z] == desc.title then ok = "allow"
						end
					end
					for z = 1, flag_max_key(nogood) do
						if nogood[z] == "__ALL__" then default_ok = "nogood"
						elseif nogood[z] == desc.title then ok = "nogood"
						end
					end
					for z = 1, flag_max_key(never) do
						if never[z] == "__ALL__" then default_ok = "never"
						elseif never[z] == desc.title then ok = "never"
						end
					end
				end
			end

			if not ok then ok = default_ok end
			if ok == "allow" or ok == "ungood" then tinsert(descs, i) end
		end
	end
	return descs
end

-- Setup the birth sequence
function birth.sequence(t)
	birth.__seq = t
end

function birth.print_desc(strs, y, start_x, wid, hgt)
	for i = 1, getn(strs) do
		local str = strs[i]
		local x = 0
		local last_space = 0
		local words = {}

		-- Break down into words
		while not nil do
			local space = strfind(str, " ", last_space + 1)
			if not space then
				tinsert(words, strsub(str, last_space + 1))
				break
			end

			tinsert(words, strsub(str, last_space + 1, space - 1))
			last_space = space
		end

		-- Output words and make sure no words get over the limit
		for j = 1, getn(words) do
			if x + strlen(words[j]) + 1 < wid then
				term.put_str(words[j], y, x + start_x)
				x = x + strlen(words[j]) + 1
			else
				y = y + 1
				term.put_str(words[j], y, start_x)
				x = strlen(words[j]) + 1
			end
		end
		y = y + 1
	end
end

-- Ask teh user to select a descriptor for this type
function birth.descriptor_select_screen(seq, descs)
	local ret, wid, hgt = term.get_size()
	term.clear()
	local names = {}
	local sel, scroll = 1, 1

	while not nil do
		for i = 1, getn(descs) do
			names[i] = strchar(strbyte('a') + i - 1)..iif(sel == i, "> ", ") ")..descriptor_info(descs[i]).title
		end

		term.print(color.YELLOW, seq.title, 0, (wid - strlen(seq.title)) / 2)
		if seq.desc then
			if type(seq.desc) == "string" then seq.desc = { seq.desc } end
			for i = 1, getn(seq.desc) do
				term.display(seq.desc[i], i, 0)
			end
		end
		display_list(5, 0, hgt - 6, 20, book_capitals(seq.descriptor_type), names, scroll, sel, color.LIGHT_GREEN)
		draw_box(5, 21, hgt - 6, wid - 22)
		birth.print_desc(strsplit(descriptor_info(descs[sel]).desc, "\n"), 6, 22, wid - 22)

		local raw = term.inkey()
		local key = strchar(raw)

		if key == '2' and sel < getn(descs) then
			sel = sel + 1
			if sel - scroll > hgt - 5 then scroll = scroll + 1 end
		elseif key == '8' and sel > 1 then
			sel = sel - 1
			if sel < scroll then scroll = scroll - 1 end
		elseif key == '\r' then
			return descs[sel]
		elseif raw == ESCAPE then
			return nil
		else
			local new_sel = strbyte(key) - strbyte('a') - scroll + 2
			if new_sel >= scroll and new_sel < scroll + hgt - 5 and new_sel <= getn(descs)then
				return descs[new_sel]
			end
		end
	end
end

-- Make up the final descriptor upon which the rest of the game will act
function birth.compute_final_descriptor(is_new, ignore)
	local descs = player.descriptors
	local d = player.descriptor

	ignore = ignore or {}

	-- A small helper function
	local add = function(dest, src, field)
		local tmp = dest[field] + src[field]
		dest[field] = tmp
	end

	-- Stats
	for i = 0, stats.MAX do d.adj[i] = 0 end

	-- Titles
	-- cptr titles[PY_MAX_LEVEL / 5];	-- Leveling titles, if any

	d.luck = 0

	d.mana = 0                      -- Mana %
	d.blow_num = 0
	d.blow_wgt = 0
	d.blow_mul = 0
	d.extra_blows = 0

	d.dis = 0		-- disarming
	d.dev = 0		-- magic devices
	d.sav = 0		-- saving throw
	d.stl = 0		-- stealth
	d.srh = 0		-- search ability
	d.fos = 0		-- search frequency
	d.thn = 0		-- combat (normal)
	d.thb = 0		-- combat (shooting)

	d.mhp = 0		-- Race hit-dice modifier
	d.exp = 0		-- Race experience factor

	d.b_age = 0		-- base age
	d.m_age = 0		-- mod age

	d.m_b_ht = 0		-- base height (males)
	d.m_m_ht = 0		-- mod height (males)
	d.m_b_wt = 0		-- base weight (males)
	d.m_m_wt = 0		-- mod weight (males)

	d.f_b_ht = 0		-- base height (females)
	d.f_m_ht = 0		-- mod height (females)
	d.f_b_wt = 0		-- base weight (females)
	d.f_m_wt = 0		-- mod weight (females)

	d.infra = 0             -- Infra-vision range

	flag_empty(d.choice)            -- Legal otehr descriptors choices

	for i = 1, 4 do d.powers[i] = 0 end         -- Powers of the race

	flag_empty(d.body_parts)      -- To help to decide what to use when body changing

	d.chart = "You are a"             -- Chart history

	flag_empty(d.flags)       -- flags

	for i = 1, PY_MAX_LEVEL + 1 do flag_empty(d.oflags[i]) end       -- flags

-- Merging skills&abilities is done latter in a more complex process
--	flag_empty(d.skill_base)
--	flag_empty(d.skill_mod)
--	flag_empty(d.abilities)

--[[
	d.obj_tval[5];
	d.obj_sval[5];
	d.obj_pval[5];
	d.obj_dd[5];
	d.obj_ds[5];
	d.obj_num = 0
]]

	for i = 1, flag_max_key(descs) do
		local desc = descriptor_info(descs[i])

		if not ignore[desc.type] then
			-- Stats
			for j = 0, stats.MAX do
				local a = flag_get(d.adj, j) + flag_get(desc.adj, j)
				d.adj[j] = a
			end

			-- Titles
			-- cptr titles[PY_MAX_LEVEL / 5];	-- Leveling titles, if any

			add(d, desc, "luck")

			add(d, desc, "mana")        -- Mana %
			add(d, desc, "blow_num")
			add(d, desc, "blow_wgt")
			add(d, desc, "blow_mul")
			add(d, desc, "extra_blows")

			add(d, desc, "dis")	   -- disarming
			add(d, desc, "dev")	   -- magic devices
			add(d, desc, "sav")	   -- saving throw
			add(d, desc, "stl")    -- stealth
			add(d, desc, "srh")    -- search ability
			add(d, desc, "fos")    -- search frequency
			add(d, desc, "thn")    -- combat (normal)
			add(d, desc, "thb")    -- combat (shooting)

			add(d, desc, "mhp")    -- Race hit-dice modifier
			add(d, desc, "exp")    -- Race experience factor

			add(d, desc, "b_age")  -- base age
			add(d, desc, "m_age")  -- mod age

			add(d, desc, "m_b_ht") -- base height (males)
			add(d, desc, "m_m_ht") -- mod height (males)
			add(d, desc, "m_b_wt") -- base weight (males)
			add(d, desc, "m_m_wt") -- mod weight (males)

			add(d, desc, "f_b_ht") -- base height (females)
			add(d, desc, "f_m_ht") -- mod height (females)
			add(d, desc, "f_b_wt") -- base weight (females)
			add(d, desc, "f_m_wt") -- mod weight (females)

			add(d, desc, "infra") -- Infra-vision range

			-- Legal other descriptors choices
			flag_add(d.choice, desc.choice)

			-- Powers of the race
			for i = 1, 4 do d.powers[i] = 0 end

			-- To help to decide what to use when body changing
			flag_add_increase(d.body_parts, desc.body_parts)

			d.chart = d.chart .. " " .. desc.title

			flag_add(d.flags, desc.flags)       -- flags

			-- flags
			for i = 1, PY_MAX_LEVEL + 1 do
				flag_add(d.oflags[i], desc.oflags[i])
			end

-- Merging skills&abilities is done latter in a more complex process(see compute_skills in skills.c)
--		flag_add(d.skill_base, desc.skill_base)
--		flag_add(d.skill_mod, desc.skill_mod)
--		flag_add(d.abilities, desc.abilities)

--[[
		d.obj_tval[5];
		d.obj_sval[5];
		d.obj_pval[5];
		d.obj_dd[5];
		d.obj_ds[5];
		d.obj_num = 0
]]
		end -- if not ignore[desc.type] then
	end

	if is_new then flag_copy(player.flags, d.flags) end
end

-- Fill up the player structure
function birth.make_player()
	hook.process(hook.BIRTH_PRE)

	-- Level one
	player.max_plv = 1
	player.lev = 1

	-- Experience factor
	player.expfact = player.descriptor.exp

	-- Initialize arena and rewards information -KMW-
	player.arena_number = 0
	player.inside_arena = 0
	player.inside_quest = 0
	player.exit_bldg = true  -- only used for arena now -KMW-

	-- Get luck
	player.luck_base = player.descriptor.luck + rng(-5, 5)
	player.luck_max = player.luck_base

	-- Hitdice
	player.hitdie = player.descriptor.mhp

	-- Initial hitpoints
	if player.mhp then player.mhp(player.hitdie) end

	-- Minimum hitpoints at highest level
	local min_value = (PY_MAX_LEVEL * (player.hitdie - 1) * 3) / 8
	min_value = min_value + PY_MAX_LEVEL

	-- Maximum hitpoints at highest level
	local max_value = (PY_MAX_LEVEL * (player.hitdie - 1) * 5) / 8
	max_value = max_value + PY_MAX_LEVEL

	-- Pre-calculate level 1 hitdice
	player_hp[1] = player.hitdie

	-- Roll out the hitpoints
	while not nil do
		if PY_MAX_LEVEL == 1 then break end
		if player.hitdie < 1 then break end

		-- Roll the hitpoint values
		for i = 2, PY_MAX_LEVEL do
			local j = rng(player.hitdie)
			player_hp[i] = player_hp[i - 1] + j
		end

		-- XXX Could also require acceptable "mid-level" hitpoints

		-- Require "valid" hitpoints at highest level
		if (player_hp[PY_MAX_LEVEL] >= min_value) and (player_hp[PY_MAX_LEVEL] <= max_value) then break end
	end

        -- Age
	player.age = player.descriptor.b_age + rng(player.descriptor.m_age);

	-- Height
	-- forget male/female for now ... DGDGDGDGDG
	player.ht = rng.normal(player.descriptor.m_b_ht, player.descriptor.m_m_ht)

	-- Weight
	-- forget male/female for now ... DGDGDGDGDG
	player.wt = rng.normal(player.descriptor.m_b_wt, player.descriptor.m_m_wt)

	-- Dungeons
	current_dungeon_idx = get_module_info("base_dungeon") or 0
	player.recall_dungeon = current_dungeon_idx
	max_dlv[current_dungeon_idx + 1] = dungeon(current_dungeon_idx).mindepth

	-- Stats at 10
	for i = 0, stats.MAX do
		player.stat_ind[i] = 10
		player.stat_cur[i] = 10
		player.stat_max[i] = 10
	end

	-- Reset timed effects
	timed_effect.reset()

	-- Reset counters
	counter.reset()

	-- Reset cooldowns
	player.spells_cooldown = {}

	-- Reset quest data
	local tbl = {}
	for k, _ in __quest_data do tbl[k] = {} end
	__quest_data = tbl

	-- Gold!!!
	player.au = rng(100, 300)

	-- Options
	player.maximize = game_options.maximize
	player.preserve = game_options.preserve
	player.special = true

	-- Select combat style
	player.combat_style = get_combat_skills()[1].skill

	-- Provide with some objects
	for i = 1, flag_max_key(player.descriptors) do
		local desc = descriptor_info(player.descriptors[i])

		for j = 1, flag_max_key(desc.objects) do
			local obj
			local data = desc.objects[j]
			local min, max = data[1], data[2]
			local obj_maker = data[0]
			if obj_maker <= 0 then
				local fct = get_function_registry("descriptor_info", -obj_maker)
				obj = new_object()
				fct(obj)
			else
				obj = new_object()
				object_prep(obj, obj_maker)
			end
			obj.found  = OBJ_FOUND_START
			obj.number = rng(min, max)

			-- x_attr fields in the k_info array aren't set until
			-- after birth; weird
			obj.x_attr = obj.d_attr

			make_item_fully_identified(obj)
			inven_carry_inven(player.inventory[INVEN_INVEN], obj, false)
		end
	end
end

-- IT'S ALIVE !!! IT'S ALIVE!
hook.new_hook_type("BIRTH_PRE")
function birth.be_born()
	local descriptors = {}
	for i = 1, getn(birth.__seq) do
		local seq = birth.__seq[i]
		-- Descriptor selection
		if seq.descriptor_type then
			local descs = birth.grab_descriptors(seq.descriptor_type, descriptors)
			-- One choice only, do not bother the user with it
			if getn(descs) == 1 then tinsert(descriptors, descs[1])
			-- More than one, let her/him choose
			elseif getn(descs) > 1 then
				local d = birth.descriptor_select_screen(seq, descs)
				if not d and i > 1 then
					-- Restart
					return birth.be_born()
				else
					tinsert(descriptors, d)
				end
			end

		-- Some predefined actions
		elseif seq.action then
			if seq.action == "finalize" then
				-- Compute the final descriptor
				for j = 1, getn(descriptors) do player.descriptors[j] = descriptors[j] end
				birth.compute_final_descriptor(true)

				-- Setup the player
				birth.make_player()

				-- Compute skills
				player_birth_skill()

				-- This means everything will be updated
				player.update = 4294967295
				update_stuff()
			elseif seq.action == "random_name" then
				if player_name() == '' or player_name() == 'PLAYER' then
					player_name(birth.create_random_name(player.get_descriptor(seq.random_name_descriptor).title))
				end
			elseif seq.action == "ask_name" then
				character_sheet.display(1)
				while not nil do
					local new_name = input_box("Character name?", 40, player_name())
					if new_name ~= "" then
						if globals()["TOME_BABY_INTERFACE"] then
							local old_savefile = savefile()
							birth.process_player_name(new_name)
							if birth.load_from_dead then
								-- Rename the savefile to the new name
								local new_savefile = savefile()
								-- If the target exists we just do not rename it, it still works fine
								if not fs.file_exist(new_savefile) then
									fs.rename(old_savefile, new_savefile)
								end
							end
						end
						player_name(new_name)
						break
					end
				end
				character_sheet.show(true, true)
			end

		-- Custom screen
		else
			if seq.custom then
				seq.custom()
			end
		end
	end

	-- Reset counters
	counter.reset()

	hook.process(hook.BIRTH)

	return true
end

hook(hook.INIT_DONE,
function()
	-- Where the player starts
	if has_flag(player, FLAG_STARTING_LEVEL) then
		dun_level     = player.flags[FLAG_STARTING_LEVEL]
		object_level  = dun_level
		monster_level = dun_level
	end

	if has_flag(player, FLAG_STARTING_DUNGEON) then
		local dungeon = player.flags[FLAG_STARTING_DUNGEON]

		if type(dungeon) == "string" then
			dungeon = globals()[dungeon]
		end

		current_dungeon_idx = dungeon
	end
end)
