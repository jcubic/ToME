option_page
{
	hidden = true
	name = "User Interface"
	options =
	{
		{ name = "Ingame contextual help" variable = "option_ingame_help" default = true },
		{ name = "Rogue-like commands" variable = "rogue_like_commands" default = nil },
		{ name = "Activate quick messages" variable = "quick_messages" default = true },
		{ name = "Prompt for various information" variable = "other_query_flag" default = nil },
		{ name = "Prompt before picking things up" variable = "carry_query_flag" default = nil },
		{ name = "Use old target by default" variable = "use_old_target" default = nil },
		{ name = "Pick things up by default" variable = "always_pickup" default = nil },
		{ name = "Prompt before picking up heavy objects" variable = "prompt_pickup_heavy" default = true },
		{ name = "Repeat obvious commands" variable = "always_repeat" default = true },
		{ name = "Show dungeon level in feet" variable = "depth_in_feet" default = nil },
		{ name = "Merge inscriptions when stacking" variable = "stack_force_notes" default = true },
		{ name = "Merge discounts when stacking" variable = "stack_force_costs" default = nil },
		{ name = "Show labels in object listings" variable = "show_labels" default = true },
		{ name = "Show weights in object listings" variable = "show_weights" default = true },
		{ name = "Show graphics in inventory list" variable = "show_inven_graph" default = true },
		{ name = "Show graphics in equipment list" variable = "show_equip_graph" default = true },
		{ name = "Show graphics in stores" variable = "show_store_graph" default = true },
		{ name = "Show choices in certain sub-windows" variable = "show_choices" default = true },
		{ name = "Show details in certain sub-windows" variable = "show_details" default = true },
		{ name = "Audible bell (on errors, etc)" variable = "ring_bell" default = nil },
		{ hidden = true name = "Use color if possible (slow)" variable = "use_color" default = true },
		{ name = "Show the experience needed for next level" variable = "exp_need" default = nil },
		{ name = "Player char represent his/her health" variable = "player_char_health" default = true },
		{ name = "Stats are represented in a linear way" variable = "linear_stats" default = true },
		{ name = "In option windows, just omit the select char" variable = "inventory_no_move" default = nil },
	},
}
option_page
{
	hidden = true
	name = "Disturbance"
	options =
	{
		{ name = "Run past stairs" variable = "find_ignore_stairs" default = nil },
		{ name = "Run through open doors" variable = "find_ignore_doors" default = true },
		{ name = "Run past known corners" variable = "find_cut" default = nil },
		{ name = "Run into potential corners" variable = "find_examine" default = true },
		{ name = "Disturb whenever any monster moves" variable = "disturb_move" default = nil },
		{ name = "Disturb whenever viewable monster moves" variable = "disturb_near" default = true },
		{ name = "Disturb whenever map panel changes" variable = "disturb_panel" default = true },
		{ name = "Disturb whenever leaving trap-detected area" variable = "disturb_detect" default = true },
		{ name = "Disturb whenever player state changes" variable = "disturb_state" default = true },
		{ name = "Disturb whenever boring things happen" variable = "disturb_minor" default = true },
		{ name = "Disturb whenever random things happen" variable = "disturb_other" default = nil },
		{ name = "Alert user to critical hitpoints" variable = "alert_hitpoint" default = nil },
		{ name = "Alert user to various failures" variable = "alert_failure" default = nil },
		{ name = "Get last words when the character dies" variable = "last_words" default = true },
		{ name = "Allow shopkeepers and uniques to speak" variable = "speak_unique" default = true },
		{ name = "No query to destroy known worthless items" variable = "auto_destroy" default = true },
		{ name = "Confirm to wear/wield known cursed items" variable = "confirm_wear" default = true },
		{ name = "Prompt before exiting a dungeon level" variable = "confirm_stairs" default = nil },
		{ name = "Disturb when visible pets move" variable = "disturb_pets" default = nil },
		{ name = "Automatically open doors" variable = "easy_open" default = true },
		{ name = "Automatically disarm traps" variable = "easy_disarm" default = true },
		{ name = "Automatically tunnel walls" variable = "easy_tunnel" default = nil },
		{ name = "Automatically clear '-more-' prompts" variable = "auto_more" default = nil },
	},
}
option_page
{
	hidden = true
	name = "Game Play"
	options =
	{
		{ name = "Auto-haggle in stores" variable = "auto_haggle" default = true },
		{ name = "Auto-scum for good levels" variable = "auto_scum" default = false },
		{ name = "Allow weapons and armour to stack" variable = "stack_allow_items" default = true },
		{ name = "Allow wands/staffs/rods to stack" variable = "stack_allow_wands" default = true },
		{ name = "Expand the power of the look command" variable = "expand_look" default = nil },
		{ name = "Expand the power of the list commands" variable = "expand_list" default = nil },
		{ name = "Map remembers all perma-lit grids" variable = "view_perma_grids" default = true },
		{ name = "Map remembers all torch-lit grids" variable = "view_torch_grids" default = nil },
		{ name = "Allow some monsters to carry light" variable = "monster_lite" default = true },
		{ name = "Generate dungeons with aligned rooms" variable = "dungeon_align" default = true },
		{ name = "Generate dungeons with connected stairs" variable = "dungeon_stair" default = true },
		{ name = "Use special symbols for the player char" variable = "player_symbols" default = nil },
		{ name = "Plain object descriptions" variable = "plain_descriptions" default = true },
		{ name = "Monsters learn from their mistakes" variable = "smart_learn" default = nil },
		{ name = "Monsters exploit players weaknesses" variable = "smart_cheat" default = nil },
		{ name = "Monsters behave stupidly" variable = "stupid_monsters" default = nil },
		{ name = "Allow unusually small dungeon levels" variable = "small_levels" default = true },
		{ name = "Allow empty 'arena' levels" variable = "empty_levels" default = true },
	},
}
option_page
{
	hidden = true
	name = "Efficiency"
	options =
	{
		{
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
		{ name = "Reduce lite-radius when running" variable = "view_reduce_lite" default = nil },
		{ name = "Reduce view-radius in town" variable = "view_reduce_view" default = nil },
		{ name = "Avoid checking for user abort" variable = "avoid_abort" default = nil },
		{ name = "Avoid extra shimmering (fast)" variable = "avoid_shimmer" default = nil },
		{ name = "Avoid processing special colors (fast)" variable = "avoid_other" default = nil },
		{ name = "Flush input on various failures" variable = "flush_failure" default = true },
		{ name = "Flush input whenever disturbed" variable = "flush_disturb" default = nil },
		{ name = "Flush input before every command" variable = "flush_command" default = nil },
		{ name = "Flush output before every command" variable = "fresh_before" default = true },
		{ name = "Flush output after every command" variable = "fresh_after" default = nil },
		{ name = "Flush output after every message" variable = "fresh_message" default = nil },
		{ name = "Compress messages in savefiles" variable = "compress_savefile" default = true },
		{ name = "Hilite the player with the cursor" variable = "hilite_player" default = nil },
		{ name = "Use special colors for torch-lit grids" variable = "view_yellow_lite" default = nil },
		{ name = "Use special colors for 'viewable' grids" variable = "view_bright_lite" default = nil },
		{ name = "Use special colors for wall grids (slow)" variable = "view_granite_lite" default = nil },
		{ name = "Use special colors for floor grids (slow)" variable = "view_special_lite" default = nil },
		{ name = "Center the view on the player (very slow)" variable = "center_player" default = nil },
	},
}
option_page
{
	hidden = true
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
		{ name = "Preserve artifacts" variable = "preserve" default = true },
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
}

option_page
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
