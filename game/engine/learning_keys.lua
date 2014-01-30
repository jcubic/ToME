-- SLAK(Smart Learning Auto Keys)!
-- Yes, it's a mouthful but I like it!

constant("slak", {})

-- The list of SLAK actions, this uses the repeat_keys framework
-- this is NOT reset on birth so actions stay bound from char to the next
slak.actions = {}
slak.trigger_actions = {}
slak.learning = nil
slak.skip_chain_destroy = nil
add_loadsave("slak.actions", {})
add_loadsave("slak.trigger_actions", {})

function slak.display()
	local list =
	{
		{
			title = "Bind a key"
			desc =
			{
				"Lets you bind *ANY* key to an action."
				"Beware that you can very well bind actions to keys which already have a use this way."
			}
			exec = function()
				local ret = msg_box("Press the key to bind, and then execute the action you want to bind.\n(Press Escape to cancel this prompt)")

				if term.apply_keymap(ret) then
					ret = term.apply_keymap(ret)

					if strlen(ret) > 1 then
						msg_box("You can't rebind that key; please pick " ..
								"another.")
						return true, true
					end
					ret = strbyte(ret)
				end

				if ret ~= ESCAPE then
					slak.learning = ret
					slak.skip_chain_destroy = true
					return true, true
				else
					return false, true
				end
			end
		}
		{
			title = "Unbind a key"
			desc =
			{
				"Unbinds a previously bound key action."
				"Once you select this option the next key on press on the main game screen will be unbound."
			}
			exec = function()
				local ret = msg_box("Press the key to unbind")

				if ret == ESCAPE then
					return false, true
				end

				local actions
				if ret < 0 then
					actions = slak.trigger_actions
					ret     = -ret
				else
					actions = slak.actions
				end

				if actions[ret] then
					msg_box("Binding removed.")
					actions[ret] = nil
				else
					msg_box("No bindings found for this key.")
				end
				return true, true
			end
		}
		{
			title = "Unbind all keys"
			desc =
			{
				"Unbinds a previously bound key action."
				"Once you select this option the next key on press on the main game screen will be unbound."
			}
			exec = function()
				local ret = msg_box("Are you sure? This will erase all your macros (y/N)")

				if ret ~= strbyte('y') and ret ~= strbyte('Y') then
					return false, true
				end

				slak.actions = {}
				slak.trigger_actions = {}

				msg_box("All bindings removed.")

				return true, true
			end
		}
		{
			title = "Access macros"
			desc =
			{
				"The macro screen allows you to create keyboard macros."
				"A macro is a key bound to a series of keys that are all fired at once when the trigger is pressed."
				"The main difference from the simple action key bindings is that it can macro anything in the game (as opposed to just some actions) but it is a bit harder to work with and more error-prone."
				"#RAdvanced #Rusers #Ronly!"
			}
			exec = function()
				return true, false
			end
		}
	}

	local old_term = term.save_to()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local scroll = 1
	local return_val = true
	while not nil do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)
		display_list(3, 0, hgt - 8, 20, "Action keys binding", list, scroll, sel, color.LIGHT_GREEN, function(e, idx)
			return strchar(strbyte('a') + idx - 1)..") "..e.title
		end)

		draw_box(3, 21, hgt - 8, wid - 22)
		term.c_put_str(color.LIGHT_BLUE, " "..list[sel].title.." ", 3, 23)

		local y = 4
		y = display_module_desc(list[sel].desc, y, 22, hgt - y - 8, wid - 22)

		local raw = term.inkey()
		local key = strchar(raw)

		if raw == ESCAPE then
			break
		elseif key == '2' then
			repeat
				sel = sel + 1
				if sel > getn(list) then sel = getn(list) end
				if sel >= scroll + hgt then scroll = scroll + 1 end
			until sel ~= getn(list) + 1
		elseif key == '8' then
			repeat
				sel = sel - 1
				if sel < 1 then sel = 1 end
				if sel < scroll then scroll = scroll - 1 end
			until sel ~= getn(list) + 1
		elseif key == '\r' then
			local ret, val = list[sel].exec()
			if ret then return_val = val break  end
		else
			local new_sel = strbyte(key) - strbyte('a') - scroll + 2
			if list[new_sel] then
				local ret, val = list[new_sel].exec()
				if ret then return_val = val break end
			end
		end
	end

	term.load_from(old_term, true)

	return return_val
end

--- @fn
-- @brief For actions that did not take energy the actions must call this explicitly.
function slak.end_chain()
	if slak.learning then
		local acts = {}

		if (slak.learning < 0) then
			slak.trigger_actions[-slak.learning] = acts
		else
			slak.actions[slak.learning] = acts
		end

		local old = repeat_keys.cur_idx
		repeat_keys.cur_idx = 0
		while true do
			local ret, act = repeat_keys.pull()
			if not ret then break end
			tinsert(acts, act)
		end

		repeat_keys.cur_idx = old
		msg_box("Action successfully recorded.")
	end
	slak.learning = nil
end

-- Weird thing, we hook on game_start to place hooks
-- This ensures that those hooks are the first to be run for their actions
-- so that modules overriding keys wont get events before us
hook(hook.GAME_START, function()
hook
{
	-- Finish recording the SLAK action
	[hook.PLAYER_USED_ENERGY] = slak.end_chain,

	-- Fires a previously learned SLAK action
	[hook.KEYPRESS] = function(key)
		-- SLAK screen
		if key == strbyte('@') then
			return slak.display()
		end

		if slak.learning == key then
			msg_box("Binding cancelled.")
			slak.learning = nil
			slak.skip_chain_destroy = false
			return true
		end

		local actions
		if key < 0 then
			actions = slak.trigger_actions
			key     = -key
		else
			actions = slak.actions
		end

		if actions[key] then
			repeat_keys.cur_idx = 0
			repeat_keys.cur_cnt = 0
			for i = 1, getn(actions[key]) do
				repeat_keys.push(actions[key][i])
			end
			command_new = strbyte('n')
			return true
		end
	end

	-- Fire up the SLAK on any unbound macro trigger.
	[hook.KEYPRESS_UNKNOWN] = function(key)
		-- If key is non-negative, then it isn't an unbound macro trigger.
--		if key >= 0 then
--			return false
--		end

		local ret = msg_box("The key you just pressed is not currently used.\nDo you wish to bind this key to an action? (y/N)")
		if ret == strbyte('y') or ret == strbyte('Y') then
			slak.learning = key
			slak.skip_chain_destroy = true
			msg_box("You've chosen to bind a key to an action.\nThe escape key and the original button will stop the binding at any time.\nYou may unbind any bound keys by pressing '@'.\n#yPress ENTER, then perform the action you wish bound.")
			return true
		end
	end

	-- The second action that is made we cancel the SLAK
	[hook.REPEAT_KEY_CHAIN_DESTROY] = function()
		if not slak.skip_chain_destroy then
			if slak.learning then
				msg_box("No action was recorded, either you cancelled the action or\nthe action is not recordable.")
				slak.learning = nil
			end
		else
			slak.skip_chain_destroy = nil
		end
	end
}
end)
