-- Dialogue Subsystem
-- Created by Bucket Man for the Dragonball T module for the T-Engine
-- lordbucket@hotmail.com
-- Updated by DarkGod for integration into T-Engine

-- This subsystem is primarily intended for heavy duty, intensive,
-- long and complicated dialogue trees

-- Note: The sample included in this file is not complete. For examples
-- of this system in use, please see scripts/dialogue.lua

-- Load it with: load_subsystem("auto_id")
-- Once loaded it replaces the default dialog system of the engine
-- (and thus is accessed using dialog.* functions)
--
-- Parameters:
-- * compatibility_mode: Enables compatibility mode so that modules using the syntax of DBT can work

-- Note: THIS version of the subsystem includes changes from Bard's Tale
-- and is not exactly as originally, nor as ultimately...intended. Call
-- it a work in process.

-- Strengths of this system:
--  Very flexible. It can do anything. For example:
--   Colors and display easy to tweak
--   Multiple answers may be made to correspond to single results
--   Inidividual NPC speech portions may be selected randomly/specifically from a list
--   Multiple speakers may participate in a conversation
--   NPC's may be given the 'last word' in a conversation
--   Non-dialogue narration can easily be added
--   Possible to have multiple pages of display between responses
--   Multiple answers may be made to lead to the same response
--   Multiple consective threads are possible withina single dialogue
--  Each dialogue is its own function, so you can easily call it from
--   anywhere you want and feed/get from it any data you want
--  Even heavily nested Dialogue functions are generally easy to read
--  Easy to comment out dialogue portions for bugtesting
--  'Answers' may be tracked and used elsewhere
--
-- Weaknesses of this system:
--  Not space efficient. Since the module writer handles all dialogue
--   display and response logic, dialogue functions will typically be
--   twice the size of dialogs using the original ToME dialog system.
--  Not as 'simple.' More understanding is probably required from a
--   module writer to use this, though personally I find it far more
--   visually intuitive. It's much easier to quickly see what's going on.
--  Incorrect term.load() or term.save() placement will break the display

-- NOTE: By default players can NOT press escape to exit a dialogue!
-- This is EXTREMELY by design. If you want them to be able to, you will
-- have to create an answer condition for it

local is_compat = get_subsystem_param("advdialog", "compatibility_mode")

-- Used to store the various dialog states
declare_globals{"dialog_data"}

-- dialog_data is for keeping track of random dialog state information
dialog_data = {}
add_loadsave("dialog_data", {})

-- Gender pronouns for the various dialogs assigned in calc.lua
constant("gendernouns", {})

-- Reset the dialog system
globals().dialog = {}
settag(dialog, TAG_NAMESPACE)

dialog.tally = 0	-- Number of dialog responses
dialog.r = {}		-- Array containing the reponses
dialog.__a_minus = strbyte('a') - 1
dialog.__A_minus = strbyte('A') - 1
dialog.__1_minus = strbyte('1') - 1

-- For bugtesting. Typographical errors in the scripts will often
-- cause the entire file containing a dialog to not load, but
-- just because your dialog doesn't work doesn't mean that
-- this has happened.
-- If you want to check for this, executing: dialog.test()
-- from wizard mode will generate a reponse if the file itself
-- is loading properly.
function dialog.test()
	message("Hello! Dialogue exists!")
end

-- Basic initialization between dialog portions.
-- This will need to execute between EVERY set of
-- dialog/response exchanges
function dialog.prep()
	dialog.__preped = true
	dialog.tally = 0
	dialog.r = {}
end

-- Registers and displays a single response from the player
-- and associates what the script expects with what the player sees
-- Will need to execute for every possible response, every time
function dialog.helper(txt, expect)
	dialog.tally = dialog.tally + 1
	dialog.r[dialog.tally] = expect
	term.text_out(color.LIGHT_GREEN, strchar(dialog.tally + dialog.__a_minus))
	local out_text = ") " .. txt .. "  \n"
	term.text_out(out_text)
end

