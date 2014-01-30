--
-- The auto identification subsystem
--
--
-- Load it with: load_subsystem("auto_id")
--
-- Parameters:
-- * default_id_level: Function that returns the default id_value for an object if it does not have one
-- * default_gid_level: Function that returns the default greater id_value for an object if it does not have one


new_flag("AUTO_ID")
new_flag("ID_SKILL")	-- Which skill is used to ID
new_flag("ID_VALUE")	-- Required skill to auto-ID

constant("auto_id", {})

auto_id.default_id_level = get_subsystem_param("auto_id", "default_id_level") or function() return 0 end
auto_id.default_gid_level = get_subsystem_param("auto_id", "default_gid_level") or function() return 0 end

function auto_id.check_skill(obj, sk, value_id, value_gid)
	if value_id == 0 then value_id = auto_id.default_id_level(obj, sk) end
	if value_gid == 0 then value_gid = auto_id.default_gid_level(obj, sk) end

	-- If the player skill is high enough, id
	if value_id > 0 and get_skill(sk) >= value_id then
		set_aware(obj)
		set_known(obj)
	end

	-- If the player skill is high enough, greater id it
	if value_gid > 0 and get_skill(sk) >= value_gid then
		make_item_fully_identified(obj)
	end
end

function auto_id.do_object(obj)
	-- If the player has full auto id, just id everything
	if player.has_intrinsic(FLAG_AUTO_ID) then
		set_aware(obj)
		set_known(obj)
	end

	-- Nothing anymore to do
	if not obj.flags[FLAG_ID_SKILL] then return end
	local skills = obj.flags[FLAG_ID_SKILL]
	if tag(skills) == TAG_NUMBER then
		auto_id.check_skill(obj, skills, flag_get(obj.flags, FLAG_ID_VALUE), flag_get2(obj.flags, FLAG_ID_VALUE))
	else
		foreach_flags(skills, function(skills, id)
			local vid, vgid = flag_get(skills, id), flag_get2(skills, id)
			if vid == 0 then vid = flag_get(%obj.flags, FLAG_ID_VALUE) end
			if vgid == 0 then vgid = flag_get2(%obj.flags, FLAG_ID_VALUE) end
			auto_id.check_skill(%obj, id, vid, vgid)
		end)
	end
end

function auto_id.do_inventory(src)
	for_inventory(src, INVEN_INVEN, INVEN_TOTAL, function(obj)
		auto_id.do_object(obj)
	end)
end

hook(hook.DROPPED_NEAR,
function(obj, chance, orig_y, orig_x, real_y, real_x)
	if player.py == real_y and player.px == real_x then
		auto_id.do_object(obj)
	end
end)

hook(hook.MOVE_OK, function()
	for_inventory_inven(cave(player.py, player.px).inventory, function(obj, item)
		auto_id.do_object(obj)
	end)
end)

hook(hook.GET_POST, auto_id.do_object)
hook(hook.WIELD_POST, auto_id.do_object)
