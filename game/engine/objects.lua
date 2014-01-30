-- SYSTEM FILE
--
-- Lua object funtions
--

declare_global_constants {
	"compute_slot",
	"item_slot_to_inven",
	"item_slot_to_item",
	"create_object",
--	"create_artifact",
	"object_make_idx",
	"get_item",
	"default_item_extra_hook",
	"__default_item_extra_hook_aux",
	"get_item_multi",
	"get_kind",
	"set_item_tester",
	"set_get_obj_num_hook",
	"inven_carry",
	"for_inventory",
	"for_inventory_inven",
	"__core_objects",
	"generate_item_floor_index",
	"armor_dams_to_str",
	"object_desc",
	"desc_obj",
	"define_objects_categorization",
	"get_max_tval",
	"get_max_sval",
	"bulk_item_increase",
	"is_inven_empty",
	"sval_to_str",
	"tval_to_str",
	"is_aware",
	"set_aware",
	"is_tried",
	"set_tried",
	"add_multiflav_tval",
	"is_multiflav_tval",
	"get_multiflav_tvals",
	"multiflav_id_to_name",
	"get_aware_multiflav_ids",
}

declare_globals {
	"__get_item",
	"__get_item_results",
	"__get_item_hook_default",
	"__get_obj_num_hook_default",
	"get_item_extra_hook_lua",
	"object_desc_tvals",
	"object_desc_details",
	"__sval_maxes",
	"__sval_strings",
	"__tval_max",
	"__tval_strings",
	"objects_on_make_all",
	"objects_on_make_all_pre",
	"objects_get_power_level",
	"combat_dam_types",
	"__multiflav",
}

combat_dam_types = {}

function object_prep(obj, k_idx, sval)
	if sval then
		%object_prep(obj, lookup_kind(k_idx, sval))
	else
		%object_prep(obj, k_idx)
	end
end

function create_object(tval, sval)
	local obj = new_object()
	object_prep(obj, lookup_kind(tval, sval))
	return (obj)
end

__get_item_hook_default = 0
function set_item_tester(tester)
	if tolua.type(tester) == "number" then
		lua_set_item_tester(tester, "")
	end
	if tolua.type(tester) == "string" then
		lua_set_item_tester(0, tester)
	end
	if tolua.type(tester) == "function" then
		__get_item_hook_default = tester
		lua_set_item_tester(0, "__get_item_hook_default")
	end
end

__get_obj_num_hook_default = 0
function set_get_obj_num_hook(tester)
	__get_obj_num_hook_default = tester
	lua_set_get_obj_num_hook("__get_obj_num_hook_default")
end

function create_artifact(a_idx)
	local art = a_info[a_idx + 1]
	local obj
	local tval, sval

	tval = art.tval
	sval = art.sval

	obj = create_object(tval, sval)
	obj.artifact_id = a_idx
	apply_magic(obj, -1, true, true, true)

	return (obj)
end

function get_kind(obj)
	return k_info[obj.k_idx + 1]
end

function get_item(ask, deny, flags, mask, tag)
	set_item_tester(mask)

	local item = __get_item(ask, deny , flags, tag)

	if item then
		return true, item
	else
		return nil
	end
end

function get_item_multi(ask1, ask2, deny, max_items, max_per_stack,
						flags, mask, tag)
	local question   = ask1
	local item_list  = {}

	if type(tag) == "number" then
		tag = strchar(tag)
	end

	while max_items > 0 do
		-- Exclude items which have already been picked
		local filter = function(obj)
						   for i = 1, getn(%item_list) do
							   if obj == %item_list[i].obj then
								   return false
							   end
						   end

						   return %mask(obj)
					   end

		set_item_tester(filter)

		local item = __get_item(question, deny , flags, tag)

		if not item then
			return item_list
		end

		local obj = get_object(item)

		if obj.number <= 0 then
			return item_list
		end

		if check_prevent_cmd(obj, tag) then
			return {}
		end

		local stack_max = min(max_items, max_per_stack)
		stack_max = min(obj.number, stack_max)

		local number

		if stack_max == 1 then
			number = 1
		else
			local prompt = "Number of " .. object_desc(obj) ..
				" (1 - " .. stack_max .. ")? "

			local num_str = get_string(prompt)

			if not num_str or strlen(num_str) == 0 then
				return item_list
			end

			number = tonumber(num_str)

			if not number or number <= 0 then
				return item_list
			end
		end

		max_items = max_items - number

		local item_info = {item = item, number = number, obj = obj}
		tinsert(item_list, item_info)

		question = ask2
		deny     = nil
	end -- while max_items > 0 do

	return item_list
end -- get_item_multi()


function compute_slot(inven, item)
	return (inven * INVEN_MAX_SIZE) + item
end

function item_slot_to_inven(slot)
	return slot / INVEN_MAX_SIZE
end
function item_slot_to_item(slot)
	return imod(slot, INVEN_MAX_SIZE)
end

-- A few usefull for all helpers

-- parchement helper
constant("display_parchement",
function (obj)
	term.save()
	show_file("book-"..obj.sval..".txt", 0, 0, 0)
	term.load()
	return false, true
end)

-- parchement helper
constant("display_map_parchement",
function (obj)
	local n = tonumber(get_line("book-"..obj.sval..".txt", TENGINE_DIR_FILE, -1))

	-- Parse all the fields
	for i = 0, n - 1, 4 do
		-- Grab the fields
		local x = tonumber(get_line("book-"..obj.sval..".txt", TENGINE_DIR_FILE, i + 0))
		local y = tonumber(get_line("book-"..obj.sval..".txt", TENGINE_DIR_FILE, i + 1))
		local w = tonumber(get_line("book-"..obj.sval..".txt", TENGINE_DIR_FILE, i + 2))
		local h = tonumber(get_line("book-"..obj.sval..".txt", TENGINE_DIR_FILE, i + 3))

		reveal_wilderness_around_player(y, x, h, w)
	end
	message("You carefully take note of this map.")
	return false, true
end)

-- food helper
constant("eat_food_good",
function (obj)
	message("That tastes good.")
	return true
end)


-----------------------------------------------------------
--------------------- Inventory code ----------------------
-----------------------------------------------------------

function inven_carry(obj, final)
	return inven_carry_inven(player.inventory[INVEN_INVEN], obj, final)
end

-- From start to stop
function for_inventory(src, start, stop, fct)
	for i = start, stop - 1 do
		local inven = src.inventory[i]
		local max = flag_max_key(inven)
		for j = 1, max do
			local obj = inven[j]
			if obj and obj.k_idx ~= 0 then fct(obj, i, j, compute_slot(i, j)) end
		end
	end
end

function for_inventory_inven(inven, fct)
	local max = flag_max_key(inven)
	for i = 1, max do
		local obj = inven[i]
		if obj then fct(obj, i) end
	end
end

__core_objects = {}

function __core_objects.get_item_letter_color(o_ptr)
	local c = color.LIGHT_WHITE
	local conv =
	{
		[color.LIGHT_WHITE] = color.WHITE,
		[color.BLUE] = color.LIGHT_BLUE,
		[color.ORANGE] = color.YELLOW,
		[color.GREEN] = color.LIGHT_GREEN,
		[color.VIOLET] = color.VIOLET,
	}

	-- Must have knowlegde
	if (not is_known(o_ptr)) then return (color.LIGHT_DARK) end

	if is_ego(o_ptr) then c = color.BLUE end
	if is_artifact(o_ptr) then c = color.ORANGE end
	if (o_ptr.artifact_id > 0) and (-1 ~= a_info[1 + o_ptr.artifact_id].set) then c = color.GREEN end
	if has_flag(o_ptr, FLAG_ULTIMATE) then c = color.VIOLET end

	-- *id*
	if (o_ptr.ident & IDENT_MENTAL) ~= 0 then c = conv[c] end

	return c
