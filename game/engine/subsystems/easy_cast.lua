--
-- The easy cast subsystem
--
-- This provides a much nicer interface for spell casting(it grabs all spells the player can currently cast and
-- displays them sorted by school, without requiring the player to select a book)
--
-- Load it with: load_subsystem("easy_cast")

constant("easy_cast", {})

-- The heart of the subsystem, this collects all spells,
function easy_cast.get(param)
	local okay_objs       = {}
	local need_wear_could = {}
	local no_okay_spells  = {}
	local spells_set = { all = {} }
	local spells = {}
	local schools_set = {}
	local schools = {}

	-- Collect all spells available
	for_inventory(player, INVEN_INVEN, INVEN_TOTAL, function(obj, inven_nb)
		if not (obj.flags[FLAG_SPELLBOOK] or
				(obj.flags[FLAG_SPELL_CONTAIN] and
				 obj.flags[FLAG_SPELL_CONTAIN] ~= - 1))
		then
			-- No spells in this objects
		elseif not %param.browse and (obj.flags[FLAG_WIELD_CAST] and
									  inven_nb < INVEN_PACK)
		then
			-- Need to wear it to use.  Any spells we *could* use if
			-- we wore it (and that we know about)?
			if known_flag(obj, FLAG_WIELD_CAST) then
				local s = grab_object_spells(obj)
				for idx, spl in s do
					if not %param.only_castable or is_ok_spell(spl, obj) then
						tinsert(%need_wear_could, object_desc(obj))
						break
					end
				end
			end
		else
			-- Object has spells and is usable
			tinsert(%okay_objs, object_desc(obj))
			local s = grab_object_spells(obj)
			local has_okay_spells
			for idx, spl in s do
				if not %param.only_castable or is_ok_spell(spl, obj) then
					has_okay_spells = true
					for _, sch in __spell_school[spl] do
						local name = school(sch).name
						%spells_set[name] = %spells_set[name] or {}
						%spells[name] = %spells[name] or {}

						if not %schools_set[name] then
							%schools_set[name] = true
							tinsert(%schools, name)
						end

						if not %spells_set[name][spl] then
							%spells_set.all[spl] = obj
							%spells_set[name][spl] = obj
							tinsert(%spells[name], spl)
						end
					end
				end
			end
			if not has_okay_spells then
				tinsert(%no_okay_spells, object_desc(obj))
			end
		end
	end)

	-- Can't cast any spells?
	if getn(schools) == 0 then
		-- Could cast some spells, if some items were equipped.
		if getn(need_wear_could) > 0 then
			message("You could cast a spell if you equiped certain items.")
			return -1
		end -- if getn(need_wear_could) > 0 then

		-- No objects from which a spell could be cast.
		if getn(okay_objs) == 0 then
			message("You have no objects from which you could cast a spell.")
			return -1
		end -- if getn(okay_objs) == 0 then

		if getn(no_okay_spells) == getn(okay_objs) then
			-- We have objects from which to cast a spell, but can't cast any
			-- of them.
			message("You are not skilled enough to cast any available " ..
					"spells.")
		else
			-- Should this ever happen?
			message("For some reason, you cannot cast any spells.")
		end
		return -1
	end -- if getn(spells) == 0 then

	-- Sort spells and schools by level and name
	sort(spells, function(a, b) return spell(a).level < spell(b).level end)
	sort(schools)
	for i = 1, getn(schools) do
		schools[i] = { desc = format("%-20s(%d spells)", schools[i], getn(spells[schools[i]])), name = schools[i] }
	end

	-- Check we dont try to repeat a spell
	local spl = nil
	local ret, rep = repeat_keys.pull()
	if ret and spells_set.all[rep] then
		if is_ok_spell(rep, spells_set.all[rep]) then
			spl = rep
		end
	else
		-- Ask for the school
		local act

		if param.force_school then
			act = {name=param.force_school}
		else
			local action_shortinfo = function(y, x, a) if a then term.blank_print(a.desc, y, x) end end
			local index, str
			act, index, str =
				choose_from_list(schools, "Spell schools",
								 "Cast a spell from which school of magic?",
								 action_shortinfo)
			if not act and str then
				-- User gave a string via '@'; might be the name of a spell
				-- rather than the name of a school
				local idx = __spells_by_name[strlower(clean_whitespace(str))]

				if idx and spells_set.all[idx] then
					if is_ok_spell(idx, spells_set.all[idx]) then
						return idx
					end
				end
				return -1
			end
		end

		if not act then return -1 end

		local book = call(make_book, spells[act.name])

		spl = get_school_spell("cast", "is_ok_spell", book, param.browse)
		delete_object(book)
	end

	return spl
end

function easy_cast.cast(force_school)
	-- Clear monster auto-casting stuff in case a Lua error prevented
	-- it from being cleared normally.  This is mainly for the
	-- convenience of developers, so they don't have to
	-- save-exit-restore to get targeting working again when errors
	-- happen.
	while getn(old_spell_caster) > 0 do
		unset_auto_cast()
	end
	while getn(old_get_aim_dir) > 0 do
		unset_get_aim_dir()
	end
	while getn(old_get_rep_dir) > 0 do
		unset_get_rep_dir()
	end
	while getn(old_tgt_pt) > 0 do
		unset_tgt_pt()
	end

	local spl = easy_cast.get{force_school=force_school, only_castable=true}

	if spl ~= -1 then
		cast_school_spell(spl, spell(spl))
	end
end

function easy_cast.browse(force_school)
	easy_cast.get{force_school=force_school, only_castable=false, browse=true}
end

easy_cast.MKEY_CAST = add_mkey{ fct = easy_cast.cast, browse = easy_cast.browse }
