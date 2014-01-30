---- Objects classification
define_objects_categorization
{
	{ "WEAPON",
		{
			"DAGGER",
			"RAPIER",
			"SMALL_SWORD",
			"CUTLASS",
			"BROAD_SWORD",
			"LONG_SWORD",
			"TWO_HANDED_SWORD",
		}
	},
	{ "ARMOR",
		{
			"FILTHY_RAG",
			"ROBE",
			"SOFT_LEATHER_ARMOR",
			"SOFT_STUDDED_LEATHER",
			"HARD_LEATHER_ARMOR",
			"METAL_SCALE_MAIL",
			"CHAIN_MAIL",
			"DOUBLE_RING_MAIL",
			"AUGMENTED_CHAIN_MAIL",
			"DOUBLE_CHAIN_MAIL",
		}
	},
	{ "POTION",
		{
			"POTION_BLOOD",
			"POTION_INVIS",
			"POTION_SPEED",
			"POTION_CURE_LIGHT",
			"POTION_CURE_SERIOUS",
			"POTION_CURE_CRITICAL",
			"POTION_HEALING",
			"POTION_RESTORATION",
			"POTION_INC_STR",
			"POTION_INC_CON",
		},
	},
	{ "LITE",
		{
			"LANTERN",
		},
	},
	{ "FOOD",
		{
			"FOOD_RATION",
		},
	},
	{ "GOLD",
		{

		},
	},
}

-- THEME_ALL
rand_obj.themed.register_new
(
	"THEME_ALL",
	function(obj_level, theme_flags, extra_filter)
		return rand_obj.get_obj(obj_level, {function() return true end, extra_filter})
	end
)

---------------------------------------------------------
------------------- Easy mode inventory -----------------
---------------------------------------------------------
local smart_inven_action = function(item,  obj)
	local ret_val = { val=true }
	local tries =
	{
		{ key='e' name="Examine" tester=function() return true end action=function() %ret_val.val = false return true end }
		{ key='q' name="Quaff" tester=potions.item_tester, action=potions.use }
		{ key='w' name="Wield" tester=inventory.wield_item_tester, action=inventory.wield }
		{ key='t' name="Takeoff" tester=inventory.takeoff_item_tester, action=inventory.takeoff }
		{ key='d' name="Drop" tester=inventory.drop_item_tester, action=inventory.drop }
		{ key='k' name="Destroy" tester=inventory.destroy_item_tester, action=inventory.destroy }
	}
	local real = {}
	local real_keys = {}
	for act in tries do
		if act.tester(obj, item) then
			tinsert(real, act)
			real_keys[act.key] = act
		end
	end

	local sel = 1
	local ret, wid, hgt = term.get_size()
	local old_term = term.save_to()
	while true do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)

		display_list((hgt - getn(real)) / 2, (wid - 20) / 2, getn(real) + 1, 20, "Actions", real, 1, sel, color.LIGHT_GREEN, function(e)
			return e.key.."> "..e.name
		end)

		local raw = term.inkey()
		local key = strchar(raw)

		if raw == ESCAPE then
			term.load_from(old_term, true)
			break
		elseif key == '2' then
			sel = sel + 1
			if sel > getn(real) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(real) end
		elseif key == '\r' then
			term.load_from(old_term, true)
			real[sel].action(item)
                        break
		elseif real_keys[key] then
			term.load_from(old_term, true)
			real_keys[key].action(item)
                        break
		end

	end

	return ret_val.val
end

-- Load the inventory subsystem with  the special helper
load_subsystem("inventory",{
	show_action = smart_inven_action
})
-- We load it after inventory because it depends on it
-- And we dont want it to auloload inventory since we need specific parameters
load_subsystem("birth_auto_wield")