end

-- Helper functions to stop printing at a certain size
function __core_objects.term_print(y_start, max_y, ...)
	if arg[getn(arg)] < 0 then arg[getn(arg)] = 0 end
	if arg[getn(arg) - 1] > y_start and arg[getn(arg) - 1] <= max_y then
		call(term.print, arg)
	end
end
function __core_objects.term_blank_print(y_start, max_y, ...)
	if arg[getn(arg)] < 0 then arg[getn(arg)] = 0 end
	if arg[getn(arg) - 1] > y_start and arg[getn(arg) - 1] <= max_y then
		call(term.blank_print, arg)
	end
end

-- Number of items matching the item filter
function __core_objects.num_items_match(inventory, start, stop)
	local number = 0

	for inven_nb = start, stop do
		local inven    = inventory[inven_nb]
		local max_size = flag_max_key(inven)

		for i = 1, max_size do
			local obj = inven[i]

			if obj and item_tester_okay(obj, compute_slot(inven_nb, i)) then
				number = number + 1
			end
		end
	end

	return number
end -- __core_objects.num_items_match()

-- Display the inventory from start to stop
-- This is NOT limited to 23 items
-- start/stop are the INVEN_foo
function __core_objects.display_inventory(inventory, start, stop, mirror, everything, force_inven_color, force_item_color, dont_show_choices, scroll_start, full_obj_show, show_scrollbar, x_start, y_start, wid, wid_scrollbar, hgt, extra_display_fct, weight_per_obj)
	local first, last = nil, nil
	local ret, real_wid, real_hgt = term.get_size()
	if scroll_start == nil then scroll_start=1 end
	local y = 2 - scroll_start
	local max_size
	local scroll_start_inven, scroll_start_item = -1, -1
	local key_table = { invens={} items={} }
	local last_displayed = 1

	hgt = hgt or (real_hgt - 1)
	wid = wid or real_wid
	wid_scrollbar = wid_scrollbar or (wid - 1)
	y_start = y_start or 0
	x_start = x_start or 0

	y = y + y_start

	if not inventory then inventory = player.inventory end

	for inven_nb = start, stop do
		local inven = inventory[inven_nb]
		max_size = flag_max_key(inven)

		-- Find the first to display
		if (scroll_start_inven == -1) and (y > y_start) then
			scroll_start_inven = inven_nb
			scroll_start_item = 1
		end

		local key = strchar(strbyte('a') + inven_nb - start - (scroll_start_inven - 1))
		local color = color.WHITE

		-- Compute and save the key to index table
		key_table.invens[key] = inven_nb
		key_table.last_inven = key

		if force_inven_color then color = force_inven_color end
		if not dont_show_choices and start < stop then
			local nb = ""
			if inven[INVEN_LIMIT_KEY] > 1 then nb = "("..inven[INVEN_LIMIT_KEY]..")" end

			-- Anything at all to display ?
			if inven[INVEN_LIMIT_KEY] > 0 then
				__core_objects.term_blank_print(y_start, hgt, "", y, 0 + x_start)
				__core_objects.term_print(y_start, hgt, color,key.."> "..inventory_slot_names[inven_nb + 1].name..nb, y, 0 + x_start)
				y = y + 1
			end
		end
		for i = 1, max_size do
			-- Find the first to display
			if (scroll_start_item == -1) and (y > y_start) then scroll_start_item = i end

			local obj = inven[i]

			-- Dont display nothings
			if obj then

				-- Get the various informations about it
				local desc = object_desc(obj, true, 3, full_obj_show)
				local color = tval_to_attr[obj.tval + 1]
				local letter_color = __core_objects.get_item_letter_color(obj)
				local letter = strchar(strbyte('a') + i - scroll_start_item)
				if force_item_color and max_size > 1
				then color = force_item_color; letter_color = force_item_color end

				-- Blank the line
				if (not dont_show_choices) and item_tester_okay(obj, compute_slot(inven_nb, i)) then
					if max_size == 1 and not force_inven_color then
						y = y - 1

						-- Display the item
						__core_objects.term_print(y_start, hgt, ":", y, 17 + x_start)
						__core_objects.term_print(y_start, hgt, color, desc, y, 19 + x_start)
					else
						__core_objects.term_blank_print(y_start, hgt, "", y, 0 + x_start)

						-- Display the letter
						__core_objects.term_print(y_start, hgt, letter_color, letter..")", y, 1 + x_start)

						-- Display the item
						__core_objects.term_print(y_start, hgt, color, desc, y, 4 + x_start)
					end
					local weight = obj.weight
					if not weight_per_obj then
						weight = weight * obj.number
					end
					__core_objects.term_print(y_start, hgt, format("%10s", (weight / 10).."."..imod(weight, 10).." lb"), y, wid - 12)
					if extra_display_fct and y <= hgt and y > y_start then extra_display_fct(obj, color, y) end

					-- Update the last displayed index
					if y > y_start and y <= hgt then
						last_displayed = i - scroll_start_item
					end

					y = y + 1
				end

				-- Note the first and lastn, to give a letetr range
				if item_tester_okay(obj, compute_slot(inven_nb, i)) then
					if not first then first = i end
					last = i

					-- Compute and save the key to index table
					key_table.items[letter] = i
					key_table.last_item = letter
				end

			end
		end
	end

	-- Display the slider bar if needed
	if max_size - 1 > hgt - (y_start + 1) then
		for i = y_start + 1, hgt do
			if i == y_start + 1 then term.print(color.YELLOW, '^', i, wid_scrollbar)
			elseif i == hgt then term.print(color.YELLOW, 'v', i, wid_scrollbar)
			else term.print(color.YELLOW, '|', i, wid_scrollbar)
			end
		end
		local max = max_size - (hgt - y_start - 1) -1
		if max <= 0 then max = 1 end
		term.print(color.YELLOW, '*', y_start + 2 + (((scroll_start - 1) * (hgt - (y_start + 1) - 2)) / max), wid_scrollbar)
	end

	-- Return some usefull values, yes this is an usefull comment ;)
	return y, max_size - 1, first, last, last_displayed, key_table
end

function __default_item_extra_hook_aux(mode_table, tag, match_name,
									   ignore_egos, strip_parens, ignore_tag)
	tag = "!" .. tag

	local desc_mode
	if ignore_egos then
		-- No pluralization, plus no egos.
		desc_mode = -2
	else
		-- Just no pluralization.
		desc_mode = -1
	end

	for mode_name, mode in mode_table do
		local inventory, start, stop = mode.inven, mode.start, mode.stop

		for inven_nb = start, stop do
			local inven    = inventory[inven_nb]
			local max_size = flag_max_key(inven)

			for i = 1, max_size do
				local obj = inven[i]

				if obj and
					item_tester_okay(obj, compute_slot(inven_nb, i))
				then
					local note = ""

					if not ignore_tag and obj.note > 0 then
						note = quark_str(obj.note)
					end

					if not strfind(note, "!*", 1, 1) and
						not strfind(note, tag, 1, 1)
					then
						local desc = object_desc(obj, 0, desc_mode)
						desc = strlower(desc)

						if strip_parens then
							desc = gsub(desc, "\([^\)]*\)", "")
							desc = gsub(desc, "\[[^\]]*\]", "")
							desc = gsub(desc, "{[^\}]*}", "")
							desc = gsub(desc, "<[^\>]*>", "")
							desc = clean_whitespace(desc)
						end

						if match_name == desc then
							return true, compute_slot(inven_nb, i)
						end
					end
				end -- if obj and item_tester_okay(...)
			end -- for i = 1, max_size do
		end -- for inven_nb = start, stop do
	end -- for mode_name, mode in mode_table do
