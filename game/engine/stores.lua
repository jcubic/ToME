-- Take care of all concerning stores

declare_global_constants {
	"store_buy_list",
}

new_flag("STORE_OWNER")
new_flag("STORE_THEME")
new_flag("STORE_THEME_CHANCE")

function store_buy_list(t)
	assert(type(t) == "table", "store_buy_list got no table")
	hook(hook.STORE_BUY,
		function (index, name, obj)
			local tbl = %t
			local elt = tbl[index]
			if not elt then
				--if type(name) ~= "string" then
				--	return
				--end
				assert(type(name) == "string", "store_buy_list no name and no "..index.." entry")
				elt = tbl[name]
			end
			if elt then
				if elt then
					if type(elt) == "function" then
						return true, elt(obj)
					elseif type(elt) == "table" then
						local k, e
						for k, e in elt do
							if type(e) == "number" then
								if obj.tval == e then return true, true end
							else
								if (obj.tval == e[1]) and (obj.sval >= e[2])  and (obj.sval <= e[3]) then return true, true end
							end
						end
					elseif elt == -1 then
						return true, FALSE
					end
				end
			end
		end
	)
end

constant("store", {})

-- Basic default obj level for stores
store.OBJ_LEVEL = 5

-- The table of object creator methods
store.make_obj = {}

-- Theme for fully random store item.  Must be filled in by module.
store.make_obj.full_random_theme = flag_new()

-- Allow module to disallow certain objects from being randomly genearted
-- in stores.
function store.make_obj.store_ok_filter(k_idx)
	local k_ptr = k_info[k_idx + 1]

	-- Don't sell worhtless things
	return (k_ptr.cost and k_ptr.cost > 0)
end

-- Store object creator: fully random selection
function store.make_obj.full_random(store_info, level)
	return rand_obj.get_themed_obj(level, store.make_obj.full_random_theme,
								   store.make_obj.store_ok_filter)
end

-- Store object creator: table reader
-- Make the store objects from the store table definition
function store.make_obj.table_reader(store_info, level)
	local table = st_info(store_info.st_idx).table
	local size = flag_used(table)

	-- Must contain something
	if size == 0 then
		if game_options.cheat_peek then
			message("Store object creation: no types of items to make")
		end
		return nil
	end

	local tries = 4
	while tries > 0 do
		local i = rng(size)
		local k_idx, chance = flag_get(table, i), flag_get2(table, i)

		-- Must pass a probability check
		-- Don't allow k_info artifacts
		if k_info(k_idx).flags[FLAG_NORM_ART] then
			if game_options.cheat_peek then
				message("Store object creation: tried to create NORM_ART.")
			end
		elseif k_info(k_idx).flags[FLAG_INSTA_ART] then
			if game_options.cheat_peek then
				message("Store object creation: tried to create INSTA_ART.")
			end
		elseif k_info(k_idx).flags[FLAG_SPECIAL_GENE] then
			if game_options.cheat_peek then
				message("Store object creation: tried to create SPECIAL_GENE.")
			end
		elseif rng.percent(chance) then
			local obj = new_object()
			object_prep(obj, k_idx)
			apply_magic(obj, level, nil, nil, nil)
			return obj
		end
		tries = tries - 1
	end

	if game_options.cheat_peek then
		message("All item creation attempts failed.")
	end

	return nil
end -- store.make_obj.table_reader()

-- Store object creator: random_theme
-- Make the store objects from the store table definition
function store.make_obj.random_theme(store_info, level)
	local theme =  st_info(store_info.st_idx).flags[FLAG_STORE_THEME]

	if not theme then
		message(color.VIOLET, "random_theme store do not have FLAG_STORE_THEME")
		return nil
	end

	local tries = 4
	while tries > 0 do
		local obj = rand_obj.get_themed_obj(level, theme, 
			store.make_obj.store_ok_filter, nil, 0, false)
		if obj and object_value(obj) > 0 then
			return obj
		end
		tries = tries - 1
	end

	if game_options.cheat_peek then
		message("All item creation attempts failed.")
	end

	return nil
end -- store.make_obj.random_theme()

function store.make_obj.theme_and_list(store_info, level)
	local odds = st_info(store_info.st_idx).flags[FLAG_STORE_THEME_CHANCE]

	if not odds then
		message(color.VIOLET, "theme_and_list do not have FLAG_STORE_THEME_CHANCE")
		return nil
	end

	if rng(1,100) <= odds then
		return store.make_obj.random_theme(store_info, level)
	else
		return store.make_obj.table_reader(store_info, level)
	end

end -- store.make_obj.theme_and_list()


