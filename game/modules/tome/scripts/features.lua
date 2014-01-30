constant("features", {})
settag(features, TAG_NAMESPACE)

-----------------------------------------------------------
-- Tunnelling
-----------------------------------------------------------

load_subsystem("tunnel",
{
	no_digger_msg = "You need to have a shovel or pick in your tool slot.",

	get_dig_power =
		function(how_type, how_what, known, feat, y, x)
			local power
			power = tunnel.default_get_dig_power(how_type, how_what,
												 known, feat, y, x)

			-- Diggable with bare hands?
			if (not power or power < 1) and
				how_type == kill_wall.how_type.INTRINSIC and
				has_flag(feat, FLAG_EASY_DIG)
			then
				return feat.flags[FLAG_EASY_DIG] + (player.stat(A_STR) / 2)
			end

			if power and power > 0 then
				-- If we're tunnelling with an object, and we aren't
				-- "tunneling" through a web, then object's weight
				-- and the player's strength increase the tunneling power.
				if how_type == kill_wall.how_type.OBJECT and
					not has_flag(feat, FLAG_SUBST_WEB) 
				then
					power = power + (how_what.weight / 10)
					-- Divide strength by 2, to keep in line with
					-- the old formula
					power = power + (player.stat(A_STR) / 2)
				end
				return power
			end

			-- If it's a web, and we're using an object, it
			-- might be able to cut or burn through the web.
			if how_type ~= kill_wall.how_type.OBJECT or
				not has_flag(feat, FLAG_SUBST_WEB)
			then
				-- Nope
				return 0
			end

			return tunnel.get_web_power(how_what, known)
		end -- get_dig_power()

	get_digger_name =
		function(how_type, how_what)
			if how_type == kill_wall.how_type.INTRINSIC then
				return "your hands", true
			end

			if how_type == kill_wall.how_type.OBJECT then
				local obj = how_what

				if wield_slot_ideal(obj, true) == INVEN_HANDS then
					return "your hands", true
				end

				local desc = object_desc(obj)
				if obj.tval == TV_LITE and obj.sval == SV_LITE_TORCH then
					-- Get rig of "turns of light" part of desc
					desc = gsub(desc, " %(.*%)", "")
				end

				if is_artifact(obj) then
					return "the " .. desc, tunnel.is_plural(obj)
				else
					return "your " .. desc, tunnel.is_plural(obj)
				end
			end
			return "CAN'T GET DIGGER NAME", false
		end -- get_digger_name()

	-- Special message for burning through web with a torch.
	get_digger_msgs =
		function(feat, y, x, how_type, how_what)
			if not has_flag(feat, FLAG_SUBST_WEB) or
				how_type ~= kill_wall.how_type.OBJECT
			then
				return nil, nil
			end

			local obj = how_what
			if obj.tval == TV_LITE and obj.sval == SV_LITE_TORCH then
				return "You burn the @feat@ with @digger@.",
				"You finishing burning the @feat@ with @digger@."
			end
			return nil, nil
		end -- get_digger_msgs()

	is_plural =
		function(obj)
			if obj.tval == TV_GLOVES or obj.tval == TV_BOOTS then
				return true
			end
			return false
		end
})

