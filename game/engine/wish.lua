--------------------------------------------------
--------------------- wish Code -------------------
---------------------------------------------------

-- NOTE: You can wish for things besides objects, but wishing is
-- mainly used for objects, so it goes in this file.  Wishing for
-- monsters or corpses uses functions in engine/monsters.lua

constant("wish", {})

wish.GRANTED   = 1
wish.UNGRANTED = 2
wish.DENIED    = 3

wish.ONE_ITEM_MSG = "You can only wish for *ONE* item."
wish.NO_ARTS_MSG  = "You can't wish for an artifact."

-- Some item kinds might want to do their own parsing of the wish string
new_flag("PARSE_WISH_STR")

-- Some item kinds might want to do their setup of the wish for item.
new_flag("GRANT_WISH")

-- Interpret wish string as a request for a monster, and see if that
-- works
function wish.for_monster(wish_str)
        -- NOT IMPLEMENTED
	return wish.UNGRANTED
end

-- Interpret wish string as a request for a corpse, and see if
-- that works.
-- NOTE: this is separate from wish.for_item(), since it has
-- to parse monster egos instead of item egos
function wish.for_corpse(wish_str)
	local suffixes = {
		corpse        = SV_CORPSE_CORPSE,
		corpses       = SV_CORPSE_CORPSE,
		skeleton      = SV_CORPSE_SKELETON,
		skeletons     = SV_CORPSE_SKELETON,
		["raw meat"]  = SV_CORPSE_MEAT,
		["raw meats"] = SV_CORPSE_MEAT
	}
	local is_plural = {
		corpses       = SV_CORPSE_CORPSE,
		skeletons     = SV_CORPSE_SKELETON,
		["raw meats"] = SV_CORPSE_MEAT
	}
		
	local name, suffix
	local sval

	for k, v in suffixes do 
		if ends_with(wish_str, " " .. k) then
			name   = strsub(wish_str, 1, strlen(wish_str) - strlen(k) - 1)
			suffix = k
			sval   = v
			break
		end
	end

	if not name then
		return wish.UNGRANTED
	end

	if is_plural[suffix] then
		message(wish.ONE_ITEM_MSG)
		return wish.DENIED
	end

	local mon_info = parse_named_monster(name, true)
	-- Maybe the user gave the name as "foo's corpse" instead of
	-- "foo corpse"?
	if not mon_info then
		if ends_with(name, "'s") then
			name = strsub(name, 1, strlen(name) - 2)
		elseif ends_with(name, "'") then
			name = strsub(name, 1, strlen(name) - 1)
		end

		mon_info = parse_named_monster(name)
	end

	if not mon_info then
		return wish.DENIED
	end

	local race = r_info(mon_info.race_idx)
	if race.flags[FLAG_UNIQUE] and not wizard then
		message("You may not wish for the corpses of unique monsters.")
		return wish.DENIED
	end

	local m_ptr = monster_type_new()
	monster_prep(m_ptr, mon_info.race_idx, mon_info.ego_idx or 0)

	local desc   = monster_desc(m_ptr, 512 | 256 | 128)
	local plural = pluralize(desc)

	if sval == SV_CORPSE_CORPSE or sval == SV_CORPSE_MEAT then
		if not m_ptr.flags[FLAG_DROP_CORPSE] then
			message(strcap(plural) .. " don't leave corpses.")
			delete_monster_type(m_ptr, true)
			return wish.DENIED
		end
	elseif sval == SV_CORPSE_SKELETON then
		if not m_ptr.flags[FLAG_DROP_SKELETON] then
			message(strcap(plural) .. " don't leave skeletons.")
			delete_monster_type(m_ptr, true)
			return wish.DENIED
		end
	else
		message("ERROR: Unknown corpse sval.")
		delete_monster_type(m_ptr, true)
		return wish.DENIED
	end

	local o_ptr = new_object()
	object_prep(o_ptr, lookup_kind(TV_CORPSE, sval))
	o_ptr.flags[FLAG_MONSTER_OBJ] = m_ptr

	local weight = race.weight
	if mon_info.ego_idx ~= 0 then
		local ego = re_info(mon_info.ego_idx)
		weight = weight + ego.weight
	end

	if sval == SV_CORPSE_SKELETON then
		weight = (weight / 4) + (rng(weight) / 40) + 1
	elseif sval == SV_CORPSE_CORPSE then
		weight = weight + (rng(weight) / 10) + 1
	elseif sval == SV_CORPSE_MEAT then
		weight = o_ptr.weight
	end
	o_ptr.weight = weight

	o_ptr.found      = OBJ_FOUND_WISH
	o_ptr.found_aux1 = current_dungeon_idx
	o_ptr.found_aux2 = level_or_feat(current_dungeon_idx, dun_level)

	hook.process(hook.CORPSE_CREATE_POST, o_ptr, m_ptr)

	if (drop_near(o_ptr, -1, player.py, player.px) == 0) then
		return wish.UNGRANTED
	end

	return wish.GRANTED
