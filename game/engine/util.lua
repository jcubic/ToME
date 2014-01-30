-- various stuff to make scripters life easier

declare_global_constants {
	"approximate_distance",
	"bool_flag",
	"compass",
	"get_flag",
	"get_flag2",
	"get_func_from_flag",
	"has_flag",
	"inc_flag",
	"flag_inc",
	"known_flag",
	"flags_set",
	"flags_set_aux",
	"msg_format",
	"new_flag",
	"new_speed",
	"set_artifact_generation",
	"set_flag",
	"set_flags",
	"set_monster_generation",
	"set_object_generation",
	"stack_pop",
	"stack_push",
	"strcap",
	"bound",
	"choose_from_list",
	"choose_print_list",
	"book_capitals",
	"foreach_flags",
	"fold",
	"wiz_print",
	"convert_desc",
	"max",
	"min",
	"abs",
	"default_value",
	"new_key_action",
	"merge_tables",
	"unpack",
	"pack",
	"average_value",
	"iif",
	"table_copy",
	"cave_plain_floor",
	"define_as",
	"strjoin",
	"strsplit",
	"signed_number_tostring",
	"safe_new_flag",
	"clean_whitespace",
	"ends_with",
	"starts_with",
	"get_string",
	"generic_or_specific",
	"level_or_feat",
	"check_prevent_cmd",
	"player_can_see_bold",
	"who_to_faction",
	"notefind",
	"KTRL",
	"function_to_index",
	"index_to_function",
	"object_pickup_simple",
	"div_round",
	"display_colored_file",
	"switch",
	"permutate",
	"is_separate_word",
	"show_arr_as_file",
	"show_arr_as_list",
	"singularize",
	"pluralize",
	"add_special_plural",
	"add_regexp_plural",
	"display_list_simple",
	"line_breaks",
	"foreach_monster",
	"choose_from_list_simple",
	"or_msg",
	"randomize_list",
	"choose_multi_column",
	"monster_strsub",
}
declare_globals {
	"__hooks_list_callback",
	"__hooks_list_callback_max",
	"__lua_hook_heads",
	"__lua_hook_type_max",
	"__timers_callback_max",
	"__function_indexing",
	"__special_plurals",
	"__regexp_plurals",
}

-- Infinity: 2^31 - 2.  "-2" so that both "infinity" and "-infinity" will
-- fit into a 32 bit singed integer
constant("infinity", 2147483646)

-- Automatically defines a variable as an index
function define_as(t, field)
	local def_as = t.define_as
	local val = t.__index__
	if field then
		def_as = t["define_"..field.."_as"]
		val = t[field]
	end

	if not t.load_script then t.load_script = "" end
	-- If not defined and not a table indexing, create the global variable
	if not globals()[def_as] and not strfind(def_as, '.', 1, 1) and not strfind(def_as, '[', 1, 1) then
		t.load_script = t.load_script..'globals()["'..def_as..'"] = '..val.."\n"
	else
		t.load_script = t.load_script..def_as.." = "..val.."\n"
	end
end

-- Better hook interface
__hooks_list_callback = {}
__hooks_list_callback_max = 0

__lua_hook_type_max = hook.HARDCODED_MAX + 1
__lua_hook_heads    = {}

-- Call as either:
-- * hook(hook.FOO, function() ... end)
-- * hook{ [hook.FOO] = function() ... end, [hook.FOO] = function() ... end, ...)
function hook.__exec_module(a, b, c)
	local k, e

	-- Compute parameters
	local h_table, name_prefix
	if (type(a) == "table") then
		h_table = a
		name_prefix = b
	else
		h_table = { [a] = b }
		name_prefix = c
	end

	if not name_prefix then name_prefix = "" end
	for k, e in h_table do
		local hookname = "__"..name_prefix.."__hooks_list_callback"..__hooks_list_callback_max
		declare_global_constants{hookname}
		hook.lua_add_script(k, hookname, hookname)
		setglobal(hookname, e)
		__hooks_list_callback_max = __hooks_list_callback_max + 1
	end
end

function hook.lua_add_script(type, script, name)
	if type > hook.HARDCODED_MAX then
		local new_head = {}
		new_head.type = hook.HOOK_TYPE_LUA
		new_head.script = script
		new_head.name = name
		new_head.next = __lua_hook_heads[type]

		__lua_hook_heads[type] = new_head
	else
		hook.add_script(type, script, name)
	end
end

--- @fn
-- @brief Add a new hook type to the hook.HOOK_NAME list
-- @param name String the hook type's name
function hook.new_hook_type(name)
	hook[name] = __lua_hook_type_max

	__lua_hook_type_max = __lua_hook_type_max + 1
end

--- @fn
-- @brief Process a series of hooks(beware, ONLY lua hooks will be processed)
-- @param type Number the hook to process
-- @param ... Args the arguments to pass to the hooks
function hook.process(type, ...)
	-- For all lua hooks
	local c

	if type > hook.HARDCODED_MAX then
		c = __lua_hook_heads[type]
	else
		c = hook.__get(type)
	end

	while c do
		if c.type == hook.HOOK_TYPE_C then
			error("Hook type "..type.. " with name "..c.name.." has a C callback, but is called by a lua process_hooks!")
		elseif c.type == hook.HOOK_TYPE_LUA then
			-- Get the function and call it
			local rets = pack(call(getglobal(c.script), arg))

			-- Done ?
			if rets[1] then return unpack(rets) end

			-- Restart is needed(because a hook was deleted) ?
			if hook.restart then
				c = hook.__get(type)
				hook.restart = nil
			else
				-- Ok try the next one
				c = c.next
			end
		else
			error("Unknown hook type "..c.type..", name "..c.name)
		end
	end
	return nil
end

-- Returns the direction of the compass that y2, x2 is from y, x
-- the return value will be one of the following: north, south,
-- east, west, north-east, south-east, south-west, north-west,
-- or nil if it is within 2 tiles.
function compass(y, x, y2, x2)
	local y_axis, x_axis, y_diff, x_diff, compass_dir

	-- is it close to the north/south meridian?
	y_diff = y2 - y

	-- determine if y2, x2 is to the north or south of y, x
	if (y_diff > -3) and (y_diff < 3) then
		y_axis = nil
	elseif y2 > y then
		y_axis = "south"
	else
		y_axis = "north"
	end

	-- is it close to the east/west meridian?
	x_diff = x2 - x

	-- determine if y2, x2 is to the east or west of y, x
	if (x_diff > -3) and (x_diff < 3) then
		x_axis = nil
	elseif x2 > x then
		x_axis = "east"
	else
		x_axis = "west"
	end


	-- Maybe it is (almost) due N/S
	if (not x_axis) and (not y_axis) then compass_dir = nil

	elseif not x_axis then compass_dir = y_axis

	-- Maybe it is (almost) due E/W
	elseif not y_axis then compass_dir = x_axis

	-- or if it is neither
	else compass_dir = y_axis.."-"..x_axis
	end

	return compass_dir
end

