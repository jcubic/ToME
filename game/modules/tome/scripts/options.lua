--
-- ToME options
--

option_page
{
	name = "User Interface"
	options =
	{
		{ name = "Ingame contextual help" variable = "option_ingame_help" default = true },
		{ name = "Rogue-like commands" variable = "rogue_like_commands" default = nil },
		{ hidden = true name = "Activate quick messages" variable = "quick_messages" default = true },
		{ advanced = true name = "Prompt for various information" variable = "other_query_flag" default = nil },
		{ advanced = true name = "Prompt before picking things up" variable = "carry_query_flag" default = nil },
		{ advanced = true name = "Use old target by default" variable = "use_old_target" default = nil },
		{ advanced = true name = "Pick things up by default" variable = "always_pickup" default = nil },
		{ hidden = true name = "Prompt before picking up heavy objects" variable = "prompt_pickup_heavy" default = true },
		{ advanced = true name = "Repeat obvious commands" variable = "always_repeat" default = true },
		{ hidden = true name = "Show dungeon level in feet" variable = "depth_in_feet" default = nil },
		{ hidden = true name = "Merge inscriptions when stacking" variable = "stack_force_notes" default = true },
		{ hidden = true name = "Merge discounts when stacking" variable = "stack_force_costs" default = nil },
		{ hidden = true name = "Show labels in object listings" variable = "show_labels" default = true },
		{ hidden = true name = "Show weights in object listings" variable = "show_weights" default = true },
		{ advanced = true name = "Always show inventory list when needed" variable = "easy_inven" default = true },
		{ advanced = true name = "Show graphics in inventory list" variable = "show_inven_graph" default = true },
		{ advanced = true name = "Show graphics in equipment list" variable = "show_equip_graph" default = true },
		{ advanced = true name = "Show graphics in stores" variable = "show_store_graph" default = true },
		{ advanced = true name = "Show choices in certain sub-windows" variable = "show_choices" default = true },
		{ advanced = true name = "Show details in certain sub-windows" variable = "show_details" default = true },
		{ name = "Audible bell (on errors, etc)" variable = "ring_bell" default = nil },
		{ hidden = true name = "Use color if possible (slow)" variable = "use_color" default = true },
		{ name = "Show the experience needed for next level" variable = "exp_need" default = nil },
  		{ name = "Numpad keys always run" variable = "keypad_autorun" default = nil },
		{ hidden = true name = "Player char represent his/her health" variable = "player_char_health" default = true },
		{ hidden = true name = "Stats are represented in a linear way" variable = "linear_stats" default = true },
		{ advanced = true name = "In option windows, just omit the select char" variable = "inventory_no_move" default = nil },
		{
			advanced = true
			name = "Item names capitalization"
			variable = "items_capitalize"
			on_set = function(val)
				if not game_options.character_generated then return end
				if val then
					if game_options.items_capitalize == game_options.CAPITALIZE_NONE then
						game_options.items_capitalize = game_options.CAPITALIZE_ARTS
					elseif game_options.items_capitalize == game_options.CAPITALIZE_ARTS then
						game_options.items_capitalize = game_options.CAPITALIZE_ALL
					end
				else
					if game_options.items_capitalize == game_options.CAPITALIZE_ALL then
						game_options.items_capitalize = game_options.CAPITALIZE_ARTS
					elseif game_options.items_capitalize == game_options.CAPITALIZE_ARTS then
						game_options.items_capitalize = game_options.CAPITALIZE_NONE
					end
				end
			end
			on_display = function(val)
				local descs = { "None", "Artifacts", "All" }
				return descs[val], strlen(descs[val])
			end
			default = function() game_options.items_capitalize = game_options.CAPITALIZE_ARTS end
		},
	},
}
option_page
{
	name = "Disturbance"
	options =
	{
		{ advanced = true name = "Run past stairs" variable = "find_ignore_stairs" default = nil },
		{ advanced = true name = "Run through open doors" variable = "find_ignore_doors" default = true },
		{ advanced = true name = "Run past known corners" variable = "find_cut" default = nil },
		{ advanced = true name = "Run into potential corners" variable = "find_examine" default = true },
		{ advanced = true name = "Disturb whenever any monster moves" variable = "disturb_move" default = nil },
		{ advanced = true name = "Disturb whenever viewable monster moves" variable = "disturb_near" default = true },
		{ advanced = true name = "Disturb whenever map panel changes" variable = "disturb_panel" default = true },
		{ advanced = true name = "Disturb whenever leaving trap-detected area" variable = "disturb_detect" default = true },
		{ advanced = true name = "Disturb whenever player state changes" variable = "disturb_state" default = true },
		{ advanced = true name = "Disturb whenever boring things happen" variable = "disturb_minor" default = true },
		{ advanced = true name = "Disturb whenever random things happen" variable = "disturb_other" default = nil },
		{ hidden = true name = "Alert user to critical hitpoints" variable = "alert_hitpoint" default = nil },
		{ advanced = true name = "Alert user to various failures" variable = "alert_failure" default = nil },
		{ advanced = true name = "Get last words when the character dies" variable = "last_words" default = true },
		{ hidden = true name = "Allow shopkeepers and uniques to speak" variable = "speak_unique" default = true },
		{ name = "No query to destroy known worthless items" variable = "auto_destroy" default = true },
		{ name = "Confirm to wear/wield known cursed items" variable = "confirm_wear" default = true },
		{ name = "Prompt before exiting a dungeon level" variable = "confirm_stairs" default = nil },
		{ advanced = true name = "Disturb when visible pets move" variable = "disturb_pets" default = nil },
		{ name = "Automatically open doors" variable = "easy_open" default = true },
		{ name = "Automatically disarm traps" variable = "easy_disarm" default = true },
		{ name = "Automatically tunnel walls" variable = "easy_tunnel" default = nil },
		{ name = "Automatically clear '-more-' prompts" variable = "auto_more" default = nil },
		{
			name = "Hitpoint Warning"
			variable = "hitpoint_warn"
			on_set = function(val)
				if not game_options.character_generated then return end
				if val then
					if game_options.hitpoint_warn < 9 then
						game_options.hitpoint_warn = game_options.hitpoint_warn + 1
					end
				else
					if game_options.hitpoint_warn > 0 then
						game_options.hitpoint_warn = game_options.hitpoint_warn - 1
					end
				end
			end
			on_display = function(val)
				val = val * 10
				val = val.."%"
				return val, strlen(val)
			end
		}
	},
}
option_page
{
	name = "Game Play"
	options =
	{
		{ hidden = true name = "Auto-haggle in stores" variable = "auto_haggle" default = true },
		{ hidden = true name = "Auto-scum for good levels" variable = "auto_scum" default = true },
		{ hidden = true name = "Allow weapons and armour to stack" variable = "stack_allow_items" default = true },
		{ hidden = true name = "Allow wands/orbs/rods to stack" variable = "stack_allow_wands" default = true },
		{ advanced = true name = "Expand the power of the look command" variable = "expand_look" default = nil },
		{ advanced = true name = "Expand the power of the list commands" variable = "expand_list" default = nil },
		{ hidden = true name = "Map remembers all perma-lit grids" variable = "view_perma_grids" default = true },
		{ hidden = true name = "Map remembers all torch-lit grids" variable = "view_torch_grids" default = nil },
		{ hidden = true name = "Allow some monsters to carry light" variable = "monster_lite" default = true },
		{ hidden = true name = "Generate dungeons with aligned rooms" variable = "dungeon_align" default = true },
		{ hidden = true name = "Generate dungeons with connected stairs" variable = "dungeon_stair" default = true },
		{ name = "Use special symbols for the player char" variable = "player_symbols" default = nil },
		{ name = "Plain object descriptions" variable = "plain_descriptions" default = true },
		{ hidden = true name = "Monsters learn from their mistakes" variable = "smart_learn" default = nil },
		{ hidden = true name = "Monsters exploit players weaknesses" variable = "smart_cheat" default = nil },
		{ hidden = true name = "Monsters behave stupidly" variable = "stupid_monsters" default = nil },
		{ hidden = true name = "Allow unusually small dungeon levels" variable = "small_levels" default = false },
		{ hidden = true name = "Allow empty 'arena' levels" variable = "empty_levels" default = true },
	},
}
option_page
{
	name = "Efficiency"
	options =
	{
		{
			advanced = true
			name = "Compress savefile factor(higher=slower)"
			variable = "savefile_compress"
			on_set = function(val)
				if not game_options.character_generated then return end
				if val then
					if game_options.savefile_compress < 9 then
						game_options.savefile_compress = game_options.savefile_compress + 1
					end
				else
					if game_options.savefile_compress > 0 then
						game_options.savefile_compress = game_options.savefile_compress - 1
					end
				end
			end
			on_display = function(val)
				if val > 0 then
					return val, strlen(val)
				else
					return "No compression", strlen("No compression")
				end
			end
			default = function() game_options.savefile_compress = 4 end
		},
		{ hidden = true name = "Reduce lite-radius when running" variable = "view_reduce_lite" default = nil },
		{ hidden = true name = "Reduce view-radius in town" variable = "view_reduce_view" default = nil },
		{ advanced = true name = "Avoid checking for user abort" variable = "avoid_abort" default = nil },
		{ advanced = true name = "Avoid extra shimmering (fast)" variable = "avoid_shimmer" default = nil },
		{ advanced = true name = "Avoid processing special colors (fast)" variable = "avoid_other" default = nil },
		{ advanced = true name = "Flush input on various failures" variable = "flush_failure" default = true },
		{ advanced = true name = "Flush input whenever disturbed" variable = "flush_disturb" default = nil },
		{ advanced = true name = "Flush input before every command" variable = "flush_command" default = nil },
		{ advanced = true name = "Flush output before every command" variable = "fresh_before" default = true },
		{ advanced = true name = "Flush output after every command" variable = "fresh_after" default = nil },
		{ advanced = true name = "Flush output after every message" variable = "fresh_message" default = nil },
		{ hidden = true name = "Compress messages in savefiles" variable = "compress_savefile" default = true },
		{ name = "Hilite the player with the cursor" variable = "hilite_player" default = nil },
		{ name = "Use special colors for torch-lit/dark grids" variable = "view_lited_grid" default = nil },
		{ name = "Center the view on the player (very slow)" variable = "center_player" default = nil },
	},
}
option_page
{
	name = "Birth Options"
	can_set = function(val)
		if game_options.character_generated then
			msg_box("Birth options are only changable at birth")
			return nil
		else
			return true
		end
	end
	options =
	{
		{ name = "Maximise stats" variable = "maximize" default = true },
		{ hidden = true name = "Preserve artifacts" variable = "preserve" default = true },
		{ name = "Specify 'minimal' stats" variable = "autoroll" default = true },
		{ name = "Generate character using a point system" variable = "point_based" default = nil },
		{ name = "Generate persistent dungeons" variable = "permanent_levels" default = nil },
		{ name = "Always generate very unusual rooms" variable = "ironman_rooms" default = nil },
		{ name = "Allow notes to be written to a file" variable = "take_notes" default = true },
		{ name = "Automatically note important events" variable = "auto_notes" default = true },
		{ name = "Fast autoroller(NOT on multiuser systems)" variable = "fast_autoroller" default = nil },
		{ name = "Allow use of some 'joke' monsters" variable = "joke_monsters" default = nil },
		{ name = "Always make small levels" variable = "always_small_level" default = nil },
	},
}