end -- __default_item_extra_hook_aux()

get_item_extra_hook_lua = 0
function default_item_extra_hook(mode_table, tag)
	local str = get_string("Which item? ")
	if not str or str == "" then
		return nil, nil
	end

	str = clean_whitespace(strlower(str))
	if str == "" then
		return nil, nil
	end

	local params = {
		{false, false, false},
		{false, false, true},
		{false, true,  false},
		{false, true,  true},
		{true,  false, false},
		{true,  false, true},
		{true,  true,  false},
		{true,  true,  true}
	}

	-- Tries to find a matching name, going from most specific (include
	-- ego names and text between parens/brackets) to least specific
	-- (no egos, strip text beteween parens/brackets).  Also, it
	-- first tries to find an item whose inscription won't cause the
	-- user to be asked "are you sure?", in the (unlikely) case the
	-- the user has multiple stacks with different inscriptions.
	for i = 1, getn(params) do
		local param = params[i]
		local res, it = __default_item_extra_hook_aux(mode_table, tag, str,
													  param[1], param[2],
													  param[3])

		if res then
			return res, it
		end
	end

	message("No appopriate items matching '" .. str .. "' found.  " ..
			"[Press any key to continue]")
	-- Pause
	term.inkey()

	-- Clear error message.
	term.prt("", 0, 0)

	return nil, nil
end -- default_item_extra_hook()

__get_item_results = {}

function __get_item(prompt, nothing, mode, tag, inven_source, full_obj_show)
	local ret, wid, hgt = term.get_size()
	local inventory = nil
	local a = strbyte('a')
	local byte0 = strbyte('0')
	local byte9 = strbyte('9')
	local start, stop = nil, nil
	local ret = nil
	local inven_color, item_color = nil, color.LIGHT_DARK
	local sel_inven = nil
	local dont_show = ((mode & USE_DISPLAY) == 0)
	local error = nil
	local scroll_start = 1
	local ignore_empty_list = false

	if game_options.easy_inven then dont_show = false end

	local ok_inven, ok_equip, ok_floor, ok_extra = ((mode & USE_INVEN) ~= 0), ((mode & USE_EQUIP) ~= 0), ((mode & USE_FLOOR) ~= 0), ((mode & USE_EXTRA) ~= 0)
	local mode_table = {}
	if (mode & USE_SHOW_NO_ITEM) ~= 0 then ignore_empty_list = true end

	-- Never show more than 26 items at once
	if hgt > 27 then hgt = 27 end

	-- Repeating an already known one ?
	local rep, rep_val = repeat_keys.pull()
	if rep then
		local o_ptr = get_object(rep_val)

		-- Validate the item
		if item_tester_okay(o_ptr, rep_val) then
			-- Forget the item_tester restrictions
			lua_set_item_tester(-1, "")
			get_item_extra_hook_lua = 0

			-- Success
			return rep_val
		end
	end

	-- If no inven_source is given, assume player)
	if not inven_source then inven_source = player.inventory end

	local num_matches = 0

	mode = nil
	if ok_floor then
		mode_table.floor = { inven={ [INVEN_INVEN] = cave(player.py, player.px).inventory }, start=INVEN_INVEN, stop=INVEN_INVEN, inven_color=color.LIGHT_DARK, item_color=nil, name="Floor" }
		mode = mode_table.floor
		num_matches = num_matches +
			__core_objects.num_items_match(mode.inven, mode.start, mode.stop)
	end
	if ok_equip then
		mode_table.equip = { inven=inven_source, start=INVEN_PACK, stop=INVEN_TOTAL - 1, inven_color=nil, item_color=color.LIGHT_DARK, name="Equip" }
		mode = mode_table.equip
		num_matches = num_matches +
			__core_objects.num_items_match(mode.inven, mode.start, mode.stop)
	end
	if ok_inven then
		mode_table.inven = { inven=inven_source, start=INVEN_INVEN, stop=INVEN_PACK - 1, inven_color=color.LIGHT_DARK, item_color=nil, name="Inven" }
		mode = mode_table.inven
		num_matches = num_matches +
			__core_objects.num_items_match(mode.inven, mode.start, mode.stop)
	end
	if not mode then
		-- Forget the item_tester restrictions
		lua_set_item_tester(-1, "")
		get_item_extra_hook_lua = 0
		message(color.VIOLET, "No get_item mode?! Contact the module maintainer.")
		return nil
	end

	if num_matches == 0 and not ignore_empty_list then
		-- Forget the item_tester restrictions
		lua_set_item_tester(-1, "")
		get_item_extra_hook_lua = 0
		message(nothing)
		return nil
	end

	inventory, start, stop, inven_color, item_color = mode.inven, mode.start, mode.stop, mode.inven_color, mode.item_color

	-- Save screen
	term.save()
	local old_screen = term.save_to()

	-- Can do extra calls
	if ok_extra then
		ok_extra = "@ extra selection, "
	else
		ok_extra = ""
	end

	if type(tag) == "number" then
		tag = strchar(tag)
	end

	local selection = nil
	while not selection do
		local y, item_nbs, first, last, last_displayed, key_table = __core_objects.display_inventory
		(
			inventory,
			start, stop,
			nil, true,
			inven_color, item_color,
			dont_show, scroll_start, full_obj_show, true
		)
		if not inven_color then
			term.blank_print("("..mode.name..": a-"..strchar(a + stop - start)..", * show/hide, "..ok_extra.."ESC) "..prompt, 0, 0)
		else
			if not first then first = 0 end
			if not last then last = 0 end
			term.blank_print("("..mode.name..": "..strchar(a + first - 1).."-"..strchar(a + last - 1)..", * show/hide, "..ok_extra.."ESC) "..prompt, 0, 0)
		end

		local cbyte = term.inkey()
		if cbyte == ESCAPE then break end

		local c = strchar(cbyte)

		-- Switch show/dont show
		if c == ' ' or c == '*' or c == '?' then dont_show = not dont_show
		-- Swicth inventory
		elseif c == '/' then
			if (mode == mode_table.inven or mode == mode_table.floor) and mode_table.equip then
				mode = mode_table.equip
				inventory, start, stop, inven_color, item_color = mode.inven, mode.start, mode.stop, mode.inven_color, mode.item_color
				scroll_start = 1
			elseif (mode == mode_table.equip or mode == mode_table.floor)  and mode_table.inven then
				mode = mode_table.inven
				inventory, start, stop, inven_color, item_color = mode.inven, mode.start, mode.stop, mode.inven_color, mode.item_color
				scroll_start = 1
			end
		elseif c == '-' then
			if mode_table.floor then
				mode = mode_table.floor
				inventory, start, stop, inven_color, item_color = mode.inven, mode.start, mode.stop, mode.inven_color, mode.item_color
				scroll_start = 1
			end

		-- Scroll up/down
		elseif c == '2' then
			scroll_start = scroll_start + 1
			if scroll_start > item_nbs - hgt + 3 then scroll_start = scroll_start - 1 end
		elseif c == '8' then
			scroll_start = scroll_start - 1
			if scroll_start < 1 then scroll_start = 1 end

		-- Call item by extra hook
		elseif (c == '@') and (ok_extra ~= "") then
			local res, it

			if type(get_item_extra_hook_lua) == "function" then
				res, it = get_item_extra_hook_lua(mode_table, tag)
			else
				res, it = get_item_extra_hook()
			end
			if res then
				ret = {
					inven_nb = item_slot_to_inven(it)
					item_nb = item_slot_to_item(it)
				}
				break
			end

		-- Inventory type selection
		else
			if not inven_color then
				if key_table.invens[c] then
					inven_color, item_color = color.LIGHT_DARK, nil
					start = key_table.invens[c]
					stop = start
					scroll_start = 1
					if flag_max_key(inventory[start]) == 0 then
						if not ignore_empty_list then
							error = nothing
							break
						end
					elseif flag_max_key(inventory[start]) == 1 then
						ret = {
							inven_nb = start
							item_nb = 1
						}
						local object = get_object(compute_slot(ret.inven_nb, ret.item_nb))
						if object and not item_tester_okay(object, compute_slot(ret.inven_nb, ret.item_nb)) then
							ret = nil
						end
						break
					end
				end

			-- Item selection
			elseif not term.inkey_dir and (cbyte >= byte0 and
										   cbyte <= byte9) then
				if tag and strlen(tag) == 1 then
					local tagstr = "@" .. tag .. c

					local fakec, item = next(key_table.items, nil)
					while fakec do
						local o_ptr = get_object(item)

						if o_ptr.note > 0 then
							local note = quark_str(o_ptr.note)

							if 	strfind(note, tagstr) then
								selection = fakec
								break
							end
						end
						fakec, item = next(key_table.items, fakec)
					end -- while fakec do
				end -- if tag and strlen(tag) == 1 then
			else -- elseif (c == '0') or (c == '1') then
				if key_table.items[c] then
					selection = c
				end
			end
		end -- Inventory type selection

		if selection then
			ret = {
				inven_nb = start
				item_nb = key_table.items[selection]
			}
		else
			term.load_from(old_screen, FALSE)
		end
	end -- while not selection do
	get_item_extra_hook_lua = 0

	term.load_from(old_screen, true)

	-- Restore screen
	term.load()

	if error then
		-- Forget the item_tester restrictions
		lua_set_item_tester(-1, "")
		message(error)
		return nil
	end
	if not ret then
		-- Forget the item_tester restrictions
		lua_set_item_tester(-1, "")
		return nil
	end

	-- Convert the return
	if mode == mode_table.inven or mode == mode_table.equip then
		-- Forget the item_tester restrictions
		lua_set_item_tester(-1, "")
		repeat_keys.push(compute_slot(ret.inven_nb, ret.item_nb))
		return compute_slot(ret.inven_nb, ret.item_nb)
	end

	local obj_idx
	if mode == mode_table.floor then
		obj_idx = object_make_idx("floor", ret.inven_nb, ret.item_nb, player.py, player.px)
	end

	assert(obj_idx, "Impoossible to make up obj_idx!")

	-- Forget the item_tester restrictions
	lua_set_item_tester(-1, "")

	repeat_keys.push(obj_idx)
	return obj_idx