end -- wish.for_corpse()

-- Check to see if user tried to wish for more than one of an item
function wish.test_multiple_item(wish_str, raw_name)
	local plural

	if strfind(raw_name, "#~") then
		-- How do you detect plurals if the "~" comes right
		-- after the "#"???
		return nil
	end

	raw_name = gsub(raw_name, "#", "")
	raw_name = gsub(raw_name, "&", "")
	raw_name = clean_whitespace(raw_name)

	-- Default to pluralizing at the end of the name
	if not strfind(raw_name, "~") then
		raw_name = raw_name .. "~"
	end

	plural = gsub(raw_name, "~", "s")
	if strfind(wish_str, plural) then
		return true
	end

	plural = gsub(raw_name, "~", "es")
	if strfind(wish_str, plural) then
		return true
	end

	-- "Wolf" / "Wolves"
	plural = gsub(raw_name, "f~", "ves")
	if strfind(wish_str, plural) then
		return true
	end

	-- "Staff" / "Staves"
	plural = gsub(raw_name, "ff~", "ves")
	if strfind(wish_str, plural) then
		return true
	end

	-- "Knife" / "Knives"
	plural = gsub(raw_name, "fe~", "ves")
	if strfind(wish_str, plural) then
		return true
	end

	-- "Octopus" / "Octopi"
	plural = gsub(raw_name, "us~", "i")
	if strfind(wish_str, plural) then
		return true
	end

	return nil
end

-- Interpret wish string as a request for an artifact, and see if
-- that works
function wish.for_artifact(wish_str)
	local art_str = nil
	local art_idx = nil

	if starts_with(wish_str, "the ") == true then
		wish_str = strsub(wish_str, 5, strlen(wish_str))
	end

	local quoted_str = "'" .. wish_str .. "'"

	-- Currently, wishing for artifacts is only allowed in wizard
	-- mode
	for i = 2, max_a_idx do
		local item = a_info[i]
		local name = item.name

		name = gsub(name, "& ", "")
		name = gsub(name, "~", "")

		-- If user wished for "FOO", and the name ends with
		-- "'FOO'", that's probably what the user wants.
		if (wish_str == name) or ends_with(name, quoted_str) then
			if wizard then
				art_str = name
				art_idx = i - 1
				break
			else
				message(wish.NO_ARTS_MSG)
				return wish.DENIED
			end
		end

		-- Was the user crazy enough to ask for a *plural* of
		-- an artifact?
		if wish.test_multiple_item(wish_str, item.name) == true then
			message(wish.ONE_ITEM_MSG)
			return wish.DENIED
		end
	end

	if not art_idx then
		return wish.UNGRANTED
	end

	local obj = create_artifact(art_idx)

	set_known(obj)
	set_aware(obj)

	obj.found      = OBJ_FOUND_WISH
	obj.found_aux1 = current_dungeon_idx
	obj.found_aux2 = level_or_feat(current_dungeon_idx, dun_level)

	drop_near(obj, -1, player.py, player.px)

	return wish.GRANTED
end

function wish.okay_ego_pair(ego1_idx, ego2_idx, give_message)
	local ego1 = e_info(ego1_idx)
	local ego2 = e_info(ego2_idx)

	local flags = flag_new()

	for i = 1, MAX_EGO_FLAG_GROUPS do
		if ego1.rar[i] >= 100 then
			flag_add(flags, ego1.flags[i])
		end
	end

	flag_add(flags, ego1.need_flags)

	if flag_intersects(flags, ego2.forbid_flags) then
		if give_message then
			message("Egos '" .. ego1.name .. "' and '" .. ego2.name ..
					"' can not be combined.")
		end

		return false
	end

	flag_free(flags, 0)

	---

	flags = flag_new()

	for i = 1, MAX_EGO_FLAG_GROUPS do
		if ego2.rar[i] >= 100 then
			flag_add(flags, ego2.flags[i])
		end
	end

	flag_add(flags, ego2.need_flags)

	if flag_intersects(flags, ego1.forbid_flags) then
		if give_message then
			message("Egos '" .. ego1.name .. "' and '" .. ego2.name ..
					"' can not be combined.")
		end

		return false
	end

	flag_free(flags, 0)

	return true
