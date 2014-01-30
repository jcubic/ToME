--
-- Informations for the various store actions
-- When action is a number it is an hardcoded action
--

new_store_action
{
	define_as = "store.ACTION_NONE"
	name = "Nothing"
	letter = '.'
	action = function() end
	action_restr = "all"
	costs = { 0, 0, 0 }
}

new_store_action
{
	define_as = "store.ACTION_SELL"
	name = "Sell an item"
	letter = 's'
	letter_aux = 'd'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info, info)
		if st_info(store_info.st_idx).max_obj == 0 then
			message(color.VIOLET, "Store can't hold anything!")
			return
		end

		local ret, item = get_item("Sell which item?", "You have no items to sell.", USE_INVEN, get_function_registry("st_info", st_info(store_info.st_idx).buy))
		if not ret then return end
		local obj = object_dup(get_object(item))

		-- Quantity ?
		local qt = 1
		if obj.number > 1 then
			qt = get_quantity("Quantity (1-"..obj.number.."):", obj.number)
			if qt < 1 then delete_object(obj) return end
		end
		obj.number = qt

		local desc, price = object_desc(obj, true, 3), store.price_item(store_info, obj, true) * obj.number

		-- Confirm
		if not get_check("Sell ".. desc .. " for " .. price .. "gp ?") then
			delete_object(obj)
			return
		end

		-- Learn about object after selling it
		make_item_fully_identified(obj)
		desc = object_desc(obj, true, 3)

		-- Remove it
		item_increase(item, -qt)
		item_optimize(item)

		-- Grab it
		inven_carry_inven(store_info.stock, obj, false)

		-- Add gold
		player.au = player.au + price

		message("Done! You sold ".. desc .. " for " .. price .. "gold.")
	end
}

new_store_action
{
	define_as = "store.ACTION_BUY"
	name = "Purchase an item"
	letter = 'p'
	letter_aux = 'g'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action =
	function(store_info, info)
		local store_type = st_info[store_info.st_idx + 1]

		if store_type.max_obj == 0 then
			message(color.VIOLET, "Store can't hold anything!")
			return
		end

		if is_inven_empty(store_info.stock) then
			message("Nothing in store to buy.")
			return
		end

		while not nil do
			local ret, key = get_com("(Items a-" .. strchar(strbyte('a') + info.last_displayed) .. ", ESC to exit) Which item are you interested in?")
			if not ret then return end

			key = strchar(key)
			if info.key_table.items[key] and (strbyte(key) - strbyte('a') <= info.last_displayed) then
				local obj = object_dup(store_info.stock[info.key_table.items[key]])

				-- Quantity ?
				local qt = 1
				if obj.number > 1 then
					qt = get_quantity("Quantity (1-"..obj.number.."):", obj.number)
					if qt < 1 then delete_object(obj) break end
				end
				obj.number = qt

				local desc, price = object_desc(obj, true, 3, true), store.price_item(store_info, obj, nil) * obj.number

				-- Check player gold
				if player.au < price then
					message("You do not have enough gold.")
					delete_object(obj)
					break
				end

				-- Confirm
				if not get_check("Buy ".. desc .. " for " .. price .. "gp ?") then
					delete_object(obj)
					break
				end

				-- Check capacity
				if not inven_carry_okay(obj) then
					message("You cannot carry that many different items.")
					delete_object(obj)
					break
				end

				-- Remember that object was bought at the store
				if obj.found == 0 then
					obj.found      = OBJ_FOUND_STORE
					obj.found_aux1 = store_info.st_idx
				end

				-- Learn about object after buying it
				make_item_fully_identified(obj)

				-- Grab it
				inven_carry_inven(player.inventory[INVEN_INVEN], obj, false)

				-- Remove it from the store
				local item = object_make_idx("store", 0, info.key_table.items[key], store_info)
				item_increase(item, -qt)
				item_optimize(item)

				-- Remove gold
				player.au = player.au - price

				message("Done! You bought ".. desc .. " for " .. price ..
						" gold.")
				break
			end
		end
	end
}