end

function object_make_idx(type, inven_nb, item_nb, ...)
	local id = { inven_nb = inven_nb, item_nb = item_nb }
	if type == "floor" then
		id.floor = { y = arg[1], x = arg[2] }
	elseif type == "monster" then
		id.monster = arg[1]
	elseif type == "store" then
		id.store = arg[1]
	end
	tinsert(__get_item_results, id)
	return -getn(__get_item_results)
end

function __core_objects.get_object(item)
	if item < 0 then
		local ret = __get_item_results[-item]

		if ret.floor then
			return cave(ret.floor.y, ret.floor.x).inventory[ret.item_nb]
		elseif ret.monster then
			return monster(ret.monster).inventory[ret.inven_nb][ret.item_nb]
		elseif ret.store then
			return ret.store.stock[ret.item_nb]
		else
			message(color.VIOLET, "*ERROR* got_object got a bad item identifier")
		end
	else
		return player.inventory[item_slot_to_inven(item)][item_slot_to_item(item)]
	end
end

-- The indexes returned by get_item are only valid until this function is
-- called
-- currently happens at level generation but could/should be moved to every few turns
function __core_objects.clean_get_item()
	__get_item_results = {}
end

function __core_objects.flag_get_obj(flags, f)
	return __wrap_ptr.to_object(flag_get_ptr(flags, f, FLAG_TYPE_OBJ))
end

function __core_objects.item_optimize(item, obj)
	if item < 0 then
		item = __get_item_results[-item]

		local inven

		if item.floor then
			inven = cave(item.floor.y, item.floor.x).inventory
		elseif item.monster then
			inven = monster(item.monster).inventory[item.inven_nb]
		elseif item.monster then
			inven = monster(item.monster).inventory[item.inven_nb]
		elseif item.store then
			inven = item.store.stock
		end
		local max = flag_max_key(inven)

		-- Erase the slot
		flag_remove(inven, item.item_nb)

		-- Slide everything down
		for i = item.item_nb, max - 1 do
			-- Slide the item
			local j_ptr = __core_objects.flag_get_obj(inven, i + 1)
			flag_remove_zap(inven, i + 1)
			flag_set_ptr(inven, i, FLAG_TYPE_OBJ, j_ptr)
		end
	else
		-- The item is in the pack
		if item_slot_to_inven(item) == INVEN_INVEN then
			local inven = player.inventory[INVEN_INVEN]

			local max = flag_max_key(inven)

			-- Erase the slot
			flag_remove(inven, item_slot_to_item(item))

			-- Slide everything down
			for i = item_slot_to_item(item), max - 1 do
				-- Slide the item
				local j_ptr = __core_objects.flag_get_obj(inven, i + 1)
				flag_remove_zap(inven, i + 1)
				flag_set_ptr(inven, i, FLAG_TYPE_OBJ, j_ptr)
			end

			-- Window stuff
			player.window[FLAG_PW_INVEN] = true

		-- The item is being wielded
		else
			-- Erase the empty slot
			flag_remove(player.inventory[item_slot_to_inven(item)], item_slot_to_item(item))

			-- Recalculate bonuses
			player.update = player.update | PU_BONUS | PU_BODY

			-- Recalculate torch
			player.update = player.update | PU_TORCH

			-- Recalculate mana XXX
			player.update = player.update | PU_MANA

			-- Window stuff
			player.window[FLAG_PW_EQUIP] = true
		end
	end
end

function __core_objects.delete_object_idx_notice(item)
	if item < 0 then
		item = __get_item_results[-item]

		if item.floor then
			lite_spot(item.floor.y, item.floor.x)
		end
	end
end

function generate_item_floor_index(y, x, item_nb)
	local ret = {
		inven_nb = INVEN_INVEN
		item_nb = item_nb
		floor = { y = y x = x }
	}

	-- Convert the return
	tinsert(__get_item_results, ret)
	return -getn(__get_item_results)
end