function store.return_level(store_info)
	local st = st_info(store_info.st_idx)
	local level

	-- If a level a specified, use it, optherwise use the default range
	if st.flags[FLAG_LEVEL] then level = get_function_registry_from_flag(st.flags, FLAG_LEVEL)()
	else level = rng(store.OBJ_LEVEL)
	end

	return level
end

-- Fill up a store stock
function store.new(store_info, num)
	local inventory = store_info.stock
	local maker = st_info(store_info.st_idx).store_maker

	num = num or st_info(store_info.st_idx).store_make_num

	local old_level = object_level

	for i = 1, num do
		object_level = store.return_level(store_info)
		local obj = store.make_obj[maker](store_info, object_level)

		if game_options.cheat_peek then
			if obj then
				message("Stocking up store: " ..
						object_desc(obj, false, 3) .. ".")
			end
		end

		if obj and obj.k_idx == 0 then
			local name = st_info[store_info.st_idx + 1].name

			message(color.VIOLET,
					 "'something' obj generated for store '" .. name .. "'")
		elseif obj then
			obj.ident = obj.ident | IDENT_STOREB | IDENT_KNOWN | IDENT_MENTAL

			if inven_carry_inven(inventory, obj, false) == -1 then
				break
			end
		end
	end

	object_level = old_level

	return inventory
end

-- Maintain a store
-- Aka: delete some objects and add some new ones
function store.maintain(store_info)
	local inventory = store_info.stock
	local maker = st_info(store_info.st_idx).store_maker
	local num = st_info(store_info.st_idx).store_make_num
	local max = flag_max_key(inventory)
	local old_level = object_level

	-- Store keeper forgives the player
	store_info.insult_cur = 0

	-- Choose the number of slots to keep
	local j = num

	-- Sell a few items
	j = j - rng(j * 3 / 5)
	if j > max then j = max end

	-- Hack -- prevent "underflow"
	if j < 0 then j = 0 end

	-- Destroy objects until only "j" slots are left
	local del_list, to_del = {}, {}
	while j > 0 do
		j = j - 1
		local i
		repeat
			i = rng(1, max)
		until (not to_del[i])

		local item = object_make_idx("store", 0, i, store_info)
		to_del[i] = true
		tinsert(del_list, {item, -255})
	end

	bulk_item_increase(del_list, true, false)

	-- Choose the number of slots to fill
	max = flag_max_key(inventory)
	j = num - max

	-- Buy some more items
	if j < 0 then j = 0 end
	if j > num then j = num end

	-- Acquire some new items
	store.new(store_info, num)

	object_level = old_level
end

-- Create a new store
function store.create(st_idx)
	local store_info = store_type_new()

	store_info.st_idx = st_idx

	-- Pick an owner
	store_info.owner = st_info(st_idx).owners[rng(4)]

	-- Initialize the store
	store_info.store_open = 0
	store_info.insult_cur = 0
	store_info.good_buy = 0
	store_info.bad_buy = 0
	store_info.last_visit = 0

	-- Create the inventory
	store_info.stock[INVEN_LIMIT_KEY] = st_info(store_info.st_idx).max_obj
	store.new(store_info)

	return store_info
end

-- Create a new store for a specific town
function store.create_for_town(t_idx, st_idx)
	local store_info = store.create(st_idx)
	local town = town_info(t_idx)

	flag_set_ptr(town.stores, st_idx, FLAG_TYPE_STORE, store_info)
end

-- A helper function for is_state
function store.is_state_aux(owner, state)
	-- Check race & class
--DGDGDGDG	return owner.races[state+1][player.prace] or owner.classes[state+1][player.pclass]
end

-- Test if the state accords with the player
function store.is_state(store_info, state)
	local owner = ow_info(store_info.owner)

	if state == STORE_NORMAL then
		if store.is_state_aux(owner, STORE_LIKED) then return nil end
		if store.is_state_aux(owner, STORE_HATED) then return nil end
		return true
	else
		return store.is_state_aux(owner, state)
	end
end


