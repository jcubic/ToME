constant("module_packer", {})

-- This function is expected by T-Engine to exist no need to declare it
function patch_init()
	module_packer.init()
	return "Module Packer", "1.0.0"
end

function module_packer.init()
	patch_dofile.module_packer("packer.lua")
end