function armor_dams_to_str(obj, flag, suffix)
	if getn(combat_dam_types) == 0 then
		return ""
	end

	suffix = suffix or ""

	local dams     = obj.flags[flag]
	local strs     = {}
	local nonzeros = 0

	for i = 1, getn(combat_dam_types) do
		local dam_type = combat_dam_types[i]
		local amnt     = dams[dam_type]

		if amnt and amnt ~= 0 then
			if flag_is_known(dams, dam_type) then
				nonzeros = nonzeros + 1
			else
				amnt = "?"
			end
		else -- if amnt then
			amnt = "0"
		end

		tinsert(strs, amnt .. suffix)
	end

	if nonzeros == 0 then
		return ""
	end

	if getn(strs) == 1 then
		return strs[1]
	end

	-- If all the strings are the same, only return one rather than the
	-- same string being repeated.
	for i = 1, getn(strs) - 1 do
		if strs[i] ~= strs[i + 1] then
			return strjoin(strs, "/")
		end
	end

	return strs[1]
end -- armor_dams_to_str()

--
-- Tells how various tvals are described
-- Set desc.base to the base name to use(defaults to item name)
-- Set desc.mod to the "modifier" that will be put in place of the # in the object name
-- Set desc.weapon to true to display basic weapon stats
-- Set desc.armor to true to display basic armor stats
--
object_desc_tvals = {}

-- Define a default capitalization option
game_options.CAPITALIZE_NONE = 1
game_options.CAPITALIZE_ARTS = 2
game_options.CAPITALIZE_ALL = 3
game_options.items_capitalize = game_options.CAPITALIZE_ARTS