function tunnel.get_web_power(obj, known)
	if known then
		obj = generic_or_specific(obj)
	end

	if obj.flags[FLAG_DIG_POWER] and
		obj.flags[FLAG_DIG_POWER][FLAG_SUBST_WEB]
	then
		-- Has the flags for cutting through webs, but power
		-- is still non-positive.  Maybe to prevent the object
		-- from cutting through webs in spite of being
		-- a bladed weapon or torch?
		if not known or
			(flag_is_known(obj.flags, FLAG_DIG_POWER) and
			 flag_is_known(obj.flags[FLAG_DIG_POWER], FLAG_SUBST_WEB))
		then
			return 0
		end
	end

	if obj.tval == TV_LITE and obj.sval == SV_LITE_TORCH and
		(not has_flag(obj, FLAG_FUEL_LITE) or
		 obj.flags[FLAG_FUEL] > 0)
	then
		return 10
	end

	-- Only bladed weapons can cut through webs.
	if obj.tval ~= TV_AXE and obj.tval ~= TV_KNIFE and
		obj.tval ~= TV_SWORD
	then
		return 0
	end

	-- Web cutting power is proportional to slashing damage.
	local slash_dams = {dam.SLASH, dam.SLASH_STUN, dam.SLASH_WOUND,
		dam.SLASH_SLOW,	dam.SLASH_STUN_WOUND, dam.SLASH_SLOW_WOUND}
	local dams = obj.flags[FLAG_DAM]

	local power = 0
	for i = 1, getn(slash_dams) do
		local dam_type = slash_dams[i]
		if dams[dam_type] then
			power = power + ((flag_get(dams, dam_type) +
							  flag_get2(dams, dam_type)) / 2)
		end
	end

	-- Bladed weapon with no slashing damage???
	if power < 1 then
		return power
	end

	-- Acid and fire brands greatly add to the power to
	-- cut through webs.
	if not has_flag(obj, FLAG_BRAND) or
		(known and not known_flag(obj, FLAG_BRAND))
	then
		return power + (obj.to_d or 0)
	end

	local brands = obj.flags[FLAG_BRAND]
	local fire, acid = 0, 0

	if (not known and flag_exists(brands, dam.FIRE)) or
		flag_is_known(brands, dam.FIRE)
	then
		fire = brands[dam.FIRE]
	end
	if (not known and flag_exists(brands, dam.ACID)) or
		flag_is_known(brands, dam.ACID)
	then
		acid = brands[dam.ACID]
	end

	local max_brand = max(acid, fire)
	if max_brand < 2 then
		return power + (obj.to_d or 0)
	end

	return (power * (max_brand - 1) * 10) + (obj.to_d or 0)
end -- tunnel.get_web_power()

hook(hook.OBJECT_DESC,
function(obj, file, trim_down)
	if not is_known(obj) then
		return
	end

	if flag_is_known(obj.flags, FLAG_DIG_POWER) and
		flag_is_known(obj.flags[FLAG_DIG_POWER], FLAG_SUBST_WEB) then
		-- Already has web destroying power in the normal way.
		return
	end

	local power = tunnel.get_web_power(obj, true)

	if not power or power < 1 then
		return
	end

	local num_str = "#G" .. signed_number_tostring(power) .. "#w"
	if obj.tval == TV_LITE then
		desc_obj.text_out("It can be used to burn down webs(" ..
						  num_str .. ") when lit. ")
	else
		desc_obj.text_out("It can be used to cut through webs(" ..
						  num_str .. "). ")
	end
end) -- hook(hook.OBJECT_DESC)


tunnel.dig_power_desc = {
	[FLAG_SUBST_ROCK]     = "tunnel into rock",
	[FLAG_SUBST_GRANITE]  = "tunnel into granite",
	[FLAG_SUBST_RUBBLE]   = "dig into rubble",
	[FLAG_SUBST_WEB]      = "cut through webs",
	[FLAG_SUBST_ICE]      = "tunnel into ice",
	[FLAG_DEAD_TREE_FEAT] = "chop down living trees",
	[FLAG_SUBST_TREE]     = "chop down trees",
	[FLAG_SUBST_SAND]     = "dig through sand",
	[FLAG_SUBST_MAGMA]    = "dig throug magma",
	[FLAG_SUBST_QUARTZ]   = "tunnel into qaurtz",
}

tunnel.add_no_digger_msgs_by_flag {
	[FLAG_SUBST_WEB]    = "You have nothing equiped which can cut or burn "..
		"through the @feat@."
}

tunnel.add_working_msgs_by_flag {
	[FLAG_SUBST_TREE]   = "You chop away at the @feat@.",
	[FLAG_SUBST_WEB]    = "You hack at the @feat@."
	[FLAG_SUBST_ICE]    = "You tunnel into the @feat@."
	[FLAG_SUBST_SAND]   = "You tunnel into the @feat@."
	[FLAG_SUBST_MAGMA]  = "You tunnel into the @feat@."
	[FLAG_SUBST_QUARTZ] = "You tunnel into the @feat@."
}