new_store_action
{
	define_as = "store.ACTION_DROP"
	name = "Drop an item"
	letter = 'd'
	letter_aux = 's'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info, info)
		local ret, item = get_item("Drop which item?", "You have no items to drop.", USE_INVEN, function(obj) return true end)
		if not ret then return end
		local obj = object_dup(get_object(item))

		-- Quantity ?
		local qt = 1
		if obj.number > 1 then
			qt = get_quantity("Quantity (1-"..obj.number.."):", obj.number)
			if qt < 1 then delete_object(obj) return end
		end
		obj.number = qt

		local desc = object_desc(obj, true, 3, true)

		-- Grab it
		if inven_carry_inven(store_info.stock, obj, false) == -1 then
			message("There is no space to store it.")
			return
		end

		-- Remove it
		item_increase(item, -qt)
		item_optimize(item)

		message("You dropped ".. desc .. ".")
	end
}

new_store_action
{
	define_as = "store.ACTION_GET"
	name = "Get an item"
	letter = 'g'
	letter_aux = 'p'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info, info)
		if is_inven_empty(store_info.stock) then
			message("Nothing in store to get")
			return
		end

		while not nil do
			local ret, key = get_com("(Items a-" .. strchar(strbyte('a') + info.last_displayed) .. ", ESC to exit) Which item are you interested in?")
			if not ret then return end

			key = strchar(key)
			if info.key_table.items[key] and (strbyte(key) - strbyte('a') <= info.last_displayed) then
				local obj = object_dup(store_info.stock[info.key_table.items[key]])

				-- Quantity ?
				local qt = 1
				if obj.number > 1 then
					qt = get_quantity("Quantity (1-"..obj.number.."):", obj.number)
					if qt < 1 then delete_object(obj) break end
				end
				obj.number = qt

				local desc = object_desc(obj, true, 3, true)

				-- Check capacity
				if not inven_carry_okay(obj) then
					message("You cannot carry that many different items.")
					delete_object(obj)
					break
				end

				-- Grab it
				inven_carry_inven(player.inventory[INVEN_INVEN], obj, false)

				-- Remove it from the store
				local item = object_make_idx("store", 0, info.key_table.items[key], store_info)
				item_increase(item, -qt)
				item_optimize(item)

				message("You picked up ".. desc .. ".")
				break
			end
		end
	end
}

new_store_action
{
	define_as = "store.ACTION_EXAMINE"
	name = "Examine an item"
	letter = 'x'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info, info)
		if is_inven_empty(store_info.stock) then
			message("Nothing in store to examine")
			return
		end

		while not nil do
			local ret, key = get_com("(Items a-" .. strchar(strbyte('a') + info.last_displayed) .. ", ESC to exit) Which item do you want to examine?")
			if not ret then return end

			key = strchar(key)
			if info.key_table.items[key] and (strbyte(key) - strbyte('a') <= info.last_displayed) then
				local obj = object_dup(store_info.stock[info.key_table.items[key]])

				-- Display it
				object_out_desc (obj)

				-- If it contains spells, browse them
				if obj.flags[FLAG_SPELLBOOK] then
					browse_school_spell(obj)
				end

				break
			end
		end
	end
}

new_store_action
{
	define_as = "store.ACTION_REST"
	name = "Rest for the night/day"
	letter = 'r'
	action_restr = "all"
	costs = function(store_info, desc)
		if desc then
			if time.is_daylight(turn) then
				return "Rest for the day", "(20gp)", color.YELLOW, color.YELLOW
			else
				return "Rest for the night", "(20gp)", color.YELLOW, color.YELLOW
			end
		else
			-- Must cure HP draining status first
			if timed_effect.get(timed_effect.POISON) or
				timed_effect.get(timed_effect.CUT)
			then
				return "You'd better heal your wounds or you might " ..
					"just die from them in your sleep."
			end

			if player.au >= 20 then
				return 20
			else
				return "You do not have enough gold to rest for the " ..
					iif(time.is_daylight(turn), "day", "night") .. "."
			end
		end
	end

	action = function()
		-- Wait for sunset/sunrise
		local was_daylight = time.is_daylight(turn)
		while time.is_daylight(turn) == was_daylight do
			turn = turn + 10 * time.MINUTE
		end

		-- Regen
		player.chp(player.mhp())
		player.csp(player.msp())

		-- Restore status
		timed_effect(timed_effect.BLIND, 0)
		timed_effect(timed_effect.CONFUSED, 0)
		timed_effect(timed_effect.STUN, 0)

		-- Message
		if not time.is_daylight(turn) then message("You awake refreshed for the new night.")
		else message("You awake refreshed for the new day.") end

		-- Dungeon stuff
		player.wild_force_side_y_ok = false
		player.wild_force_side_x_ok = false
		player.regen_town()
	end
}