--
-- Creates a description of the item "oobj", and returns it
--
-- One can choose the "verbosity" of the description, including whether
-- or not the "number" of items should be described, and how much detail
-- should be used when describing the item.
--
-- If "pref" then a "numeric" prefix will be pre-pended.
--
-- Mode:
--  -2 -- No pluralization, ego prefixes/suffixes, or modifiers
--  -1 -- No pluralization
--   0 -- The Cloak of Death
--   1 -- The Cloak of Death [1,+3]
--   2 -- The Cloak of Death [1,+3] (+2 to Stealth)
--   3 -- The Cloak of Death [1,+3] (+2 to Stealth) {nifty}
--
function object_desc(obj, pref, mode, show_all)
	-- Display them all(this function calls us back for the actual display)
	if show_all == true then return object_desc_store(obj, pref, mode) end

	local item = k_info[1 + obj.k_idx]
	local desc
	local prefix = ""
	local aware = is_aware(obj)
	local known = is_known(obj)
	local ego_prefix = nil
	local ego_suffix = nil
	local name = { base=item.name }

	mode = mode or 0
	pref = pref or 0

	-- Use artifact name if needed
	if obj.artifact_id > 0 and known then
		name.base = a_info[1 + obj.artifact_id].name
	elseif obj.artifact_id > 0 and a_info[1 + obj.artifact_id].unknown_name and
		strlen(a_info[1 + obj.artifact_id].unknown_name) > 0 then
		name.base = a_info[1 + obj.artifact_id].unknown_name
	else
		name.base = k_info[1 + obj.k_idx].name
		-- Include ego names
		for i = 1, MAX_EGO_PER_OBJ do
			if known and obj.ego_id[i] > 0 then
				local ego = e_info[1 + obj.ego_id[i]]
				if ego.before then
					if ego_prefix then
						ego_prefix = ego_prefix .. " " .. ego.name
					else
						ego_prefix = ego.name
					end
				else
					if ego_suffix then
						ego_suffix = ego_suffix .. " " .. ego.name
					else
						ego_suffix = ego.name
					end
				end
			end
		end

	end

	local user_note = nil
	if (obj.note > 0) then
		user_note = quark_str(obj.note)
	end

	-- Get modifiers and all ?
	if object_desc_tvals[obj.tval] then
		name.aware = aware
		name.known = known
		name.note  = user_note
		if tag(object_desc_tvals[obj.tval]) == TAG_FUNCTION then
			object_desc_tvals[obj.tval](obj, name, mode)
			if name.done then return name.base end
		else
			for k, e in object_desc_tvals[obj.tval] do name[k] = e end
		end

		if user_note or name.note then
			if user_note and name.note and name.note == "" and
				user_note ~= ""
			then
				user_note = nil
			else
				user_note = name.note
			end
		end -- if user_note or name.note then
	end -- if object_desc_tvals[obj.tval] then

	if obj.flags[FLAG_FULL_NAME] and known then
		desc = item.name
	else
		desc = name.base
	end

	if not desc then
		message(color.VIOLET, format("WARNING, we got object with desc = nil: tval(%d), sval(%d), k_idx(%d), art_id(%d)", obj.tval, obj.sval, obj.k_idx, obj.artifact_id))
		return "UNKNOWN OBJECT: ERROR"
	end

	-- Anything in the user-supplied user_note after a '^' gets put in front
	-- of the description, and anything after a '$' gets put after it.
	-- Thus, if a "ration of food" is inscribed with
	-- "^rotten$that's filled with worms", the resulting description is
	-- "a rotten ration of food that's filled with worms"
	if user_note and mode >= 3 and (strfind(user_note, "%$") or
									strfind(user_note, "%^")) then
		local prefix, suffix
		local start,  finish

		start, finish, prefix = strfind(user_note, "%^([^%$]*)")
		prefix = prefix or ""
		-- Very bad if we put another '#' into the description at
		-- this point
		if strfind(prefix, "#") then
			prefix = ""
		end

		start, finish, suffix = strfind(user_note, "%$([^%^]*)")
		suffix = suffix or ""
		-- Very bad if we put another '#' into the description at
		-- this point
		if strfind(suffix, "#") then
			suffix = "#"
		end

		if strlen(suffix) > 0 then
			if ego_suffix then
				ego_suffix = ego_suffix ..  " " .. suffix
			else
				ego_suffix = suffix
			end
		end

		if strlen(prefix) > 0 then
			if ego_prefix then
				ego_prefix = prefix .. " " .. ego_prefix
			else
				ego_prefix = prefix
			end
		end

		-- Find first occurance of '$' or '^', whichever comes
		-- first
		start = strfind(user_note, "[%$%^]")

		-- Remove everything after the first '$' or '^'; this might
		-- leave the user_note empty
		user_note = strsub(user_note, 1, start - 1)
	end -- if user_note and mode >= 3 and (strfind(user_note, "#") or ...

	-- Compute the prefix
	if strsub(desc, 1, 1) == "&" then
		-- Prepend the ego name, if any
		if ego_prefix and mode > -2 then
			desc = "& " .. ego_prefix .. strsub(desc, 2)
		end

		-- No prefix
		if name.no_article then
			-- Nothing
		elseif not pref or pref <= 0 then
			-- Nothing
		-- Hack -- None left
		elseif obj.number <= 0 then
			prefix = "no more"
		-- Extract the number
		elseif obj.number > 1 then
			prefix = tostring(obj.number)
		-- The only one of its kind
		elseif known and is_artifact(obj) then
			prefix = "The"
		-- A single one, with a vowel
		elseif is_a_vowel(strbyte(strsub(desc, 3, 3))) then
			prefix = "an"
		-- A single one, without a vowel
		else
			prefix = "a"
		end
	-- Objects that "never" take an article
	else
		-- Prepend the ego name, if any
		if ego_prefix and mode > -2 then
			desc = ego_prefix .. " " .. desc
		end

		-- No pref
		if not pref or pref <= 0 then
			-- Nothing
		-- Hack -- all gone
		elseif obj.number <= 0 then
			prefix = "no more"
		-- Prefix a number if required
		elseif obj.number > 1 then
			prefix = tostring(obj.number)

		-- The only one of its kind
		elseif known and is_artifact(obj) then
			prefix = "The"
		end
	end

	-- Pluralizer
	local i = strfind(desc, '~')

	if i and mode <= -1 then
		-- No plurals if mode is less than 0
		desc = gsub(desc, "~", "")
	elseif i then
		local plural  = ""
		local eat_len = 0
		-- Add a plural if needed
		if (obj.number > 1) and (pref >= 0) then
			local k1 = strsub(desc, i - 1, i - 1)
			local k2 = ""

			if (i - 2) > 1 then
				k2 = strsub(desc, i - 2, i - 1)
			end

			if k1 == 's' or k1 == 'h' or k1 == 'x'then
				-- "Cutlass-es" and "Torch-es" and "Box-es"
				plural  = "es"
				eat_len = 0
			elseif k2 == "ff" or k2 == "fe" then
				-- "Staff" -> "Staves", "Knife" -> "Knives"
				plural  = "ves"
				eat_len = 2
			elseif k1 == 'f' then
				-- "Shelf" -> "Shelves"
				plural  = "ves"
				eat_len = 1
			else
				plural  = "s"
				eat_len = 0
			end
		end
		-- Prefix the "~" with eat_len number "."s, which will "eat"
		-- that number of characters before the "~"
		desc = gsub(desc, "(" .. strrep('.', eat_len) .. "~)", plural)
	end

	-- Add the ego suffix if needed
	if ego_suffix and mode > -2 then
		if strfind(ego_suffix, ",") == 1 then
			-- If the suffix starts with "," we don't need to put a
			-- space in front of it
			desc = desc .. ego_suffix
		else
			desc = desc .. " " .. ego_suffix
		end
	end

	-- Capitalize if needed
	if (game_options.items_capitalize == game_options.CAPITALIZE_ALL) or
	   ((game_options.items_capitalize == game_options.CAPITALIZE_ARTS) and
		is_artifact(obj) and is_known(obj)) then
	   	desc = book_capitals(desc)
	end

	if mode <= -2 then
		-- No prefixes or mods if mode is less than -1
		desc = gsub(desc, "(&)", "")
		desc = gsub(desc, "(#)", "")
	else
		-- Replace & by the prefix
		desc = gsub(desc, "(&)", prefix)

		-- Replace # by the modifier, if any
		desc = gsub(desc, "(#)", name.mod or "")
	end

	-- Objecting is recharging
	if obj_mana.is_autocharging(obj, true) and mode >= 3 and
		not name.hide_charging and not obj_mana.is_full(obj)
	then
		local source = obj_mana.to_source(obj)

		if source.flags[FLAG_AUTO_CHARGE_TYPE] or
			obj.flags[FLAG_AUTO_CHARGE_TYPE]
		then
			desc = desc .. " (charging) "
		end
	end

	-- Suffix that goes before armor/weapon mods
	if name.suffix1 then
		desc = desc .. " " .. name.suffix1
	end

	-- Display the item like a weapon
	if has_flag(obj, FLAG_WEAPON) then
		name.weapon = true
	end

	-- Force display of to_a, to_d, an to_h, even if 0
	if has_flag(obj, FLAG_SHOW_MODS) or name.show_mods then
		name.show_mods        = true
		name.show_ac_mods     = true
		name.show_combat_mods = true
	end

	-- Force display of to_a, even if 0
	if has_flag(obj, FLAG_SHOW_AC_MODS) or name.armor then
		name.show_ac_mods = true
	end

	-- Force display of to_d and to_h, even if 0
	if has_flag(obj, FLAG_SHOW_COMBAT_MODS) then
		name.show_combat_mods = true
	end

	if name.weapon and mode >= 1 then
		-- Always show damage info for weapons.  However, if object
		-- isn't known, get and display generic info for that
		-- type of object using generic_or_specific(), rather
		-- than the damage that this particular object does
		local dam_info = generic_or_specific(obj)
                if flag_is_known(obj.flags, FLAG_DAM) then dam_info = obj end

		-- Start with the basic damage
		local min, max = 0, 0
		local dams = dam_info.flags[FLAG_DAM]

		-- Compute the max damage
		for i = 1, dams.size do
			if (dams.node[i].flags & FLAG_FLAG_USED) ~= 0 then
				local dam_type = dams.node[i].key
				min, max = min + flag_get(dams, dam_type), max + flag_get2(dams, dam_type)
			end
		end

		local intermediate = ' to '
		local display_max = max
		if globals()['FLAG_DAM_COMPUTE'] then
			if has_flag(obj, FLAG_DAM_COMPUTE) and
			   get_flag(obj, FLAG_DAM_COMPUTE) == combat.COMPUTE_ROLL then
				intermediate = 'd'
				display_max = max / min
			end
		end

		desc = desc .. format(" (%d%s%d)", min, intermediate, display_max)
	end

	-- If known add some more info
	if known and mode >= 1 then
		if has_flag(obj, FLAG_LEVELS) then
			desc = desc .. format(" (E:%d,L:%d)", obj.exp, obj.elevel)
		end

		-- Always show "(to_h, to_d)" if a weapon, even if one or
		-- both is zero.  Also always show them if SHOW_COMBAT_MODS
		-- is set
		if name.weapon or name.show_combat_mods or (obj.to_h ~= 0 and
													obj.to_d ~= 0) then
			desc = desc .. format(" (%s,%s)",
					      signed_number_tostring(obj.to_h),
					      signed_number_tostring(obj.to_d))
		elseif obj.to_h ~= 0 then
			desc = desc .. format(" (%s)", signed_number_tostring(obj.to_h))
		elseif obj.to_d ~= 0 then
			desc = desc .. format(" (%s)", signed_number_tostring(obj.to_d))
		end

		-- Show armor modifiers?
		local armor_descs = {}

		if name.armor then
			tinsert(armor_descs, format("%d", obj.ac))
		end

		if name.show_ac_mods or obj.to_a ~= 0 then
			tinsert(armor_descs, signed_number_tostring(obj.to_a))
		end

		if name.show_ac_mods and known_flag(obj, FLAG_RESIST) then
			local str = armor_dams_to_str(obj, FLAG_RESIST, "%")

			if str and strlen(str) > 0 then
				tinsert(armor_descs, "R:" .. str)
			end
		end

		if name.show_ac_mods and known_flag(obj, FLAG_ABSORB) then
			local str = armor_dams_to_str(obj, FLAG_ABSORB)

			if str and strlen(str) > 0 then
				tinsert(armor_descs, "A:" .. str)
			end
		end

		if getn(armor_descs) > 0 then
			desc = desc .. " [" .. strjoin(armor_descs, ",") .. "]"
		end
	end

	-- Suffix that goes after armor/weapon mods
	if name.suffix2 then
		desc = desc .. " " .. name.suffix2
	end

	-- Charge
	if known and mode >= 1 and obj.timeout > 0 then
		desc = desc .. " (charging)"
	end

	-- Sensed stuff
	local note = nil
	if obj.ident & IDENT_SENSE ~= 0 then
		if not note then
			note = sense_desc[1 + obj.sense]
		else
			note = note .. "," .. sense_desc[1 + obj.sense]
		end
	end

	if has_flag(obj, FLAG_COULDNT_DESTROY) and
		not is_known(obj) and
		not (flavour.get_flavour(obj.k_idx) and is_aware(obj))
	then
		if not note then
			note = "indestructable?"
		else
			note = note .. ",indestructable?"
		end
	end

	-- Even more info
	if known and mode >= 3 then
		-- Note "cursed" if the item is 'known' and cursed
		if obj.ident & IDENT_CURSED ~= 0 and
			obj.ident & IDENT_SENSE == 0
		then
			if not note then
				note = "cursed"
			else
				note = note .. ",cursed"
			end
		end

	end

	-- Use the standard inscription if available
	if obj.note > 0 and mode >= 3 then
		if not note or not known then
			note = user_note
		elseif strlen(user_note) > 0 then
			note = note .. ", " .. user_note
		end
	end

	-- Add the note, if any
	if note and strlen(note) > 0 then
		desc = desc .. " {" .. note .. "}"
	end

	-- Very last thing, add object_desc_tvals supplied prefix (if it exists)
	if name.prefix then
		desc = name.prefix .. " " .. desc
	end

	return clean_whitespace(desc)
end


desc_obj = {}

desc_obj.text_out = term.text_out

--
-- @fn
-- @brief Parses a list of flags to get to the final container and calls flag_fct on it
-- @param flags Flags the first flags container
-- @param flags_list "Number or List" the flags to recurse in. I.e: { FLAG_FOO, FLAG_BAR, FLAG_ZOG } will
-- call flag_fct(flags[FLAG_FOO][FLAG_BAR], FLAG_ZOG)
function desc_obj.flag_get(flags, flags_list, flag_fct)
	if not flags then return nil end
	if tag(flags_list) == TAG_NUMBER then flags_list = { flags_list } end

	for i = 1, getn(flags_list) - 1 do
		if not flags[flags_list[i]] then
			return nil
		else
			flags = flags[flags_list[i]]
		end
	end
	return flag_fct(flags, flags_list[getn(flags_list)])
end


--
-- @fn
-- @brief Describes a boolean flag
--
function desc_obj.bool_flag(flag, text)
	if desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_exists) and desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_is_known) then
		desc_obj.text_out(text)
		desc_obj.text_out(" ")
	end
