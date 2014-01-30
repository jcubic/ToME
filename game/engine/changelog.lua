-- Handles changelogs

constant("changelog", {})

function changelog.run(source, dest)
	dofile(source)

	local table = strsplit(changelog.text, '\n')
	local last_type = nil
	local new = {}

	for i = 1, getn(table) do
		local str = table[i]
		local y, z, type = strfind(str, ".(.).- ")
		if y then
			local prefix = "  "
			if y == 1 and z == 5 then
				last_type = type
				prefix = "- "
			end
			if last_type == 'V' then prefix = "" end
			new[last_type] = new[last_type] or {}
			tinsert(new[last_type], prefix..strsub(str, 6))
		end
	end

	local fff = fs.open(dest, "w")
	local version = new.V[1]
	new.V = nil
	fs.write(fff, version)
	for k, data in new do
		fs.write(fff, "")
		fs.write(fff, (changelog.keys[k] or "?? ").." changes:")
		for i = 1, getn(data) do
			fs.write(fff, data[i])
		end
	end
	fs.close(fff)
	return version
end

-- Special handling if the release process needs a nifty changelog
if globals().__CHANGELOG_RELEASE_MODE__ then
	for i in fs.get_search_path() do
		fs.remove_from_search_path(i)
	end
	fs.add_to_search_path(fs.get_base_dir(), true)
	fs.set_write_dir(__CHANGELOG_RELEASE_MODE__)

	print(changelog.run("/changes.log", "/changes.txt"))
	exit()
end