-- Returns an approximation of the 'distance' of y2, x2 from y, x.
function approximate_distance(y, x, y2, x2)
	local y_diff, x_diff, most_dist, how_far

	-- how far to away to the north/south
	y_diff = y2 - y

	-- make sure it's a positive integer
	if y_diff < 0 then
		y_diff = 0 - y_diff
	end

	-- how far to away to the east/west
	x_diff = x2 - x

	-- make sure it's a positive integer
	if x_diff < 0 then
		x_diff = 0 - x_diff
	end

	-- find which one is the larger distance
	if x_diff > y_diff then
		most_dist = x_diff
	else
		most_dist = y_diff
	end

	-- how far away then?
	if most_dist >= 41 then
		how_far = "a long way"
	elseif most_dist >= 11 then
		how_far = "quite some way"
	else
		how_far = "not very far"
	end

	return how_far

end

-- better timer add function
__timers_callback_max = 0

function time.new_timer(t,a,b)
	assert(t.delay > 0, "no timer delay")
	assert(t.callback, "no timer callback")

	local timer
	if type(t.callback) == "function" then
		declare_global_constants("__timers_callback_"..__timers_callback_max)
		setglobal("__timers_callback_"..__timers_callback_max, t.callback)
		timer = time.__new_timer("__timers_callback_"..__timers_callback_max, t.delay)
		__timers_callback_max = __timers_callback_max + 1
	else
		timer = time.__new_timer(t.callback, t.delay)
	end

	timer.enabled = t.enabled

	return timer
end

function time.save_timer(name)
	add_loadsave(name..".enabled", nil)
	add_loadsave(name..".delay", 1)
	add_loadsave(name..".countdown", 1)
end

-- People can store their timers here
time.timer = {}

------------------ RT timer ---------------
time.rt = {}
time.rt.__timers = {}
time.rt.__timer_ids = {}

function time.rt.exec(interval)
	if not time.rt.__timers[interval] then
		error("RT timer called with interval "..interval.." but no functions defined for this interval")
		return
	end
	for i = 1, getn(time.rt.__timers[interval]) do time.rt.__timers[interval][i](interval) end
end

function time.rt.add(interval, func)
	if not time.rt.__timers[interval] then
		time.rt.__timers[interval] = {}
		time.rt.__timer_ids[interval] = time.__rt_add(interval)
	end
	tinsert(time.rt.__timers[interval], func)
end

function time.rt.del(interval, func)
	if not time.rt.__timers[interval] then return nil end

	for i = 1, getn(time.rt.__timers[interval]) do
		if time.rt.__timers[interval][i] == func then
			tremove(time.rt.__timers[interval], i)
			break
		end
	end

	if getn(time.rt.__timers[interval]) == 0 then
		time.__rt_del(time.rt.__timer_ids[interval])
		time.rt.__timers[interval] = nil
		time.rt.__timer_ids[interval] = nil
	end
end

-- Easier access to special gene stuff
function set_monster_generation(monster, state)
	if type(monster) == "string" then
		m_allow_special[test_monster_name(monster) + 1] = state
	else
		m_allow_special[monster + 1] = state
	end
end
function set_object_generation(obj, state)
	if type(obj) == "string" then
		m_allow_special[test_item_name(obj) + 1] = state
	else
		m_allow_special[obj + 1] = state
	end
end
function set_artifact_generation(obj, state)
	m_allow_special[obj + 1] = state
end


-- Flags

-- Convenience function for multiple flag-setting at once
function flags_set(flags, ...)
	flags_set_aux(flags,arg)
end

function flags_set_aux(flags,arg)
	for i = 1, getn(arg) do
		local flag, val

		if type(arg[i]) == "table" then
			flag = arg[i][1]
			val = arg[i][2]
		else
			flag = arg[i]
			val = 1
		end

		flag_set(flags, flag, val)
	end
end

-- Wrappers around above functions, assuming that the flags are in a member
-- called 'flags' (a common convention)

function has_flag(obj, flag)
	return flag_exists(obj.flags, flag)
end
function get_flag(obj, flag)
	return flag_get(obj.flags, flag)
end
function get_flag2(obj, flag)
	return flag_get2(obj.flags, flag)
end
function set_flag(obj, flag, v)
	flag_set(obj.flags, flag, v)
end
function set_flags(obj, ...)
	flags_set_aux(obj.flags, arg)
end
function bool_flag(obj, flag)
	flag_set(obj.flags, flag, true)
end
function inc_flag(obj, flag, v)
	flag_set(obj.flags, flag, v + flag_get(obj.flags, flag))
end
function known_flag(obj, flag)
	return flag_exists(obj.flags, flag) and flag_is_known(obj.flags, flag)
end

constant("__shared_flag", {})
function new_flag(name, shared)
	local f = register_flag(name)
	declare_global_constants{"FLAG_"..name}
	setglobal("FLAG_"..name, f)

	if shared then
		__shared_flag[f] = true
	end

	return f
end

function flag_inc(obj, flag, v)
	local known = flag_is_known(obj, flag)
	flag_set(obj, flag, v + flag_get(obj, flag))
	flag_learn(obj, flag, known)
end


function new_speed(name, desc)
	local f = register_speed(name, desc)
	declare_global_constants{"SPEED_"..name}
	setglobal("SPEED_"..name, f)
	return f
end

-- Init the object tag
do
	local a = new_object()
	local b = data_buffer_alloc()
	local c = monster_blow_new()
	local d = monster_type_new()
	local e = store_type_new()
	constant("TAG_MONSTER", tag(d))
	constant("TAG_OBJECT", tag(a))
	constant("TAG_STRING", tag(""))
	constant("TAG_TABLE", tag({}))
	constant("TAG_STORE", tag(e))
	constant("TAG_BUFFER", tag(b))
	constant("TAG_MONST_BLOW", tag(c))
	constant("TAG_TO_FLAG_TYPE",
	{
		[TAG_MONSTER] = { FLAG_TYPE_MONSTER, }
		[TAG_OBJECT] = { FLAG_TYPE_OBJ, }
		[TAG_STRING] = { FLAG_TYPE_STRING, "dup_string_to_void" }
		[TAG_STORE] = { FLAG_TYPE_STORE, }
		[TAG_BUFFER] = { FLAG_TYPE_DATA_BUFFER, "dup_buffer_to_void"}
	})
	constant("FLAG_TYPE_TO_PTR",
	{
		[FLAG_TYPE_MONSTER] = "to_monster_type"
		[FLAG_TYPE_OBJ] = "to_object"
		[FLAG_TYPE_STRING] = "to_string"
		[FLAG_TYPE_STORE] = "to_store_type"
		[FLAG_TYPE_DATA_BUFFER] = "to_buffer"
	})
	delete_object(a)
	data_buffer_delete(b)
	monster_blow_del(c)
	monster_type_free_memory(d)
	store_type_del(e)
end