end

function wish.okay_ego_combo(combo, give_message)
	for i = 1, getn(combo) do
		for j = i + 1, getn(combo) do
			if not wish.okay_ego_pair(combo[i], combo[j], give_message) then
				return false
			end
		end
	end

	return true
end -- wish.okay_ego_combo()

function wish.get_possible_ego_combos(k_idx, str, is_prefix)
	if not str or str == "" then
		return {{}}
	end

	local ok_egos        = {}
	local bad_pos_match  = false

	-- Look for egos whose name matches chunks of whole words in the
	-- string.  Don't use plain strfind(), since that could match
	-- for egos whose names are embedded in a larger word in the string
	for group, egos_by_k_idx in rand_obj.egos_by_k_idx do
		local list = egos_by_k_idx[k_idx + 1]

		for i = 1, getn(list) do
			local ego = e_info(list[i])

			if is_separate_word(strlower(ego.name), str) then
				if ego.before == is_prefix then
					tinsert(ok_egos, list[i])
				else
					bad_pos_match = ego.name
				end
			end
		end
	end

	-- Any matching egos?
	if getn(ok_egos) == 0 then
		if bad_pos_match then
			message("The ego '" .. bad_pos_match .. "' should go " ..
					iif(is_prefix, "after", "before") .. " the item name.")
			return nil
		end

		local any_egos = false

		for i = 2, max_e_idx do
			if is_separate_word(strlower(e_info[i].name), str) then
				any_egos = true
				break
			end
		end

		if any_egos then
			message("'" .. str .. "' can't be combined " ..
					"with this item.")
		else
			message("No egos match '" .. str .. "'.")
		end

		return nil
	end

	-- Are all the words in the ego string accounted for?
	local words = strsplit(str, " ")
	local word_table = {}

	for i = 1, getn(words) do
		word_table[words[i]] = true
	end

	for i = 1, getn(ok_egos) do
		words = strsplit(strlower(e_info(ok_egos[i]).name), " ")

		for j = 1, getn(words) do
			word_table[words[j]] = nil
		end
	end

	local unacounted_words = {}

	for i, v in word_table do
		tinsert(unacounted_words, i)
	end

	if getn(unacounted_words) > 0 then
		local word = unacounted_words[1]

		-- Maybe it's in an ego for the other end of the item?
		for i = 1, getn(egos_for_k_idx) do
			local ego = e_info(egos_for_k_idx[i])

			if is_separate_word(word, strlower(ego.name)) and
				is_separate_word(strlower(ego.name), str)
			then
				message("Ego '" .. ego.name .. "' must go " ..
						iif(is_prefix, "after", "before") ..
							" the item name.")

				return nil
			end
		end -- for i = 1, getn(egos_for_k_idx) do

		-- Okay then, is the word found at *all* in any of the egos?
		local word_found = false

		for i = 2, max_e_idx do
			if is_separate_word(word, strlower(e_info[i].name)) then
				word_found = true

				if is_separate_word(strlower(e_info[i].name), str) then
					message("Ego '" .. e_info[i].name .. "' can not " ..
							"be combined with this type of item.")
					return nil
				end
			end
		end -- for i = 2, max_e_idx do

		if word_found then
			message("Can't turn '" .. str .. "' into ego, maybe '" ..
					word .. "' is the problem?")
		else
			message("'" .. word .. "' isn't found in any ego name.")
		end

		return nil
	end -- if getn(unacounted_words) > 0 then

	-- No unacounted words, now lets get all permuations of the allowed
	-- egos and see if any combination can reproduce the input string.
	local combos    = permutate(ok_egos)
	local matches   = {}
	local sig_table = {}

	for i = 1, getn(combos) do
		local combo = combos[i]

		local match_str  = nil
		local match_egos = {}

		for j = 1, getn(combo) do
			tinsert(match_egos, combo[j])

			if not match_str then
				match_str = strlower(e_info(combo[j]).name)
			else
				match_str = match_str .. " " .. strlower(e_info(combo[j]).name)
			end

			local match_signature = strjoin(match_egos, ":")

			if sig_table[match_signature] then
				-- Already have exact same match
				break
			end

			if match_str == str then
				sig_table[match_signature] = true
				tinsert(matches, match_egos)
				break
			end
		end -- for j = 1, getn(combo) do
	end -- for i = 1, getn(combos) do

	if getn(matches) == 0 then
		message("Can't figure out '" .. str .. "' as an ego.")
		return nil
	end

	local good_matches = {}
	local bad_matches  = {}
	local too_long     = {}

	for i = 1, getn(matches) do
		if wish.okay_ego_combo(matches[i]) then
			if getn(matches[i]) > MAX_EGO_PER_OBJ then
				tinsert(too_long, matches[i])
			else
				tinsert(good_matches, matches[i])
			end
		else
			tinsert(bad_matches, matches[i])
		end
	end

	if getn(good_matches) == 0 then
		if getn(too_long) > 0 then
			message("Too many egos in '" .. str .. "'.")
			return nil
		end

		-- All solutions have egos that can't go together; inform user.
		for i = 1, getn(bad_matches) do
			wish.okay_ego_combo(bad_matches[i], true)
		end

		return nil
	end

	return(good_matches)
