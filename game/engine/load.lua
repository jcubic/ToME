-- Savefile stuff
-- Do not meddle in the affairs of savefiles for they are subtle and quick to be become incompatible

declare_global_constants {
	"add_loadsave",
}
declare_globals {
	"__loadsave_max",
	"__loadsave_name",
	"__loadsave_tmp",
}

__loadsave_name = {}
__loadsave_max = 0
__loadsave_tmp = 0

function add_loadsave(name, default, preserve_from_birth)
	assert(name, "No variable name to save")
--	assert(default, "No default value")

	-- if it is a table we must create many entries
--[[	if type(default) == "table" then
		for k, e in default do
			add_loadsave(name.."."..k, e)
		end
	else ]]--
		__loadsave_name[__loadsave_max] = { name = name, default = default preserve_from_birth=preserve_from_birth }
		__loadsave_max = __loadsave_max + 1
	--end
end