-- Sets easy of use tag methods for flags
-- get flag: flags[FLAG_FOO]
-- set flag: flags[FLAG_FOO] = x
-- set many flags: flags{ FOO=x BAR=y BAZ=z ... }
-- del flag: flags[FLAG_FOO] = nil
constant("TAG_FUNCTION", tag(function() end))
constant("TAG_NUMBER", tag(0))
constant("TAG_FLAG", tag(player.descriptors))
global("old_flags_type_tags", {})
old_flags_type_tags.gettable = settagmethod(tag(player.flags), "gettable",
	function(flags, f)
		if tag(f) == TAG_NUMBER then
			if flag_exists(flags, f) then
				local node = flag_get_node(flags, f)
				if (node.flags & FLAG_FLAG_FLAGS) ~= 0 then
					return flag_get_flags(flags, f)
				elseif (node.flags & FLAG_FLAG_PTR) ~= 0 then
					local typ = flag_node_get_ptr_type(node)
					return __wrap_ptr[FLAG_TYPE_TO_PTR[typ]](flag_get_ptr(flags, f, typ))
				else
					return flag_get(flags, f)
				end
			else
				return nil
			end
		else
			return old_flags_type_tags.gettable(flags, f)
		end
	end
)
old_flags_type_tags.settable = settagmethod(tag(player.flags), "settable",
	function(flags, f, v)
		if tag(f) == TAG_NUMBER then
			if v == nil then
				flag_remove(flags, f)
			else
				if tag(v) == TAG_NUMBER then
					flag_set(flags, f, v)
				elseif tag(v) == TAG_TABLE then
					flag_set_full(flags, f, FLAG_FLAG_BOTH, v[1], v[2])
				elseif tag(v) == TAG_FLAG then
					flag_set_full(flags, f, FLAG_FLAG_FLAGS, 0, 0, v)
				elseif tag(v) == TAG_FUNCTION then
					local ret = get_flag_value["function"](v, {__index__=0 __type__=-1000})
					flag_set_full(flags, f, FLAG_FLAG_BOTH, ret[1], ret[2])
				else
					local typ = TAG_TO_FLAG_TYPE[tag(v)]
					if typ[2] then v = __wrap_ptr[typ[2]](v) end
					flag_set_ptr(flags, f, typ[1], v)
				end
			end
		else
			old_flags_type_tags.settable(flags, f, v)
		end
	end
)
old_flags_type_tags.index = settagmethod(tag(player.flags), "index",
	function(flags, f, v)
		if tag(f) == TAG_NUMBER then
			if v == nil then
				flag_remove(flags, f)
			else
				if tag(v) == TAG_NUMBER then
					flag_set(flags, f, v)
				elseif tag(v) == TAG_FLAG then
					flag_set_full(flags, f, FLAG_FLAG_FLAGS, 0, 0, v)
				else
					local typ = TAG_TO_FLAG_TYPE[tag(v)]
					if typ[2] then v = __wrap_ptr[typ[2]](v) end
					flag_set_ptr(flags, f, typ[1], v)
				end
			end
		else
			old_flags_type_tags.index(flags, f, v)
		end
	end
)
old_flags_type_tags.fct = settagmethod(tag(player.flags), "function",
	function(...)
		if type(arg[2]) == "table" then
			grab_flags(arg[2], arg[1], nil)
		elseif tag(arg[2]) == TAG_NUMBER then
			return arg[1][arg[2]] or 0
		else
			return call(old_flags_type_tags.fct, arg)
		end
	end
)

