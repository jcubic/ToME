declare_globals {
        "flagsedit",
}

-- This function is expected by T-Engine to exist no need to declare it
function patch_init()
	flagsedit.init()
	return "Flags Editor", flagsedit.version
end

flagsedit = {}

function flagsedit.init()
	patch_dofile.flagseditor("flagsedit.lua")
end