tunnel.add_done_msgs_by_flag {
	[FLAG_SUBST_TREE]   = "You have cleared away the @feat@.",
	[FLAG_SUBST_WEB]    = "You have cleared away the @feat@."
	[FLAG_SUBST_ICE]    = "You have removed the @feat@."
	[FLAG_SUBST_SAND]   = "You have removed the @feat@."
	[FLAG_SUBST_MAGMA]  = "You have removed the @feat@."
	[FLAG_SUBST_QUARTZ] = "You have removed the @feat@."
}

tunnel.add_min_power_by_flags {
	[FLAG_SUBST_TREE]   = 10,
	[FLAG_SUBST_SAND]   = 5,
	[FLAG_SUBST_MAGMA]  = 10,
	[FLAG_SUBST_QUARTZ] = 20,
}

tunnel.add_hardness_by_flags {
	[FLAG_SUBST_TREE]   = 400,
	[FLAG_SUBST_SAND]   = 250,
	[FLAG_SUBST_MAGMA]  = 400,
	[FLAG_SUBST_QUARTZ] = 800,
}


-----------------------------------------------------------
-- Void jumpgates
-----------------------------------------------------------

function features.make_between_pair(y1, x1, y2, x2)
	local c_ptr1 = cave(y1, x1)
	local feat1  = f_info[c_ptr1.feat + 1]

	local c_ptr2 = cave(y2, x2)
	local feat2  = f_info[c_ptr2.feat + 1]

	if c_ptr1.feat == FEAT_BETWEEN or c_ptr2.feat == FEAT_BETWEEN then
		wiz_print("One of the jumpgate ends already had a jumpgate.")
		return false
	end

	if feat1.flags[FLAG_PERMANENT] or feat2.flags[FLAG_PERMANENT] then
		wiz_print("One of the jumpgate ends was on a permanent feature.")
		return false
	end

	if feat1.flags[FLAG_WALL] or feat2.flags[FLAG_WALL] then
		wiz_print("One of the jumpgate ends was in a wall.")
		return false
	end

	if not feat1.flags[FLAG_FLOOR] or not feat2.flags[FLAG_FLOOR] then
		wiz_print("One of the jumpgate ends had no floor.")
		return false
	end

	cave_set_feat(y1, x1, FEAT_BETWEEN)
	cave_set_feat(y2, x2, FEAT_BETWEEN)

	flag_set_full(c_ptr1.flags, FLAG_COORD, FLAG_FLAG_BOTH, y2, x2)
	flag_set_full(c_ptr2.flags, FLAG_COORD, FLAG_FLAG_BOTH, y1, x1)

	return true
end

function features.do_between()
	local c_ptr = cave(player.py, player.px)

	if c_ptr.feat ~= FEAT_BETWEEN then
		wiz_print("features.do_between() called without a jumpgate")
		return false
	end

	if not c_ptr.flags[FLAG_COORD] then
		wiz_print("Jumpgate doesn't have a destination coordinate")
		return false
	end

	if has_flag(level_flags, FLAG_NO_TELEPORT) or
		player.has_intrinsic(FLAG_RES_CONTINUUM) or
		player.has_intrinsic(FLAG_NO_TELE) then
		message("Something prevents you from entering the jumpgate.")
		return false
	end

	message("You fall into the void.");
	message("Brrrr! It's deadly cold.");

	local y = flag_get( c_ptr.flags, FLAG_COORD)
	local x = flag_get2(c_ptr.flags, FLAG_COORD)

	swap_position(y, x)

	return true
end

-------------------------------------------------------------------------
-- Fountains
-------------------------------------------------------------------------

function features.fountain_potion_tester(k_idx)
	local item = k_info[k_idx + 1]

	if item.tval == TV_POTION and item.flags[FLAG_FOUNTAIN] then
		return true
	end

	return false
end

