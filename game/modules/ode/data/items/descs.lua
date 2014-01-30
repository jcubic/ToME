--
-- Describe items based on tvals
--

object_desc_tvals=
{
	[TV_WEAPON] = { weapon=true }
	[TV_ARMOR] = { armor=true }

	[TV_CORPSE] = function(obj, desc)
		desc.mod = desc.base

		local monst = obj.flags[FLAG_MONSTER_OBJ]
		if not monst then
			desc.mod  = desc.base
			desc.base = "& BUGGY CORPSE #~"
			return
		end

		local name = monster_desc(monst, 512 | 256 | 128)
		if has_flag(monst, FLAG_UNIQUE) then
			name = book_capitals(name) .. "'s"
			desc.no_article = true
		end

		desc.base = "& ".. name .. " #~"
	end
}
