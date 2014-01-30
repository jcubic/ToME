-- This file defines the monster memory.
-- It consists of categories containing lists of properties, and have
-- formatting specifications. There is also a global formatting function
-- which specifies how the categories are linked together, and also things
-- that are not part of categories.

-- deep_get{a,b,c,d} will return a[b][c][d], which will be a table
declare_globals{"deep_get","flag2table","m_prt","prt_markup"}
function deep_get(list)
	if not list then return end
	local e = list[1]
	if not e then e = {} end
	for i = 2,getn(list) do
		if not e[list[i]] then e[list[i]] = {} end
		e = e[list[i]]
	end
	return e
end
-- copies flags from flagset into table
function flag2table(flagset,table)
	flag_iterate(flagset,function(f,i,t)t[i]=f[i]end,table)
end

function m_prt(str)
	local col
	str = gsub(str, '<([%w_]+)',
			   function(i)
				   if color[i] then
					   return "<c"..color[i]
				   else
					   return "<"..i
			   end
		   end)
	local a, b, c = strfind(str,"^c%d+")
	if a then
		col = strsub(str,a+1,b) a = b+2
	else
		return
	end
	while a do
		b, c = strfind(str, "%b<>", a)
		if not b then term.text_out(col,strsub(str,a,strlen(str))) break end
		term.text_out(col,strsub(str,a,b-1))
		m_prt(strsub(str,b+1,c-1))
		a = c+1
	end
end

function prt_markup(str) return m_prt("c"..color.WHITE.." "..str) end

declare_globals {"memory"}

-- The namespace
memory = {}

-- The monster memory itself
-- a triple table of monst chars, svals and egos. each cell contains
-- a table of properties with categories as indices.
-- the knowall entry specifies that everything should be known about
-- this monster. it has several possible values:
-- 0: no special knowlege, just what you have experienced
-- 1: the player is being taught about this monster
--	where applicable memory entries are generated from monster information.
--	This is automatically incrented at the end of the main function
-- 2: player knows all details about this monster
-- So at the beastmaster the variable would be set to 1 for the monster
-- the player chooses, and the monster information can then be recalled
-- after the recall knowall will be 2.
-- knowall is shared by all ego variants of a monster
-- Perhaps knowall should not be incremented to 2. If the monster information
-- changes after it has become 2 the memory will be out of date. this may
-- or may not be a good thing.

memory.entries = {}

-- some categories:
-- RT_RESIST: resistance value to damage types
-- RT_SPELLS: spell indices
-- RT_MISC: types that don't correspond to any player structure, and
--	thus don't benefit from grouping here (without a player structure
--	each flag must be checked individually here for the beastmaster
--	anyway, so there is not point in grouping them)

-- Generates an unused property id.
function memory.def_prop(cat, desc)
	local c = memory.cats[cat]
	local n = c.max_props
	c.max_props = n+1
	return n
end

-- Defines an entry if necessary, and returns it
function memory.get_entry(rptr,ego)
	return deep_get{memory.entries,rptr.d_char,rptr.sval,ego}
end

-- Commits a property to memory
-- data is optional
function memory.add(category,property,m_ptr,data)
	local v if data then v = data else v = 0 end
	deep_get{memory.get_entry(race_info(m_ptr),m_ptr.ego),category}[property] = v
end

-- Commits a two-part property to memory
-- data is optional
function memory.add2(category,property1,property2,m_ptr,data)
	local v if data then v = data else v = 0 end
	deep_get{memory.get_entry(race_info(m_ptr),m_ptr.ego),category,property1}[property2] = v
end

-- takes a list of the form [key]=val, and returns a sorted list of
-- the form [1]={key,val}, and so on, which is needed in list1 and
-- list2
function memory.copy_sort(list)
	local res = {}
	for i,v in list do tinsert(res,{key=i, val=v}) end
	sort(res,function(a,b)return a.key<b.key end)
	return res
end

-- Sorts a list of strings, ignoring any color codes
function memory.color_sort(list)
	local arr   = {}
	local table = {}

	for i = 1, getn(list) do
		local no_color = gsub(list[i], "#.", "")
		arr[i]          = no_color
		table[no_color] = list[i]
	end

	sort(arr)

	local out = {}

	for i = 1, getn(arr) do
		out[i] = table[arr[i]]
	end

	return out
