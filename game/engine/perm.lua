-- Take care fo preserving permanent informations
-- That is, information between savefiles

declare_globals
{
	"__perm_data",
	"set_perm_data",
	"get_perm_data",
	"load_perm_data",
	"save_perm_data",
}

__perm_data = {}

function set_perm_data(name, val)
	local v = get_module_info("version")
	__perm_data[game_module] = __perm_data[game_module] or {}
	__perm_data[game_module][v[1]] = __perm_data[game_module][v[1]] or {}
	__perm_data[game_module][v[1]][v[2]] = __perm_data[game_module][v[1]][v[2]] or {}


	__perm_data[game_module][v[1]][v[2]][name] = val
end

function get_perm_data(name)
	local v = get_module_info("version")
	__perm_data[game_module] = __perm_data[game_module] or {}
	__perm_data[game_module][v[1]] = __perm_data[game_module][v[1]] or {}
	__perm_data[game_module][v[1]][v[2]] = __perm_data[game_module][v[1]][v[2]] or {}

	return __perm_data[game_module][v[1]][v[2]][name]
end

function load_perm_data()
	tome_dofile_anywhere("/save", "perm.svg", FALSE)
end

function save_perm_data()
	local fff = fs.open("/save/perm.svg", "w")
	if fff then
		fs.write(fff, "__perm_data = unpickle"..pickle(__perm_data))
		fs.close(fff)
	end
end