new_store_action
{
	define_as = "store.ACTION_CHAT"
	name = "Chat"
	letter = 'c'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info)
		local owner = ow_info(store_info.owner)
		local c_ptr = cave(player.py, player.px)
		local funcname = c_ptr.flags[FLAG_STORE_OWNER]
		if funcname and dialogs[funcname] then
			dialogs[funcname]()
		end
	end
}

new_store_action
{
	define_as = "store.ACTION_RESEARCH_MON"
	name = "Research a monster"
	letter = 'r'
	letter_aux = 'r'
	action_restr = "all"
	costs = function(store_info, desc)
		if desc then
			return "Research a monster", "(1500 gp per monster)",
			       color.WHITE, color.YELLOW
		else
			return 0
		end
	end
	action = function(store_info, info)
			if player.au < 1500 then
				message("You don't have enough gold to research a monster.")
				return
			end

			local ret, char = get_com("Enter character to be identified " ..
									 "or (Ctrl-A, Ctrl-U, Ctrl-N, Ctrl-M):")

			if not ret then return end

			local key = strchar(char)

			local by_char, name, unique, non_unique

			if char == KTRL('M') then
				name = get_string("Name: ")

				if not name then
					return
				end
				name = clean_whitespace(name)

				if strlen(name) == 0 then
					return
				end

				name = strlower(name)
			elseif char == KTRL('A') then
				-- All monsters
			elseif char == KTRL('U') then
				unique = true
			elseif char == KTRL('N') then
				non_unique = true
			else
				if get_symbol_info(key) == "Unknown symbol" then
					message("Unknown symbol '" .. key .. "'")
					return
				end
				by_char = true
			end

			local list = {}
			for i = 1, max_r_idx do
				local r_ptr = r_info[i]

				if by_char and r_ptr.d_char ~= char then
				elseif unique and not r_ptr.flags[FLAG_UNIQUE] then
				elseif non_unique and r_ptr.flags[FLAG_UNIQUE] then
				elseif name and not strfind(strlower(r_ptr.name), name) then
				else
					tinsert(list, i - 1)
				end
			end

			if getn(list) == 0 then
				if name then
					message("No monsters matching '" .. name .. "'")
					return
				elseif by_char then
					message("No monster with symbol '" .. key .. "'")
					return
				else
					message(color.VIOLET, "ERROR: No monsters found!")
					return
				end
			end

			-- Remove monsters we already know everything about,
			-- unless the user is researching by monster name (just
			-- in case, for some weird reason, they want to
			-- re-research something they already know everything
			-- about).
			if not name then
				local i = 1
				while i <= getn(list) do
					local r_ptr = r_info[list[i] + 1]
					local mem_entry = memory.get_entry(r_ptr, 0)

					if mem_entry.knowall and mem_entry.knowall ~= 0 then
						-- Already know everything about it
						tremove(list, i)
					else
						i = i + 1
					end
				end

				if getn(list) == 0 then
					message("You already know everything about every " ..
							"monster of that type.  To research a " ..
								" particular alrady-known monster anyways, " ..
								"look it up by name (CTRL-M).")
					return
				end
			end

			local pos = 1
			while true do
				local r_idx = list[pos]
				local r_ptr = r_info[r_idx + 1]
				local str

				term.erase(0, 0, 255)
				term.gotoxy(0, 0)

				if r_ptr.flags[FLAG_UNIQUE] then
					str = book_capitals(r_ptr.name)
				else
					str = "The " .. strcap(r_ptr.name)
				end

				term.addstr(-1, color.WHITE, str)
				term.addstr(-1, color.WHITE, " ('")
				term.addstr(-1, r_ptr.d_attr, strchar(r_ptr.d_char))
				term.addstr(-1, color.WHITE, "')")

				term.addstr(-1, color.WHITE,
							" [(r)esearch, (n)ext, (p)rev]")

				char = term.inkey()

				if char == ESCAPE then
					break
				end

				key = strlower(strchar(char))

				if key == "r" then
					-- Remember/learn the monster
					if r_ptr.r_sights == 0 then
						r_ptr.r_sights = 1
					end

					local mem_entry = memory.get_entry(r_ptr, 0)
					mem_entry.knowall = 1

					term.save()
					local old_screen = term.save_to()

					-- Actually show it
					term.gotoxy(1, 0)
					memory.main(r_idx, 0)

					-- Pause
					term.inkey()

					term.load_from(old_screen, true)
					term.load()

					-- Charge money
					player.au = player.au - 1500

					break
				elseif key == "n" then
					pos = pos + 1
				elseif key == "p" then
					pos = pos - 1
				end

				-- Wrap-around
				if pos > getn(list) then
					pos = 1
				elseif pos < 1 then
					pos = getn(list)
				end
			end -- while true do

			term.erase(0, 0, 255)
			term.gotoxy(0, 0)
		end
} -- store.ACTION_RESEARCH_MON