function features.fountain_do()
	local c_ptr = cave(player.py, player.px)

	if player.has_intrinsic(FLAG_HALLUCINATE) then
		message("Is that a fountain you see, or the gaping maw of some " ..
				"horrid monster?")
		return
	end

	if c_ptr.feat ~= FEAT_EMPTY_FOUNTAIN and c_ptr.feat ~= FEAT_FOUNTAIN then
		message("You see no fountain here.")
		return
	end

	if c_ptr.feat == FEAT_EMPTY_FOUNTAIN then
		message("The fountain is dried out.")
		return
	end

	-- Do some sanity checking
	local obj = c_ptr.flags[FLAG_FOUNTAIN]

	if not obj then
		message("Strange, the fountain seems to already be dry.")
		cave_set_feat(player.py, player.px, FEAT_EMPTY_FOUNTAIN)
		return
	end

	if obj.number == 0 then
		message("Strange, the fountain seems to already be dry.")
		c_ptr.flags[FLAG_FOUNTAIN] = nil
		cave_set_feat(player.py, player.px, FEAT_EMPTY_FOUNTAIN)
		return
	end

	if not obj.flags[FLAG_ON_QUAFF] then
		message(color.VIOLET, "ERROR: Fountain contains something other " ..
				"than a potion.")
		return
	end

	if not obj.flags[FLAG_FOUNTAIN] then
		message(color.VIOLET, "ERROR: Fountain contains a potion which " ..
				"shouldn't show up in fountains.")
		return
	end

	-- Sanity checks were all good

	-- Can only fill into a bottle when not blind, since if it was
	-- done when blind the character would get info on the color/type
	-- of potion, which is something that has to be seen.
	local prompt = "Do you want to [Q]uaff or [F]ill from the fountain? "
	if player.has_intrinsic(FLAG_BLIND) then
		prompt = "Quaff from the fountain? [y/n] "
	end

	local ret, key = get_com(prompt)
	if not ret then
		return
	end
	key = strlower(strchar(key))

	if player.has_intrinsic(FLAG_BLIND) then
		if key == 'y' then
			features.fountain_quaff()
		end
		return
	end

	if key == 'q' then
		features.fountain_quaff()
	elseif key == 'f' then
		features.fountain_fill()
	end
end -- features.fountain_do()

function features.fountain_consume(amount)
	local c_ptr = cave(player.py, player.px)
	local obj   = c_ptr.flags[FLAG_FOUNTAIN]

	if obj.number < amount then
		obj.number = 0
	else
		obj.number = obj.number - amount
	end

	if obj.number == 0 then
		c_ptr.flags[FLAG_FOUNTAIN] = nil
		cave_set_feat(player.py, player.px, FEAT_EMPTY_FOUNTAIN)
		message("You have emptied the fountain.")
	end
end -- features.fountain_consume()

function features.fountain_learn(y, x)
	if player.has_intrinsic(FLAG_BLIND) or
		player.has_intrinsic(FLAG_HALLUCINATE)
	then
		return
	end

	if has_flag(level_flags, FLAG_DOUBLE) then
		local disp = { {0, 0}, {0, 1}, {1, 0}, {1, 1}}

		y, x = dungeons.to_even_coords(y, x)

		for i = 1, 4 do
			local tmp = disp[i]

			flag_learn(cave(y + tmp[1], x + tmp[2]).flags,
					   FLAG_FOUNTAIN, true)
		end
	else
		flag_learn(cave(y, x).flags, FLAG_FOUNTAIN, true)
	end
end

function features.fountain_quaff()
	local c_ptr  = cave(player.py, player.px)
	local potion = c_ptr.flags[FLAG_FOUNTAIN]

	features.fountain_learn(player.py, player.px)
	potions.use_aux(potion)
	features.fountain_consume(1)
end -- features.fountain_quaff()