end -- wish.get_possible_ego_combos()

-- MATT XXX
function wish.extract_egos(wish_str, base_str, item, item_idx)
	local egos = {}

	-- Does this item kind have it's own strange way of parsing a
	-- wish string?
	if item.flags[FLAG_PARSE_WISH_STR] then
		local func = get_func_from_flag(item.flags, FLAG_PARSE_WISH_STR)
		local base, success

		success, base, egos = func(wish_str, item)

		if success ~= wish.GRANTED then
			return success
		end

		if not egos then
			egos = {}
		end

		return wish.GRANTED, egos
	end

	-- Extract egos normally
	local base_pos = strfind(wish_str, base_str, 1, true)
	local base_len = strlen(base_str)
	local item_len = strlen(wish_str)

	local prefix_str, suffix_str = "", ""

	if base_pos > 1 then
		prefix_str = strsub(wish_str, 1, base_pos - 2)
	end

	local prefix_combos =
		wish.get_possible_ego_combos(item_idx, prefix_str, true)

	if not prefix_combos then
		return wish.DENIED
	end

	if base_pos + base_len <= strlen(wish_str) then
		suffix_str = strsub(wish_str, base_pos + base_len + 1,
							strlen(wish_str))
	end

	local suffix_combos =
		wish.get_possible_ego_combos(item_idx, suffix_str, false)

	if not suffix_combos then
		return wish.DENIED
	end

	-- Are there any egos at all?
	if getn(suffix_combos[1]) == 0 and getn(prefix_combos[1]) == 0 then
		return wish.GRANTED, {}
	end

	-- Now generate and test cross-product of possible prefix and
	-- suffix ego combos
	local cross_product = {}

	for i = 1, getn(prefix_combos) do
		for j = 1, getn(suffix_combos) do
			local product = table_copy(prefix_combos[i])

			for k = 1, getn(suffix_combos[j]) do
				tinsert(product, suffix_combos[j][k])
			end

			tinsert(cross_product, product)
		end
	end

	local okay     = {}
	local bad      = {}
	local too_long = {}

	for i = 1, getn(cross_product) do
		local product = cross_product[i]

		if wish.okay_ego_combo(product) then
			if getn(product) > MAX_EGO_PER_OBJ then
				tinsert(too_long, product)
			else
				tinsert(okay, product)
			end
		else
			tinsert(bad, product)
		end
	end -- for i = 1, getn(cross_product)

	if getn(okay) == 0 then
		if getn(too_long) > 0 then
			message("Too many egos.")
			return wish.DENIED
		end

		-- All solutions have egos that can't go together; inform user.
		for i = 1, getn(bad) do
			wish.okay_ego_combo(bad[i], true)
		end

		return wish.DENIED
	end

	if getn(okay) > 1 then
		message("More than one way to interpret egos.")

		return wish.DENIED
	end

	return wish.GRANTED, okay[1]
end -- wish.extract_egos()

