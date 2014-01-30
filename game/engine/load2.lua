-- Savefile helpers

declare_global_constants {
	"__assign_loaded_value",
	"__savefile_load",
	"__savefile_recurse",
	"__savefile_recurse_one",
	"__savefile_recurse_table",
	"__savefile_register",
	"__savefile_save",
	"dump_loadsave",
	"reconstruct_table",
	"reset_savefile_data",
}

function __assign_loaded_value(key, val)
	if val then
		if type(val) == "string" then
			dostring(format("%s = %q", key, val))
		else
			dostring(key.." = "..val)
		end
	else
		dostring(key.." = nil")
	end
end

-- function called when a key in the variable part ofthe savefile is read
-- if the key matches what we need, we use it, otehrwise just ignore it
function __savefile_load(key, val)
	local index, elem,found

	found = FALSE
	for index, elem in __loadsave_name do
		if (key == elem.name) then
			__assign_loaded_value(elem.name, val)
			found = true
		elseif (strfind(key,elem.name,1,1) == 1) then
			-- Initial segment is good enough if the next char is '.' or '['
			local next = strsub(key,strlen(elem.name)+1,strlen(elem.name)+1)
			if (next == '.' or next == '[') then
				reconstruct_table(key)
				__assign_loaded_value(key, val)
				found = true
			end
		end
	end
	if found == FALSE then message(color.VIOLET, "Unrecognised key "..key) end
end

function dump_loadsave()
	local k, e
	for k, e in __loadsave_name do
		if type(e.default) ~= "table" then
			message(k.." :: ".. e.name.." ["..e.default.."]")
		else
			message(k.." :: ".. e.name.." [<table>]")
		end
	end
end

--[[
	Generalised recursion for savefile data in __loadsave_name
	Recurse down through table structures eventually calling
	some function f at leaves. f is called with two parameters:
	The extended key name (i.e. with all the .foo.bar[j] tacked on)
	and the value at that leaf
]]--

function __savefile_recurse(f)
	local index, elem
	for index, elem in __loadsave_name do
		__savefile_recurse_one(elem.name,f)
	end
end

function __savefile_recurse_one(name,f)
	local __loadsave_tmp = dostring("return "..name)
	if type(__loadsave_tmp) == "table" then
		__savefile_recurse_table(name,__loadsave_tmp,f)
	else
		f(name, __loadsave_tmp)
	end
end

function __savefile_recurse_table(basename,table,f)
	local name
	for key,data in table do
		if key ~= ".get" and key ~= ".set" then
			if type(key) == "number" then
				name = basename.."["..key.."]"
			else
				name = basename.."."..key
			end
			__savefile_recurse_one(name,f)
		elseif key == ".get" then
			__savefile_recurse_table(basename, data, f)
		end
	end
end

-- called when the game is saved, can only save numbers
-- assosiate a key with them to allow the loading code to recognize them
-- uses recurse, at leaves it calls save_number_key (which is C code)
-- to save a single number associated with a (string) key.

declare_global_constants"__savefile_types"
__savefile_types =
{
	["nil"] = function(name, val) save_nil_key(name) end,
	["number"] = save_number_key,
	["string"] = save_string_key,
}

function __savefile_save()
	local save_fct = function(name, val)
		if __savefile_types[type(val)] then
			__savefile_types[type(val)](name, val)
		else
			error("Unsavable type "..type(val).." in "..name)
		end
	end
	__savefile_recurse(save_fct)
end

-- Called just before the date is actually is saved, to count
-- how many key-value pairs there will be

function __savefile_register()
	-- This says, recurse over the tree of tables to save,
	-- counting each leaf (which must be a number) as 1
	__savefile_recurse(function () register_savefile(1) end)
end

hook.add_script(hook.SAVE_REGISTER, "__savefile_register", "__hook_register")
hook.add_script(hook.LOAD_GAME, "__savefile_load", "__hook_load")
hook.add_script(hook.SAVE_GAME, "__savefile_save", "__hook_save")

-- Parse a flattened(i.e: foo.bar.zog) table path and recrate tables
function reconstruct_table(name)
	for i = 1, strlen(name) - 1 do
		if strsub(name, i, i) == "." or strsub(name, i, i) == "[" then
			local tbl = strsub(name, 1, i - 1)

			if dostring("return "..tbl) == nil then
				dostring(tbl.."={}")
			end
		end
	end
end

-- Automagically set unkown variables, otherwise the savefile code
-- might get VERY upset
function reset_savefile_data(no_force_reset)
	local k, e
	-- We need to be able to check for unknown globals
	unset_safe_globals()
	for k, e in __loadsave_name do
		reconstruct_table(e.name)
		if (not no_force_reset and not e.preserve_from_birth) or dostring("return "..(e.name)) == nil then
			if e.default then
				if type(e.default) == "string" then
					dostring(format("%s = %q", e.name, e.default))
				else
					globals()["____set_data"] = e.default
					dostring(e.name.." = globals()['____set_data']")
					globals()["____set_data"] = nil
				end
			else
				dostring((e.name).." = nil")
			end
		end
	end
	-- Now taht we did, we set it back, for it is usefull ;)
	set_safe_globals()
end
-- Just reconstruct the unset ones
reset_savefile_data(true)