end -- memory.color_sort()

-- Output: foo1 foo2 foo3
function memory.list1(list, dfunc)
	if not list then return end
	if getn(list) < 1 then return end
	local str = dfunc(list[1])
	for i = 2, getn(list) do str = str.." "..dfunc(list[i]) end
	return str
end

-- Output: foo1, foo2 and foo3 ("and" can be replaced by something else
-- by putting in a third parameter)
function memory.list2(list, dfunc, join)
	if not list then return end
	if not join then join = " and " end
	local len = getn(list)
	if len < 1 then return end
	local str = dfunc(list[1])
	for i = 2, len-1 do
		str = str..", "..dfunc(list[i])
	end
	if len > 1 then str = str..join..dfunc(list[len]) end
	return str
end

-- Output: foo1 and foo2 and foo3
function memory.list3(list, dfunc)
	if not list then return end
	local len = getn(list)
	if len < 1 then return end
	local str = dfunc(list[1])
	for i = 2, len-1 do
		str = str.." and "..dfunc(list[i])
	end
	if len > 1 then str = str.." and "..dfunc(list[len]) end
	return str
end


-- dfunc for list1 and list2 when given a list of strings
function memory.string_desc(i)
	return i
end

-- Formatting for damage types
-- the lists here are of the [type]=prop form
function memory.dam_desc(i) return get_dam_type_info(i.key, "desc") end
function memory.dam(list) return memory.list2(list,memory.dam_desc) end
function memory.dam_desc2(i)
	local col = "WHITE"
	if i.val < 0 then col = "GREEN"
	elseif i.val > 85 then col = "LIGHT_RED"
	elseif i.val > 60 then col = "RED" end
	return get_dam_type_info(i.key, "desc").." (<"..col.." "..i.val.."%>)" end
function memory.dam2(list) return memory.list2(list,memory.dam_desc2) end

-- Resists
function memory.res_helper(list,str, show_pct)
	local res
	if show_pct then
		res = memory.dam2(memory.copy_sort(list))
	else
		res = memory.dam(memory.copy_sort(list))
	end
	if res then return " @It@ "..str.." "..res.."."
	else return "" end
end

function memory.res_sort(list)
	if not list then return end
	local div = { -1, 0, 60, 85, 99, 100 }
	local tab = {} for i,v in div do tab[v]={} end
	for i,v in list do
		for k,d in div do
			if v <= d then tab[d][i]=v break end
		end
	end
	return tab[-1], tab[60], tab[85], tab[99], tab[100]
end

function memory.resists(rptr, entry)
	local fullres  = deep_get{entry,RT_RESIST}
	local show_pct = false
	if entry.knowall > 0 then
		if entry.knowall == 1 and rptr.flags[FLAG_RESIST] then
			flag2table(rptr.flags[FLAG_RESIST],fullres)
		end
		show_pct = true
	end

	local hurt, resl, res, resh, imm = memory.res_sort(fullres)
	return memory.res_helper(hurt,"is hurt by", show_pct)
		.. memory.res_helper(resl,"weakly resists", show_pct)
		.. memory.res_helper(res,"resists", show_pct)
		.. memory.res_helper(resh,"*resists*", show_pct)
		.. memory.res_helper(imm,"is is completely immune to")
end

-- spells
function memory.spell_desc(s)
	if s.memory then
		return s.memory
	else
		return s.name
	end
end

