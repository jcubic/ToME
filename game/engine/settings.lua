-- Engine settings
esettings = {}

-- Make the settings table
-- But be smart about it and make it so that it auto creates subtables as needed
local tag_settings = newtag()
settings = {}
settag(settings, tag_settings)
settagmethod(tag_settings, "index", function(t, k, e)
	if e == nil then t[k] = settag({}, %tag_settings) end
	return t[k]
end)

function esettings.save_recurs(state, fff, t, prefix)
	local list = {}
	for k, _ in t do
		tinsert(list, k)
	end
	sort(list, function(a, b)
		if type(%t[a]) == "table" and type(%t[b]) == "table" then return a < b
		elseif type(%t[a]) == "table" then return nil
		elseif type(%t[b]) == "table" then return 1 end
		return a < b
	end)

	for k in list do
		local e = t[k]
		if type(e) == "table" then
			esettings.save_recurs(state, fff, e, prefix..k..".")
			if state.last_writen then fs.write(fff, "") state.last_writen = false end
		elseif type(e) == "number" then
			fs.write(fff, prefix..k.." = "..e)
			state.last_writen = true
		elseif type(e) == "string" then
			fs.write(fff, prefix..k.." = "..format("%q", e))
			state.last_writen = true
		else
			_ALERT("Setting '"..k.."' is not number/string/table: "..type(e))
		end
	end
end

function esettings.save()
	local old_dir = fs.get_write_dir()
	fs.set_write_dir(ENGINE_DIR_DEP)
	fs.mkdir("/settings")

	for base, table in settings do
		local fff = fs.open("/settings/"..base..".cfg", "wb")
		if fff then
			local state = {}
			esettings.save_recurs(state, fff, table, "")
			fs.close(fff)
		end
	end

	fs.set_write_dir(old_dir)
end

function esettings.load()
	local oldget = gettagmethod(tag(nil), "getglobal")
	local oldset = gettagmethod(tag(nil), "setglobal")

	for file in fs.enumerate_files("/settings/", false) do
		if strlen(file) > 4 and strfind(file, ".cfg", strlen(file) - 4 + 1, true) then
			local base = gsub(file, "(\.cfg)", "")

			settagmethod(tag(nil), "getglobal", function(x)
				local v = settings[%base][x]
				return v
			end)
			settagmethod(tag(nil), "setglobal", function(x, o, v)
				settings[%base][x] =v
			end)

			dofile("/settings/"..file)
		end
	end

	settagmethod(tag(nil), "getglobal", oldget)
	settagmethod(tag(nil), "setglobal", oldset)
end

function esettings.set(k, e)
	if type(e) == "string" then
		dostring("settings."..k.." = "..format("%q", e))
	else
		dostring("settings."..k.." = "..e)
	end
end

function esettings.get(k, default)
	local v = dostring("return settings."..k)
	if not v or type(v) == "table" then
		esettings.set(k, default)
		return default
	else
		return v
	end
end