-- Gets a response from the player, from the list of
-- responses that have been registered above
function dialog.answer()
	if dialog.tally == 0 then
		message("Error! No dialog answers! Proceeding would generate a perma-loop! Breaking out...")
		return
	end
	while not nil do
		local key = term.inkey()
		key = strchar(key)
		for i = 1, dialog.tally do
			if key == strchar(i + dialog.__a_minus) or key == strchar(i + dialog.__A_minus) or key == strchar(i + dialog.__1_minus) then -- dialog.r[i] then
				local foo = dialog.r[i]
				dialog.prep()
				return foo
			end
		end
	end
end
-- Stores handle the screen differently, and regularly term.(save/load/clear)'s
-- don't work as they do elsewhere. This function wipes an arbitrary number of lines
-- from the display, and places the cursor back up top.
function dialog.store_hack(lines_to_wipe)
	for i = 0, (lines_to_wipe or 24) do
		term.blank_print("                                                                                ",i,0)
	end
	--reposition the cursor
	term.blank_print("",1,0)
end

-- To end a dialog with no possible response
function dialog.conclude()
	term.text_out(color.ORANGE, "(Press space) ")
	local key = 0
	while key ~= strbyte(' ') do
		key = term.inkey()
	end
	return
end

-- To allows dialog to use the correct gender pronoun
-- Updates whenever the character state is updated
-- in case the characters gender changes. Feel free to
-- add to this and use as desired. Should be friendly to
-- more-than-two genders as well.
hook(hook.CALC_BONUS, function ()
	if player.get_sex().title == "Female" then
		gendernouns.heshe = "she"
		gendernouns.himher = "her"
		gendernouns.hisher = "her"
		gendernouns.guygirl = "girl"
		gendernouns.boygirl = "girl"
		gendernouns.ladlass = "lass"
		gendernouns.demeaning = "little girl"
		gendernouns.french = "mademoiselle"
	else
		gendernouns.heshe = "he"
		gendernouns.himher = "him"
		gendernouns.hisher = "his"
		gendernouns.guygirl = "guy"
		gendernouns.boygirl = "boy"
		gendernouns.ladlass = "lad"
		gendernouns.demeaning = "kid"
		gendernouns.french = "monsieur"
	end
end)

-- New helper functions
dialog.speak = term.text_out

function dialog.speaker(col, who)
	if dialog.__preped then dialog.__preped = false dialog.__answered = nil end

	if not who then
		col, who = color.LIGHT_BLUE, col
		if who == player then col, who = color.LIGHT_RED, "You" end
	else
		if who == player then who = "You" end
	end
	-- Still no who, use the default one for this dialog
	if not who then who = dialog.__speak_who end
	term.text_out(col, who..": ")
end

function dialog.reply(what, txt)
	if dialog.__preped then dialog.__preped = false dialog.__answered = nil end

	if not txt then what, txt = strchar(dialog.tally + 1 + dialog.__a_minus), what end
	dialog.helper(txt, what)
end
function dialog.reply_cond(what, txt, cond)
	if dialog.__preped then dialog.__preped = false dialog.__answered = nil end

	if not what then what = strchar(dialog.tally + 1 + dialog.__a_minus) end
	if cond and type(cond) == "function" then cond = cond() end
	if cond then dialog.helper(txt, what) end
end

dialog.start = function(who, reset)
	dialog.__speak_who = who
	dialog.__term = term.save_to()
	dialog.prep()
	dialog.__preped = false
	dialog.__answered = nil
	if reset then term.gotoxy(0, 0) end
end

dialog.stop = function(conclude)
	if conclude then dialog.conclude() end
	if dialog.__term then term.load_from(dialog.__term, true) end
	dialog.__term = nil
end

dialog.__exec_module = function()
	if not dialog.__answered then
		dialog.__answered = dialog.answer()
	end
	return dialog.__answered
end

function dialog.on_quest_status(q, ...)
	local statuses = arg
	for i = 1, getn(statuses) do
		if quest(q).status == statuses[i] then return true end
	end
	return false
end


-- Compatibility mode
if is_compat then
	constant("dialogue", dialog)
	constant("dialogue_data", dialog_data)
end