function memory.spells(rptr, entry)
	local list = deep_get{entry,RT_SPELL}
	if entry.knowall == 1 then
		-- reveal full spell list if not already known
		flag2table(rptr.spells,list)
	end

	local by_heading = {}

	for i = 1, getn(list) do
		if list[i] then
			local s = __tmp_spells[i]

			local school = s.school

			if type(school) == "table" then
				school = school[1]
			end

			local heading
			if __schools[school].memory then
				if __schools[school].memory ~= "" then
					heading = "may " .. __schools[school].memory .. " "
				else
					heading = "may "
				end
			else
				heading = "may cast "
			end

			by_heading[heading] = by_heading[heading] or {}

			tinsert(by_heading[heading], memory.spell_desc(s))
		end
	end

	-- Display the list with the shortest heading first, longest last,
	-- and alphabetical order for headings with equal length.
	local heading_list = {}

	for heading, list in by_heading do
		tinsert(heading_list, heading)
	end

	sort(heading_list,
		 function(a, b)
			 local lena = strlen(a)
			 local lenb = strlen(b)
			 if lena < lenb then
				 return true
			 elseif lena == lenb then
				 return (a < b)
			 else
				 return false
			 end
		 end)

	local out_list = {}

	for i = 1, getn(heading_list) do
		local heading = heading_list[i]
		local spell_list = by_heading[heading]

		local out

		out = heading .. memory.list2(memory.color_sort(spell_list),
									  memory.string_desc, " or ")

		tinsert(out_list, out)
	end

	if getn(out_list) == 0 then
		return ""
	end

	local f = rptr.freq_spell
	if f < 1 then f = 1 end

	local suffix
	if getn(out_list) == 1 then
		suffix = " one time in "..(100/f).."."
	else
		suffix = "; one time in "..(100/f).."."
	end

	return " @It@ "..memory.list3(out_list, memory.string_desc) ..
		suffix
end -- memory.spells()

-- blows
function memory.blows(r_ptr, entry)
	local list = deep_get{entry,RT_BLOWS}
	local blowtable = {}
	if flag_used(r_ptr.blow) > 0 then
		for i=0,flag_max_key(r_ptr.blow) do
			local count = list[i+1]
			if not count then count = 0 end
			if count > 0 or entry.knowall > 0 then
			tinsert(blowtable,{
				["blow"] = flag_get_blow(r_ptr.blow, i),
				["count"] = count,
				})
			end
		end
	end
	local str = memory.list2(blowtable,function(i)
		local str = __monster_attack_methods[i.blow.method].desc
		if i.blow.method > 0 then
			str=str.." to <YELLOW "..__monster_attack_types[i.blow.effect].desc..">"
		end
		local d1, d2 = i.blow.d_dice, i.blow.d_side
		local d = d1*d2
		local lvl = %r_ptr.level
		if %entry.knowall > 0 or (4+lvl)*i.count > 80*d or
			(has_flag(%r_ptr,FLAG_UNIQUE) and 2*(4+lvl)*i.count > 80*d)
		then
			if d2 == 1 then str=str.." with damage <LIGHT_GREEN "..d1..">"
			else str=str.." with damage <LIGHT_GREEN "..d1.."d"..d2..">" end
		end
		return str
	end)
	if str then return " @It@ can "..str.."."
	else return "" end
end

-- helper function for making a descripton array from flags
memory.descs = {}
function memory.flagdesc(flag)
	local flag2 = nil

	if tag(flag) == TAG_TABLE then
		flag2 = flag[2]
		flag  = flag[1]
	end

	local s = memory.descs[flag]
	if not s then
		s = strlower(get_flag_name(flag2 or flag))
	elseif flag2 then
		s = s[flag2]

		if not s then
			s = strlower(get_flag_name(flag2))
		end
	end

	return s
end
function memory.flagdesc_add(list)
	local flags = {}
	for i = 1,getn(list) do
		local cell = list[i]

		if getn(cell) == 2 then
			memory.descs[cell[1]] = cell[2]
			tinsert(flags, cell[1])
		elseif getn(cell) == 3 then
			memory.descs[cell[1]] = memory.descs[cell[1]] or {}
			memory.descs[cell[1]][cell[2]] = cell[3]
			tinsert(flags, {cell[1], cell[2]})
		else
			error("Invalid cell in list given to memory.flagdesc_add()")
		end
	end
	return flags
end

-- Formatting for types with descriptions in memory.desc
-- the description function is optional; flagdesc is used by
-- default.
function memory.prop1(list, fun)
	local f if fun then f = fun else f = memory.flagdesc end
	local s = memory.list1(list,f)
	if s then return " "..s else return "" end