option_page
{
	name = "Windows Options"
	variable = "dummy_windows"
	on_set = function(val)
		if not game_options.character_generated then game_options.automatizer_enabled = val
		else
			do_cmd_options_win()
		end
	end
	on_display = function()
		return "", 0, color.WHITE
	end
}

option_page
{
	name = "Autosave Options"
	options =
	{
		{ name = "Autosave when entering new levels" variable = "autosave_l" default = nil },
		{ name = "Timed autosave" variable = "autosave_t" default = nil },
		{
			name = "Autosave Frequency", variable = "autosave_freq", default = 1000
			on_set = function(val)
				local freqs = { 10, 50, 250, 500, 1000, 2500, 5000, 10000, 25000 }
				local cur = 1
				for i, f in freqs do
					if f == game_options.autosave_freq then cur = i break end
				end
				if val then
					game_options.autosave_freq = freqs[min(cur + 1, getn(freqs))]
				else
					game_options.autosave_freq = freqs[max(cur - 1, 1)]
				end
			end,
			on_display = function(val)
				return tostring(val), strlen(tostring(val))
			end
		}
	}
}

option_page
{
	name = "Automatizer"
	variable = "automatizer_enabled"
	on_set = function(val)
		if not game_options.character_generated then game_options.automatizer_enabled = val
		else
			if val then
				do_cmd_automatizer()
			else
				game_options.automatizer_enabled = nil
			end
		end
	end
}