new_store_action
{
	define_as = "store.ACTION_BOUNTY_QUEST"
	name = "Bounty offer"
	letter = 'b'
	letter_aux = 'b'
	action_restr = "all"
	costs = { 0, 0, 0 }
	action = function(store_info, info)
			local status = quest(QUEST_BOUNTY).status

			local ret, raw_key, key

			if status == QUEST_STATUS_IGNORED or
				status == QUEST_STATUS_UNTAKEN or
				bounty.quest_r_idx == 0 then
				message("I'm in the need of the flesh of a certain " ..
						"monster, but I'm stuck here minding the shanty. " ..
							"If you'll fetch some for me, I'll teach you " ..
							"some of what I know.")

				ret, raw_key = get_com("Accept the quest? [y/n]")

				if not ret then
					return
				end

				key = strlower(strchar(raw_key))

				if key ~= "y" then
					return
				end

				acquire_quest(QUEST_BOUNTY)
			end

			if bounty.quest_r_idx ~= 0 then
				local found = {}
				local r_ptr = r_info[bounty.quest_r_idx + 1]

				for_inventory(player, INVEN_INVEN, INVEN_TOTAL,
					function(obj, i, j, item)
						if obj.tval ~= TV_CORPSE then
							return
						elseif obj.sval ~= SV_CORPSE_CORPSE and
							obj.sval ~= SV_CORPSE_MEAT then
							return
						end

						local monst = obj.flags[FLAG_MONSTER_OBJ]

						if monst.r_idx ~= bounty.quest_r_idx then
							return
						end

						%found.obj  = obj
						%found.item = item
					end)

				if not found.obj then
					message("You don't have any " ..
							r_ptr.name .. " flesh to give the beastmaster.")
					return
				end

				ret, raw_key = get_com("Hand over your " ..
									   object_desc(found.obj) .. " to the " ..
										   "beastmaster? [y/n]")

				if not ret then
					return
				end

				key = strlower(strchar(raw_key))

				if key ~= 'y' then
					return
				end

				item_increase(found.item, -1)
				item_describe(found.item)
				item_optimize(found.item)
			end

			-- Either we just returned the quest item to the beast
			-- master, or we're acquiring the quest for the first time
			local prev_idx = bounty.quest_r_idx
			bounty.get_new_monster()

			if bounty.quest_r_idx == 0 then
				message("The beastmaster has nothing for you to do " ..
						"at the moment.")
				-- Oops!!
				quest_desc(QUEST_BOUNTY, 0, "Nothing to do right now.")
				return
			end

			local name  = monster_race_desc(bounty.quest_r_idx, 0)

			if prev_idx ~= 0 then
				message("Ah, well done!")
				bounty.increase_skill()
				message("Now I need some flesh from " .. name ..
						".  If you bring it to me I'll teach you more " ..
							" monster lore.")
			else
				message("You need to bring me back the corpse of " ..
						name .. ", or a piece of its flesh.")
			end

			quest_desc(QUEST_BOUNTY, 0, "You need to bring the beastmaster the corpse of")
			quest_desc(QUEST_BOUNTY, 1, name .. ", or a piece of its flesh.")
		end
} -- store.ACTION_BOUNTY_QUEST