end
function memory.prop2(list, fun)
	local f if fun then f = fun else f = memory.flagdesc end
	local s = memory.list2(list,f)
	if s then return " "..s else return "" end
end

-- Returns an anonymous function, to be given to memory.check(),
-- which tests for the existence of flags.
function memory.make_flag_checker(r_ptr)
	return function(flag)
			   local flag2
			   if tag(flag) == TAG_TABLE then
					flag2 = flag[2]
					flag  = flag[1]
				end

				if not has_flag(%r_ptr,flag) then
					return false
				end

				if flag2 then
					return flag_exists(%r_ptr.flags[flag], flag2)
				end

				return true
			end
end

-- checks wether a monster has each in a list of properties using fun
-- and adds to category if they pass.
function memory.check(entry, list, category, fun)
	for i=1,getn(list) do
		local flag = list[i]

		if fun(flag) then
			if tag(flag) == TAG_TABLE then
				local flag2

				flag2 = flag[2]
				flag  = flag[1]

				deep_get{entry,category,flag}[flag2] = 0
 			else
				deep_get{entry,category}[flag] = 0
			end
		end
	end
end

-- takes a set of flags, checks if the monster has these, updates the
-- memory as necessary and produces a string describing the flags using
-- the passed formatting function
function memory.flags2str(r_ptr, entry, list, category, formatfun, knowall)
	-- we may wish to override knowall, since some things always are known
	-- independently of other things
	local know
	if knowall then know = knowall
	else know = entry.knowall end
	if know == 1 then
		memory.check(entry, list, category,
					 memory.make_flag_checker(r_ptr))
	end

	-- Now that knowall is taken care of, format the stuff
	local has = {}
	for i=1,getn(list) do
		local flag = list[i]

		if tag(flag) == TAG_TABLE then
			local flag2

			flag2 = flag[2]
			flag  = flag[1]

			if deep_get{entry,category,flag}[flag2] then
				tinsert(has,list[i])
			end
		else
			if deep_get{entry,category}[flag] then
				tinsert(has,list[i])
			end
		end
	end -- for i=1,getn(list)

	return formatfun(has)
end

---------------
-- armor and hp
---------------
function memory.toughness(r_ptr,entry)
	local kills = r_ptr.r_tkills
	local level = r_ptr.level
	local hp
	if has_flag(r_ptr, FLAG_FORCE_MAXHP) then hp = r_ptr.hdice*r_ptr.hside
	else hp = ""..r_ptr.hdice.."d"..r_ptr.hside end
	if entry.knowall > 0 or
			(has_flag(r_ptr, FLAG_UNIQUE) and
				kills > 304 / (38 + (5*level) / 4)) or
			kills > 304 / (4 + level) then
		return " @It@ has an armor rating of <LIGHT_GREEN "..r_ptr.ac
			.."> and a life rating of <LIGHT_GREEN "..hp..">."
	else return "" end
end

-----------------------------
-- monster friends and escort
-----------------------------
memory.flag_friends = memory.flagdesc_add{
	{FLAG_ESCORT,"@It@ arrives with an escort."},
	{FLAG_ESCORTED_BY_GROUPS,"@It@ arrives with several escorts."},
	{FLAG_FRIEND,"@It@ usually appears in groups."},
	{FLAG_FRIENDS,"@It@ appears in large groups."}
	}
function memory.friends(r_ptr,entry)
	return memory.flags2str(r_ptr, entry, memory.flag_friends, RT_MISC, memory.prop1)
end

-- monster type and experience. I don't like having to list every
-- possibility here, but unlike spells and resists we don't have
-- a r_ptr entry for these. it defeats some of the purpose of the
-- categories.

memory.flag_align = {}
function memory.flag_align_add(t)
	if tag(t) == TAG_TABLE then
		for i = 1, getn(t) do
			tinsert(memory.flag_align, t[i])
		end
	else
		tinsert(memory.flag_align)
	end
end

memory.flag_race = {
	FLAG_UNIQUE,
}
function memory.flag_race_add(t)
	if tag(t) == TAG_TABLE then
		for i = 1, getn(t) do
			tinsert(memory.flag_race, t[i])
		end
	else
		tinsert(memory.flag_race)
	end
