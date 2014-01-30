if (globals()["declare_globals"]) then
declare_globals {
        "shell",
}
end

function patch_init()
	shell.init()
	return "Lua Shell", shell.version
end

shell={}

function shell.init()
	patch_dofile.luashell("shell.lua")
	patch_dofile.luashell("util.lua")
	patch_dofile.luashell("tokenise.lua")
	patch_dofile.luashell("parse.lua")
	patch_dofile.luashell("grammar.lua")
end