-- Price an item
function store.price_item(store_info, obj, buying)
	local factor, adjust, price
	local store_type = st_info(store_info.st_idx)
	local owner = ow_info(store_info.owner)
	local greed = owner.min_inflate

	-- Get the value of one of the items
	price = object_value(obj)

	-- Worthless items
	if price <= 0 then return 0 end

	-- Compute the racial factor
	if store.is_state(store_info, STORE_LIKED) then
		factor = owner.costs[STORE_LIKED]
	elseif store.is_state(store_info, STORE_HATED) then
		factor = owner.costs[STORE_HATED]
	else
		factor = owner.costs[STORE_NORMAL]
	end

	-- Add in the charisma factor
	factor = factor + player.calc_price_factor()

	-- Shop is buying
	if buying then
		-- Adjust for greed
		adjust = 100 + (300 - (greed + factor))

		-- Never get "silly"
		if (adjust > 100) then adjust = 100 end

		-- Mega-Hack -- Black market sucks
		if store_type.flags[FLAG_ALL_ITEM] then price = price / 2 end

	-- Shop is selling
	else
		-- Adjust for greed
		adjust = 100 + ((greed + factor) - 300)

		-- Never get "silly"
		if (adjust < 100) then adjust = 100 end

		-- Mega-Hack -- Black market sucks
		if store_type.flags[FLAG_ALL_ITEM] then price = price * 2 end
	end

	-- Compute the final price (with rounding)
	price = (price * adjust + 50) / 100

	-- Note -- Never become "free"
	if price <= 0 then return 1 end

	-- Return the price
	return price
end

function store.display_store_aux(store_info)
	local store_type = st_info(store_info.st_idx)
	local actions = store_type.actions
	local owner = ow_info(store_info.owner)
	local ret, wid, hgt = term.get_size()

	local action_costs = {}

	-- Print gold
	term.print("Gold Remaining: " .. format("%9d", player.au), hgt - 4, wid - 28)

	-- Print actions
	term.print("You may:", hgt - 3, 0)
	term.color_text(color.YELLOW, "ESC#w) Exit.", hgt - 2, 1)
	local idx = 1
	for j = hgt - 3, hgt - 1 do
		for i = 17 + 2, 17 + 2 + 30, 30 do
			local action = ba_info(actions[idx])

			if actions[idx] > 0 then
				local action_name  = action.name
				local action_color = color.WHITE
				local extra_color
				local action_extra
				local action_cost

				if (action.costs[1] < 0) then
					-- Cost is a function.  Cost function can do the
					-- work of deciding on restrictions.
					local func =
						get_function_registry("ba_info",
											  -action.costs[1])
					action_name, action_extra, action_color, extra_color =
						func(store_info, true)

					action_name  = action_name or action.name
					action_extra = action_extra or ""
					action_color = action_color or colow.WHITE
					action_cost  = func
				elseif (action.action_restr == 0) then
					if ((store.is_state(store_info, STORE_LIKED) and (action.costs[1+STORE_LIKED] == 0)) or
					                (store.is_state(store_info, STORE_HATED) and (action.costs[1+STORE_HATED] == 0)) or
				        	        (store.is_state(store_info, STORE_NORMAL) and (action.costs[1+STORE_NORMAL] == 0))) then
						action_color = color.WHITE
						action_extra = ""
						action_cost  = 0
					elseif (store.is_state(store_info, STORE_LIKED)) then
						action_color = color.LIGHT_GREEN
						action_cost  = action.costs[1+STORE_LIKED]
						action_extra = "(" .. action_cost .. "gp)"
					elseif (store.is_state(store_info, STORE_HATED)) then
						action_color = color.RED
						action_cost  = action.costs[1+STORE_HATED]
						action_extra = "(" .. action_cost .. "gp)"
					else
						action_color = color.YELLOW
						action_cost  = action.costs[1+STORE_NORMAL]
						action_extra = "(" .. action_cost .."gp)"
					end
				elseif (action.action_restr == 1) then
					if ((store.is_state(store_info, STORE_LIKED) and (action.costs[1+STORE_LIKED] == 0)) or
					                (store.is_state(store_info, STORE_NORMAL) and (action.costs[1+STORE_NORMAL] == 0))) then
						action_color = color.WHITE
						action_extra = ""
						action_cost  = 0
					elseif (store.is_state(store_info, STORE_LIKED)) then
						action_color = color.LIGHT_GREEN
						action_cost  = action.costs[1+STORE_LIKED]
						action_extra = "(" .. action_cost .."gp)"
					elseif (store.is_state(store_info, STORE_HATED)) then
						action_color = color.LIGHT_DARK
						action_extra = "(closed)"
					else
						action_color = color.YELLOW
						action_cost  = action.costs[1+STORE_NORMA]
						action_extra = "(" .. action_cost .. "gp)"
					end
				else
					if (store.is_state(store_info, STORE_LIKED) and (action.costs[1+STORE_LIKED] == 0)) then
						action_color = color.WHITE
						action_extra = ""
						action_cost  = 0
					elseif (store.is_state(store_info, STORE_LIKED)) then
						action_color = color.LIGHT_GREEN
						action_cost  = action.costs[1+STORE_LIKED]
						action_extra = "(" .. action_cost .."gp)"
					else
						action_color = color.LIGHT_DARK
						action_extra = "(closed)"
					end
				end
				extra_color = extra_color or action_color

				term.color_text(color.YELLOW, strchar(action.letter).."#w)",
								j, i)
				term.print(action_color, action_name, j, i + 3)
				term.print(extra_color,  action_extra,
						   j, i + 4 + strlen(action_name))

				action_costs[idx] = action_cost
			end

			idx = idx + 1
		end
	end

	-- Print header
	term.print("Item Description", 4, 3)
	if not st_info(store_info.st_idx).flags[FLAG_FREE] then
		term.print("Weight", 4, wid - 20)
		term.print("Price", 4, wid - 8)
	else
		term.print("Weight", 4, wid - 8)
	end

	-- Print store info
	term.print(owner.name, 2, 11)
	if store_type.flags[FLAG_FREE] then
		term.print(store_type.name, 2, 40)
	else
		term.print(store_type.name.." ("..owner.max_cost..")", 2, 40)
	end
	term.print(" Capacity("..(flag_used(store_info.stock) - 1).."/"..store_info.stock[INVEN_LIMIT_KEY]..")", 2, 60)

	return action_costs