function features.fountain_fill()
	local c_ptr  = cave(player.py, player.px)
	local potion = c_ptr.flags[FLAG_FOUNTAIN]

	-- Collect all the empty bottles we can use
	local bottle_objs = {}

	__core_objects.clean_get_item()
	for_inventory(player, INVEN_INVEN, INVEN_TOTAL,
		function(obj, inven, slot, item)
			if obj.tval == TV_BOTTLE and
				obj.sval == SV_BOTTLE_EMPTY then
				tinsert(%bottle_objs, {obj=obj, item=item})
			end
		end)

	for_inventory_inven(c_ptr.inventory,
		function(obj, item)
			if obj.tval == TV_BOTTLE and
				obj.sval == SV_BOTTLE_EMPTY then
				item = generate_item_floor_index(obj.iy, obj.ix, item)
				tinsert(%bottle_objs, {obj=obj, item=item})
			end
		end)

	local bottle_amount = 0
	for i = 1, getn(bottle_objs) do
		bottle_amount = bottle_amount + bottle_objs[i].obj.number
	end

	if bottle_amount == 0 then
		message("You have no empty bottles into which to place the potions.")
		return
	end

	local get_num = get_quantity("How many bottles to fill? (1 - " ..
								 bottle_amount ..") ", bottle_amount)

	if get_num <= 0 then
		return
	end

	if get_num > potion.number then
		get_num = potion.number
	end

	if get_num > bottle_amount then
		get_num = bottle_amount
	end

	local out_obj = new_object()
	object_prep(out_obj, potion.k_idx)
	out_obj.number = get_num

	message("You extract " .. object_desc(out_obj, true) .. " from " ..
			"the fountain.")

	for i = getn(bottle_objs), 1, -1 do
		if get_num <= 0 then
			break
		end

		local bottle = bottle_objs[i]

		bottle_amount = bottle.obj.number

		if bottle_amount > get_num then
			bottle_amount = get_num
		end

		item_increase(bottle.item, -bottle_amount)
		item_optimize(bottle.item)

		get_num = get_num - bottle_amount

		features.fountain_consume(bottle_amount)
	end

	local slot = inven_carry(out_obj, false)

	if slot ~= -1 then
		local obj = get_object(item_slot_to_item(slot))

		if obj then
			hook.process(hook.GET_POST, obj, slot)
		end
	end

	-- Now we know what sort of potion is in there
	features.fountain_learn(player.py, player.px)
end -- features.fountain_fill()

hook(hook.KEYPRESS,
function (key)
	if key == strbyte('H') then
		features.fountain_do()
		return true
	end
end)

-----------------------------------------------------------
-- Glyphs
-----------------------------------------------------------

function features.can_place_glyph(y, x, feat, silent)
	local c_ptr = cave(y, x)
	local f_ptr = f_info(c_ptr.feat)
	local name  = f_ptr.name
	local mimic

	if c_ptr.mimic ~= 0 then
		mimic = f_info(c_ptr.mimic)
		name  = mimic.name
	end

	if c_ptr.flags[FLAG_GLYPH_POWER] then
		if not silent then
			message("A glyph is already present there.")
		end
		return false
	end

	if c_ptr.flags[FLAG_ORIG_FEAT] then
		if not silent then
			message("The " .. name .. " resists the spell.")
		end
		return false
	end

	if not f_ptr.flags[FLAG_FLOOR] then
		if not silent then
			if not mimic or not mimic.flags[FLAG_FLOOR] then
				message("A glyph needs a floor on which to be placed.")
			else
				message("The " .. name .. " resists the spell.")
			end
		end
		return false
	end

	if f_ptr.flags[FLAG_PERMANENT] then
		if not silent then
			message("The " .. name .. " resists the spell.")
		end
		return false
	end

	return true
end -- features.can_place_glyph()

function features.try_break_glyph(y, x, monst, msg)
	local c_ptr = cave(y, x)
	local flags = c_ptr.flags

	if rng(flags[FLAG_GLYPH_POWER]) < monst.level then
		if (c_ptr.info & CAVE_MARK) ~= 0 then
			message(msg)
		end

		c_ptr.feat = flags[FLAG_ORIG_FEAT]

		flags[FLAG_ORIG_FEAT]   = nil
		flags[FLAG_GLYPH_POWER] = nil

		if c_ptr.feat == FEAT_GLYPH_WARDING or
			c_ptr.feat == FEAT_EXPLOSIVE_RUNE
		then
			message(color.LIGHT_RED, "ERROR: Original terrain was also " ..
					"a glyph!")
			place_floor(y, x)
		end

		return true
	end

	return false
end