end

--
-- @fn
-- @brief Describes a valued flag.
-- @param text String the text to display, $V will be replaced by the actual value.
-- @param percent Boolean if true it will display the number as a percent.
-- @param percent Boolean if true negative values are considered good.
--
function desc_obj.value_flag(flag, text, percent, reverse)
	if desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_exists) and desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_is_known) then
		local val = desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_get)
		if (not reverse and val < 0) or (reverse and val > 0) then
			text = gsub(text, "($V)", "#R"..signed_number_tostring(val)..iif(percent, "%", "").."#w")
		else
			text = gsub(text, "($V)", "#G"..signed_number_tostring(val)..iif(percent, "%", "").."#w")
		end
		desc_obj.text_out(text)
		desc_obj.text_out(" ")
	end
end

--
-- @fn
-- @brief Describes a boolean group of flag.
-- @param text String the text to display
-- @param percent Boolean if true it will display the number as a percent.
--
function desc_obj.bool_group(flags, text, first, join)
	local values = {}

	for flag, text in flags do
		if desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_exists) and desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_is_known) then
			local val = desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_get)
			if val < 0 then
				tinsert(values, text)
			else
				tinsert(values, text)
			end
		end
	end

	if getn(values) >= 1 then
		-- Intro
		desc_obj.text_out(text)

		-- List them all
		for i = 1, getn(values) do
			-- Connectives
			if i == 1 then desc_obj.text_out(first or "")
			elseif i < getn(values) then desc_obj.text_out(", ")
			else desc_obj.text_out(join or " and ") end

			-- Dump the stat
			desc_obj.text_out(values[i])
		end
		desc_obj.text_out(". ")
	end
end

--
-- @fn
-- @brief Describes a valued group of flag, where the values aren't
-- integers.
-- @param text String the text to display
-- @param places Number Number of decimal places after the dot to display
function desc_obj.value_group_frac(flags, text, places, first, join)
	local values = {}

	local divisor = 1

	for i = 1, places do
		divisor = divisor * 10
	end

	local frmt_str = "%0" .. places .. "d"

	for flag, text in flags do
		if desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_exists) and desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_is_known) then
			local val = desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_get)
			if val < 0 then
				tinsert(values, text.."(#R"..
						signed_number_tostring(val / divisor) .. "." ..
							format(frmt_str, imod(abs(val), divisor)) .. "#w)")
			else
				tinsert(values, text.."(#G"..
						signed_number_tostring(val / divisor) .. "." ..
							format(frmt_str, imod(abs(val), divisor)) .. "#w)")
			end
		end
	end

	if getn(values) >= 1 then
		-- Intro
		desc_obj.text_out(text)

		-- List them all
		for i = 1, getn(values) do
			-- Connectives
			if i == 1 then desc_obj.text_out(first or "your ")
			elseif i < getn(values) then desc_obj.text_out(", ")
			else desc_obj.text_out(join or " and ") end

			-- Dump the stat
			desc_obj.text_out(values[i])
		end
		desc_obj.text_out(". ")
	end
end







--
-- @fn
-- @brief Describes a valued group of flag.
-- @param text String the text to display
function desc_obj.value_group(flags, text, first, join, suffixes, prefixes)
	local values = {}

	suffixes = suffixes or {}
	prefixes = prefixes or {}

	suffixes.color   = suffixes.color   or ""
	suffixes.nocolor = suffixes.nocolor or ""

	prefixes.color   = prefixes.color   or ""
	prefixes.nocolor = prefixes.nocolor or ""


	for flag, text in flags do
		if desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_exists) and desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_is_known) then
			local val = desc_obj.flag_get(desc_obj.cur_obj.flags, flag, flag_get)
			local color

			if val < 0 then
				color = "#R"
			else
				color = "#G"
			end

			tinsert(values, text .. "(" .. prefixes.nocolor .. color ..
					prefixes.color .. signed_number_tostring(val) ..
					suffixes.color .. "#w" .. suffixes.nocolor .. ")")
		end
	end

	if getn(values) >= 1 then
		-- Intro
		desc_obj.text_out(text)

		-- List them all
		for i = 1, getn(values) do
			-- Connectives
			if i == 1 then desc_obj.text_out(first or "your ")
			elseif i < getn(values) then desc_obj.text_out(", ")
			else desc_obj.text_out(join or " and ") end

			-- Dump the stat
			desc_obj.text_out(values[i])
		end
		desc_obj.text_out(". ")
	end
end

--
-- Describe an item in defails(*ID*)
-- By default do nothing, the module should override it if needed
-- (and surely, it is needed)
--
function object_desc_details(obj, trim_down)
end

constant("object_desc_configuration", {})
object_desc_configuration.details_require_id = true
object_desc_configuration.details_require_id_message = true

new_flag("DESC_DETAIL", true)

hook(hook.OBJECT_DESC, function(obj, file, trim_down)
	if object_desc_configuration.details_require_id and not is_known(obj) then
		-- Avoiding telling user about obvious flags if the object
		-- isn't known
		return
	end

	obj = generic_or_specific(obj)

	local array = obj.flags[FLAG_DESC_DETAIL]

	object_desc_details(obj, trim_down)
	if array then
		for i = 1, flag_max_key(array) do
			local detail = array[i]

			if type(detail) == "string" then
				desc_obj.text_out(detail .. "  ")
			end
		end
	end
end)

__tval_strings = {}
__sval_strings = {}

-- Start max at 1, since 1 is the tval used by corpses
__tval_max   = 1
__sval_maxes = {}

--
-- @fn
-- @brief Gets maximum tval
-- @return Number The maximum tval
function get_max_tval()
	return __tval_max
end

