-- Ingame contextual help

declare_global_constants {
	"ingame_clean",
	"ingame_help",
	"ingame_help_doc",
}
declare_globals {
	"__ingame_help_max",
	"__ingame_hooks",
}

-- We use our own hook list as to not overburn the hook proccessor
-- with many hooks that would slow down things
-- It would be very meaningless if the option is not even on
__ingame_hooks = {}

__ingame_help_max = 0

function ingame_help(t, ...)
	-- This function can also be used to call the callbacks
	if type(t) == "string" then
		local f = getglobal("__ingame_help_fct_"..t)
		call(f, arg)
		return
	end

	assert(t.desc or t.fct, "no ingame help desc/fct")
	assert(t.hook or t.callback, "no ingame help hook/callback")
	if t.hook then assert(t.event, "no ingame hepl event needed by hook") end

	-- Set it to only trigger once
	declare_globals{"__ingame_help_activated_"..__ingame_help_max}
	setglobal("__ingame_help_activated_"..__ingame_help_max, FALSE)
	-- Save/load it
	add_loadsave("__ingame_help_activated_"..__ingame_help_max, FALSE)

	if tag(t.desc) == TAG_TABLE then
		t.desc = strjoin(t.desc, " ")
		t.desc = gsub(t.desc, "  ", " ")
		t.desc = strsplit(t.desc, "\n")
	end

	if t.hook then
		-- If the hok list didnt exist yet, add it
		if not __ingame_hooks[t.hook] then
			-- Set it to empty, we'll fill it later
			__ingame_hooks[t.hook] = {}
			-- Add the global hook
			hook(t.hook,
				function (...)
					if not game_options.option_ingame_help then return end
					local k, e
					for k, e in __ingame_hooks[%t.hook] do
						if k ~= "n" then
							call(e, arg)
						end
					end
				end
			)
		end
		if t.desc then
			tinsert(__ingame_hooks[t.hook],
				function (...)
					local tbl = %t
					if getglobal("__ingame_help_activated_"..%__ingame_help_max) == FALSE then
						if call(tbl.event, arg) == true then
							local k, e
							for k, e in tbl.desc do
								if k ~= "n" then
									message(color.YELLOW, e)
								end
							end
							setglobal("__ingame_help_activated_"..%__ingame_help_max, true)
						end
					end
				end
			)
		elseif t.fct then
			tinsert(__ingame_hooks[t.hook],
				function (...)
					local tbl = %t
					if getglobal("__ingame_help_activated_"..%__ingame_help_max) == FALSE then
						if call(tbl.event, arg) == true then
							if tbl.fct() == true then
								setglobal("__ingame_help_activated_"..%__ingame_help_max, true)
							end
						end
					end
				end
			)
		end
	else
		local no_test = FALSE
		if t.no_test == true then no_test = true end
		if t.desc then
			declare_global_constants{"__ingame_help_fct_"..(t.callback)}
			setglobal
			(
				"__ingame_help_fct_"..(t.callback),
				function (...)
					local tbl = %t
					if ((game_options.option_ingame_help) or (%no_test == true)) and (getglobal("__ingame_help_activated_"..%__ingame_help_max) == FALSE) then
						local k, e
						for k, e in tbl.desc do
							if k ~= "n" then
								message(color.YELLOW, e)
							end
						end
						setglobal("__ingame_help_activated_"..%__ingame_help_max, true)
					end
				end
			)
		elseif t.fct then
			declare_global_constants{"__ingame_help_fct_"..(t.callback)}
			setglobal
			(
				"__ingame_help_fct_"..(t.callback),
				function (...)
					local tbl = %t
					if ((game_options.option_ingame_help) or (%no_test == true)) and (getglobal("__ingame_help_activated_"..%__ingame_help_max) == FALSE) then
						if call(tbl.fct, arg) == true then
							setglobal("__ingame_help_activated_"..%__ingame_help_max, true)
						end
					end
				end
			)
		end
	end

	__ingame_help_max = __ingame_help_max + 1
end

-- Clean up the ingame help seen at birth
hook(hook.BIRTH_OBJECTS,
	function()
		local i
		for i = 0, __ingame_help_max - 1 do
			setglobal("__ingame_help_activated_"..i, FALSE)
		end
	end
)

function ingame_clean()
	local i
	for i = 0, __ingame_help_max - 1 do
		setglobal("__ingame_help_activated_"..i, FALSE)
	end
end

-- helper function, brings up a doc
function ingame_help_doc(name, anchor)
	-- Save screen
	term.save()

	-- Peruse the help file
	if not anchor then anchor = 0 end
	show_file(name, 0, -anchor, 0)

	-- Load screen
	term.load()
end