end

function memory.exp(r_ptr,entry)
	if not (r_ptr.r_tkills > 0 or entry.knowall > 0) then return "" end

	-- DGDGDGDG fix me formula are wrong and need to be module based
	local xp1 = r_ptr.mexp*r_ptr.level/player.lev
	local xp2 = 100*r_ptr.mexp*r_ptr.level/iif(player.lev-100 == 0, 1000000, player.lev-100)*xp1
	local xp = ""..xp1
	if xp1 < 100 and xp2 > 0 then xp = xp.."."..xp2 end

	local typ = memory.flags2str(r_ptr, entry, memory.flag_race, RT_MISC, memory.prop1)
	local align = memory.flags2str(r_ptr, entry, memory.flag_align, RT_MISC, memory.prop1)
	if typ == "" then typ = " <WHITE creature>" end
	return " A kill of this<VIOLET "..align..typ.."> is worth <ORANGE "..xp.."> points to a level "..player.lev.." character."
end

------------------
-- misc properties
------------------
memory.flag_misc = memory.flagdesc_add{
	{FLAG_OPEN_DOOR,"open doors"},
	{FLAG_BASH_DOOR,"bash down doors"},
	{FLAG_MOVE_BODY,"push past weaker monsters"},
	{FLAG_KILL_BODY,"destroy weaker monsters"},
	{FLAG_TAKE_ITEM,"pick up objects"},
	{FLAG_KILL_ITEM,"destroy objects"},
	{FLAG_HAS_LITE,"illuminate @its@ surroundings"},
	{FLAG_PASS_WALL, FLAG_PASS_INCORP, "pass through solid matter"},
	{FLAG_PASS_WALL, FLAG_PASS_STONE, "pass through stone"},
	{FLAG_PASS_WALL, FLAG_PASS_DOOR, "pass through doors"},
	{FLAG_PASS_WALL, FLAG_PASS_LIQUID,
		"pass through non-watertight obstables"},
	{FLAG_PASS_WALL, FLAG_PASS_GASEOUS,
		"pass through non-airtight obstacles"},
	{FLAG_PASS_WALL, FLAG_PASS_CLIMB, "climb over obstacles"},
	{FLAG_KILL_WALL, FLAG_WALL, "destroy all obstacles in @its@ path"},
	{FLAG_KILL_WALL, FLAG_PASS_WALL, "destroy most obstacles in @its@ path"},
	{FLAG_KILL_WALL, FLAG_SUBST_ROCK, "bore through rock"},
	{FLAG_KILL_WALL, FLAG_SUBST_GRANITE, "bore through granite"},
	{FLAG_KILL_WALL, FLAG_SUBST_ROCK, "dig through rubble"},
	{FLAG_KILL_WALL, FLAG_DOOR, "destroy doors"},
}
memory.flag_misc2 = memory.flagdesc_add{
	{FLAG_INVISIBLE,"<GREEN @It@ is invisible.>"},
	{FLAG_COLD_BLOOD,"@It@ is cold blooded."},
	{FLAG_EMPTY_MIND,"@It@ is not detected by telepathy."},
	{FLAG_WEIRD_MIND,"@It@ is rarely detected by telepathy."},
	{FLAG_MULTIPLY,"<LIGHT_UMBER @It@ breeds explosively.>"},
	{FLAG_REGENERATE,"<LIGHT_WHITE @It@ regenerates quickly.>"},
	{FLAG_MORTAL,"<RED @It@ is a mortal being.>"},
	{FLAG_REFLECTING,"@It@ <LIGHT_UMBER reflects> bolt spells."},
}

function memory.flag_misc_add(t)
	local flags

	if tag(t) == TAG_TABLE then
		flags = memory.flagdesc_add(t)
	else
		flags = memory.flagdesc_add({t})
	end

	for i = 1, getn(flags) do
		tinsert(memory.flag_misc, flags[i])
	end
end