-- Make module(not game modules, code modules, we'll call them namespaces) also act as functions
constant("TAG_NAMESPACE", tag(fs))
settagmethod(TAG_NAMESPACE, "function",
	function(module, ...)
		assert(module.__exec_module, "This namespace is not callable!")
		return call(module.__exec_module, arg)
	end
)

-- Assign the various namespaces self function
rng.__exec_module = function(a, b)
	if b then
		return rng.range(a, b)
	else
		return rng.range(1, a)
	end
end

rng.arange = function(a, b)
	if a < b then
		return rng.range(a, b)
	else
		return rng.range(b, a)
	end
end

-- Switch the simple rng on/off
rng.simple = function(seed)
	-- Switch on
	if seed then
		rng.Rand_quick = true
		rng.Rand_value = seed
	-- Switch off
	else
		rng.Rand_quick = nil
	end
end

-- Is the simple rng on ?
rng.is_simple = function(seed)
	return rng.Rand_quick
end

-- Describe a roll
rng.desc_roll = function(x, y)
	return x.."to"..(x*y)
end

-- Describe a roll
rng.desc_range = function(x, y)
	return x.."to"..y
end

message.__exec_module = function(color, msg)
	if msg then message.cmsg_print(color, msg)
	else message.msg_print(color)
	end
end

term.print = function(color, msg, y, x)
	if x then
		term.c_put_str(color, msg, y, x)
	else
		term.put_str(color, msg, y)
	end
end
term.blank_print = function(color, msg, y, x)
	if x then
		term.c_prt(color, msg, y, x)
	else
		term.prt(color, msg, y)
	end
end
term.text_out = function(c, msg)
	if msg then
		term.text_out_c(c, msg)
	else
		term.text_out_c(color.WHITE, c)
	end
end

term.display = function(c, msg, y ,x)
	if not x then
		c, msg, y, x = color.WHITE, c, msg, y
	end
	message.display(x, y, strlen(msg), c, msg)
end

function term.drop_text_left(c, str, y, o)
	local i = strlen(str)
	local x = 39 - (strlen(str) / 2) + o
	while (i > 0)
	do
		local a = 0
		local time = 0

		if (strbyte(str, i) ~= strbyte(" ", 1)) then
			while (a < x + i - 1)
			do
				term.putch(a - 1, y, c, 32)
				term.putch(a, y, c, strbyte(str, i))
				time = time + 1
				if time >= 4 then
					term.xtra(TERM_XTRA_DELAY, 1)
					time = 0
				end
				term.redraw_section(a - 1, y, a, y)
				a = a + 1

				term.inkey_scan = true
				if (term.inkey() ~= 0) then
					return true
				end
			end
		end

		i = i - 1
	end
	return false
end

function term.drop_text_right(c, str, y, o)
	local x = 39 - (strlen(str) / 2) + o
	local i = 1
	while (i <= strlen(str))
	do
		local a = 79
		local time = 0

		if (strbyte(str, i) ~= strbyte(" ", 1)) then
			while (a >= x + i - 1)
			do
				term.putch(a + 1, y, c, 32)
				term.putch(a, y, c, strbyte(str, i))
				time = time + 1
				if time >= 4 then
					term.xtra(TERM_XTRA_DELAY, 1)
					time = 0
				end
				term.redraw_section(a, y, a + 1, y)
				a = a - 1

				term.inkey_scan = true
				if (term.inkey() ~= 0) then
					return true
				end
			end
		end

		i = i + 1
	end
	return false
end

function term.get(y, x)
	local ret, a, c = term.Term_what(x, y)
	if ret ~= 0 then return nil end
	return a, c
end

-- Strings
function strcap(str)
	if strlen(str) > 1 then
		return strupper(strsub(str, 1, 1))..strsub(str, 2)
	elseif strlen(str) == 1 then
		return strupper(str)
	else
		return str
	end
end

function msg_format(...)
	message(call(format, arg))
end

-- Stacks
function stack_push(stack, val)
	tinsert(stack, val)
end
function stack_pop(stack)
	if getn(stack) >= 1 then
		return tremove(stack)
	else
		error("Tried to unstack an empty stack")
		return nil
	end
end

-- Bounds a value
function bound(val, min, max)
	if val < min then return min
	elseif max and val > max then return max
	else return val
	end
end

-- choose_from_list
--
-- Reasonably generic chooser in LUA for ToME
--
-- It tries as far as possible to imitate the look of the built in
-- spell choosers, and should be usable for a wide variety of special
-- powers that module authors might devise
--
-- parameters:
--
-- array: The array of 'things' to choose from.  They can be whatever
--        you like: numbers, strings, tables, etc, as long as they are
--        in an array
-- what:  What to call them for the prompt.  E.g. "Spells"
-- prompt:The last bit of the prompt, e.g. "Cast which spell?"
-- shortprinter:
--        Shortprinter is the function called to print each line of the
--				list. It is called with three parameters : y,x,thing. It should
--        use prt or c_prt to print information at y,x. The thing is
--        some element of the array you gave to choose_from_list.
--				Shortprinter should also expect to be called with 'thing' set to
--				nil, in which case it should print a 'column heading' line.
-- longprinter:
--        Longprinter is the function called to print the long description,
--        requested by the user when they press a capital letter key. It is
--        called with two parameters, a y position and the thing to describe.
--        Consider using text_out or text_out_c because they wraps
--        lines for you.
--
-- Bugs:
--
-- Pressing a capital letter to request a long description will try to print
-- a list anyway.  Make sure that shortprinter is defined if longprinter is.
--
-- ToDo:
--  '@' style extended selection

function choose_from_list(array,what,prompt,shortprinter,longprinter)
	local max = getn(array)
	local redraw = game_options.easy_inven
	local ret = {nil,nil}
	local dolist,promptlist = false,""
	local dodesc,promptdesc = false,""

	-- We can only display a list if the caller provided a
	-- shortprinter function
	if shortprinter then
		dolist = true
		promptlist = ", *=List"
	end

	-- We can only display long descriptions if the caller provided a
	-- longprinter function
	if longprinter then
		dodesc = true
		promptdesc = format(", Descs A-%c",strbyte("A")-1+max)
	end

	term.blank_print(format("(%s a-%c%s%s, ESC=exit) %s",what,strbyte("a")-1+max,promptdesc,promptlist,prompt), 0, 0)

	term.save()

	if redraw then choose_print_list(array,shortprinter) end

	while not nil do
		local c = term.inkey()

		if (c == ESCAPE) then
			ret = {nil,nil}
			break
		end

		if c == strbyte('@') then
			-- '@' style extended selection
			if getn(array) == 0 then
				ret = {nil,nil}
				break
			end

			local typ = type(array[1])
			if not (typ == "string" or typ == "table") then
				-- Only do strings and tables as of now
				ret = {nil,nil}
				break
			end

			local str = get_string(prompt .. " [by name] ") or ""
			str = clean_whitespace(str)

			if typ == "string" and str == "" then
				ret = {nil,nil}
				break
			end

			ret = nil
			for i = 1, getn(array) do
				if typ == "string" then
					if strlower(str) == strlower(array[i]) then
						ret = {array[i], i}
						break
					end
				else
					if array[i][str] or (array[i].name and
										 strlower(array[i].name) ==
											 strlower(str))
					then
						ret = {array[i], i}
						break
					end
				end
			end -- for i = 1, getn(array) do
			if ret then
				break
			end

			-- No match found
			ret = {nil,nil,str}
			break
		end -- '@' style extended selection

		if (dolist == true and (c == strbyte('*') or c == strbyte(' ') or c == strbyte('?'))) then
			if (redraw == true) then -- list already displayed, remove it
				term.load()
				term.save()
				redraw = false
			else
				choose_print_list(array,shortprinter)
				redraw = true
			end
		end

		if (c - strbyte('a') >= 0) and (c - strbyte('a') < max) then
			ret = { array[(c-strbyte('a')+1)], c-strbyte('a')+1 }
			break
		end

		if (dodesc == true and (c - strbyte('A') >= 0) and (c - strbyte('A') < max)) then
			if (redraw == true) then
				term.load()
				term.save()
			end

			local index,line,y
			y = choose_print_list(array,shortprinter)
			term.blank_print("",y,0)
			longprinter(y,array[1+c-strbyte('A')])

			redraw = true
		end
	end

	term.load()
	term.blank_print("",0,0)
	return unpack(ret)
end

function choose_print_list(array,shortprinter)
	local col
	local y=1

	term.blank_print("",y,0)
	shortprinter(y,3,nil) -- heading
	y = y + 1

	for i = 1,getn(array) do
		term.blank_print(color.LIGHT_BLUE,format("%c) ",strbyte("a")+i-1),y,0)
		shortprinter(y,3,array[i])
		y = y + 1
	end

	return y
end

function book_capitals(s)
	local words = strsplit(s, " ")

	for i = 1, getn(words) do
		local word = words[i]

		-- Don't capitalize certain words unless they are at the begining
		-- of the string.
		if i == 1 or (word ~= "of" and word ~= "the" and word ~= "and" and
					  word ~= "a" and word ~= "an")
		then
			words[i] = gsub(word, "^(.)",
							function(x)
								return strupper(x)
							end)
		end
	end

	return strjoin(words, " ")
end

-- Apply a function to each flags
function foreach_flags(flags, fct)
	for i = 1, flags.size do
		if (flags.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			fct(flags, flags.node[i].key)
		end
	end
end

-- Fold together an array using a start value and a binary function
-- f.  (E.g., take a sum)

function fold(array,start,f)
	local acc = start
	for i = 1,getn(array) do
		acc = f(acc,array[i])
	end
	return acc
end

-- Print only in wiz mode
function wiz_print(s)
	if (wizard == true) then message(s) end
end

-- Convert a desc array to a desc string with \n
function convert_desc(tdesc, sep)
	if not sep then sep = " " end
	if type(tdesc) == "string" then return tdesc end
	local desc = ""
	for i = 1, getn(tdesc) do
		desc = desc..tdesc[i]
		if i < getn(tdesc) then desc = desc..sep end
	end
	return desc
end

function max(a, b)
	if a > b then return a else return b end
end
function min(a, b)
	if a < b then return a else return b end
end
function abs(v)
	if v < 0 then return -v else return v end
end

function default_value(table, index, v)
	if not table[index] then table[index] = v end
end

-- Physfs
fs.add_to_search_path_real = fs.add_to_search_path
fs.add_to_search_path = function(file, append, mount)
	if mount then fs.PHYSFS_specific_mount = mount end
	local ret = fs.add_to_search_path_real(file, append)
	fs.PHYSFS_specific_mount = nil
	return ret
end

-- Add keypresses
global("__new_key_action_next", -8000)
function new_key_action(t)
	assert(t.action, "no key action")
	if type(t.key) == "string" then t.key = strbyte(t.key) end

	if t.extended then
		assert(t.desc, "No extended key description")
		if not t.key then
			t.key = __new_key_action_next
			__new_key_action_next = __new_key_action_next + 1
		end
		cli_add(tostring(t.key), t.extended, t.desc)
	end

	-- Add it
	hook
	{
	        [hook.KEYPRESS] = function (key)
        	                if key == %t.key then
                               		return %t.action()
                	        end
        	end,
	}
end

-- Merge 2 tables
function merge_tables(t1, t2)
	for i = 1, getn(t2) do
		tinsert(t1, t2[i])
	end

	return t1
end

function pack(...)
	return arg
end

function unpack(table, idx)
	if getn(table) == 0 then return
	elseif getn(table) == 1 then return table[1]
	else
		if not idx then
			return table[1], unpack(table, 2)
		else
			if idx < getn(table) then
				return table[idx], unpack(table, idx + 1)
			else
				return table[idx]
			end
		end
	end
end

-- Display text with embedded color info
function term.color_text(color, t, y, x)
	message.display(x, y, strlen(t), color, t)
end

-- Compute average vlue
function average_value(...)
	local avg, nb = 0, 0
	if nb == getn(arg) then error("Average of 0 values!") end
	for i = 1, getn(arg) do
		if type(arg[i]) == "table" then
			avg = avg + (arg[i][1] * arg[i][2])
			nb = nb + arg[i][2]
		else
			avg = avg + arg[i]
			nb = nb + 1
		end
	end
	return avg / nb
end

-- A way to  check if the game is now running(as opposed to initialization/character gen)
global("game", {})
settag(game, TAG_NAMESPACE) -- Tag game as a namespace
hook(hook.GAME_START, function() game.started = true end)


-- A flagset iterator function
constant("flag_iterate", function(flags, f, ...)
	for i = 1, flags.size do
		if (flags.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			f(flags, flags.node[i].key, unpack(arg))
		end
	end
end)

function iif(a,b,c)
	if a then return b else return c end
end

-- Copy a table recursively(copies only lua native elements, wont work for userdata)
function table_copy(t1, t2)
	local dest, src = nil, nil

	if not t2 then
		-- Return a copy of t1
		dest = {}
		src  = t1
	else
		-- Copy everything from t2 into t1
		dest = t1
		src  = t2
	end

	-- Handle metatag
	if tag(dest) == TAG_TABLE and tag(src) ~= TAG_TABLE then
		settag(dest, tag(src))
	elseif tag(dest) == tag(src) then
	else
		error("table_copy cannot handle different tags")
	end

	for k, e in src do
		if tag(e) == TAG_FLAG then
			if not dest[k] then
				dest[k] = flag_new()
			end
			flag_copy(dest[k], e)
		elseif type(e) == "table" then
			if dest[k] then
				table_copy(dest[k], e)
			else
				dest[k] = table_copy(e)
			end
		else
			dest[k] = e
		end
	end
	return dest
end

-- Join an array of strings into one, separated by the given
-- character(s)
function strjoin(strs, char, last)
	local out = ""
	for i = 1, getn(strs) do
		out = out .. tostring(strs[i])

		if last and i == (getn(strs) - 1) then
			out = out .. last
		elseif i < getn(strs) then
			out = out .. char
		end
	end

	return out
end

-- Split a string by the given character(s)
function strsplit(str, char)
	local ret   = {}
	local len   = strlen(str)
	local start = 1

	while true do
		local split_start, split_end = strfind(str, char, start)

		if not split_start then
			tinsert(ret, strsub(str, start))
			break
		end

		tinsert(ret, strsub(str, start, split_start - 1))

		if split_end == len then
			break
		end

		start = split_end + 1
	end

	return ret
end

-- Is the spot very much plain ?
function cave_plain_floor(c, y)
	if y then c = cave(c , y) end
	return has_flag(f_info[1 + c.feat], FLAG_FLOOR) and not has_flag(f_info[1 + c.feat], FLAG_REMEMBER)
end

-- Converts a number into a string with the sign
-- I.e: 3 => +3 ; -3 => -3
function signed_number_tostring(v)
	if v >= 0 then return "+"..v
	else return tostring(v) end
end

-- Create a new flag only if it doesn't exist
function safe_new_flag(s)
	if not globals()["FLAG_"..s] then new_flag(s) end
end

-- Strip multiple spaces, leading whitespace and trailing whitespace.
function clean_whitespace(str)
	if strlen(str) == 0 then
		return str
	end

	str = gsub(str, "\t", " ")

	-- Split string on space, then put the string back together,
	-- ignoring empty elements (which were multiple spaces in a row)
	local parts  = strsplit(str, " ")
	local outstr = ""

	for i = 1, getn(parts) do
		if strlen(parts[i]) > 0 then
			if strlen(outstr) > 0 then
				outstr = outstr .. " "
			end
			outstr = outstr .. parts[i]
		end
	end

	return outstr
end

-- Tests if str ends with 'ending'
function ends_with(str, ending)
	local len     = strlen(str)
	local end_len = strlen(ending)

	if len < end_len then
		return nil
	end

	if strfind(str, ending, len - end_len + 1, true) then
		return true
	end

	return nil
end

-- Tests if str starts with 'start'
function starts_with(str, start)
	local len     = strlen(str)
	local start_len = strlen(start)

	if len < start_len then
		return nil
	end

	if strfind(str, start, 1, true) == 1 then
		return true
	end

	return nil
end

-- Gives prompt to user and gets back a string
function get_string(prompt, default)
	if not prompt then
		prompt = ""
	end

	-- Clear message area
	message.msg_print()

	-- Display prompt
	term.prt(prompt, 0, 0)

	-- Get result
	local str = term.askfor_aux(default)

	-- Clear prompt
	term.prt("", 0, 0)

	return str
end

--
-- If the object is not known/identified, then return the object kind
-- structure for the object, to tell the user generic info about
-- that type of object
--
constant("__generic_obj", new_object())
function generic_or_specific(obj)
	local kind

	if tag(obj) == TAG_OBJECT then
		if is_known(obj) then
			return obj
		end
		kind = k_info(obj.k_idx)
	else
		kind = obj
		obj  = {k_idx = lookup_kind(kind.tval, kind.sval)}
	end

	-- Set up a generic version of the object
	__generic_obj.k_idx  = obj.k_idx
	__generic_obj.d_attr = obj.d_attr or kind.d_attr
	__generic_obj.x_attr = obj.x_attr or kind.x_attr
	__generic_obj.number = obj.number or 1
	__generic_obj.marked = obj.marked or 0
	__generic_obj.weight = obj.weight or kind.weight
	__generic_obj.note   = obj.note   or 0
	__generic_obj.tval   = kind.tval
	__generic_obj.sval   = kind.sval

	flag_empty(__generic_obj.flags)
	flag_copy(__generic_obj.flags, kind.flags)

	-- Recursively remove anything that gets its value randomly
	-- assigned at object creation time, since those random values
	-- are specific to each created object and not generic at all.
	flag_remove_rand(__generic_obj.flags)

	return __generic_obj
end

--
-- Returns the dungeon level or the feature, if the player is not
-- in a dungeon
--
function level_or_feat(dtype, dlevel)
	if dlevel == 0 or player.wild_mode then
		return wild_map(player.wilderness_y, player.wilderness_x).feat
	else
		return dlevel
	end
end

--
-- Determine if a command should be prevented, based on the item's
-- inscription
--
function check_prevent_cmd(obj, cmd)
	if not cmd then
		cmd = command_cmd
	end

	if type(cmd) == "string" then
		if strlen(cmd) ~= 1 then
			error("'cmd' must be a single character")
		end

		cmd = strbyte(cmd)
	end

	return check_prevent_cmd_aux(obj, cmd)
end

--
-- Determine if a "legal" grid can be "seen" by the player
--
function player_can_see_bold(y, x)
	return (cave(y, x).info & CAVE_SEEN ~= 0)
end

function who_to_faction(who)
	if who == 0 then
		return FACTION_PLAYER
	elseif who > 0 then
		local monst = monster(who)

		assert(monst, "No monsters for 'who' " .. who)

		return monst.faction
	elseif who == WHO_DUNGEON then
		return FACTION_DUNGEON
	elseif who == WHO_TRAP then
		error("No faction for WHO_TRAP")
	elseif who == WHO_FLOOR then
		error("No faction for WHO_FLOOR")
	end

	error("Unnown 'who' value " .. who)
end

---
--- A shortened name for get_function_registry_from_flag()
---
function get_func_from_flag(flags, flag_name)
	return get_function_registry_from_flag(flags, flag_name)
end

---
--- Try to find a string in the object's note, if it has a note
---
function notefind(obj, str)
	if obj.note == 0 then
		return false
	end

	return strfind(quark_str(obj.note), str, 1, true)
end

--
-- Returns the CTRL key equivalent of a letter
--
function KTRL(char)
 char = strlower(char)
	local byte = strbyte(char)

	if byte < strbyte('a') or byte > strbyte('z') then
		return nil
	end

	return (byte - strbyte('a') + 1)
end

--
-- The following is something like get_function_registry_from_flag(),
-- but for functions that aren't in the registry.  It saves the name
-- of the function, rather than its bytecode, so anonymous functions
-- can't be used.
--

__function_indexing = {}
__function_indexing.indexes = {}
__function_indexing.strings = {}

add_loadsave("__function_indexing.indexes", {})
add_loadsave("__function_indexing.strings", {})

function function_to_index(func)
	assert(tag(func) == TAG_STRING, "Function must be specified via its name")
	assert(func ~= "", "Function name is empty")

	if __function_indexing.strings[func] then
		return __function_indexing.strings[func]
	end

	local index = getn(__function_indexing.indexes) + 1

	__function_indexing.indexes[index] = func
	__function_indexing.strings[func]  = index

	return index
end

function index_to_function(index)
	assert(index <= getn(__function_indexing.indexes),
		   "No such function is indexed: " .. index)
	assert(index >= 1, "No such function is indexed: " .. index)

	local name = __function_indexing.indexes[index]

	return getglobal(name), name
end

-- Pick up an item without having to deal with internals
function object_pickup_simple(obj, item)
	__core_objects.clean_get_item()
	item = generate_item_floor_index(obj.iy, obj.ix, -item)
	return object_pickup(item)
end

-- Division that rounds up/down to the nearest integer
function div_round(quotient, divisor)
	if quotient == 0 then
		return 0
	end

	local out = quotient / divisor
	local remainer = imod(abs(quotient), divisor)

	if (remainer * 2) >= divisor then
		out = out + (quotient / abs(quotient))
	end

	return out
end

--- @fn
-- @brief Displays a color coded file. The screen should probably be saved firts with term.save()
function display_colored_file(file)
	local fff = fs.open(file, "r")
	if fff then
		local line = nil
		local y = 0
		repeat
			line = fs.read(fff)
			if line then
				term.display(line, y, 0)
				y = y + 1
			end
		until line == nil
		fs.close(fff)
		return true
	end
end

--- @fn
-- @brief Swicth implementation, use as: swicth(cond){ [val1] = function()...end [val2] = function()...end ... }
function switch(cond)
	return function(t)
		t[%cond]()
	end
end

--- @fn
-- @brief Returns whether or not a skill is known
function skills.is_known(skill)
	if wizard then
		return true
	end

	if get_skill(skill) > 0 or s_info(skill).mod > 0 then
		return true
	end

	for i = 0, max_s_idx - 1 do
		if s_info(i).father == skill and skills.is_known(i) then
			return true
		end
	end

	return false
end

--- @fn
-- @brief Returns whether or not a skill has any children skills
function skills.has_child(skill)
	for i = 0, max_s_idx - 1 do
		if s_info(i).father == skill and skills.is_known(i) then
			return true
		end
	end

	return false
end

--- @fn
-- @brief Returns a tree data-structure representation of the list of
-- skills
function skills.get_tree(father)
	local skill_nodes = {}

	father = father or -1

	for i = 0, max_s_idx - 1 do
		if s_info(i).father == father and skills.is_known(i) and
			not s_info(i).hidden
		then
			local info = {}

			info.skill    = i
			info.children = skills.get_tree(i)

			if getn(info.children) == 0 then
				info.children = nil
			end

			tinsert(skill_nodes, info)
		end
	end -- for i = 1, max_s_idx do

	return skill_nodes
end -- skills.get_tree()

--- @fn
-- @brief Produces all possible ordered combinations of a list of
-- elements
function permutate(list)
	if getn(list) == 1 then
		return {{list[1]}}
	end

	local out_list = {}

	for i = 1, getn(list) do
		local first = tremove(list, i)

		local small_list = permutate(list)

		for j = 1, getn(small_list) do
			local arr = small_list[j]
			tinsert(arr, 1, first)
			tinsert(out_list, arr)
		end

		tinsert(list, i, first)
	end

	return out_list
end -- permutate()

--- @fn
-- @brief Determines if the given substring has a word boundry immediatly
-- before and after it in the target string.
function is_separate_word(word, str)
	-- Test for 'word == string' before escaping pattern characters
	-- since escaping might change length of word
	if word == str then
		return true
	end

	-- First, escape any characters that strfind() might interpret
	-- as a pattern
	local word_len = strlen(word)
	word = gsub(word, "([%$%^%%%(%)%.%[%]%*%+%-%?])", "%%%1")

	if not strfind(str, word) then
		return false
	end

	-- Is word?
	if strlen(word) == strlen(str) then
		return true
	end

	-- Word is somewhere in the middle?
	if strfind(str, "[^%w_-]" .. word .. "[^%w_-]") then
		return true
	end

	-- Starts with word?
	if strfind(str, word .. "[^%w_-]") == 1 then
		return true
	end

	-- Ends with word?
	local pos = strlen(str) - word_len
	if strfind(str, "[^%w_-]" .. word, pos) == pos then
		return true
	end

	return false
end -- is_separate_word()

function show_arr_as_file(arr, what, line, mode)
	what = what or ""
	line = line or 0
	mode = mode or 0

	if not make_temp_file() then
		return false
	end

	local file = get_temp_name()

	if not file or file == "" then
		end_temp_file()
		return false
	end

	for i = 1, getn(arr) do
		write_temp_file(arr[i])
	end

	-- Close temp file to flush contents to disk
	close_temp_file()

	local ret = show_file(file, what, line, mode)

	-- Delete temp file
	end_temp_file()

	return ret
end -- show_arr_as_file()

-- An auxilliary function which takes care of the moving up or down
-- in a displayed list.  See show_arr_as_list() for an example of
-- how to use.
function display_list_simple(array, title, state)
	assert(state and type(state) == "table",
		  "Needs a table to store state passed as third argument")

	if not state.x then
		local ret, wid, hgt = term.get_size()

		state.x   = 0
		state.y   = 0
		state.hgt = hgt
		state.wid = wid

		state.color = state.color or color.LIGHT_BLUE
		state.begin = state.begin or 1
		state.sel   = state.sel   or 1
	end

	local per_page = state.per_page or state.hgt - 2

	local size = getn(array)

	display_list(state.y, state.x, state.hgt - 1, state.wid - 1, title,
				 array, state.begin, state.sel, state.color,
				 nil, true)

	if state.footer then
		term.c_put_str(state.color, state.footer,
					   state.y + state.hgt - 1,
					   (state.wid - strlen(state.footer)) / 2 + state.x)
	end

	local key = term.inkey()
	local c   = strchar(key)

	if c == "8" then
		-- up key
		state.sel = state.sel - 1
	elseif c == "2" then
		-- down key
		state.sel = state.sel + 1
	elseif c == "9" then
		-- page up key
		state.sel = state.sel - per_page + 1
	elseif c == "3" then
		-- page down key
		state.sel = state.sel + per_page
	elseif c == "7" then
		-- home key
		state.sel = 1
	elseif c == "1" then
		-- End key
		state.sel = size
	elseif c == " " then
		-- Space key pages down, but wraps around to start if it
		-- goes past the end
		if state.sel >= size - per_page + 1 then
			state.sel   = 1
			state.begin = 1
		else
			state.sel = state.sel + per_page
		end
	end

	if state.sel < 1 then
		state.sel = 1
	elseif state.sel > size then
		state.sel = size
	end

	if state.sel > state.begin + per_page then
		state.begin = state.sel - per_page + 1
	end

	if state.sel < state.begin then
		state.begin = state.sel
	end

	if state.begin > size then
		state.begin = state.sel
	end

	if state.begin < 1 then
		state.begin = 1
	end

	return state.sel, key
end -- display_list_simple()

function show_arr_as_list(to_show, title, dont_save)
    local ret, wid, hgt = term.get_size()

    local begin      = 1
	local per_page   = hgt - 2
	local std_footer = "ESC to quit"

	local arr, func, state, info, footer
	local to_func_chars, to_func_keys = {}, {}

	local __state = {
		color = color.WHITE,
		begin = 1,
		sel   = 1,
	}

	if type(to_show) == "table" then
		arr = to_show
		footer = std_footer
	elseif tag(to_show) == TAG_FUNCTION then
		func = to_show

		arr, info, state = func()

		if info.title then
			title = info.title
		end

		if info.footer then
			footer = std_footer .. ", " .. info.footer
		else
			footer = std_footer
		end

		if info.to_func_chars then
			to_func_chars = info.to_func_chars
		end

		if info.to_func_keys then
			to_func_keys = info.to_func_keys
		end

		if info.begin then
			__state.begin = info.begin
			__state.sel   = info.begin
		end
	else
		error("Can't display type '" .. type(to_show) .. "'")
	end

	if not dont_save then
		term.save()
	end

	while true do
		local size    = getn(arr)
		local no_move = false

		if size <= per_page then
			no_move = true
		end

		if not footer then
			footer = std_footer
		end

		local header

		if no_move then
			header = title
		else
			header = title .. " (showing " .. __state.begin .. "-" ..
				min((__state.begin + per_page - 1), size) ..
				" of " .. size .. " lines)"
		end

		term.clear()
		__state.footer = footer
		local sel, key = display_list_simple(arr, header, __state)

		message.display((wid - strlen(footer)) / 2, hgt - 1,
						strlen(footer) + 2, color.LIGHT_BLUE,
						"[" .. footer .. "]")

        local c = strchar(key)

        if key == ESCAPE then
            break
		elseif to_func_chars[c] or to_func_keys[key] then
			arr, info, state = func(state, c, key, begin, arr)

			if info.title then
				title = info.title
			end

			if info.footer then
				footer = std_footer .. ", " .. info.footer
			else
				footer = std_footer
			end

			if info.to_func_chars then
				to_func_chars = info.to_func_chars
			end

			if info.to_func_keys then
				to_func_keys = info.to_func_keys
			end

			if info.begin then
				__state.begin = info.begin
				__state.sel   = info.begin
			end
		end

		if no_move then
			__state.sel = 1
		end

		-- We're just diplaying the list, not selecting an item, so
		-- make the start of the list equal to the selected item.  That
		-- way, the arrow key will cause the list to scroll down
		-- immediatly.
		__state.begin = __state.sel
	end -- while true do

	if not dont_save then
		term.load()
	end
end -- show_arr_as_list()

function singularize(str)
	local letter = strlower(strsub(str, 1, 1))
	local vowels = {
		a = true,
		e = true,
		i = true,
		o = true
		u = true
	}

	if vowels[letter] then
		return "an " .. str
	else
		return "a " .. str
	end
end

----------
-- Plurals
----------
__special_plurals = {
}

__regexp_plurals = {
	-- Other than "human", words ending in "man" that *don't* pluralize
	-- to men are pretty rare.  If a module uses such a word, it
	-- can add the pluralization to the list of special plurals.
	-- This case also takes care of "foowoman" to "foowomen"
	-- pluralizations as well.
	["(%S*man)$"] =
		function(x)
			if strlower(x) == "human" then
				return x .. "s"
			else
				return strsub(x, 1, strlen(x) - 3) .. "men"
			end
		end
}

function pluralize(str)
	-- If it's "Foo of Bar", then pluraize Foo, not Bar
	local of_pos = strfind(str, " [Oo]f ")
	if of_pos then
		return pluralize(strsub(str, 1, of_pos - 1)) ..
			strsub(str, of_pos, -1)
	end

	if __special_plurals[strlower(str)] then
		-- Lower-case it, then restore original capitalization
		local new_str    = __special_plurals[strlower(str)]
		local first_char = strsub(str, 1, 1)

		if first_char == strupper(first_char) then
			new_str = gsub(new_str, "^(.)",
						   function(x)
							   return strupper(x)
						   end)
		end

		return new_str
	end

	for regexp, replace in __regexp_plurals do
		if strfind(str, regexp) then
			str = gsub(str, regexp, replace)
			return str
		end
	end

	-- Okay, lets do it normally
	local len = strlen(str)
	local one_char  = strsub(str, len, len)
	local two_chars = ""

	if len > 1 then
		two_chars = strsub(str, len - 1, len)
	end

	local plural, eat_len

	if one_char == 's' or one_char == 'h' or one_char == 'x'then
		-- "Cutlass-es" and "Torch-es" and "Box-es"
		plural  = "es"
		eat_len = 0
	elseif one_char == 'y' then
		-- "Pony" -> "Ponies"
		plural  = "ies"
		eat_len = 1
	elseif two_chars == "ff" or two_chars == "fe" then
		-- "Staff" -> "Staves", "Knife" -> "Knives"
		plural  = "ves"
		eat_len = 2
	elseif one_char == 'f' then
		-- "Shelf" -> "Shelves"
		plural  = "ves"
		eat_len = 1
	else
		plural  = "s"
		eat_len = 0
	end

	str = gsub(str, strrep('.', eat_len) .. "$", plural)
	return str
end -- pluralize()

function add_special_plural(single, plural)
	if type(single) == "table" then
		for i, v in single do
			__special_plurals[i] = v
		end
	else
		__special_plurals[single] = plural
	end
end

function add_regexp_plural(regexp, plural)
	if type(regexp) == "table" then
		for i, v in regexp do
			__regexp_plurals[i] = v
		end
	else
		__regexp_plurals[regexp] = plural
	end
end

function line_breaks(str, max_len)
	if strlen(str) < max_len then
		return {str}
	end

	-- Split into words.
	local parts = strsplit(str, " ")

	-- Removing leading blanks
	while true do
		if not parts[1] or parts[1] == "" or parts[1] == " " then
			tremove(parts, 1)
		else
			break
		end
	end

	local lines = {}
	str = ""

	while getn(parts) > 0 do
		local len = strlen(str)
		len = len + strlen(parts[1]) + 1

		if len >= max_len then
			tinsert(lines, clean_whitespace(str))
			str = tremove(parts, 1)
		else
			str = str .. " " .. tremove(parts, 1)
		end
	end -- if len >= 80 then

	if str ~= "" then
		tinsert(lines, clean_whitespace(str))
	end

	if lines[1] == "" then
		tremove(lines, 1)
	end

	return lines
end -- line_breaks()

-- Apply a function to each monster on level
function foreach_monster(fct)
	for i = 1, monst_list.size do
		if (monst_list.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local m_idx = monst_list.node[i].key
			fct(monster(m_idx), m_idx)
		end
	end
end -- foreach_monster()

-- Let user choose from a list of strings
function choose_from_list_simple(array, title, dont_save, start_sel, footer)
	if not dont_save then
		term.save()
	end

    footer = footer or "[ESC to quit]"

	local ret, wid, hgt = term.get_size()
	local lines = hgt - 2
	local begin = start_sel
	if start_sel and getn(array) <= lines then
		begin = 1
	end

	local disp_state = {sel = start_sel, begin = begin, footer = footer}
	local sel, key = nil, nil
	while true do
		term.clear()
		sel, key = display_list_simple(array, title,
									   disp_state)
		if key == ESCAPE then
			sel, key = nil, nil
			break
		elseif key == strbyte("\n") or key == strbyte("\r") then
			break
		end
	end

	if not dont_save then
		term.load()
	end

	return sel
end -- choose_from_list_simple()

-- Wrapper around Lua strchar(), since inkey() can return negative
-- values.
function strchar(key)
	if key < 0 then
		return "\0"
	else
		return %strchar(key)
	end
end

-- Accepts a default message and a list of variables.  If any of the
-- variables is true, it returns true, and returns false otherwise.
-- Also, it returns true, it will try to give a message to the user:
-- if any of the true variables is a  string, it will use the first
-- one which is a string as the message, otherwise it will use the
-- default message.
function or_msg(msg, ...)
	local ret = false

	for i = 1, getn(arg) do
		if arg[i] then
			ret = true
			if tag(arg[i]) == TAG_STRING then
				msg = arg[i]
				break
			end
		end
	end

	if ret and msg then
		message(msg)
	end

	return ret
end

function randomize_list(list)
	local size = getn(list)

	for i = 1, size do
		local j = rng(1, size)
		list[i], list[j] = list[j], list[i]
	end
end

function choose_multi_column(prompt, strs, dont_save, min_cols, colors)
	local chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1',
		'2', '3', '4', '5', '6', '7', '8', '9', '0', '[', ']', '{', '}', ';',
		':', '\'', '\"', ',', '.', '<', '>', '\\', '|', '`', '~'}
	local char_to_num = {}

	for i = 1, getn(chars) do
		char_to_num[chars[i]] = i
	end

	if getn(strs) > getn(chars) then
		message(color.LIGHT_RED, "WARNING: choose_multi_column() can " ..
				"display at most " .. getn(chars) .. " choices; some" ..
				"choices will be missing.")
	end

	local ret, wid, hgt = term.get_size()

	local num  = min(getn(strs), getn(chars))
	local rows = hgt - 2
	local cols = (num / rows) + iif(imod(num, rows) > 0, 1, 0)
	cols = max(cols, (min_cols or 0))

	local max_wid = (wid - (4 * cols - 1)) / cols

	if not dont_save then
		term.save()
	end

	colors = colors or {}
	local choice
	while not choice do
		term.clear()

		local cur_row, cur_col = 1, 1

		for i = 1, num do
			local str = chars[i] .. ") " .. strsub(strs[i], 1, max_wid)
			local clr = colors[i] or color.WHITE
			term.c_put_str(clr, str,
						   cur_row + 1, (cur_col - 1) * (max_wid + 4))

			cur_row = cur_row + 1
			if cur_row > rows then
				cur_row = 1
				cur_col = cur_col + 1
			end
		end -- for i = 1, num do

		term.prt(prompt, 0, 0)

		local key  = term.inkey()
        local char = strchar(key)

		if key == ESCAPE then
			break
		end
		choice = char_to_num[char]

		if choice and choice > num then
			choice = nil
		end
	end -- while true do

	if not dont_save then
		term.load()
	end

	return choice
end -- choose_multi_column()

function monster_strsub(str, monst, flags)
	flags = flags or 0

	local name       = monster_desc(monst, flags)
	local pronoun    = monster_desc(monst, flags | 32)
	local possessive = monster_desc(monst, flags | 32 | 2)
	local reflexive  = monster_desc(monst, flags | 32 | 2 | 1)


	str = gsub(str, ("@name@"), name)
	str = gsub(str, ("@Name@"), strcap(name))

	str = gsub(str, ("@pronoun@"), pronoun)
	str = gsub(str, ("@Pronoun@"), strcap(pronoun))

	str = gsub(str, ("@possessive@"), possessive)
	str = gsub(str, ("@Possessive@"), strcap(possessive))

	str = gsub(str, ("@reflexive@"), reflexive)
	str = gsub(str, ("@Reflexive@"), strcap(reflexive))

	return str
end