--
-- @fn
-- @brief Gets maximum sval for a given tval
-- @param tval Number The tval
-- @return Number The maximum sval
function get_max_sval(tval)
	return __sval_maxes[tval]
end

--
-- @fn
-- @brief Defines the TVal and SVal used to categorize objects
-- @param t List A list of tables in this format:
-- { "TVALNAME", { "SVAL1", "SVAL2, ... } },
--
function define_objects_categorization(t)
	for i = 1, getn(t) do
		local tval
		if not globals()["TV_"..t[i][1]] then
			__tval_max         = __tval_max + 1
			tval               = __tval_max
			__sval_maxes[tval] = 0

			globals()["TV_"..t[i][1]] = tval
			__tval_strings[tval] = "TV_"..t[i][1]
			__sval_strings[tval] = {}
		else
			tval = globals()["TV_"..t[i][1]]
		end

		for j = 1, getn(t[i][2]) do
			assert(not globals()["SV_"..t[i][2][j]], "Sval ".."SV_"..t[i][2][j].." alredy defined")

			__sval_maxes[tval] = __sval_maxes[tval] + 1
			local sval         = __sval_maxes[tval]

			globals()["SV_"..t[i][2][j]] = sval
			__sval_strings[tval][sval] = "SV_"..t[i][2][j]
		end
	end
end

--- @fn
-- @brief Tries to turn a tval/sval pair into a "SV_FOO" string
-- @param tval Number The tval
-- @param sval Number The sval
-- @return String The string
function sval_to_str(tval, sval)
	if __sval_strings[tval] and __sval_strings[tval][sval] then
		return __sval_strings[tval][sval]
	end

	return nil
end

--- @fn
-- @brief Tries to turn a tval into a "TV_FOO" string
-- @param tval Number The tval
-- @return String The string
function tval_to_str(tval)
	if __tval_strings[tval] then
		return __tval_strings[tval]
	end

	return nil
end

--
-- Do item_increase(), item_describe() and item_optimize() on a list
-- of items.  This function is needed because of item_increase()
-- removes an item from inventory, then the item indexs of objects
-- after it in the inventory will change; thus, the items need
-- to be modified in the correct order to prevent weirdness
function bulk_item_increase(list, dont_describe, dont_optimize)
	-- Sort by decreasing item index
	sort(list,
		 function(a, b)
			 return (a[1] > b[1])
		 end)

	for i = 1, getn(list) do
		item_increase(list[i][1], list[i][2])

		if not dont_describe then
			item_describe(list[i][1])
		end

		if not dont_optimize then
			item_optimize(list[i][1])
		end
	end
end

function is_inven_empty(inven)
	return (flag_used(inven) == 1)
end

function is_artifact(obj)
	if tag(obj) == TAG_OBJ_KIND then
		return (obj.artifact or obj.flags[FLAG_NORM_ART] or
				obj.flags[FLAG_INSTA_ART])
	end

	return %is_artifact(obj)
end

-- Wield things
hook
{
	[hook.WIELD_SLOT] = function(obj, ideal)
		if obj.flags[FLAG_WIELD_SLOT] then
			if ideal then
				return true, obj.flags[FLAG_WIELD_SLOT]
			else
				return true, get_slot(obj.flags[FLAG_WIELD_SLOT])
			end
		end
	end
}

hook(hook.INFO_DATA_LOADED, function()
	constant("TAG_OBJ_KIND", tag(k_info[1]))
end)


------------------ Leveling objects ---------------------

new_flag("OBJ_EXP_PLAYER_EXP")
hook(hook.PLAYER_EXP, function(exp)
	local state = { num = 1, list = {} }

	for_inventory(player, INVEN_PACK, INVEN_TOTAL, function(obj)
		local state = %state
		if obj.flags[FLAG_OBJ_EXP_PLAYER_EXP] then
			state.num = state.num + 1
			tinsert(state.list, obj)
		end
	end)

	for i = 1, getn(state.list) do
		local obj = state.list[i]

		obj.exp = obj.exp + (exp / state.num)
		print("xp for obj", obj, exp, state.num)
		check_experience_obj(obj)
	end

	return true, exp / state.num
end)


--------------- Bulk enchant and stuff, does nothing by default --------------
--- @fn
-- @brief Modules should override this to define how items are enchanted
function objects_on_make_all(obj, power)
end

--- @fn
-- @brief Modules should override this to define how items are enchanted
function objects_on_make_all_pre(obj, power)
end

--- @fn
-- @brief Modules should override this to define how the engine determines object "power" for latter enchanting
function objects_get_power_level(obj, lev, okay, good, great)
	return 0
end

------------------ Artifact sets  ---------------------
constant("artifact_sets", {})

function artifact_sets.gather_sets()

end

-------------------- Aware/tried functions -------------

-- Querying and setting whether or not the user has tried and/or is
-- aware of different object kinds.  Done in Lua to make it easier to
-- deal with objects kinds where one k_idx has multiple flavors, like
-- wands and orbs in ToME.

__multiflav = {}
__multiflav.tvals = {}
__multiflav.aware = {}
__multiflav.tried = {}
add_loadsave("__multiflav.aware", {})
add_loadsave("__multiflav.tried", {})

function is_aware(obj)
	if obj.artifact_id ~= 0 then
		return a_info(obj.artifact_id).known
	elseif not __multiflav.tvals[obj.tval] then
		return k_info(obj.k_idx).aware
	else
		local id = obj.flags[FLAG_FLAVOUR_ID]
		if id then
			return __multiflav.aware[obj.tval][id]
		else
			return false
		end
	end
end

function set_aware(obj, forget)
	local aware = (not forget)

	if (obj.artifact_id ~= 0) then
		a_info(obj.artifact_id).known = aware
		k_info(obj.k_idx).aware       = aware
	elseif not __multiflav.tvals[obj.tval] then
		k_info(obj.k_idx).aware = aware
	else
		__multiflav.aware[obj.tval][obj.flags[FLAG_FLAVOUR_ID]] = aware
	end
end

function is_tried(obj)
	if not __multiflav.tvals[obj.tval] then
		return k_info(obj.k_idx).tried
	else
		return __multiflav.tried[obj.tval][obj.flags[FLAG_FLAVOUR_ID]]
	end
end

function set_tried(obj, forget)
	local tried = (not forget)

	if not __multiflav.tvals[obj.tval] then
		k_info(obj.k_idx).tried = tried
	else
		__multiflav.tried[obj.tval][obj.flags[FLAG_FLAVOUR_ID]] = tried
	end
end

function add_multiflav_tval(tval, info)
	assert(not __multiflav.tvals[tval],
		   "tval " .. tval_to_str(tval) .. " already registered")

	__multiflav.tvals[tval] = info
end

hook(hook.GAME_START,
function()
	for tval, _ in __multiflav.tvals do
		__multiflav.aware[tval] = __multiflav.aware[tval] or {}
		__multiflav.tried[tval] = __multiflav.tried[tval] or {}
	end
end)

function get_multiflav_tvals()
	local list = {}
	for k, _ in __multiflav.tvals do
		tinsert(list, k)
	end

	return list
end

function is_multiflav_tval(tval)
	return __multiflav.tvals[tval]
end

function multiflav_id_to_name(tval, id)
	return __multiflav.tvals[tval].id_to_name(id)
end

function get_aware_multiflav_ids(tval)
	local aware_ids = __multiflav.aware[tval]

	local list = {}
	for k, _ in aware_ids do
		tinsert(list, k)
	end

	return list
end