function memory.flag_misc2_add(t)
	local flags

	if tag(t) == TAG_TABLE then
		flags = memory.flagdesc_add(t)
	else
		flags = memory.flagdesc_add({t})
	end

	for i = 1, getn(flags) do
		tinsert(memory.flag_misc2, flags[i])
	end
end

function memory.misc(r_ptr, entry)
	local str = ""
	local a = memory.flags2str(r_ptr, entry, memory.flag_misc, RT_MISC, memory.prop2)
	if a ~= "" then str = str.." @It@ may"..a.."." end
	str = str..memory.flags2str(r_ptr,entry,memory.flag_misc2,RT_MISC,memory.prop1)
	return str
end

-------------------
-- monster movement
-------------------
function memory.move(r_ptr,entry)
	local str = " @It@"
	local old
	-- AAAAA friendliness needs faction info in r_ptr

	if r_ptr.level == 0 then
		str = str.." lives in the town or the wilderness"
		old = true
	end
	if r_ptr.r_tkills > 0 then
		local col
		if r_ptr.level > dun_level then col = "LIGHT_RED"
		else col = "LIGHT_GREEN" end
		if old then str = str.."," end
		str = str.." is normally found on lvl <"..col.." "..r_ptr.level..">"
		old = true
	end
	if old then str = str..", and" end
	str = str.." moves"
	if has_flag(r_ptr,FLAG_RAND_MOVEMENT) then
		if get_flag(r_ptr,FLAG_RAND_MOVEMENT) >= 75 then
			str = str.." extremely"
		elseif get_flag(r_ptr,FLAG_RAND_MOVEMENT) >= 50 then
			str = str.." somewhat"
		else str = str.." a bit" end
		str = str.." erratically"
		if r_ptr.speed ~= 110 then str = str..", and" end
	end
	if r_ptr.speed > 110 then
		if r_ptr.speed > 130 then str = str.." <RED incredibly>"
		elseif r_ptr.speed > 120 then str = str.." <ORANGE very>" end
		str = str.." <LIGHT_RED quickly>"
	elseif r_ptr.speed < 110 then
		if r_ptr.speed < 90 then str = str.." <LIGHT_GREEN incredibly>"
		elseif r_ptr.speed < 100 then str = str.." <BLUE very>" end
		str = str.." <BLUE slowly>"
	else str = str.." at normal speed" end
	if has_flag(r_ptr,FLAG_NEVER_MOVE) then
		str = str..", but does not deign to chase intruders"
	end
	return str.."."
end

function memory.alertness(r_ptr,entry)
	local str = ""
	if r_ptr.r_wake*r_ptr.r_wake > r_ptr.sleep or
		(r_ptr.sleep == 0 and r_ptr.r_tkills >= 10) or
		entry.knowall > 0
	then
		if r_ptr.sleep > 200 then str = "prefers to ignore"
		elseif r_ptr.sleep > 95 then str = "pays very little attention to"
		elseif r_ptr.sleep > 75 then str = "pays little attention to"
		elseif r_ptr.sleep > 45 then str = "tends to overlook"
		elseif r_ptr.sleep > 25 then str = "takes quite a while to see"
		elseif r_ptr.sleep > 10 then str = "takes a while to see"
		elseif r_ptr.sleep > 5 then str =  "is fairly observant of"
		elseif r_ptr.sleep > 3 then str = "is observant of"
		elseif r_ptr.sleep > 1 then str = "is very observant of"
		elseif r_ptr.sleep > 0 then str = "is vigilant for"
		else str = "is ever vigilant for" end
		str = " @It@ "..str.." intruders, which @it@ may notice from "
			.. 10*r_ptr.aaf.." feet."
	end
	return str
end