-- Interpret wish string as a request for an item, and see if
-- that works
function wish.for_item(wish_str)
	local item_str = ""
	local item_idx = nil

	-- Find item with the longest matching name.  We don't want to
	-- match on "pear" when the user is asking for "spear", or
	-- given them an ordinary cloak when asking for an elven cloak
	for i = 1, max_k_idx-1 do
		local item = k_info[i+1]
		local name = {}
		local raw_name
		local dbg = false

		if object_desc_tvals[item.tval] and not is_artifact(item) then
			name.aware = 1
			name.known = 1

			if item.flags[FLAG_PARSE_WISH_STR] then
				-- The item kind might have a weird way of parsing
				-- the item description
				local func = get_func_from_flag(item.flags,
												FLAG_PARSE_WISH_STR)
				local success, base = func(wish_str, item)

				if success == wish.GRANTED then
					name["base"] = base
				else
					name["base"] = ""
				end
			elseif tag(object_desc_tvals[item.tval]) == TAG_FUNCTION then
				object_desc_tvals[item.tval](item, name, 0)
			else
				for k, e in object_desc_tvals[item.tval] do
					name[k] = e
				end
			end

			if name["base"] then
				name = name["base"]
				raw_name = name
				name = gsub(name, "#", item.name)
			else
				name = item.name
				raw_name = name
			end
		else
			name = item.name
			raw_name = name
		end -- Extract the object's base description

		if raw_name ~= "" and wish.test_multiple_item(wish_str, raw_name) then
			message(wish.ONE_ITEM_MSG)
			return wish.DENIED
		end

		name = gsub(name, "& ", "")
		name = gsub(name, "~", "")
		name = gsub(name, " ?#", "")

		-- We have a separate function for getting corpses,
		-- raw meats and such
		if strfind(wish_str, strlower(name), 1, true) and 
			item.tval ~= TV_CORPSE then
			-- Is the matching name longer than the previously longest
			-- matching name?
			if strlen(name) > strlen(item_str) then
				item_str = strlower(name)
				item_idx = i
			end
		end
	end ----  for i = 1, max_k_idx-1 do

	if not item_idx then
		-- No message, the wish might be for something besides
		-- an item
		return wish.UNGRANTED
	end

	-- Now make sure that what we *think* is the item name isn't
	-- actually in the middle of a word.  A wish for an item should
	-- be in the form of "blah ITEM blah", not "blahITEMblah".
	-- If the item name is in the middle of a word, it might be a
	-- wish for a non-item.  For intance, the monster "cloaker"
	-- contains "cloak", but we don't want to give the user a
	-- cloak when wishing for a cloaker.
	if not is_separate_word(item_str, wish_str) then
		return wish.UNGRANTED
	end -- Is the item string embeded in a word?

	local item = k_info[item_idx+1]
	local obj, success

	if is_artifact(item) and not wizard then
		message(wish.NO_ARTS_MSG)
		return wish.DENIED
	end

	if item.flags[FLAG_GRANT_WISH] then
		-- This item type has its own way of setting up the object
		-- that is to be created.  The item's wishing function must
		-- attach the egos to the object and call "apply_magic()".
		local func = get_func_from_flag(item.flags, FLAG_GRANT_WISH)
		local success

		success, obj = func(wish_str, item)

		if success ~= wish.GRANTED then
			return success
		end
	else
		-- We set up the object the normal way, then.
		local egos

		success, egos = wish.extract_egos(wish_str, item_str, item, item_idx)

		if success ~= wish.GRANTED then
			return success
		end

		obj = create_object(item.tval, item.sval)

		for i = 1, getn(egos) do
			obj.ego_id[i] = egos[i]
		end

		-- Make it as if it was at created at level 99, since wishes
		-- are powerful and should give you something good.  Also
		-- make it a "good" object, so it won't be randomly cursed.
		apply_magic(obj, 99, FALSE, true, FALSE)
	end

	set_known(obj)
	set_aware(obj)

	drop_near(obj, -1, player.py, player.px)

	obj.found      = OBJ_FOUND_WISH
	obj.found_aux1 = current_dungeon_idx
	obj.found_aux2 = level_or_feat(current_dungeon_idx, dun_level)

	return wish.GRANTED
end

-- Attempt to grant what the player's wish
function wish.make()
	for i = 1, 10 do
		local wish_str

		repeat
			wish_str = get_string("Wish for what? ")

			if not wish_str then
				wish_str = ""
			end

			wish_str = clean_whitespace(wish_str)

			if strlen(wish_str) == 0 then
				message("To wish for nothing, simply enter " ..
					"\"nothing\" (without the quotes)")
			end
		until wish_str and strlen(wish_str) > 0

		wish_str = strlower(wish_str)

		if wish_str == "nothing" then
			return nil
		end

		local ret_val
		local wish_types = {wish.for_corpse, wish.for_monster,
			wish.for_artifact, wish.for_item}

		for j = 1, getn(wish_types) do
			local func = wish_types[j]

			ret_val = func(wish_str)

			if ret_val == wish.GRANTED then
				return true
			end
			if ret_val == wish.DENIED then
				break
			end
		end

		if ret_val ~= wish.DENIED then
			message("Nothing matching '" .. wish_str ..
				"' exists in the game")
		end
	end

	message("You took too long to make your wish, and it " ..
		"was wasted")
	return nil
end

-- vampiric bug-slaying fiery icy rapier of melting of shocking