end

function store.display(store_info)
	term.save()
	term.clear()

	local ret, wid, hgt = term.get_size()
	local hgt_list = hgt - 5
	local scroll_start = 1
	local cmd_table = {}

	for i = 1, 6 do
		local action = ba_info(st_info(store_info.st_idx).actions[i])

		if action.letter ~= strbyte('.') then
			cmd_table[strchar(action.letter)] = {action, i}
		end
		if action.letter_aux ~= strbyte('.') then
			cmd_table[strchar(action.letter_aux)] = {action, i}
		end
	end

	-- A function to display the price of an item
	local display_item_store = function(obj, item_color, y)
		local price = store.price_item(%store_info, obj, nil)
		local c = color.WHITE

		if price > player.au then c = color.LIGHT_DARK end

		-- Display the price
		term.print(c, format("%9s", price), y, %wid - 12)
	end

	-- Should the price be displayed
	local is_free = st_info(store_info.st_idx).flags[FLAG_FREE]

	while not nil do
		-- Display the store inventory, including the price
		term.clear_from(1)
		local y, item_nbs, first, last, last_displayed, key_table = __core_objects.display_inventory
		(
			{ [INVEN_INVEN] = store_info.stock },
			INVEN_INVEN, INVEN_INVEN,
			nil, true,
			color.LIGHT_DARK, nil,
			nil, scroll_start, true, true,
			-1, 4, wid - iif(is_free, 0, 12), wid - 1, hgt_list,
			iif(is_free, nil, display_item_store), true
		)
		local action_costs   = store.display_store_aux(store_info)
		local items_onscreen = hgt_list - 4

		-- Erase old message
		message()

		-- Place cursor out of the way
		term.gotoxy(hgt - 1, wid - 1)


		-- Get keypress from player
		local key_raw = term.inkey()
		local key = strchar(key_raw)

		-- Escape
		if key_raw == ESCAPE then break

		-- Allow debug/wizard commands while inside a shop
		elseif key_raw == KTRL('A') then
			do_cmd_debug()
		-- Scroll up/down
		elseif key == '2' then
			scroll_start = scroll_start + 1
			if scroll_start > item_nbs - hgt_list + 6 then scroll_start = scroll_start - 1 end
		elseif key == '8' then
			scroll_start = scroll_start - 1
			if scroll_start < 1 then scroll_start = 1 end
		elseif key == ' ' and item_nbs > items_onscreen then
			if scroll_start == item_nbs - hgt_list + 6 then scroll_start = 1
			else
				scroll_start = scroll_start + items_onscreen
				if scroll_start > item_nbs - hgt_list + 6 then scroll_start = item_nbs - hgt_list + 6 end
			end
		-- Check action
		elseif cmd_table[key] then
			local action = cmd_table[key][1]
			local idx    = cmd_table[key][2]
			local cost   = action_costs[idx]

			if tag(cost) == TAG_FUNCTION then
				cost = cost(store_info)
			end

			if not cost then
				-- Player not permitted to use that action
			elseif tag(cost) == TAG_NUMBER and cost > 0 and	cost > player.au
			then
				message("You do not have enough money to " ..
						strlower(action.name))
			elseif tag(cost) == TAG_STRING then
				-- Cost function returned it's own "can't do this" message
				message(cost)
			else
				local func = get_function_registry("ba_info",
												   -action.action)

				func(store_info,
					 {
						 key_table      = key_table,
						 last_displayed = last_displayed,
						 cost           = cost,
					 })

				if tag(cost) == TAG_NUMBER then
					player.au = player.au - cost
				end
			end
		end -- elseif cmd_table[key] then
	end

	term.load()
end