----------------
-- monster drops
----------------
function memory.drops(r_ptr,entry)
	local num
	if entry.knowall > 0 then
		-- calculate max drops
		num = 0
		if has_flag(r_ptr,FLAG_DROP_NUMBER) then num = num + get_flag(r_ptr,FLAG_DROP_NUMBER) end
	end
	if r_ptr.r_drop_gold > 0 or r_ptr.r_drop_item > 0 or
		(entry.knowall > 0 and num > 0) then
		if not num then num = max(r_ptr.r_drop_gold,r_ptr.r_drop_item) end
		local sin = nil -- we may need an "n"
		local str = " @It@ may carry"
		if num == 1 then str=str.." a" sin = true
		elseif num == 2 then str=str.." one or two"
		else str=str.." up to "..num end
		if has_flag(r_ptr,FLAG_DROP_GREAT) then
			if sin then str=str.."n" end
			sin = nil
			str=str.." <LIGHT_GREEN exceptional>"
		elseif has_flag(r_ptr,FLAG_DROP_GOOD) then
			sin = nil
			str=str.." <ORANGE good>"
		end
		local had_item
		if r_ptr.r_drop_item > 0 or
			(entry.knowall > 0 and not has_flag(r_ptr,FLAG_ONLY_GOLD)) then
			if sin then str = str.."n" end
			sin = nil
			str = str.." object"
			if num > 1 then str = str.."s" end
			had_item = true
		end
		if r_ptr.r_drop_gold > 0 or
			(entry.knowall > 0 and not has_flag(r_ptr,FLAG_ONLY_ITEM)) then
			sin = nil
			if had_item then str = str.." or" end
			str = str.." treasure"
			if num > 1 then str = str.."s" end
		end
		str = str.."."
		return str
	end
	return ""
end

----------------------
-- death and vengeance
----------------------
function memory.death(r_ptr,entry)
	local str = ""
	if has_flag(r_ptr, FLAG_UNIQUE) then
		local dead
		if r_ptr.max_num == 0 then dead = true end
		if r_ptr.r_deaths > 0 then
			str=str.." @It@ has slain at least "..r_ptr.r_deaths.." of your ancestors"
			if dead then str=str..", but you have avenged them!"
			else str=str..", who remain"..iif(r_ptr.r_deaths>1,"","s")
				.." unavenged." end
		elseif dead then str = " You have slain this foe." end
	elseif r_ptr.r_deaths > 0 then
		str=str.." "..r_ptr.r_deaths.." of your ancestors "
			..iif(r_ptr.deaths>1,"have","has")
			.." been killed by this creature,"
		if r_ptr.r_pkills > 0 then
			str=str.." and you have exterminated at least <LIGHT_GREEN "
				..r_ptr.r_pkills.."> of the creatures."
		elseif r_ptr.r_tkills > 0 then
			str=str.." and your ancestors have exterminated at least "
				..r_ptr.r_tkills.." of the creatures."
		else str=str.." and @it@ is not ever known to have been defeated." end
	else
		if r_ptr.r_pkills > 0 then
			str=str.." You have killed at least <LIGHT_GREEN "
				..r_ptr.r_pkills.."> of these creatures."
		elseif r_ptr.r_tkills > 0 then
			str=str.." Your ancestors have killed at least "
				..r_ptr.r_tkills.." of these creatures."
		else str=str.." No battles to the death are recalled." end
	end
	return str
end

-- The @@s must be substituted away using a dictionary
memory.dict = {}
function memory.dict_fill(r_ptr)
	memory.dict = {}
	if has_flag(r_ptr,FLAG_MALE) then
		memory.dict["@It@"] = "He"
		memory.dict["@it@"] = "he"
		memory.dict["@Its@"] = "His"
		memory.dict["@its@"] = "his"
	elseif has_flag(r_ptr,FLAG_FEMALE) then
		memory.dict["@It@"] = "She"
		memory.dict["@it@"] = "she"
		memory.dict["@Its@"] = "Her"
		memory.dict["@its@"] = "her"
	else
		memory.dict["@It@"] = "It"
		memory.dict["@it@"] = "it"
		memory.dict["@Its@"] = "Its"
		memory.dict["@its@"] = "its"
	end
	memory.dict["@Name@"] = r_ptr.name
end

function memory.dict_sub(str)
	for i,v in memory.dict do str = gsub(str,i,v) end
	return str
end

-- a helpful debugging function
function memory.reveal(m_idx)
	memory.get_entry(race_info(monster(m_idx)),0).knowall = 1
end

--------------------------------------------------
-- Let modules mark certain flags as being obvious
--------------------------------------------------
memory.flag_obvious = {}