option_page
{
	name = "Base Delay Factor"
	variable = "delay_factor"
	on_set = function(val)
		if not game_options.character_generated then return end
		if val then
			if game_options.delay_factor < 9 then
				game_options.delay_factor = game_options.delay_factor + 1
			end
		else
			if game_options.delay_factor > 0 then
				game_options.delay_factor = game_options.delay_factor - 1
			end
		end
	end
	on_display = function(val)
		val = val * val * val
		val = val.." msec"
		return val, strlen(val)
	end
	default = function() game_options.delay_factor = 3 end
}

option_page
{
	advanced = true
	name = "Cheat Options"
	can_set = function(val)
		if wizard then return true end
		msg_box("Cheat options are only changable for wizards")
		return nil
	end
	options =
	{
		{ variable = "cheat_peek" name = "Peek into object creation" default = nil },
		{ variable = "cheat_hear" name = "Peek into monster creation" default = nil },
		{ variable = "cheat_room" name = "Peek into dungeon creation" default = nil },
		{ variable = "cheat_xtra" name = "Peek into something else" default = nil },
		{ variable = "cheat_know" name = "Know complete monster info" default = nil },
		{ variable = "cheat_live" name = "Allow player to avoid death" default = nil }
	}
}

option_page{ name = "Show advanced options" variable = "use_advanced_options" default = nil }
