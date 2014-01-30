-- ToME global variable discipline

-- Lua files should declare which globals they use.  The tag methods
-- here catch (a) undeclared globals (b) declared but never defined globals
-- (c) assignments to globals which were declared read-only

-- There is a separate "scope" for each file which tome_dofile is
-- called on.  There is one outer called "NONE" for globals which
-- might be declared outside of any tome_dofile call (like hooks
-- created at runtime)

__globals_by_file = {["NONE"]={}}
__globals_by_name = {}
__global_dec_file = "NONE"

__global_save_globals = {}

-- Intercept dofile to set up the global protection

function protect_tome_dofile(s,p)
	local save_dec_file = __global_dec_file

--	message(color.VIOLET,"Loading "..s.." in protection mode < "..save_dec_file)

	__globals_by_file[s] = {}
	__global_dec_file = s

	local save = settagmethod(tag(nil),"setglobal",__global_new_global)

	p()

	settagmethod(tag(nil),"setglobal",save)

	__global_dec_file = save_dec_file

	for k,v in __globals_by_file[s] do
		if strsub(v,1,4) == "DECL" then -- hackish strsub ;-(
			message(color.VIOLET,"WARNING: "..k.." declared but never defined in "..s)
		end
	end
end

__global_tome_dofile = tome_dofile
__global_tome_dofile_anywhere = tome_dofile_anywhere

function tome_dofile(f,b)
	protect_tome_dofile(f,function () __global_tome_dofile(%f,%b or FALSE) end)
end

function tome_dofile_anywhere(d,f,t,b)
	protect_tome_dofile(path_build(d,f),function ()__global_tome_dofile_anywhere(%d,%f,%t or true,%b or FALSE) end)
end

-- tag function to protect new globals

function __global_new_global(k,o,v)
	if __globals_by_file[__global_dec_file][k] == "DECLARED" then
		__globals_by_file[__global_dec_file][k] = "DEFINED"
	elseif __globals_by_file[__global_dec_file][k] == "DECLARED READ-ONLY" then
		__globals_by_file[__global_dec_file][k] = "DEFINED READ-ONLY"
		v = {value=v}
		settag(v,__global_readonly_tag)
	else
		message(color.VIOLET,"ERROR: defining undeclared global "..k.." in "..__global_dec_file)
		__globals_by_file[__global_dec_file][k] = "DEFINED BUT UNDECLARED"
	end
	rawset(globals(),k,v)
end

-- tag setup for read-only globals

__global_readonly_tag = newtag()

function __global_set_readonly(k,o,v)
	message(color.VIOLET,"ERROR: Attempt to set read-only global "..k)
end

function __global_get_readonly(k)
	return rawget(globals(),k).value
end

settagmethod(__global_readonly_tag,"setglobal",__global_set_readonly)
settagmethod(__global_readonly_tag,"getglobal",__global_get_readonly)

-- functions for files to declare their globals

function declare_globals(t)
	__globals_declare("declare_globals","DECLARED",t)
end

function declare_global_constants(t)
	__globals_declare("declare_global_constants","DECLARED READ-ONLY",t)
end

function global(n, v)
	declare_globals(n)
	setglobal(n, v)
end

function constant(n, v)
	declare_global_constants(n)
	setglobal(n, v)
end

function __globals_declare(funcname,decstr,t)
	if type(t) == "table" then
		if __global_dec_file ~= "" then
			for i=1,getn(t) do
				if __globals_by_name[t[i]] then
					message(color.VIOLET,"ERROR: "..t[i].." redeclared in "..__global_dec_file..
								" original declaration in "..__globals_by_name[t[i]])
				else
					__globals_by_file[__global_dec_file][t[i]] = decstr
					__globals_by_name[t[i]]=__global_dec_file
				end
			end
		end
	elseif type(t) == "string" then
		__globals_declare(funcname, decstr, {t})
	else
		error(funcname.." must be called with table. (Did you say (foo) when you meant {foo}?)")
	end
end


-- Beware of the scary undefined globals
function safe_getglobal(x)
	local v = rawget(globals(), x)

	if v then
		return v
	else
		error("undefined global variable '"..x.."'")
	end
end

function safe_setglobal(x,o,v)
	if __globals_by_file["NONE"][x] then
		-- Runtime-declared
		rawset(globals(),x,v)
	else
		error("Attempt to create new global '"..x.."' outside of initialisation phase")
	end
end

function set_safe_globals()
	settagmethod(tag(nil), "getglobal", safe_getglobal)
	settagmethod(tag(nil), "setglobal", safe_setglobal)
end

function unset_safe_globals()
	settagmethod(tag(nil), "getglobal", nil)
end

set_safe_globals()


-- XXX This little helper function is not used by ToME
-- I used it to mostly automatically produce the global decs
-- for current ToME files, it might be useful to someone else
-- for that purpose also

--function dump_declaration(f)
--	local names = {}
--	local capsnames = {}
--	for k,v in __globals_by_file[f] do
--		if v == "DEFINED BUT UNDECLARED" then
--			if k == strupper(k) then
--				tinsert(capsnames,k)
--			elseif type(rawget(globals(),k)) == "function" then
--				tinsert(capsnames,k)
--			else
--				tinsert(names,k)
--			end
--		end
--	end
--	sort(names)
--	sort(capsnames)
--	print("declare_global_constants {")

--	local do_one = function(n)
--									 local hack = ""
--									 if strfind(n,"[%.%[]") then
--										 hack = "  -- XXX Jules fix this"
--									 end
--									 print('	"'..n..'",'..hack)
--								 end

--	for i=1,getn(capsnames) do
--		do_one(capsnames[i])
--	end
--	print("}")
--	print("declare_globals {")
--	for i=1,getn(names) do
--		do_one(names[i])
--	end
--	print("}")
--end