function memory.obviousflag_add(t)
	tinsert(memory.flag_obvious, t)
end

-- The core memory has a few obvious flags to deal with
memory.obviousflag_add(FLAG_HAS_LITE)
memory.obviousflag_add(FLAG_MORTAL)

-- The functions that actually describe the memory entry.  We do this
-- as an ordered list of named functions so that:
-- 1) Modules and addons can easily add new types of info that would
--    be akward to fit into the "misc" or "misc2" part of the standard
--    monster memory.
-- 2) The new function can be inserted before or after particular named
--    functions, so the info they provide can go into arbitrary but
--    non-random places in the middle of a monster description, rather
--    than being tacked onto the end.
memory.entrydesc_funcs = {}
memory.entrydesc_order = {}

function memory.entrydesc_add(name, func)
	assert(not memory.entrydesc_funcs[name],
		   "Already a monster memory entry desc func named '" .. name .. "'")

	memory.entrydesc_funcs[name] = func
	tinsert(memory.entrydesc_order, name)
end

function memory.entrydesc_add_before(name, func, before)
	assert(not memory.entrydesc_funcs[name],
		   "Already a monster memory entry desc func named '" .. name .. "'")
	assert(memory.entrydesc_funcs[before],
		   "No monster memory entry desc func named '" .. before .. "'")

	memory.entrydesc_funcs[name] = func

	for i = 1, getn(memory.entrydesc_order) do
		if memory.entrydesc_order[i] == before then
			tinsert(memory.entrydesc_order, i, name)
			break
		end
	end
end

function memory.entrydesc_add_after(name, func, after)
	assert(not memory.entrydesc_funcs[name],
		   "Already a monster memory entry desc func named '" .. name .. "'")
	assert(memory.entrydesc_funcs[after],
		   "No monster memory entry desc func named '" .. after .. "'")

	memory.entrydesc_funcs[name] = func

	for i = 1, getn(memory.entrydesc_order) do
		if memory.entrydesc_order[i] == after then
			tinsert(memory.entrydesc_order, i + 1, name)
			break
		end
	end
end

----------------------------------
-- The master formatting function:
----------------------------------
function memory.main_text(r_idx,ego)
	local r_ptr = race_info_idx(r_idx,ego)
	local entry = memory.get_entry(r_ptr,ego)
	-- prepare knowall, shared between all egos
	local entry0 = memory.get_entry(r_ptr,0)
	if not entry0.knowall then entry0.knowall = 0 end
	if not entry.knowall then entry.knowall = 0 end
	-- omniscience
	if memory.omniscient or game_options.cheat_know then entry0.knowall = 1 end
	if entry.knowall < 1 and entry0.knowall > 0 then entry.knowall = 1 end

	-- notice some obvious things (things that are written so that
	-- it is easy to make them non-obvious, but are obvious now)
	for i,v in {
		memory.flag_align,
		memory.flag_race,
		memory.flag_friends,
		memory.flag_obvious,
	} do
		memory.check(entry, v, RT_MISC, memory.make_flag_checker(r_ptr))
	end

	local str = r_ptr.text -- main description

	for i = 1, getn(memory.entrydesc_order) do
		local func = memory.entrydesc_funcs[memory.entrydesc_order[i]]

		str = str .. func(r_ptr, entry)
	end

	-- fill dictionary
	memory.dict_fill(r_ptr)

	return memory.dict_sub(str)
end

-- Support efficiency hack in display_roff
memory.main_last_printed = nil
memory.main_last_monster = nil
function memory.main_changed(r_idx, ego)
	local str = memory.main_text(r_idx, ego)
	local retval = (not memory.main_last_printed) or
	               (memory.main_last_printed ~= str) or
	               (memory.main_last_monster.ego ~= ego) or
	               (memory.main_last_monster.r_idx ~= r_idx)
	memory.main_last_printed = str
	memory.main_last_monster = { ego = ego r_idx = r_idx }
	return retval
end

function memory.main(r_idx, ego)
	prt_markup(memory.main_text(r_idx, ego))
end

add_loadsave("memory.entries", {}, true)
add_loadsave("memory.omniscient", nil, true)
